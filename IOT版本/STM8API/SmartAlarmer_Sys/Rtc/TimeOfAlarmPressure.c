#include "TimeOfAlarmPressure.h"

#if ALARMTIME_PRESSURE_EN
#define ALARMTIME_PRINT_16NUM_R(info) PrintU16int2String_(info)
#define ALARMTIME_PRINT_R(info) debug_send_no_end(info)
#warning TimeOfAlarmPressureģ��ʹ��!
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
//// ����:Get_RTCTime
//// ����:
//// ����:����RTC�ۻ�ʱ�䣨��λ�����ӣ�
//// ����:�õ�RTC�ۻ�ʱ�䣨��λ�����ӣ�
//// **************************************************************
//u16 Get_RTCTime(void)
//{
//  RTC_TimeTypeDef get_rtctime;
//  u16 temp_minute = 0;
//  RTC_GetTime(RTC_Format_BIN,&get_rtctime);  
//  temp_minute += get_rtctime.RTC_Seconds;     //rtcʱ�ӷ�Ƶ64������ǰrtc 1sΪ0.93����
//  temp_minute += get_rtctime.RTC_Minutes * 60;
//  temp_minute += get_rtctime.RTC_Hours * 3600;
//  return temp_minute;
//}
// **************************************************************
// ����:Get_RTCTime
// ����:
// ����:����RTC�ۻ�ʱ�䣨��λ���룩
// ����:�õ�RTC�ۻ�ʱ�䣨��λ���룩
// **************************************************************
u32 Get_RTCTime(void)
{
  RTC_TimeTypeDef get_rtctime;
  u32 temp_minute = 0;
  RTC_GetTime(RTC_Format_BIN,&get_rtctime);  
  temp_minute += get_rtctime.RTC_Seconds;     //rtcʱ�ӷ�Ƶ64������ǰrtc 1sΪ0.93����
  temp_minute += get_rtctime.RTC_Minutes * 60;
  temp_minute += get_rtctime.RTC_Hours * 3600;
  return temp_minute;
}
// **************************************************************
// ����:clear_RTCTime
// ����:��
// ����:����ֵ����1���Ը澯  0���ܸ澯
// ����:ˢ�¶�Ӧ�澯���ʱ�䣬����ѯ�澯��־
// **************************************************************
void clear_RTCTime(void)
{
  RTC_TimeTypeDef get_rtctime;
  RTC_TimeStructInit(&get_rtctime);
  RTC_SetTime(RTC_Format_BIN,&get_rtctime);
//  PrintString_("RTCʱ������");
}
// **************************************************************
// ����:init_all_alarmtime
// ����:��
// ����:��
// ����:ȫ���澯�����ʼ��
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
// ����:Reset_alarmtime
// ����:��Ӧ�澯ʱ��ͱ�־����Ӧ�ĵ�λΪ�롣
// ����:��
// ����:���ø澯��������ڴ����Ӧ�澯��־ʱ��
// **************************************************************
void Reset_alarmtime(ALARM_TIME *p_alarmtime,u32 set_time)
{
  p_alarmtime->flag = 0;
  if(set_time == 65535) //65535�رո澯�������
  {
    p_alarmtime->minute = 0;
  }
  else
  {
    p_alarmtime->minute = set_time*60;
  } 
}
// **************************************************************
// ����:Refresh_alarmtime
// ����:��Ӧ�澯ʱ��ͱ�־
// ����:��
// ����:ˢ�¶�Ӧ�澯���ʱ��
// **************************************************************
void Refresh_alarmtime(ALARM_TIME *p_alarmtime)
{
  
  u32 temptran;
  temptran = Get_RTCTime();
  if(p_alarmtime->minute > temptran) //��ȥ�˶�Ӧ��ʱ�䡣
  {
    p_alarmtime->minute -= temptran;
  }
  else if(p_alarmtime->minute <= temptran) //�����˼�����ʱ�䡣
  {
    p_alarmtime->minute = 0;
  }
  if(p_alarmtime->minute <= 60)//����1���ӣ���1�����㡣
  {
    p_alarmtime->minute = 0;
    p_alarmtime->flag = 1;
  }
}
// **************************************************************
// ����:Refresh_all_alarmtime
// ����:��
// ����:��
// ����:ˢ��ȫ���澯���ʱ��
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
// ����:Is_flag_alarmtime
// ����:��Ӧ�澯ʱ��ͱ�־
// ����:����ֵ����1���Ը澯  0���ܸ澯
// ����:ˢ��ȫ���澯���ʱ�䣬����ѯ�澯��־
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
//**������:       s8 ShowAlarmTimer()
//**��������:     --
//**�������:     --
//**�������:     --
//**��ע:         Answer.c,2017.12.10,By Haixing.Huang
//****************************************************************************************
s8 ShowAlarmTimer()
{
  ALARMTIME_PRINT_R("���޼��ʣ��ʱ�䣺");
  ALARMTIME_PRINT_16NUM_R(alarmtime_all.PressureUpLimit.minute/60);
  ALARMTIME_PRINT_R("���޼��ʣ��ʱ�䣺");
  ALARMTIME_PRINT_16NUM_R(alarmtime_all.PressureDownLimit.minute/60);
  ALARMTIME_PRINT_R("δ���Ӵ��������ʣ��ʱ�䣺");
  ALARMTIME_PRINT_16NUM_R(alarmtime_all.NoSensor.minute/60);
  ALARMTIME_PRINT_R("�ֱ����ϱ����ʣ��ʱ�䣺");
  ALARMTIME_PRINT_16NUM_R(alarmtime_all.ResolutionAlarm.minute/60);
  ALARMTIME_PRINT_R("�����ϱ����ʣ��ʱ�䣺");
  ALARMTIME_PRINT_16NUM_R(alarmtime_all.TouchOff.minute/60);
  return 0;
}
#endif