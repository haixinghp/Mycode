/***************************************************************************************
**�ļ���:     
**����:       Haixing.Huang
**�汾:       V1.00
**˵��:       
W25Q80��1M�������ֳ�16�飨Block��
ÿ��Ĵ�СΪ64K��ÿ�����ַ�Ϊ16��������Sector��
ÿ��������СΪ4K����Ϊ��С�Ĳ�����λ��Ҳ����ÿ�β���4k�ֽڡ�
����������Ҫ��W25QXX��������4K�Ļ�������������SRAMҪ��Ƚϸߣ�Ҫ��оƬ������4���ϵ�SRAM���ܺܺõز���
Ŀǰ���ڲ��Ե�оƬΪSTM8L��RAM��СΪ4k�������޷�ʵ���ֽڵĶ�д�����Դ��ļ���ΪFLASH��д�ľ���汾��
256Byteһ��������
16��256Byte�ϳ�һ��4kByte
16��4kByte�ϳ�64kByte
**�޸ļ�¼:   �汾     �޸���    ʱ��                     
**						
**��ע:       2017/9/6
****************************************************************************************/
#ifndef   __W25QXX_H__
#define   __W25QXX_H__
//#include "W25QXX.h"
#include "stm8l15x.h"

#define W25QXX_POWER_ENABLE() 
#define W25QXX_POWER_DISABLE() 

#define W25QXX_CS_ENABLE() (SPI2_FLASH_CS_ENABLE()) //����Ľӿ�ΪSPI CS�ţ���ֲʱע���޸ġ� 
#define W25QXX_CS_DISABLE() (SPI2_FLASH_CS_DISABLE())

/* ��2���ֽںϲ�Ϊ1�����֣�aΪ���ֽڣ�bΪ���ֽ� */
#define MAKEWORD(a, b) ((uint16_t)(((uint8_t)(a)) | ((uint16_t)((uint8_t)(b))) << 8))
/* ��2�����ֺϲ�Ϊ1���֣�aΪ�Ͱ��֣�bΪ�߰��� */
#define MAKELONG(a, b) ((uint32_t)(((uint16_t)(a)) | ((uint32_t)((uint16_t)(b))) << 16))
/* ��ȡ1���ֵĵͰ��� */
#define LOWORD(l) ((uint16_t)(l))
/* ��ȡ1���ֵĸ߰��� */
#define HIWORD(l) ((uint16_t)(((uint32_t)(l) >> 16) & 0xFFFF))
/* ��ȡ���ֵĵ��ֽ� */
#define LOBYTE(w) ((uint8_t)(w))
/* ��ȡ���ֵĸ��ֽ� */
#define HIBYTE(w) ((uint8_t)(((uint16_t)(w) >> 8) & 0xFF))	

/* ι������ʱ6s */
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