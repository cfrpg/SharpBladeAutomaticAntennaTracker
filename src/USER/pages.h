#ifndef __PAGES_H
#define __PAGES_H
#include "sys.h"
#include "mavlink.h"
#include "oled.h"
#include "keyboard.h"

#define PageNum 4
#define KeyTestPage 4
#define AATPage 0
#define FlightPage 1
#define ServoPage 2
#define GPSPage 3

typedef struct
{
	s32 loclat;
	s32 loclon;
	s32 localt;
	s32 lochdg;
	s32 rmtlat;
	s32 rmtlon;
	s32 rmtalt;
	s32 rmthdg;
	float yaw;
	float pitch;
	double range;
	double direction;
	s16 magdata[3];
	double azimuth;
	u8 enable;
} systemState;

extern systemState sysState;
extern u8 lastKey;
extern u8 currKey;
extern s32 lastWheel;
extern s32 currWheel;

extern u8 currpage;

extern Mav_Attitude attpkg;

void PagesInit(void);
void PagesChangeTo(u8 p);
void PagesNext(s8 d);
void PagesUpdate(void);
void PagesDrawHeader(u8 n,u8 *name);

void aat_update(void);
void aat_init(u8 f);

void keytest_update(void);
void keytest_init(u8 f);

void flight_update(void);
void flight_init(u8 f);

void servo_init(u8 f);
void servo_update(void);

void gps_init(u8 f);
void gps_update(void);


#endif