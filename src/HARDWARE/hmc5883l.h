#ifndef __HMC5883L_H
#define __HMC5883L_H
#include "sys.h"

#define HMC5883L_ADDR 0x3C

void HMC5883Init(void);
u8 HMC5883ReadByte(u8 addr);
void HMC5883WriteByte(u8 addr,u8 b);
void HMC5883ReadData(s16* data);

#endif