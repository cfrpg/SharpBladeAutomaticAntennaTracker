#include "pages.h"


struct
{
	u8 state;
} aat;

void aat_refresh_local(void)
{
	OledDispString(0,1,"Local     ",0);	
	OledDispFixed(4,2,sysState.loclat,7,0);
	OledDispFixed(4,3,sysState.loclon,7,0);
	OledDispFixed(4,4,sysState.localt,2,0);
	OledDispFixed(14,4,sysState.lochdg,2,0);
}

void aat_refresh_remote(void)
{
	OledDispString(0,1,"Remote     ",0);	
	OledDispFixed(4,2,sysState.rmtlat,7,0);
	OledDispFixed(4,3,sysState.rmtlon,7,0);
	OledDispFixed(4,4,sysState.rmtalt,2,0);
	OledDispFixed(14,4,sysState.rmthdg,2,0);
}

void aat_refresh_state(void)
{
	s32 dist=(s32)(sysState.range*10);
	s32 dir=(s32)(sysState.direction*10);
	s32 yaw=(s32)(sysState.yaw*10);
	s32 pitch=(s32)(sysState.pitch*10);
	OledDispFixed(5,5,dist,1,0);
	OledDispFixed(14,5,dir,1,0);
	OledDispFixed(4,6,yaw,1,0);
	OledDispFixed(14,6,pitch,1,0);
}

void aat_init(u8 f)
{
	if(f)
	{
		aat.state=0;
	}
	OledClear(0);
	PagesDrawHeader(AATPage,"Tracker");
	OledDispString(0,2,"Lat:",0);
	OledDispString(0,3,"Lon:",0);
	OledDispString(0,4,"ALT:",0);
	OledDispString(10,4,"HDG:",0);
	OledDispString(0,5,"Dist:",0);
	OledDispString(10,5,"Dir:",0);
	OledDispString(0,6,"Yaw:",0);
	OledDispString(10,6,"Pit:",0);	
	OledDispString(0,7,"SET",0);
	OledDispString(16,7,"SWAP",0);
}

void aat_update(void)
{
	u8 key=currKey&(currKey^lastKey);
	switch(aat.state)
	{
		case 0:
		if(key&KEY_C)
			aat.state=1;		
		aat_refresh_local();
		break;
		case 1:
		if(key&KEY_C)
			aat.state=0;
		aat_refresh_remote();
		break;
		case 2:
		
		break;
	}
	if(key&KEY_A)
	{
		sysState.localt=sysState.rmtalt;
		sysState.loclon=sysState.rmtlon;
		sysState.lochdg=sysState.rmthdg;
		sysState.loclat=sysState.rmtlat;
	}
	if(key&KEY_D)
		sysState.enable^=1;
	aat_refresh_state();
	
	if(currWheel>lastWheel)
		PagesNext(1);
	if(currWheel<lastWheel)
		PagesNext(-1);
}

