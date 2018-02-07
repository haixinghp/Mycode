#include "TimeOfAlarm.h"


#if ALARMTIME_EN
ALARMTIME_ALL alarmtime_all;
u8 en_alarmtime = 0;

// **************************************************************
// ����:Get_RTCTime
// ����:
// ����:����RTC�ۻ�ʱ�䣨��λ�����ӣ�
// ����:�õ�RTC�ۻ�ʱ�䣨��λ�����ӣ�
// **************************************************************
u16 Get_RTCTime(void)
{
  RTC_TimeTypeDef get_rtctime;
  u16 temp_minute = 0;
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
  alarmtime_all.opencover.flag = 1;
  alarmtime_all.opencover.minute = 0;
  alarmtime_all.tipcover.flag = 1;
  alarmtime_all.tipcover.minute = 0;
  alarmtime_all.waterover.flag = 1;
  alarmtime_all.waterover.minute = 0;
}
// **************************************************************
// ����:Reset_alarmtime
// ����:��Ӧ�澯ʱ��ͱ�־
// ����:��
// ����:���ø澯��������ڴ����Ӧ�澯��־ʱ��
// **************************************************************
void Reset_alarmtime(ALARM_TIME *p_alarmtime,u16 set_time)
{
  p_alarmtime->flag = 0;
  if(set_time == 65535) //65535�رո澯�������
    p_alarmtime->minute = 0;
  else
    p_alarmtime->minute = set_time;
}
// **************************************************************
// ����:Refresh_alarmtime
// ����:��Ӧ�澯ʱ��ͱ�־
// ����:��
// ����:ˢ�¶�Ӧ�澯���ʱ��
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
  if(p_alarmtime->minute <= 1){//����һ���ӣ���һ������20171127
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
// ����:Is_flag_alarmtime
// ����:��Ӧ�澯ʱ��ͱ�־
// ����:����ֵ����1���Ը澯  0���ܸ澯
// ����:ˢ��ȫ���澯���ʱ�䣬����ѯ�澯��־
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