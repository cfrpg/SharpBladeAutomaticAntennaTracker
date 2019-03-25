#include "pages.h"
#include "parameter.h"
#include "pwm.h"

struct
{
	s16 yawRng;
	s16 pitRng;
	u8 cursorx;
	s8 cursory;
	u8 state;
	
} serset;

void servo_refershall(void)
{
	OledDispString(0,2,"Min:       |      ",0);
	OledDispString(0,3,"Cen:       |      ",0);
	OledDispString(0,4,"Max:       |      ",0);	
	OledDispString(0,5,"Rev:       |      ",0);
	OledDispString(0,6,"Rng:       |      ",0);
	OledDispInt(5,2,params.pwm_yaw_min,0);
	OledDispInt(5,3,params.pwm_yaw_center,0);
	OledDispInt(5,4,params.pwm_yaw_max,0);
	OledDispInt(5,5,params.pwm_yaw_rev,0);
	OledDispInt(5,6,serset.yawRng,0);
	
	OledDispInt(13,2,params.pwm_pitch_min,0);
	OledDispString(14,3,"N/A",0);
	OledDispInt(13,4,params.pwm_pitch_max,0);
	OledDispInt(13,5,params.pwm_pitch_rev,0);
	OledDispInt(13,6,serset.pitRng,0);
}

void servo_init(u8 f)
{
	if(f)
	{
		serset.cursorx=0;
		serset.cursory=0;
		serset.state=0;	
		return;		
	}
	serset.yawRng=(s16)params.yaw_range;
	serset.pitRng=(s16)params.pitch_range;
	OledClear(0);
	PagesDrawHeader(ServoPage,"Servo Cfg.");
	OledDispString(0,1,"     Yaw     Pitch",0);
	
	OledDispString(0,7,"SET  ",0);
	servo_refershall();	
}

u16 servo_clampu16(u16 v,u16 u,u16 l)
{
	if(v>u)
		return u;
	if(v<l)
		return l;
	return v;
}


void servo_update(void)
{
	u8 key;
	s8 t;
	u8 flag;
	key=currKey&(currKey^lastKey);
	switch(serset.state)
	{
		case 0:
		if(key&KEY_A)
		{
			serset.state=1;
			OledDispString(0,7,"SAVE",0);		
		}
		if(currWheel>lastWheel)
			PagesNext(1);
		if(currWheel<lastWheel)
			PagesNext(-1);
		break;
		case 1:
		if(key&KEY_A)
		{
			params.pitch_range=(float)serset.pitRng;
			params.yaw_range=(float)serset.yawRng;			
			ParamWrite();			
			serset.state=0;
			OledDispString(0,7,"SET  ",0);
			servo_refershall();
			break;
		}
		t=0;
		if(currWheel>lastWheel)
			t=5;
		if(currWheel<lastWheel)
			t=-5;
		flag=0;
		if(key&KEY_DOWN)
		{
			serset.cursory++;
			flag=1;
		}
		if(key&KEY_UP)
		{
			serset.cursory--;
			flag=1;
		}
		if(key&(KEY_LEFT|KEY_RIGHT))
		{
			serset.cursorx^=1;
			flag=1;
		}
		if(serset.cursory<0)
			serset.cursory=0;
		if(serset.cursory>4)
			serset.cursory=4;
		if(flag)
			servo_refershall();
		OledDispString((serset.cursorx<<3)+5,serset.cursory+2,"    ",1);
		if(serset.cursorx==0)
		{
			if(serset.cursory==0)
			{
				params.pwm_yaw_min=servo_clampu16(params.pwm_yaw_min+t,params.pwm_yaw_center-1,800);
				OledDispInt(5,2,params.pwm_yaw_min,1);
				PWMSet(-params.yaw_range,0);
			}
			if(serset.cursory==1)
			{
				params.pwm_yaw_center=servo_clampu16(params.pwm_yaw_center+t,params.pwm_yaw_max-1,params.pwm_yaw_min+1);
				OledDispInt(5,3,params.pwm_yaw_center,1);
				PWMSet(0,0);				
			}
			if(serset.cursory==2)
			{
				params.pwm_yaw_max=servo_clampu16(params.pwm_yaw_max+t,2200,params.pwm_yaw_center+1);
				OledDispInt(5,4,params.pwm_yaw_max,1);
				PWMSet(params.yaw_range,0);
			}
			if(serset.cursory==3)
			{
				params.pwm_yaw_rev=servo_clampu16(params.pwm_yaw_rev+t/5,1,0);
				OledDispInt(5,5,params.pwm_yaw_rev,1);
				PWMSet(0,0);
			}
			if(serset.cursory==4)
			{
				serset.yawRng=servo_clampu16(serset.yawRng+t,180,10);
				OledDispInt(5,6,serset.yawRng,1);
				PWMSet(0,0);
			}
		}
		else
		{
			if(serset.cursory==0)
			{
				params.pwm_pitch_min=servo_clampu16(params.pwm_pitch_min+t,params.pwm_pitch_max-1,800);
				OledDispInt(13,2,params.pwm_pitch_min,1);
				PWMSet(0,0);
			}
			if(serset.cursory==1)
			{
				OledDispString(14,3,"N/A",1);
				PWMSet(0,0);				
			}
			if(serset.cursory==2)
			{
				params.pwm_pitch_max=servo_clampu16(params.pwm_pitch_max+t,2200,params.pwm_pitch_min+1);
				OledDispInt(13,4,params.pwm_pitch_max,1);
				PWMSet(0,params.pitch_range);
			}
			if(serset.cursory==3)
			{
				params.pwm_pitch_rev=servo_clampu16(params.pwm_pitch_rev+t/5,1,0);
				OledDispInt(13,5,params.pwm_pitch_rev,1);
				PWMSet(0,0);
			}
			if(serset.cursory==4)
			{
				serset.pitRng=servo_clampu16(serset.pitRng+t,180,10);
				OledDispInt(13,6,serset.pitRng,1);
				PWMSet(0,0);
			}
		}
		break;
	}
}
