#include "stm32f10x.h"
#include "string.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "oled.h"
#include "math.h"
#include "keyboard.h"
#include "mavlink.h"
#include "pages.h"
#include "parameter.h"
#include "pwm.h"

#define REarth 6378245.0
#define D2R 0.0174532925199432957692369
#define R2D 57.295779513082320876798154
#define PI 3.1415926535897932384626433

u16 tick[3]={0,0,0};
u8 package[263];
u8 seq=0;

u8 lastKey=0;
u8 currKey=0;
s32 lastWheel=0;
s32 currWheel=0;

u8 currpage;

Mav_Attitude attpkg;
Mav_GolbalPositionInt pospkg;

systemState sysState;

void getDist()
{
	double lat1,lat2,lon1,lon2,c,s,dir;
	double slat2;
	lat1=(90-sysState.loclat/1.0e7)*D2R;
	lat2=(90-sysState.rmtlat/1.0e7)*D2R;
	slat2=sin(lat2);
	lon1=sysState.loclon/1.0e7*D2R;
	lon2=sysState.rmtlon/1.0e7*D2R;
	c=sin(lat1)*slat2*cos(lon2-lon1)+cos(lat1)*cos(lat2);
	sysState.range=REarth*acos(c);
	s=sqrt(1-c*c);
	dir=asin((slat2*sin(lon2-lon1))/s);
	if(sysState.rmtlat>sysState.loclat&&lon2<lon1)
		dir=2*PI+dir;
	if(sysState.rmtlat<sysState.loclat)
		dir=PI-dir;
	sysState.direction=dir*R2D;
	sysState.pitch=atan2((sysState.rmtalt-sysState.localt)/100.0,sysState.range)*R2D;
	sysState.yaw=sysState.direction-sysState.lochdg/100.0;
	if(sysState.yaw>180)
		sysState.yaw-=360;
	else if(sysState.yaw<-180)
		sysState.yaw+=360;
}


int main(void)
{	
	u8 i,j,k;
	u8 oled=0;

	u8 t;
	
	delay_init();
	NVIC_Configuration();
	uart_init(115200);
	
	OledInit();
	KeyInit();
	
	
	currpage=255;
	
	
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	OledClear(0);	
	ParamRead();
	if(params.headFlag!=0xCFCF||params.tailFlag!=0xFCFC)
	{
		ParamReset();
		ParamWrite();
		OledDispString(1,2,"Setup parameters.",0);
	}
	PWMInit();
	OledDispString(0,0,"== Sharp Blade AAT ==",0);
	OledDispString(1,3,"Telemetry baudrate?",0);
	OledDispString(0,7,"115200          57600",0);
	OledRefresh();

	while(1)
	{
		
		currKey=GPGetData();
		if(currKey&KEY_A)
		{
			MavlinkInit(115200);
			break;
		}
		if(currKey&KEY_C)
		{
			MavlinkInit(57600);
			break;
		}
			
	}
	
	MainClockInit();
	
	PagesInit();
	PagesChangeTo(AATPage);
//	for(i=0;i<21;i++)
//	{
//		for(j=0;j<16;j++)
//		{
//			OledSetChar(i,j,'A'+i*j*2,0);
//		}
//	}
	
	while(1)
	{		
		if(tick[0]>100)
		{			
			tick[0]=0;	
			if(sysState.enable)
			{
				getDist();
				if(sysState.range>10)
				{
					PWMSet(sysState.yaw,sysState.pitch);
				}
			}
		}
		if(tick[1]>10)
		{
			tick[1]=0;
			if(mavlinkReady)
			{				
				memcpy(package,mavlinkBuff[mavlinkCurr],mavlinkLen[mavlinkCurr]);				
				mavlinkReady=0;
				seq++;				
				seq=package[2];
				
				if(package[5]==30)
				{
					memcpy(&attpkg,package+6,sizeof(Mav_Attitude));
				}
				if(package[5]==33)
				{
					memcpy(&pospkg,package+6,sizeof(Mav_GolbalPositionInt));
					sysState.rmtlat=pospkg.lat;
					sysState.rmtlon=pospkg.lon;
					sysState.rmtalt=pospkg.relalt/10;
					sysState.rmthdg=pospkg.hdg;
				}
			}			
		}
		
		if(tick[2]>100)
		{
			tick[2]=0;
			currKey=GPGetData();
			currWheel=WheelGetValue();
			PagesUpdate();
			lastKey=currKey;
			lastWheel=currWheel;
//			oled++;
//			
//			oled&=0x0F;
//			//OledClear(oled);
//			
//			OledDispString(0,0,"ASDFGHJKL",0);
//			//OledRefresh();
//			printf("%d\n",tick[2]);		
		}		
	}
}

void update_init(u8 key)
{
	
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
		u8 n=3;
		while(n--)
		{
			tick[n]++;
		}
	}
}
