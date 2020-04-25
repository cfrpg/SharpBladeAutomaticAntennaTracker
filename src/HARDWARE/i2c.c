#include "i2c.h"
#include "delay.h"

void I2CInit(void)
{
	GPIO_InitTypeDef gi;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	gi.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;
	gi.GPIO_Mode=GPIO_Mode_Out_OD;
	gi.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gi);
	
	I2CStop();
}

void i2cDelay(void)
{
	delay_us(4);
}

void I2CStart(void)
{
	I2C_SDA_OUT=1;
	I2C_SCL_OUT=1;
	i2cDelay();
	I2C_SDA_OUT=0;
	i2cDelay();
	I2C_SCL_OUT=0;
	i2cDelay();
}

void I2CStop(void)
{
	I2C_SDA_OUT=0;
	I2C_SCL_OUT=1;
	i2cDelay();
	I2C_SDA_OUT=1;
}

void I2CSendByte(u8 b)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(b&0x80)
			I2C_SDA_OUT=1;
		else
			I2C_SDA_OUT=0;
		i2cDelay();
		I2C_SCL_OUT=1;
		i2cDelay();
		I2C_SCL_OUT=0;
		if(i==7)
			I2C_SDA_OUT=1;
		b<<=1;
		i2cDelay();
	}
}

u8 I2CReadByte(void)
{
	u8 i,res;
	res=0;
	for(i=0;i<8;i++)
	{
		res<<=1;
		I2C_SCL_OUT=1;
		i2cDelay();
		if(I2C_SDA_IN)
			res|=1;
		I2C_SCL_OUT=0;
		i2cDelay();
	}
	return res;
}

u8 I2CWaitAck(void)
{
	u8 res;
	I2C_SDA_OUT=1;
	i2cDelay();
	I2C_SCL_OUT=1;
	i2cDelay();
	if(I2C_SDA_IN)
		res=1;
	else
		res=0;
	I2C_SCL_OUT=0;
	i2cDelay();
	return res;
}

void I2CAck(void)
{
	I2C_SDA_OUT=0;
	i2cDelay();
	I2C_SCL_OUT=1;
	i2cDelay();
	I2C_SCL_OUT=0;
	i2cDelay();
	I2C_SDA_OUT=1;
}

void I2CNAck(void)
{
	I2C_SDA_OUT=1;
	i2cDelay();
	I2C_SCL_OUT=1;
	i2cDelay();
	I2C_SCL_OUT=0;
	i2cDelay();
}

u8 I2CCheckDevice(u8 addr)
{
	
}
