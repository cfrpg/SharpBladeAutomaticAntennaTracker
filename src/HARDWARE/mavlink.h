#ifndef __MAVLINK_H
#define __MAVLINK_H

#include "sys.h" 

extern u8 mavlinkBuff[2][263];
extern u8 mavlinkCurr;
extern s16 mavlinkLen[2];
extern u8 mavlinkReady;

typedef struct
{
	u32 time;
	float roll;
	float pitch;
	float yaw;
	float rollspeed;
	float pitchspeed;
	float yawspeed;
} Mav_Attitude;

typedef struct
{
	u32 time;
	s32 lat;
	s32 lon;
	s32 alt;
	s32 relalt;
	s16 vx;
	s16 vy;
	s16 vz;
	u16 hdg;
} Mav_GolbalPositionInt;
 
void MavlinkInit(u32 baud);
//u8 MAVlinkPrase(u8 *data);


#endif