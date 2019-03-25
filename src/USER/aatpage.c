#include "pages.h"


struct
{
	u8 state;
} aat;

void aat_refresh_local(void)
{	
	OledDispFixed(4,3,sysState.loclat,7,0);
	OledDispFixed(4,4,sysState.loclon,7,0);
	OledDispFixed(4,5,sysState.localt,2,0);
	OledDispFixed(14,5,sysState.lochdg,2,0);
}

void aat_refresh_remote(void)
{	
	OledDispFixed(4,8,sysState.rmtlat,7,0);
	OledDispFixed(4,9,sysState.rmtlon,7,0);
	OledDispFixed(4,10,sysState.rmtalt,2,0);
	OledDispFixed(14,10,sysState.rmthdg,2,0);
}

void aat_refresh_state(void)
{
	s32 dist=(s32)(sysState.range*10);
	s32 dir=(s32)(sysState.direction*10);
	s32 yaw=(s32)(sysState.yaw*10);
	s32 pitch=(s32)(sysState.pitch*10);
	OledDispFixed(5,12,dist,1,0);
	OledDispFixed(14,12,dir,1,0);
	OledDispFixed(4,13,yaw,1,0);
	OledDispFixed(14,13,pitch,1,0);
}

void aat_init(u8 f)
{
	if(f)
	{
		aat.state=0;
		return;
	}
	OledClearBuff();
	OledClear(0);
	PagesDrawHeader(AATPage,"Tracker");
	OledDispString(0,2,"Local     ",0);	
	OledDispString(0,3,"Lat:",0);
	OledDispString(0,4,"Lon:",0);
	OledDispString(0,5,"ALT:",0);
	OledDispString(10,5,"HDG:",0);
	OledDispString(0,7,"Remote     ",0);	
	OledDispString(0,8,"Lat:",0);
	OledDispString(0,9,"Lon:",0);
	OledDispString(0,10,"ALT:",0);
	OledDispString(10,10,"HDG:",0);
	OledDispString(0,12,"Dist:",0);
	OledDispString(10,12,"Dir:",0);
	OledDispString(0,13,"Yaw:",0);
	OledDispString(10,13,"Pit:",0);	
	OledDispString(0,15,"SET",0);
	OledDispString(16,15,"SWAP",0);
	//OledRefresh();
}

void aat_update(void)
{	
	u8 key=currKey&(currKey^lastKey);
	switch(aat.state)
	{
		case 0:				
		aat_refresh_local();
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

