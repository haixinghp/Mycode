/***************************************************************************************
**文件名:     
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   版本     修改者    时间                     
**						
**备注:       2016/3/23
****************************************************************************************/
#ifndef   __TICK_H__
#define   __TICK_H__
//#include "tick.h"
#include "stm8l15x.h"


#ifdef  __cplusplus
extern "C" {
#endif

#ifdef TICK_GLOBALS
#define TICK_EXT
#else
#define TICK_EXT extern
#endif
  TICK_EXT u8 HexToBCD(u8 par);
  TICK_EXT u8 BCDToHex(u8 par);
  TICK_EXT u8 TickGetTime(int32_t tick);
  TICK_EXT int32_t TimeGetTick(bool showtime);
  
//TICK_EXT void YY_Init(void);
//TICK_EXT void YY_Service(void);
//TICK_EXT void YY_UART_Service(void);
//TICK_EXT void YY_Timer_Service(void);
#ifdef  __cplusplus
}
#endif
#endif
/************************************Code End******************************************/
