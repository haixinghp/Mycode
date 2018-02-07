/***************************************************************************************
**文件名:     
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       
W25Q80将1M的容量分成16块（Block）
每块的大小为64K，每个块又分为16个扇区（Sector）
每个扇区大小为4K，作为最小的擦除单位，也就是每次擦除4k字节。
这样我们需要给W25QXX开辟至少4K的缓冲区，这样对SRAM要求比较高，要求芯片必须有4以上的SRAM才能很好地操作
目前用于测试的芯片为STM8L，RAM大小为4k，所以无法实现字节的读写。所以此文件作为FLASH读写的精简版本。
256Byte一个缓冲区
16个256Byte合成一个4kByte
16个4kByte合成64kByte
**修改记录:   版本     修改者    时间                     
**						
**备注:       2017/9/6
****************************************************************************************/
#ifndef   __W25QXX_H__
#define   __W25QXX_H__
//#include "W25QXX.h"
#include "stm8l15x.h"

#define W25QXX_POWER_ENABLE() 
#define W25QXX_POWER_DISABLE() 

#define W25QXX_CS_ENABLE() (SPI2_FLASH_CS_ENABLE()) //这里的接口为SPI CS脚，移植时注意修改。 
#define W25QXX_CS_DISABLE() (SPI2_FLASH_CS_DISABLE())

/* 将2个字节合并为1个半字，a为低字节，b为高字节 */
#define MAKEWORD(a, b) ((uint16_t)(((uint8_t)(a)) | ((uint16_t)((uint8_t)(b))) << 8))
/* 将2个半字合并为1个字，a为低半字，b为高半字 */
#define MAKELONG(a, b) ((uint32_t)(((uint16_t)(a)) | ((uint32_t)((uint16_t)(b))) << 16))
/* 截取1个字的低半字 */
#define LOWORD(l) ((uint16_t)(l))
/* 截取1个字的高半字 */
#define HIWORD(l) ((uint16_t)(((uint32_t)(l) >> 16) & 0xFFFF))
/* 截取半字的低字节 */
#define LOBYTE(w) ((uint8_t)(w))
/* 截取半字的高字节 */
#define HIBYTE(w) ((uint8_t)(((uint16_t)(w) >> 8) & 0xFF))	

/* 喂狗，定时6s */
//#define WDT_RESET() (IWDG->KR = 0xAA)
#define W25Q80_ID                      (0xEF13)
#define W25QXX_READ                    (0XFF) 
#define W25QXX_READ_STATUS_REGISTER1   (0X05)
#define W25X_WriteEnable		0x06
#define W25X_WriteDisable		0x04
#define W25X_ReadStatusReg		0x05
#define W25X_WriteStatusReg		0x01
#define W25X_ReadData			0x03
#define W25X_FastReadData		0x0B
#define W25X_FastReadDual		0x3B
#define W25X_PageProgram		0x02
#define W25X_BlockErase			0xD8
#define W25X_SectorErase		0x20
#define W25X_ChipErase			0xC7
#define W25X_PowerDown			0xB9
#define W25X_ReleasePowerDown	        0xAB
#define W25X_DeviceID			0xAB
#define W25X_ManufactDeviceID	        0x90
#define W25X_JedecDeviceID		0x9F
#ifdef  __cplusplus
extern "C" {
#endif

#ifdef W25QXX_GLOBALS
#define W25QXX_EXT
#else
#define W25QXX_EXT extern
#endif
W25QXX_EXT u8 W25QXX_Init(); 
W25QXX_EXT u8 W25QXX_Test();
W25QXX_EXT u8 W25QXX_GetStatus(void);
W25QXX_EXT void W25QXX_WaitReady(void);
W25QXX_EXT u16 W25QXX_ReadID(void);
W25QXX_EXT void W25QXX_HighSpeedReadData(uint32_t StartAddress,uint8_t *DataBuffer0 , uint32_t ByteCount);
W25QXX_EXT void W25QXX_ChipSectorErase(uint32_t StartAddress);
W25QXX_EXT void W25QXX_ChipBlockErase(uint32_t StartAddress);
W25QXX_EXT void W25QXX_ChipErase(void);
W25QXX_EXT void W25QXX_PageProgram(uint32_t StartAddress,unsigned char *DataBuffer , uint32_t ByteCount);
W25QXX_EXT int W25QXX_Write_nByte(unsigned int StartAddress, unsigned char *DataBuffer, unsigned int ByteCount);
W25QXX_EXT void W25QXX_Write_NoCheck(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite);
W25QXX_EXT void W25QXX_PowerDown(void);
W25QXX_EXT void W25QXX_WAKEUP(void);
W25QXX_EXT void W25QXX_LWP();
W25QXX_EXT void W25QXX_EXIT_LWP();
#ifdef  __cplusplus
}
#endif
#endif
/************************************Code End******************************************/