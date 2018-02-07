
#ifndef _TIMEOFALARM_H_
#define _TIMEOFALARM_H_
#include "config.h"
//#include "stm8l15x.h"
//#define ALARMTIME_EN 1
#if ALARMTIME_EN
  typedef struct _ALARM_TIME_
  {
    _Bool flag;
    u16 minute;
  }ALARM_TIME;

  typedef struct _ALARMTIME_ALL_
  {
    ALARM_TIME waterover;
    ALARM_TIME opencover;
    ALARM_TIME tipcover;
  }ALARMTIME_ALL;

  extern ALARMTIME_ALL alarmtime_all;
  extern u8 en_alarmtime;


extern  u16 Get_RTCTime(void);
extern  void init_all_alarmtime(void);
extern  void Reset_alarmtime(ALARM_TIME *p_alarmtime,u16 set_time);
extern  void Refresh_alarmtime(ALARM_TIME *p_alarmtime);
extern  void Refresh_all_alarmtime();
extern  void Enable_alarmtime(ALARM_TIME *p_alarmtime);
extern  void Disable_alarmtime(ALARM_TIME *p_alarmtime);
extern  _Bool Is_flag_alarmtime(ALARM_TIME *p_alarmtime);
  #endif
#endif
