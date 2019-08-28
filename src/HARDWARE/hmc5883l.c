#include "hmc5883l.h"
#include "i2c.h"

void HMC5883Init(void)
{
	HMC5883WriteByte(0x00,0x70);
	HMC5883WriteByte(0x01,0x20);
	HMC5883WriteByte(0x02,0x00);
}

u8 HMC5883ReadByte(u8 addr)
{
	u8 res;
	I2CStart();
	I2CSendByte(HMC5883L_ADDR);
	I2CWaitAck();
	I2CSendByte(addr);
	I2CWaitAck();
	I2CStart();
	I2CSendByte(HMC5883L_ADDR+1);
	I2CWaitAck();
	res=I2CReadByte();
	I2CNAck();
	I2CStop();
	return res;
}

void HMC5883WriteByte(u8 addr,u8 b)
{
	I2CStart();
	I2CSendByte(HMC5883L_ADDR);
	I2CWaitAck();
	I2CSendByte(addr);
	I2CWaitAck();
	I2CSendByte(b);
	I2CWaitAck();
	I2CStop();
}

void HMC5883ReadData(s16* data)
{
	u8 buf[7];
	u8 i;
	I2CStart();
	I2CSendByte(HMC5883L_ADDR);
	I2CWaitAck();
	I2CSendByte(0x03);
	I2CWaitAck();
	I2CStart();
	I2CSendByte(HMC5883L_ADDR+1);
	I2CWaitAck();
	for(i=0;i<6;i++)
	{
		buf[i]=I2CReadByte();
		I2CAck();		
	}
	buf[6]=I2CReadByte();
	I2CNAck();
	I2CStop();
	for(i=0;i<3;i++)
	{
		data[i]=buf[i<<1];
		data[i]<<=8;
		data[i]|=buf[(i<<1)+1];
	}
	
}
