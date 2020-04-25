#include "pages.h"
#include "parameter.h"
#include "stdio.h"
struct
{
	u8 state;
	u16 datacnt;
	s16 xmax;
	s16 xmin;
	s16 ymax;
	s16 ymin;
	s32 xsum;
	s32 ysum;
	double azimuthsum;
} gpsstate;

void gps_init(u8 f)
{
	if(f)
	{
		gpsstate.state=0;
		return;
	}
	gpsstate.state=0;
	OledClear(0);
	PagesDrawHeader(GPSPage,"GPS");
	OledDispString(0,15,"CALI",0);
}

void gps_update(void)
{
	u8 key;
	key=currKey&(currKey^lastKey);
	switch(gpsstate.state)
	{
		case 0:
			if(key&KEY_A)
			{
				gpsstate.state=1;
				gpsstate.datacnt=0;
				gpsstate.xmax=-9999;
				gpsstate.xmin=9999;
				gpsstate.ymax=-9999;
				gpsstate.ymin=9999;
				gpsstate.xsum=0;
				gpsstate.ysum=0;
				OledClear(0);
				params.mag_x_offset=0;
				params.mag_x_scale=1;
				params.mag_y_offset=0;
				params.mag_y_scale=1;
				sysState.enable=0;
				PagesDrawHeader(GPSPage,"GPS");	
				OledDispString(0,2,"Cali start",0);
				OledDispString(0,3,"MAG Cali",0);
				OledDispString(0,4,"Rotate",0);
				OledDispString(0,5,"Data:   /200",0);
			}
			if(currWheel>lastWheel)
				PagesNext(1);
			if(currWheel<lastWheel)
				PagesNext(-1);	
		break;
		case 1:
			if(sysState.magdata[0]>gpsstate.xmax)
				gpsstate.xmax=sysState.magdata[0];
			if(sysState.magdata[0]<gpsstate.xmin)
				gpsstate.xmin=sysState.magdata[0];
			if(sysState.magdata[2]>gpsstate.ymax)
				gpsstate.ymax=sysState.magdata[2];
			if(sysState.magdata[2]<gpsstate.ymin)
				gpsstate.ymin=sysState.magdata[2];
			gpsstate.xsum+=sysState.magdata[0];
			gpsstate.ysum+=sysState.magdata[2];
			gpsstate.datacnt++;
			OledDispInt(5,5,gpsstate.datacnt,3,0);
			if(gpsstate.datacnt>=200)
			{				
				params.mag_x_offset=(gpsstate.xmax+gpsstate.xmin)/2;
				params.mag_y_offset=(gpsstate.ymax+gpsstate.ymin)/2;
				if(gpsstate.xmax-gpsstate.xmin>gpsstate.ymax-gpsstate.ymin)
				{
					params.mag_y_scale=1;
					params.mag_x_scale=(double)(gpsstate.xmax-gpsstate.xmin)/(gpsstate.ymax-gpsstate.ymin);
				}
				else
				{
					params.mag_x_scale=1;
					params.mag_y_scale=(double)(gpsstate.ymax-gpsstate.ymin)/(gpsstate.xmax-gpsstate.xmin);
				}
				printf("MAG cali\r\n");
				printf("x off:%d\tx scl:%f\r\n",params.mag_x_offset,params.mag_x_scale);
				printf("y off:%d\ty scl:%f\r\n",params.mag_y_offset,params.mag_y_scale);
				gpsstate.state=2;
				OledDispString(0,6,"North Cali",0);
				OledDispString(0,15,"CALI",0);
			}		
		break;
		case 2:
			if(key&KEY_A)
			{
				gpsstate.state=3;
				gpsstate.datacnt=0;
				gpsstate.azimuthsum=0;
				params.mag_azi_offset=0;
				OledDispString(0,15,"    ",0);
				OledDispString(0,7,"Data:   /200",0);
			}
		break;
		case 3:
			gpsstate.azimuthsum+=sysState.azimuth;
			gpsstate.datacnt++;
			OledDispInt(5,7,gpsstate.datacnt,3,0);
			if(gpsstate.datacnt>=200)
			{
				OledDispString(0,7,"Cali complete",0);
				OledDispString(0,15,"OK",0);
				params.mag_azi_offset=gpsstate.azimuthsum/gpsstate.datacnt;
				printf("North cali\r\n");
				printf("offset:%f\r\n",params.mag_azi_offset);
				ParamWrite();
				gpsstate.state=4;				
			}
		break;
		case 4:
			if(key&KEY_A)
			{
				gps_init(0);
			}
		break;
			
	}
	
}