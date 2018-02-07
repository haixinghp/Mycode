/**
  ******************************************************************************
  * @file   
  * @author  xiao bo ce kong
  * @version V1.2.0
  * @date    09/2012
  * @brief   http://shop36885018.taobao.com
             QQ:503769032
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8L15x_CONF_H
#define __STM8L15x_CONF_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*********************************************************************************************/

	#define DEBUG(format, ...) 
/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
/*#include "icc_measure.h"*/
/*#include "clk.h"*/
//#include "rtc.h"
//#include "delay.h"
//#include "watchdog.h"
//#include "lowpower.h"
//#include "lowpower_uart.h"
//#include "lowpower_spi.h"
////#include "exitinterrupt.h"
//#include "stdio.h"
//#include "stdlib.h"
//#include "math.h" 
//#include "string.h" 
//
//#include "EEprom.h"
//
//#include "Radar_And_Ultrasonic.h"
//#include "Timer.h"
//#include "IOTMode.h"
//#include "sys.h"
//
//#include "ringbuf.h"
//#include "global_config.h"
//#include "Protocol.h"
/////////////////////////////////////////
//
//#include "MCU.h"
//
///*功能模块开关头文件*/
//#include "moduleSelect.h"
//
///*设备系统全局量头文件*/
//#include "data.h"
//
///*API函数头文件*/
//#include "api.h"
//
///*GPIO初始化定义*/
//#include "GpioMap.h"	
//
///*系统定时器模块驱动头文件*/
//#include "LowPrcisionTime.h"		
//
///*WatchDog模块驱动头文件*/
//#include "WatchDog.h"
//
///*IAP模块驱动头文件*/
//#include "IAP.h"
//
///*公共函数头文件*/
//#include "basicFunc.h"
//
///*串口驱动头文件*/
//#include "USART.h"
//
///*LED模块驱动头文件*/
//#include "LED.h"
//
///*LED模块驱动头文件*/
//#include "CESHI.h"
//
///*flash模块驱动头文件*/
//#include "Spi_Flash.h"
//
///*协议解析驱动头文件*/
//#include "Protocol.h"
//
///*邮电部协议解析驱动头文件*/
//#include "PostProDa.h"
//
///*邮电部协议数据上报口协议头文件*/
//#include "Uploading_Pro.h"
//
///*IAP程序升级协议头文件*/
//#include "DownLoad.h"
//
///*升级协议函数头文件*/
//#include "DownLoad_Pro.h"
//
///*协议接收回调函数处理*/
//#include "ProtocolRecCallBack.h"
//
///*IAP升级驱动头文件*/
//#include "iap_updata.h"
//
///*网络模块驱动头文件*/
//#include "qwl_net.h"
//
///*NB模块驱动头文件*/
//#include "nblot.h"
//
///*CDMA模块驱动头文件*/
//#include "cdma.h"
//
///*协议主动发送上位机文件*/
//#include "Protocol_send.h"

/* Uncomment the line below to enable peripheral header file inclusion */
 //#include "stm8l15x_adc.h" 
/* #include "stm8l15x_beep.h" */
 #include "stm8l15x_clk.h" 
/* #include "stm8l15x_comp.h" */
/* #include "stm8l15x_dac.h" */
 #include "stm8l15x_dma.h" 
 #include "stm8l15x_exti.h"
 #include "stm8l15x_flash.h"
 #include "stm8l15x_gpio.h"

/* #include "stm8l15x_i2c.h" */
/* #include "stm8l15x_irtim.h" */
 #include "stm8l15x_itc.h" 
 #include "stm8l15x_iwdg.h" 
 /*#include "stm8l15x_lcd.h" */
 /*#include "stm8l15x_pwr.h" */
/* #include "stm8l15x_rst.h" */
 #include "stm8l15x_rtc.h" 
 #include "stm8l15x_spi.h" 
 #include "stm8l15x_syscfg.h" 
 #include "stm8l15x_tim1.h"
/* #include "stm8l15x_tim2.h" */
/* #include "stm8l15x_tim3.h" */
 #include "stm8l15x_tim4.h" 
 #include "stm8l15x_usart.h"
/* #include "stm8l15x_wfe.h" */
 #include "stm8l15x_wwdg.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    (1) */ 

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed. 
  *   If expr is true, it returns no value.
  * @retval : None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __STM8L15x_CONF_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
