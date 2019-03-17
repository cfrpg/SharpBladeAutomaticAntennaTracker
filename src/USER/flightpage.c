#include "pages.h"

void flight_init(u8 f)
{
	OledClear(0);
	PagesDrawHeader(FlightPage,"Flight");
	OledDispString(0,2,"Roll:",0);
	OledDispString(0,3,"Pitch:",0);
	OledDispString(0,4,"Yaw:",0);
}

void flight_update(void)
{
	OledDispFixed(7,2,(s32)(attpkg.roll*5730000),5,0);
	OledDispFixed(7,3,(s32)(attpkg.pitch*5730000),5,0);
	OledDispFixed(7,4,(s32)(attpkg.yaw*5730000),5,0);
	if(currWheel>lastWheel)
		PagesNext(1);
	if(currWheel<lastWheel)
		PagesNext(-1);	
}
