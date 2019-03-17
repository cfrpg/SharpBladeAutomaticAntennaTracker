#include "oled.h"
#include "delay.h"
#include "string.h"
#include "font.h"
#include "stdio.h"

u8 oledBuff[8][22];
u8 oledUpdateFlag[21];
u8 oledRevFlag[21];

void OledInit(void)
{
	GPIO_InitTypeDef gi;
	SPI_InitTypeDef si;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	//Init SPI GPIO
	gi.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7;
	gi.GPIO_Mode=GPIO_Mode_AF_PP;
	gi.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gi);
	//Init common GPIO
	gi.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_6;
	gi.GPIO_Mode=GPIO_Mode_Out_PP;
	gi.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gi);
	gi.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOB,&gi);
	
	//Init SPI1
	si.SPI_Direction=SPI_Direction_1Line_Tx;
	si.SPI_Mode=SPI_Mode_Master;
	si.SPI_DataSize=SPI_DataSize_8b;
	si.SPI_CPOL=SPI_CPOL_Low;
	si.SPI_CPHA=SPI_CPHA_1Edge;
	si.SPI_NSS=SPI_NSS_Soft;
	si.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;
	si.SPI_FirstBit=SPI_FirstBit_MSB;
	si.SPI_CRCPolynomial=7;
	SPI_Init(SPI1,&si);
	SPI_Cmd(SPI1,ENABLE);
	
	memset(oledBuff,0,168);
	memset(oledUpdateFlag,0,21);
	memset(oledRevFlag,0,21);
	OledStartUp();	
}

void OledSendByte(u8 b)
{
	OLED_CS=0;
	//while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI1,b);	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
	//delay_us(100);
	OLED_CS=1;
}

void OledStartUp(void)
{
	OLED_RST=1;
	delay_ms(100);
	OLED_RST=0;
	delay_ms(100);
	OLED_RST=1;
	delay_ms(100);
	OLED_DC=OLED_CMD;
	OledSendByte(0xAE);//--turn off oled panel
	OledSendByte(0x02);//---set low column address
	OledSendByte(0x10);//---set high column address
	OledSendByte(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OledSendByte(0x81);//--set contrast control register
	OledSendByte(0xCF); // Set SEG Output Current Brightness
	OledSendByte(0xA1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OledSendByte(0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OledSendByte(0xA6);//--set normal display
	OledSendByte(0xA8);//--set multiplex ratio(1 to 64)
	OledSendByte(0x3f);//--1/64 duty
	OledSendByte(0x81); //对比度设置
	OledSendByte(0xfF); //1~255;默认0X7F (亮度设置,越大越亮)
	OledSendByte(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OledSendByte(0x00);//-not offset
	OledSendByte(0xd5);//--set display clock divide ratio/oscillator frequency
	OledSendByte(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OledSendByte(0xD9);//--set pre-charge period
	OledSendByte(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OledSendByte(0xDA);//--set com pins hardware configuration
	OledSendByte(0x12);
	OledSendByte(0xDB);//--set vcomh
	OledSendByte(0x20);//Set VCOM Deselect Level
	OledSendByte(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OledSendByte(0x02);//
	OledSendByte(0x8D);//--set Charge Pump enable/disable
	OledSendByte(0x14);//--set(0x10) disable
	OledSendByte(0xA4);// Disable Entire Display On (0xa4/0xa5)
	OledSendByte(0xA6);// Disable Inverse Display On (0xa6/a7) 
	OledSendByte(0xAF);//--turn on oled panel
	
	//OledSendByte(0xAF); /*display ON*/ 
}

void OledDispOn(void)
{
	OLED_DC=OLED_CMD;
	OledSendByte(0x8D);//--set Charge Pump enable/disable
	OledSendByte(0x14);
	OledSendByte(0xAF);//display ON 
}

void OledDispOff(void)
{
	OLED_DC=OLED_CMD;
	OledSendByte(0x8D);//--set Charge Pump enable/disable
	OledSendByte(0x10);
	OledSendByte(0xAE);//display ON 
}

void OledSetPos(u8 x,u8 y)
{
	OLED_DC=OLED_CMD;
	OledSendByte(0xB0+y);
	OledSendByte(((x&0xF0)>>4)|0x10);
	OledSendByte((x&0x0F)|0x01);
}

void OledClear(u8 c)
{
	u8 i,j;
	for(i=0;i<8;i++)
	{
		OLED_DC=OLED_CMD;
		//delay_us(100);
		OledSendByte(0xB0+i);
		OledSendByte(0x00);
		OledSendByte(0x10);
		OLED_DC=OLED_DATA;
		//delay_us(100);
		for(j=0;j<128;j++)
			OledSendByte(c);
	}
}

void OledRefresh(void)
{
	u8 i,j,k,m;
	u8 str[10]={0};
	for(i=0;i<8;i++)
	{
		m=1<<i;
		OLED_DC=OLED_CMD;
		//delay_us(100);
		OledSendByte(0xB0+i);
		OledSendByte(0x00);
		OledSendByte(0x10);
		//delay_us(100);
		OLED_DC=OLED_DATA;
		
		for(j=0;j<21;j++)
		{
			for(k=0;k<6;k++)
			{
				if(oledRevFlag[j]&m)
					OledSendByte((F6x8[oledBuff[i][j]][k]^0xFF));
				else
					OledSendByte(F6x8[oledBuff[i][j]][k]);
			}
		}
//		if(oledRevFlag[1]&m)
//			str[i]='1';
//		else
//			str[i]='0';	
	}
	//printf("%s\r\n",str);
	//memset(oledUpdateFlag,0,21);
}

void OledSetChar(u8 x,u8 y,s8 c,u8 f)
{
	oledBuff[y][x]=c-' ';
	if(f)
		oledRevFlag[x]|=(0x01<<y);
	else
		oledRevFlag[x]&=((0x01<<y)^0xFF);
	//oledUpdateFlag[x]|=(1<<y);
}

u8 OledSetString(u8 x,u8 y,s8 c[],u8 f)
{
	u8 i;
	
	for(i=0;c[i]!=0;i++)
	{
		oledBuff[y][x+i]=c[i]-' ';
		if(f)
			oledRevFlag[x+i]|=(0x01<<y);
		else
			oledRevFlag[x+i]&=((0x01<<y)^0xFF);
	}
	return i;
}

void OledSetRev(u8 x,u8 y,s8 len,u8 f)
{	
	while(len--)
	{
		if(f)
			oledRevFlag[x+len]|=(0x01<<y);
		else
			oledRevFlag[x+len]&=((0x01<<y)^0xFF);
	}
}

void OledRefreshPart(u8 x,u8 y,s8 len)
{
	s8 i,j;
	u8 m=1<<y;
	OledSetPos(x*6,y);
	OLED_DC=OLED_DATA;
	for(i=0;i<len;i++)
	{
		for(j=0;j<6;j++)
		{
			if(oledRevFlag[x+i]&m)
				OledSendByte((F6x8[oledBuff[y][x+i]][j]^0xFF));
			else
				OledSendByte(F6x8[oledBuff[y][x+i]][j]);
		}
	}
}

void OledDispChar(u8 x,u8 y,s8 c,u8 f)
{
	OledSetChar(x,y,c,f);
	OledRefreshPart(x,y,1);	
}

void OledDispString(u8 x,u8 y,s8 c[],u8 f)
{
	u8 t=OledSetString(x,y,c,f);
	OledRefreshPart(x,y,t);
}

void OledDispInt(u8 x,u8 y,s32 v,u8 f)
{
	s8 buf[12],p=0,len=0;
	s32 t;
	if(v<0)
	{
		v=-v;
		p=1;
		buf[0]='-';
		len++;
	}
	t=v;
	
	do{
		t/=10;
		len++;
	}while(t>0);		
	buf[len]=0;
	len--;
	while(len>=p)
	{
		buf[len]=v%10+'0';
		v/=10;
		len--;
	}
	OledDispString(x,y,buf,f);	
}

void OledDispDouble(u8 x,u8 y,float v,s8 len,s8 pre,u8 f)
{
	
}

void OledDispFixed(u8 x,u8 y,s32 v,s8 pre,u8 f)
{
	s8 buf[14],p=0,len=0;
	s32 t;
	if(v<0)
	{
		v=-v;
		p=1;
		buf[0]='-';		
	}
	t=v;	
	do{
		t/=10;
		len++;
	}while(t>0);	
	if(len<=pre)
		len=pre+1;
	if(p)
		len++;
	pre=len-pre;
	buf[len+1]=0;
	while(len>=p)
	{
		if(len==pre)
			buf[len]='.';
		else
		{
			buf[len]=v%10+'0';
			v/=10;
		}
		len--;
	}
	OledDispString(x,y,buf,f);	
}
