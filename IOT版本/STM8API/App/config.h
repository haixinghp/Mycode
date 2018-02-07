#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************/
/*系统头文件**/
#include "stm8l15x.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*********************************************************************************************/

	#define DEBUG(format, ...) 
//常用变量类型定义
  
/*********************************************************************************************/
/*--底层驱动模块--*/
/*MCU模块*/

//#include "stm8l15x_conf.h"
  

#include "flash.h"
#include "MCU.h"

/*功能模块开关头文件*/
#include "moduleSelect.h"

/*设备系统全局量头文件*/
#include "data.h"

/*API函数头文件*/
#include "api.h"

/*GPIO初始化定义*/
#include "GpioMap.h"	

/*系统定时器模块驱动头文件*/
#include "LowPrcisionTime.h"		

/*WatchDog模块驱动头文件*/
#include "WatchDog.h"

/*IAP模块驱动头文件*/
#include "IAP.h"

/*公共函数头文件*/
#include "basicFunc.h"

/*串口驱动头文件*/
#include "USART.h"

/*LED模块驱动头文件*/
#include "LED.h"

/*flash模块驱动头文件*/
//#include "Spi_Flash.h"

/*协议解析驱动头文件*/
#include "Protocol.h"

/*邮电部协议解析驱动头文件*/
#include "PostProDa.h"

/*邮电部协议数据上报口协议头文件*/
#include "Uploading_Pro.h"

/*IAP程序升级协议头文件*/
#include "DownLoad.h"

/*升级协议函数头文件*/
#include "DownLoad_Pro.h"

/*协议接收回调函数处理*/
#include "ProtocolRecCallBack.h"

/*IAP升级驱动头文件*/
#include "iap_updata.h"

/*网络模块驱动头文件*/
#include "qwl_net.h"

/*NB模块驱动头文件*/
#include "nblot.h"

/*CDMA模块驱动头文件*/
#include "cdma.h"
  
/*G510模块驱动头文件*/
#include "g510.h"  
/*协议主动发送上位机文件*/
#include "Protocol_send.h"
 
  
#include "iic_eep.h"

#include "watchdog.h"
  
#include "Public.h"

#include "W25QXX.h"
  
#include "hardwareSTM8L_SPI.h"
//////////////////////////////////////////////
#include "rtc.h"
#include "delay.h"
#include "lowpower.h"
#include "lowpower_uart.h"
#include "lowpower_spi.h"
#include "exitinterrupt.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h" 
#include "string.h" 

#include "EEprom.h"

  
#include "Radar_And_Ultrasonic.h"
#include "Timer.h"
#include "sys.h"

#include "SA_Protocol.h"
#include "ringbuf.h"
  
#include "xiaofang.h" 

#include "tick.h"
#include <time.h>
#include "ADC.h"
#include "RTC_SD3078.h" 
#include "FLASH_Queue.h"
#include "Pressure.h"
#include "NewInactAngle.h"
#include "Cover.h"
#include "TimeOfAlarm.h"
#include "PressureSys.h"
#include "Interval.h"
#include "TimeOfAlarmPressure.h"

///////////////////////////////////////
/*********************************************************************************************/
#ifdef __cplusplus
} // extern "C"
#endif

#endif

