/***************************************************************************************
**文件名:     
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       
这个程序用于控制设备的告警间隔。控制的时基使用RTC时间。
具体配置查询 void RTC_Config(void)
RTC_InitStr.RTC_AsynchPrediv = 0x00;   
RTC_InitStr.RTC_SynchPrediv = 51;  //1s/10的的频率为1/10HZ。
RTC_InitStr.RTC_AsynchPrediv = 0x00;   
RTC_InitStr.RTC_SynchPrediv = 512;  //1s的的频率为1HZ。 
**修改记录:   版本     修改者    时间                     
**						
**备注:       2017-12-25 
****************************************************************************************/
#ifndef _TIMEOFALARM_PRESSURE_H_
#define _TIMEOFALARM_PRESSURE_H_
#include "config.h"
//#include "stm8l15x.h"
//#define ALARMTIME_PRESSURE_EN 1
#if ALARMTIME_PRESSURE_EN
  typedef struct _ALARM_TIME_
  {
    _Bool flag;
    u32 minute;  //这里的单位为秒。
  }ALARM_TIME;

  typedef struct _ALARMTIME_ALL_
  {
    ALARM_TIME PressureUpLimit;
    ALARM_TIME PressureDownLimit;
    ALARM_TIME NoSensor;
    ALARM_TIME ResolutionAlarm;
    ALARM_TIME TouchOff;
  }ALARMTIME_ALL;

  extern ALARMTIME_ALL alarmtime_all;
  extern u8 en_alarmtime;


//extern  u16 Get_RTCTime(void);
extern  u32 Get_RTCTime(void); 
extern  void init_all_alarmtime(void);
extern  void Reset_alarmtime(ALARM_TIME *p_alarmtime,u32 set_time);
extern  void Refresh_alarmtime(ALARM_TIME *p_alarmtime);
extern  void Refresh_all_alarmtime();
extern  void Enable_alarmtime(ALARM_TIME *p_alarmtime);
extern  void Disable_alarmtime(ALARM_TIME *p_alarmtime);
extern  _Bool Is_flag_alarmtime(ALARM_TIME *p_alarmtime);
extern ALARM_TIME *getAlarmTimePressureUpLimit();
extern ALARM_TIME *getAlarmTimePressureDownLimit();
extern ALARM_TIME *getAlarmTimeNoSensor();
extern ALARM_TIME *getAlarmTimeResolutionAlarm();
extern ALARM_TIME *getAlarmTimeTouchOff();
extern s8 ShowAlarmTimer();

#endif
#endif
