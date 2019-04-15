#ifndef __PARAMETER_H
#define __PARAMETER_H
#include "sys.h"

#define FLASH_SIZE 64
#if FLASH_SIZE<256
  #define SECTOR_SIZE 1024   
#else 
  #define SECTOR_SIZE 2048
#endif

#define TGT_SECTOR_ADDR 0x0000FC00

typedef struct
{
	u16 headFlag;
	float yaw_range;
	u16 pwm_yaw_min;
	u16 pwm_yaw_max;
	u16 pwm_yaw_center;
	u16 pwm_yaw_rev;
	float pitch_range;
	u16 pwm_pitch_min;
	u16 pwm_pitch_max;
	u16 pwm_pitch_rev;
	float min_range;
	s32 aat_loc_lat;
	s32 aat_loc_lon;
	s32 aat_loc_alt;
	s32 aat_loc_hdg;
	u16 tailFlag;
} ParameterSet;

extern ParameterSet params;

u8 ParamRead(void);
u8 ParamWrite(void);
void ParamReset(void);

#endif