#include "tick.h"
#include <time.h>
#include "RTC_SD3078.h"
#define RTC_SD3078_PRINT_NUM(info)  PrintS8int2String_(info)
#define RTC_SD3078_PRINT_R(info) debug_send_no_end(info);

/***************************************************************************************
**������:       u8 HexToBCD(u8 par)
**��������:     
��ʮ�����Ƶ�ת��ΪBCD
ֻת����2λ
���磺
ʮ��������0X56��ת��ΪBCD�룬56.
**�������:     
**�������:     ֱ�ӷ���BCD��
**��ע:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
u8 HexToBCD(u8 par)
{
  union _uni_u8_tmp
  {
    u8 tmp;
    struct
    {
      u8 a:4;
      u8 b:4;
    }stru_tmp;
  };
  union _uni_u8_tmp uni_u8_tmp;
  u8 tmp_u8 =0;
  tmp_u8 = par;
  if(par >= 100)
  {
    tmp_u8 = tmp_u8 % 100;
  }
  uni_u8_tmp.stru_tmp.a = tmp_u8%10; //��õ�λ
  uni_u8_tmp.stru_tmp.b = tmp_u8/10; //��ø�λ
  return uni_u8_tmp.tmp;
}
/***************************************************************************************
**������:       u8 BCDToHex(u8 par)
**��������:     
��BCDת��Ϊʮ������
ֻת����2λ
BCD�룬56.��ת��Ϊʮ��������0X56
**�������:     
**�������:     ֱ�ӷ���BCD��
**��ע:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
u8 BCDToHex(u8 par)
{
  union _uni_u8_tmp
  {
    u8 tmp;
    struct
    {
      u8 a:4;
      u8 b:4;
    }stru_tmp;
  };
  union _uni_u8_tmp uni_u8_tmp;
  uni_u8_tmp.tmp = par;
  return (uni_u8_tmp.stru_tmp.a + uni_u8_tmp.stru_tmp.b*10);
}

/***************************************************************************************
**������:       SetTime
**��������:     �ⲿ�ӿڣ�����RTC��ֱ������,�ɸ��ݲ�ͬ��RTCģ�����á�
**�������:     RTC_DateTypeDef *tdata,RTC_TimeTypeDef *ttime
**�������:     0�ɹ���1ʧ�ܡ�
**��ע:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
static u8 SetTimeFormRtc(RTC_DateTypeDef *tdata,RTC_TimeTypeDef *ttime)
{
  struRTC_SD3078_Time.year = HexToBCD(tdata->RTC_Year);
  struRTC_SD3078_Time.month = HexToBCD((u8)(tdata->RTC_Month));
  struRTC_SD3078_Time.day = (HexToBCD((tdata->RTC_Date)));  
  struRTC_SD3078_Time.week = HexToBCD((u8)(tdata->RTC_WeekDay));
  struRTC_SD3078_Time.hour = HexToBCD(ttime->RTC_Hours)|0x80;//12_24=1 24Сʱ��
  struRTC_SD3078_Time.minute = HexToBCD(ttime->RTC_Minutes);
  struRTC_SD3078_Time.second = HexToBCD(ttime->RTC_Seconds);
  
  RTC_SD3078_SetTime((u8 *)(&struRTC_SD3078_Time));  //�������ݵ�ָ����RTCģ���С�
  return 0;
}
/***************************************************************************************
**������:       SetTime
**��������:     �ⲿ�ӿڣ�����RTC��ֱ������,�ɸ����޸Ķ�Ӧ��RTCģ�顣
**�������:     RTC_DateTypeDef *tdata,RTC_TimeTypeDef *ttime
**�������:     0�ɹ���1ʧ�ܡ�
**��ע:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
static u8 GetTimeFormRtc(RTC_DateTypeDef *tdata,RTC_TimeTypeDef *ttime)
{
  RTC_SD3078_ReadTime((u8 *)(&struRTC_SD3078_Time));   //��RTCģ���л�ȡ���ݡ�
  tdata->RTC_Year = BCDToHex(struRTC_SD3078_Time.year);
  tdata->RTC_Month = (RTC_Month_TypeDef)BCDToHex(struRTC_SD3078_Time.month);
  tdata->RTC_WeekDay = (RTC_Weekday_TypeDef)BCDToHex(struRTC_SD3078_Time.week);
  tdata->RTC_Date  = BCDToHex(struRTC_SD3078_Time.day);
  ttime->RTC_Hours = BCDToHex((struRTC_SD3078_Time.hour&~0x80)); //24Сʱ����Ҫ����12_24λ
  ttime->RTC_Minutes = BCDToHex(struRTC_SD3078_Time.minute);
  ttime->RTC_Seconds = BCDToHex(struRTC_SD3078_Time.second);
  return 0;
}
/***************************************************************************************
**������:       GetTime
**��������:     ����ʱ������ʱ��,ps,��Ҫ��ʱ��ת��Ϊstm8L�ı�׼��ʽ��
**�������:     int32_t tick
**�������:     0�ɹ���1ʧ�ܡ�
**��ע:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
u8 TickGetTime(int32_t tick)
{
  RTC_TimeTypeDef   RTC_TimeStr;
  RTC_DateTypeDef   RTC_DateStr;
  
  tick = tick+28800;    //��8��Сʱ.�й����ڶ�8����
  struct tm *stm;
  stm = localtime(&tick);  
  
  RTC_DateStr.RTC_Year = stm->tm_year-100;
  RTC_DateStr.RTC_Month =(RTC_Month_TypeDef)(stm->tm_mon+1);
  RTC_DateStr.RTC_Date = stm->tm_mday;
  RTC_DateStr.RTC_WeekDay = (RTC_Weekday_TypeDef)(stm->tm_wday); 
  RTC_TimeStr.RTC_Hours = stm->tm_hour;
  RTC_TimeStr.RTC_Minutes = stm->tm_min;
  RTC_TimeStr.RTC_Seconds = stm->tm_sec;
  SetTimeFormRtc(&RTC_DateStr,&RTC_TimeStr);
  return 0;
}

/***************************************************************************************
**������:       TimeGetTick
**��������:     ����ʱ����ʱ���
**�������:     
**�������:     ʱ���int32_t
**��ע:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
int32_t TimeGetTick(bool showtime)  
{  
  RTC_TimeTypeDef   RTC_TimeStr;
  RTC_DateTypeDef   RTC_DateStr;
  struct tm stm;  
  int iY, iM, iD, iH, iMin, iS;  
  GetTimeFormRtc(&RTC_DateStr,&RTC_TimeStr);
//  PrintU8int2String_(RTC_DateStr.RTC_Year);
//  PrintU8int2String_(RTC_DateStr.RTC_Month);
//  PrintU8int2String_(RTC_DateStr.RTC_Date);
//  PrintU8int2String_(RTC_TimeStr.RTC_Hours);
//  PrintU8int2String_(RTC_TimeStr.RTC_Minutes);
//  PrintU8int2String_(RTC_TimeStr.RTC_Seconds);
//  
  if(showtime)
  {
    RTC_SD3078_PRINT_R("20");
    if(0 == RTC_DateStr.RTC_Year)
    {
      RTC_SD3078_PRINT_NUM(0x00);
      RTC_SD3078_PRINT_NUM(0x00);
    }
    else
    {
      RTC_SD3078_PRINT_NUM(RTC_DateStr.RTC_Year);
    }
    RTC_SD3078_PRINT_R("��");
    RTC_SD3078_PRINT_NUM(RTC_DateStr.RTC_Month);
    RTC_SD3078_PRINT_R("��");
    RTC_SD3078_PRINT_NUM(RTC_DateStr.RTC_Date);
    RTC_SD3078_PRINT_R("��");
    RTC_SD3078_PRINT_NUM(RTC_TimeStr.RTC_Hours);
    RTC_SD3078_PRINT_R("ʱ");
    RTC_SD3078_PRINT_NUM(RTC_TimeStr.RTC_Minutes);
    RTC_SD3078_PRINT_R("��");
    RTC_SD3078_PRINT_NUM(RTC_TimeStr.RTC_Seconds);
    RTC_SD3078_PRINT_R("��");
    RTC_SD3078_PRINT_R("\n");
  }
  memset(&stm,0,sizeof(stm));  
  iY = RTC_DateStr.RTC_Year;  
  iM = RTC_DateStr.RTC_Month;  
  iD = RTC_DateStr.RTC_Date;  
  iH = RTC_TimeStr.RTC_Hours-8;//����ʱ��-8=UTC  
  iMin = RTC_TimeStr.RTC_Minutes;  
  iS = RTC_TimeStr.RTC_Seconds;   
  stm.tm_year=iY+100;
  stm.tm_mon=iM-1;  
  stm.tm_mday=iD;  
  stm.tm_hour=iH;  
  stm.tm_min=iMin;  
  stm.tm_sec=iS;  
  return mktime(&stm);  
}

  