#include "TimeOfAlarm.h"


#if ALARMTIME_EN
ALARMTIME_ALL alarmtime_all;
u8 en_alarmtime = 0;

// **************************************************************
// 函数:Get_RTCTime
// 参数:
// 返回:返回RTC累积时间（单位：分钟）
// 描述:得到RTC累积时间（单位：分钟）
// **************************************************************
u16 Get_RTCTime(void)
{
  RTC_TimeTypeDef get_rtctime;
  u16 temp_minute = 0;
  RTC_GetTime(RTC_Format_BIN,&get_rtctime);  
  temp_minute += get_rtctime.RTC_Seconds;     //rtc时钟分频64，即当前rtc 1s为0.93分钟
  temp_minute += get_rtctime.RTC_Minutes * 60;
  temp_minute += get_rtctime.RTC_Hours * 3600;
  return temp_minute;
}
// **************************************************************
// 函数:clear_RTCTime
// 参数:无
// 返回:布尔值——1可以告警  0不能告警
// 描述:刷新对应告警间隔时间，并查询告警标志
// **************************************************************
void clear_RTCTime(void)
{
  RTC_TimeTypeDef get_rtctime;
  RTC_TimeStructInit(&get_rtctime);
  RTC_SetTime(RTC_Format_BIN,&get_rtctime);
//  PrintString_("RTC时间清零");
}
// **************************************************************
// 函数:init_all_alarmtime
// 参数:无
// 返回:无
// 描述:全部告警间隔初始化
// **************************************************************
void init_all_alarmtime(void)
{
  alarmtime_all.opencover.flag = 1;
  alarmtime_all.opencover.minute = 0;
  alarmtime_all.tipcover.flag = 1;
  alarmtime_all.tipcover.minute = 0;
  alarmtime_all.waterover.flag = 1;
  alarmtime_all.waterover.minute = 0;
}
// **************************************************************
// 函数:Reset_alarmtime
// 参数:对应告警时间和标志
// 返回:无
// 描述:重置告警间隔，用于打包对应告警标志时候
// **************************************************************
void Reset_alarmtime(ALARM_TIME *p_alarmtime,u16 set_time)
{
  p_alarmtime->flag = 0;
  if(set_time == 65535) //65535关闭告警间隔功能
    p_alarmtime->minute = 0;
  else
    p_alarmtime->minute = set_time;
}
// **************************************************************
// 函数:Refresh_alarmtime
// 参数:对应告警时间和标志
// 返回:无
// 描述:刷新对应告警间隔时间
// **************************************************************
void Refresh_alarmtime(ALARM_TIME *p_alarmtime)
{
  
  u16 temptran;
  temptran = Get_RTCTime();
  if(p_alarmtime->minute > temptran){
    p_alarmtime->minute -= temptran;
  }else if(p_alarmtime->minute <= temptran){
    p_alarmtime->minute = 0;
  }
  if(p_alarmtime->minute <= 1){//不满一分钟，当一分钟算20171127
    p_alarmtime->minute = 0;
    p_alarmtime->flag = 1;
  }
}
// **************************************************************
// 函数:Refresh_all_alarmtime
// 参数:无
// 返回:无
// 描述:刷新全部告警间隔时间
// **************************************************************
void Refresh_all_alarmtime(void)
{
  Refresh_alarmtime(&alarmtime_all.opencover);
  Refresh_alarmtime(&alarmtime_all.tipcover);
  Refresh_alarmtime(&alarmtime_all.waterover);
  clear_RTCTime();
}
void Enable_alarmtime(ALARM_TIME *p_alarmtime)
{
}
void Disable_alarmtime(ALARM_TIME *p_alarmtime)
{
  
}
// **************************************************************
// 函数:Is_flag_alarmtime
// 参数:对应告警时间和标志
// 返回:布尔值——1可以告警  0不能告警
// 描述:刷新全部告警间隔时间，并查询告警标志
// **************************************************************
_Bool Is_flag_alarmtime(ALARM_TIME *p_alarmtime)
{  
  PrintU16int2String_(alarmtime_all.opencover.minute);
  PrintU16int2String_(alarmtime_all.tipcover.minute);
  PrintU16int2String_(alarmtime_all.waterover.minute);
  Refresh_all_alarmtime();
  PrintU16int2String_(alarmtime_all.opencover.minute);
  PrintU16int2String_(alarmtime_all.tipcover.minute);
  PrintU16int2String_(alarmtime_all.waterover.minute);
  return p_alarmtime->flag;
}

#endif