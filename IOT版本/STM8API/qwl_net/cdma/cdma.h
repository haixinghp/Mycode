
#ifndef _cdma_H
#define _cdma_H
#include "stm8l15x.h"

void cdma_init(void);
u8 cdma_send(u8 *p,u16 len);
void cdma_reconnect(void);
u16 strsearch_h(u8 *ptr2,u8 *ptr1_at);//查字符串*ptr2在*ptr1中的位置
void cdma_open_GPRS(void);
void CDMA_IO_Init(void);
void cdma_init_0(void);
void clear_SystemBuf(void);
void IOTMode_RESET(void);
void Start_GSM(void);	

extern u8 light_TCP[22];
extern u8 SIM_ID[20];
extern u8 g_flag;//G网标志
extern u8 cdma_RESET_flag;
#define CDMA_RESET GPIO_Pin_2	 	//管脚宏定义PE13
#define CDMA_ON_OFF GPIO_Pin_3	 	//管脚宏定义PE13
#define CDMA_IO GPIOG	 	//管脚宏定义

#endif

