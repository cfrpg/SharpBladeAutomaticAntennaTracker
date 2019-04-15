#include "pages.h"
#include "parameter.h"

struct
{
	u8 state;
} aat;

void aat_refresh_local(void)
{		
	OledDispFixed(4,3,sysState.loclat,7,12,0);
	OledDispFixed(4,4,sysState.loclon,7,12,0);
	OledDispFixed(4,5,sysState.localt,1,6,0);
	OledDispFixed(14,5,sysState.lochdg,2,6,0);
}

void aat_refresh_remote(void)
{		
	OledDispFixed(4,8,sysState.rmtlat,7,12,0);
	OledDispFixed(4,9,sysState.rmtlon,7,12,0);
	OledDispFixed(4,10,sysState.rmtalt,1,6,0);
	OledDispFixed(14,10,sysState.rmthdg,2,6,0);
}

void aat_refresh_state(void)
{
	
	s32 dist=(s32)(sysState.range*10);
	s32 dir=(s32)(sysState.direction*10);
	s32 yaw=(s32)(sysState.yaw*10);
	s32 pitch=(s32)(sysState.pitch*10);	
	OledDispFixed(4,12,dist,1,6,0);
	OledDispFixed(14,12,dir,1,6,0);
	OledDispFixed(4,13,yaw,1,6,0);
	OledDispFixed(14,13,pitch,1,6,0);
}

void aat_init(u8 f)
{
	if(f)
	{
		aat.state=0;
		sysState.localt=params.aat_loc_alt;
		sysState.loclon=params.aat_loc_lon;
		sysState.lochdg=params.aat_loc_hdg;
		sysState.loclat=params.aat_loc_lat;	
		printf("LOAD %d %d\r\n",params.aat_loc_lat,sysState.loclat);		
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
	OledDispString(0,12,"Rng:",0);
	OledDispString(10,12,"Dir:",0);
	OledDispString(0,13,"Yaw:",0);
	OledDispString(10,13,"Pit:",0);	
	OledDispString(0,15,"SET",0);
	OledDispString(8,15,"SAVE",0);
	OledDispString(17,15,"CLR",0);
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
	//OledRefresh();
	if(key&KEY_A)	//SET
	{
		printf("SET %d %d\r\n",params.aat_loc_lat,sysState.loclat);
		sysState.localt=sysState.rmtalt;
		sysState.loclon=sysState.rmtlon;
		sysState.lochdg=sysState.rmthdg;
		sysState.loclat=sysState.rmtlat;
	}
	if(key&KEY_B)	//SAVE
	{
		printf("SAVE %d %d\r\n",params.aat_loc_lat,sysState.loclat);
		params.aat_loc_alt=sysState.localt;
		params.aat_loc_hdg=sysState.lochdg;
		params.aat_loc_lat=sysState.loclat;
		params.aat_loc_lon=sysState.loclon;
		ParamWrite();
	}
	if(key&KEY_C)	//CLR
	{
		printf("CLR %d %d\r\n",params.aat_loc_lat,sysState.loclat);
		sysState.localt=0;
		sysState.loclon=0;
		sysState.lochdg=0;
		sysState.loclat=0;
		params.aat_loc_alt=0;
		params.aat_loc_hdg=0;
		params.aat_loc_lat=0;
		params.aat_loc_lon=0;
		ParamWrite();
	}
	if(key&KEY_D)
		sysState.enable^=1;
	aat_refresh_state();
	
	if(currWheel>lastWheel)
		PagesNext(1);
	if(currWheel<lastWheel)
		PagesNext(-1);
}

