/***************************************************************************************
**文件名:     
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   版本     修改者    时间                     
**						
**备注:       2016/3/23
****************************************************************************************/
#ifndef   __HARDWARESTM8L_SPI_H__
#define   __HARDWARESTM8L_SPI_H__
//#include "hardwareSTM8L_SPI.h"

#include "config.h"
#include "stm8l15x.h"

#define SPI2_CS_GPIO (GPIOG)
#define SPI2_CS_GPIO_PIN (GPIO_Pin_4)
#define SPI2_FLASH_CS_ENABLE() (GPIO_ResetBits(SPI2_CS_GPIO,SPI2_CS_GPIO_PIN)) //拉低片选管脚电平，选中外设  
#define SPI2_FLASH_CS_DISABLE() (GPIO_SetBits(SPI2_CS_GPIO,SPI2_CS_GPIO_PIN)) //拉高片选管脚电平，失去能外设
#ifdef  __cplusplus
extern "C" {
#endif

#ifdef HARDWARESTM8L_SPI_GLOBALS
#define HARDWARESTM8L_SPI_EXT
#else
#define HARDWARESTM8L_SPI_EXT extern
#endif
  HARDWARESTM8L_SPI_EXT void SPI2_Init(void);
  HARDWARESTM8L_SPI_EXT void SPI2_Disable();
  HARDWARESTM8L_SPI_EXT u8 SPI2_SendByte(u8 byte);
//  HARDWARESTM8L_SPI_EXT u16 SPI_FLASH_ReadID(void);
//HARDWARESTM8L_SPI_EXT void YY_Init(void);
//HARDWARESTM8L_SPI_EXT void YY_Service(void);
//HARDWARESTM8L_SPI_EXT void YY_UART_Service(void);
//HARDWARESTM8L_SPI_EXT void YY_Timer_Service(void);
#ifdef  __cplusplus
}
#endif
#endif
/************************************Code End******************************************/