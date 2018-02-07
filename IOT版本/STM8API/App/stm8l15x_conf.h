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
///*����ģ�鿪��ͷ�ļ�*/
//#include "moduleSelect.h"
//
///*�豸ϵͳȫ����ͷ�ļ�*/
//#include "data.h"
//
///*API����ͷ�ļ�*/
//#include "api.h"
//
///*GPIO��ʼ������*/
//#include "GpioMap.h"	
//
///*ϵͳ��ʱ��ģ������ͷ�ļ�*/
//#include "LowPrcisionTime.h"		
//
///*WatchDogģ������ͷ�ļ�*/
//#include "WatchDog.h"
//
///*IAPģ������ͷ�ļ�*/
//#include "IAP.h"
//
///*��������ͷ�ļ�*/
//#include "basicFunc.h"
//
///*��������ͷ�ļ�*/
//#include "USART.h"
//
///*LEDģ������ͷ�ļ�*/
//#include "LED.h"
//
///*LEDģ������ͷ�ļ�*/
//#include "CESHI.h"
//
///*flashģ������ͷ�ļ�*/
//#include "Spi_Flash.h"
//
///*Э���������ͷ�ļ�*/
//#include "Protocol.h"
//
///*�ʵ粿Э���������ͷ�ļ�*/
//#include "PostProDa.h"
//
///*�ʵ粿Э�������ϱ���Э��ͷ�ļ�*/
//#include "Uploading_Pro.h"
//
///*IAP��������Э��ͷ�ļ�*/
//#include "DownLoad.h"
//
///*����Э�麯��ͷ�ļ�*/
//#include "DownLoad_Pro.h"
//
///*Э����ջص���������*/
//#include "ProtocolRecCallBack.h"
//
///*IAP��������ͷ�ļ�*/
//#include "iap_updata.h"
//
///*����ģ������ͷ�ļ�*/
//#include "qwl_net.h"
//
///*NBģ������ͷ�ļ�*/
//#include "nblot.h"
//
///*CDMAģ������ͷ�ļ�*/
//#include "cdma.h"
//
///*Э������������λ���ļ�*/
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
