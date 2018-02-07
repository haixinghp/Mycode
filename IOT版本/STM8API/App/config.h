#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************/
/*ϵͳͷ�ļ�**/
#include "stm8l15x.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*********************************************************************************************/

	#define DEBUG(format, ...) 
//���ñ������Ͷ���
  
/*********************************************************************************************/
/*--�ײ�����ģ��--*/
/*MCUģ��*/

//#include "stm8l15x_conf.h"
  

#include "flash.h"
#include "MCU.h"

/*����ģ�鿪��ͷ�ļ�*/
#include "moduleSelect.h"

/*�豸ϵͳȫ����ͷ�ļ�*/
#include "data.h"

/*API����ͷ�ļ�*/
#include "api.h"

/*GPIO��ʼ������*/
#include "GpioMap.h"	

/*ϵͳ��ʱ��ģ������ͷ�ļ�*/
#include "LowPrcisionTime.h"		

/*WatchDogģ������ͷ�ļ�*/
#include "WatchDog.h"

/*IAPģ������ͷ�ļ�*/
#include "IAP.h"

/*��������ͷ�ļ�*/
#include "basicFunc.h"

/*��������ͷ�ļ�*/
#include "USART.h"

/*LEDģ������ͷ�ļ�*/
#include "LED.h"

/*flashģ������ͷ�ļ�*/
//#include "Spi_Flash.h"

/*Э���������ͷ�ļ�*/
#include "Protocol.h"

/*�ʵ粿Э���������ͷ�ļ�*/
#include "PostProDa.h"

/*�ʵ粿Э�������ϱ���Э��ͷ�ļ�*/
#include "Uploading_Pro.h"

/*IAP��������Э��ͷ�ļ�*/
#include "DownLoad.h"

/*����Э�麯��ͷ�ļ�*/
#include "DownLoad_Pro.h"

/*Э����ջص���������*/
#include "ProtocolRecCallBack.h"

/*IAP��������ͷ�ļ�*/
#include "iap_updata.h"

/*����ģ������ͷ�ļ�*/
#include "qwl_net.h"

/*NBģ������ͷ�ļ�*/
#include "nblot.h"

/*CDMAģ������ͷ�ļ�*/
#include "cdma.h"
  
/*G510ģ������ͷ�ļ�*/
#include "g510.h"  
/*Э������������λ���ļ�*/
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

