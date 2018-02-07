/***************************************************************************************
**文件名:     
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   版本     修改者    时间                     
**						
**备注:       2016/3/23
****************************************************************************************/
#ifndef   __RTC_SD3078_H__
#define   __RTC_SD3078_H__
//#include "AA.h"
#include "h_IIC_Soft.h"

#define RTC_SD3078_WRITE_ADDR (0X64)
#define RTC_SD3078_READ_ADDR (0X65)

#define RTC_SD3078_SECOND               (0x00)
#define RTC_SD3078_MINUTE               (0x01)
#define RTC_SD3078_HOUR                 (0x02)
#define RTC_SD3078_WEEK                 (0x03)
#define RTC_SD3078_DAY                  (0x04)
#define RTC_SD3078_MONTH                (0x05)
#define RTC_SD3078_YEAR                 (0x06)
#define RTC_SD3078_MONTH                (0x05)
#define RTC_SD3078_YEAR                 (0x06)
#define RTC_SD3078_SECOND_ALARM         (0x07)
#define RTC_SD3078_MINUTE_ALARM         (0x08)
#define RTC_SD3078_HOUR_ALARM           (0x09)
#define RTC_SD3078_WEEK_ALARM           (0x0A)
#define RTC_SD3078_DAY_ALARM            (0x0B)
#define RTC_SD3078_MONTH_ALARM          (0x0C)
#define RTC_SD3078_YEAR_ALARM           (0x0D)
#define RTC_SD3078_ALARM_ALLOWED        (0x0E)
#define RTC_SD3078_CTR1                 (0x0F)
#define RTC_SD3078_CTR2                 (0x10)
#define RTC_SD3078_CTR3                 (0x11)
#define RTC_SD3078_25TTF                (0x12)
#define RTC_SD3078_COUNTDOWN1           (0x13)
#define RTC_SD3078_COUNTDOWN2           (0x14)
#define RTC_SD3078_COUNTDOWN4           (0x15)
#define RTC_SD3078_AGTC                 (0x17)
#define RTC_SD3078_CHARGE               (0x18)
#define RTC_SD3078_CTR4                 (0x19)
#define RTC_SD3078_CTR5                 (0x1A)
#define RTC_SD3078_BAT_VAL              (0x1B)
//#define RTC_SD3078_USER_RAM             (0x2C)
#define RTC_SD3078_USER_RAM             (0x30)
#define RTC_SD3078_ID                   (0x72)

typedef	struct
{
  uchar second;
  uchar	minute;
  uchar	hour;
  uchar	week;
  uchar	day;
  uchar	month;
  uchar	year;
}_struRTC_SD3078_Time;	

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef RTC_SD3078_GLOBALS
#define RTC_SD3078_EXT
#else
#define RTC_SD3078_EXT extern
#endif
  RTC_SD3078_EXT _struRTC_SD3078_Time struRTC_SD3078_Time;
  RTC_SD3078_EXT void RTC_SD3078_Init(void);
  RTC_SD3078_EXT void RTC_SD3078_WriteON();
  RTC_SD3078_EXT void RTC_SD3078_WriteOFF();
  RTC_SD3078_EXT void RTC_SD3078_ReadTime(u8 *pPar);
  RTC_SD3078_EXT void RTC_SD3078_SetTime(u8 *pPar);
  RTC_SD3078_EXT void RTC_SD3078_CountDownAlarm(u8 freq,u8 time);
  RTC_SD3078_EXT void RTC_SD3078_ClearCountDownAlarm(void);
  RTC_SD3078_EXT s8 RTC_SD3078_CheckInitFlag();
  RTC_SD3078_EXT s8 RTC_SD3078_WriteInitFlag();
#ifdef  __cplusplus
}
#endif
#endif
/************************************Code End******************************************/