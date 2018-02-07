#include "TimeOfAlarmPressure.h"

#if ALARMTIME_PRESSURE_EN
#define ALARMTIME_PRINT_16NUM_R(info) PrintU16int2String_(info)
#define ALARMTIME_PRINT_R(info) debug_send_no_end(info)
#warning TimeOfAlarmPressure模块使能!
static ALARMTIME_ALL alarmtime_all;
u8 en_alarmtime = 0;

ALARM_TIME *getAlarmTimePressureUpLimit()
{
  return &alarmtime_all.PressureUpLimit;
}
ALARM_TIME *getAlarmTimePressureDownLimit()
{
  return &alarmtime_all.PressureDownLimit;
}
ALARM_TIME *getAlarmTimeNoSensor()
{
  return &alarmtime_all.NoSensor;
}
ALARM_TIME *getAlarmTimeResolutionAlarm()
{
  return &alarmtime_all.ResolutionAlarm;
}
ALARM_TIME *getAlarmTimeTouchOff()
{
  return &alarmtime_all.TouchOff;
}
//// **************************************************************
//// 函数:Get_RTCTime
//// 参数:
//// 返回:返回RTC累积时间（单位：分钟）
//// 描述:得到RTC累积时间（单位：分钟）
//// **************************************************************
//u16 Get_RTCTime(void)
//{
//  RTC_TimeTypeDef get_rtctime;
//  u16 temp_minute = 0;
//  RTC_GetTime(RTC_Format_BIN,&get_rtctime);  
//  temp_minute += get_rtctime.RTC_Seconds;     //rtc时钟分频64，即当前rtc 1s为0.93分钟
//  temp_minute += get_rtctime.RTC_Minutes * 60;
//  temp_minute += get_rtctime.RTC_Hours * 3600;
//  return temp_minute;
//}
// **************************************************************
// 函数:Get_RTCTime
// 参数:
// 返回:返回RTC累积时间（单位：秒）
// 描述:得到RTC累积时间（单位：秒）
// **************************************************************
u32 Get_RTCTime(void)
{
  RTC_TimeTypeDef get_rtctime;
  u32 temp_minute = 0;
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
  alarmtime_all.PressureUpLimit.flag = 1;
  alarmtime_all.PressureUpLimit.minute = 0;
  alarmtime_all.PressureDownLimit.flag = 1;
  alarmtime_all.NoSensor.flag = 1;
  alarmtime_all.NoSensor.minute = 0;
  alarmtime_all.ResolutionAlarm.flag = 1;
  alarmtime_all.ResolutionAlarm.minute = 0;
  alarmtime_all.TouchOff.flag = 1;
  alarmtime_all.TouchOff.minute = 0;
//  alarmtime_all.opencover.flag = 1;
//  alarmtime_all.opencover.minute = 0;
//  alarmtime_all.tipcover.flag = 1;
//  alarmtime_all.tipcover.minute = 0;
//  alarmtime_all.waterover.flag = 1;
//  alarmtime_all.waterover.minute = 0;
}
// **************************************************************
// 函数:Reset_alarmtime
// 参数:对应告警时间和标志，对应的单位为秒。
// 返回:无
// 描述:重置告警间隔，用于打包对应告警标志时候
// **************************************************************
void Reset_alarmtime(ALARM_TIME *p_alarmtime,u32 set_time)
{
  p_alarmtime->flag = 0;
  if(set_time == 65535) //65535关闭告警间隔功能
  {
    p_alarmtime->minute = 0;
  }
  else
  {
    p_alarmtime->minute = set_time*60;
  } 
}
// **************************************************************
// 函数:Refresh_alarmtime
// 参数:对应告警时间和标志
// 返回:无
// 描述:刷新对应告警间隔时间
// **************************************************************
void Refresh_alarmtime(ALARM_TIME *p_alarmtime)
{
  
  u32 temptran;
  temptran = Get_RTCTime();
  if(p_alarmtime->minute > temptran) //减去了对应的时间。
  {
    p_alarmtime->minute -= temptran;
  }
  else if(p_alarmtime->minute <= temptran) //超过了计数的时间。
  {
    p_alarmtime->minute = 0;
  }
  if(p_alarmtime->minute <= 60)//少于1分钟，当1分钟算。
  {
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
  Refresh_alarmtime(&alarmtime_all.PressureUpLimit);
  Refresh_alarmtime(&alarmtime_all.PressureDownLimit);
  Refresh_alarmtime(&alarmtime_all.NoSensor);
  Refresh_alarmtime(&alarmtime_all.ResolutionAlarm);
  Refresh_alarmtime(&alarmtime_all.TouchOff);
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
//  PrintU16int2String_(alarmtime_all.PressureUpLimit.minute);
//  PrintU16int2String_(alarmtime_all.PressureDownLimit.minute);
//  PrintU16int2String_(alarmtime_all.NoSensor.minute);
//  PrintU16int2String_(alarmtime_all.ResolutionAlarm.minute);
//  PrintU16int2String_(alarmtime_all.TouchOff.minute);
  Refresh_all_alarmtime();
//  PrintU16int2String_(alarmtime_all.PressureUpLimit.minute);
//  PrintU16int2String_(alarmtime_all.PressureDownLimit.minute);
//  PrintU16int2String_(alarmtime_all.NoSensor.minute);
//  PrintU16int2String_(alarmtime_all.ResolutionAlarm.minute);
//  PrintU16int2String_(alarmtime_all.TouchOff.minute);
  return p_alarmtime->flag;
}
///***************************************************************************************
//**函数名:       s8 ShowAlarmTimer()
//**功能描述:     --
//**输入参数:     --
//**输出参数:     --
//**备注:         Answer.c,2017.12.10,By Haixing.Huang
//****************************************************************************************
s8 ShowAlarmTimer()
{
  ALARMTIME_PRINT_R("上限间隔剩余时间：");
  ALARMTIME_PRINT_16NUM_R(alarmtime_all.PressureUpLimit.minute/60);
  ALARMTIME_PRINT_R("下限间隔剩余时间：");
  ALARMTIME_PRINT_16NUM_R(alarmtime_all.PressureDownLimit.minute/60);
  ALARMTIME_PRINT_R("未连接传感器间隔剩余时间：");
  ALARMTIME_PRINT_16NUM_R(alarmtime_all.NoSensor.minute/60);
  ALARMTIME_PRINT_R("分辨率上报间隔剩余时间：");
  ALARMTIME_PRINT_16NUM_R(alarmtime_all.ResolutionAlarm.minute/60);
  ALARMTIME_PRINT_R("触发上报间隔剩余时间：");
  ALARMTIME_PRINT_16NUM_R(alarmtime_all.TouchOff.minute/60);
  return 0;
}
#endif