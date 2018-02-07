/***************************************************************************************
**�ļ���:     
**����:       Haixing.Huang
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   �汾     �޸���    ʱ��                     
**						
**��ע:       2016/3/23
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
