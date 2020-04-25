#include "parameter.h"
#include "mb85rs.h"

ParameterSet params;

u16 readHalfWord(u32 addr)
{
	return *(__IO uint16_t*)addr;
}


u8 ParamRead(void)
{
//	u16 *data=(u16*)(&params);
//	u16 i;
//	for(i=0;i<sizeof(params)/2;i++)
//	{
//		data[i]=readHalfWord(FLASH_BASE+TGT_SECTOR_ADDR+i*2);
//	}
	FRAMRead(0,sizeof(params),(u8*)(&params));
	return 0;
}


u8 ParamWrite(void)
{
//	u16 *data;
//	u16 i;
//	FLASH_Unlock();
//	u32 sectorAddr=FLASH_BASE+TGT_SECTOR_ADDR;
//	FLASH_ErasePage(sectorAddr);
//	data=(u16*)(&params);
//	for(i=0;i<sizeof(params)/2;i++)
//	{
//		FLASH_ProgramHalfWord(sectorAddr+i*2,data[i]);
//	}
//	FLASH_Lock();
	FRAMWrite(0,sizeof(params),(u8*)(&params));
	return 0;
}

void ParamReset(void)
{
	params.yaw_range=80;
	params.pwm_yaw_rev=1;
	params.pwm_yaw_min=845;
	params.pwm_yaw_max=2290;
	params.pwm_yaw_center=1555;
	params.pitch_range=90;
	params.pwm_pitch_rev=1;
	params.pwm_pitch_min=930;
	params.pwm_pitch_max=1840;
	params.min_range=10;
	params.aat_loc_lat=0;
	params.aat_loc_lon=0;
	params.aat_loc_hdg=0;
	params.aat_loc_alt=0;
	params.mag_x_offset=0;
	params.mag_x_scale=1;
	params.mag_y_offset=0;
	params.mag_y_scale=1;
	params.mag_azi_offset=0;
	params.headFlag=0xCFCF;
	params.tailFlag=0xFCFC;
}