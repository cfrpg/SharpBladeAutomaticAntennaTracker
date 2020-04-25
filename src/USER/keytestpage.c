#include "pages.h"
#include "parameter.h"
#include "pwm.h"
#include "hsi.h"
struct
{
	s32 a,b,c,d,e;
} keytest;

void keytest_init(u8 f)
{
	if(f)
	{
		keytest.a=500;
		keytest.b=500;
		keytest.c=500;
		keytest.d=500;
		keytest.e=0;
		return;
	}
	sysState.enable=0;
	OledClear(0);
	OledClearBuff();
	PagesDrawHeader(KeyTestPage,"KeyTest");
	//OledDispInt(5,1,params.pwm_pitch_max,6,0);
	//OledDispInt(5,2,params.pwm_pitch_min,6,0);
	//OledDispInt(5,3,params.pwm_yaw_max,6,0);
	//OledDispInt(5,3,params.pwm_yaw_min,6,0);
//	OledDispBitmap(0,8,128,112,gImage_hsi);
//	OledDispString(0,0, "W3 /4    023   8.2 A",0);
//	OledDispBitmap(0,8,128,112,gImage_adi);
//	OledDispString(0,2, "         ^",0);
//	OledDispString(0,1, "---W----296------|-",0);
//	OledDispString(0,3, "",0);
//	OledDispString(0,4, "     10-----10",0);
//	OledDispString(0,5, "",0);
//	OledDispString(0,6, "      5-----5",0);
//	OledDispString(0,7, "               +0.0",0);
//	OledDispString(0,8, " 0.0>---------<464 B",0);
//	OledDispString(0,9, "     ",0);
//	OledDispString(0,10, "      5-----5",0);
//	OledDispString(0,11,"",0);
//	OledDispString(0,12,"     10-----10",0);
//	OledDispString(0,13,"        0.0",0);

	//OledDispString(0,14,"---              ",0);
//	OledDispString(0,15,"        SUPT",0);
//	OledDispString(0,15,"HSI",1);
//	OledDispString(17,15,"ADI",0);
//	OledRefresh();
}

void keytest_update(void)
{
	u8 t;
	t=currKey&(currKey^lastKey);
	if(t&KEY_UP)
		keytest.a++;
	if(t&KEY_DOWN)
		keytest.a--;
	if(t&KEY_LEFT)
		keytest.b++;
	if(t&KEY_RIGHT)
		keytest.b--;
	if(t&KEY_A)
		keytest.c++;
	if(t&KEY_B)
		keytest.c--;
	if(t&KEY_C)
		keytest.d++;
	if(t&KEY_D)
		keytest.d--;
	if(currWheel>lastWheel)
		keytest.e++;
	if(currWheel<lastWheel)
		keytest.e--;
	
	PWMSet(0,keytest.e);
	OledDispInt(0,1,keytest.a,6,0);
	OledDispInt(0,2,keytest.b,6,0);
	OledDispInt(0,3,keytest.c,6,0);
	OledDispInt(0,4,keytest.e,6,0);	
	
	
	if(t&KEY_D)
		PagesNext(1);
}
