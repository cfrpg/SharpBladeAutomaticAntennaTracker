#ifndef __I2C_H
#define __I2C_H
#include "sys.h"

#define I2C_SCL_IN PBin(5)
#define I2C_SCL_OUT PBout(5)
#define I2C_SDA_IN PBin(6)
#define I2C_SDA_OUT PBout(6)

#define I2C_WR 0
#define I2C_RD 1

void I2CInit(void);
void I2CStart(void);
void I2CStop(void);
void I2CSendByte(u8 b);
u8 I2CReadByte(void);
u8 I2CWaitAck(void);
void I2CAck(void);
void I2CNAck(void);
u8 I2CCheckDevice(u8 addr);

#endif