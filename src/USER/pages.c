#include "pages.h"


void PagesInit(void)
{	
	sysState.localt=0;
	sysState.lochdg=0;
	sysState.loclat=0;
	sysState.loclon=0;
	sysState.pitch=0;
	sysState.range=0;
	sysState.rmtalt=0;
	sysState.rmtlat=0;
	sysState.rmtlon=0;
	sysState.yaw=0;
	sysState.enable=0;
	
	aat_init(1);
	keytest_init(1);
	flight_init(1);
	servo_init(1);
}

void PagesChangeTo(u8 p)
{
	
	if(p!=currpage)
	{
		currpage=p;
		switch(p)
		{
			case KeyTestPage:
				keytest_init(0);
			break;
			case AATPage:
				aat_init(0);
			break;
			case FlightPage:
				flight_init(0);
			break;
			case ServoPage:
				servo_init(0);
			break;
			default:
				currpage=AATPage;
				aat_init(0);
			break;
		}
	}
}

void PagesNext(s8 d)
{
	if(d<0)
	{
		if(currpage==0)
			PagesChangeTo(PageNum-1);
		else
			PagesChangeTo(currpage-1);
	}
	if(d>0)
		PagesChangeTo((currpage+1)%PageNum);
}

void PagesUpdate(void)
{
	switch(currpage)
	{
		case KeyTestPage:
			keytest_update();
		break;
		case AATPage:
			aat_update();
		break;
		case FlightPage:
			flight_update();
		break;
		case ServoPage:
			servo_update();
		break;
		default:
			currpage=AATPage;
			aat_init(0);
			aat_update();
		break;
	}
}

void PagesDrawHeader(u8 n,u8 *name)
{
	OledDispString(0,0,"=",0);
	OledDispString(19,0,"=",0);
	OledDispString(4,0,"/",0);
	OledDispInt(5,0,PageNum,0,0);
	if(n>8)
		OledDispInt(2,0,n+1,0,0);
	else
		OledDispInt(3,0,n+1,0,0);
	OledDispString(8,0,name,0);
}
