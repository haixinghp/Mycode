#include "tick.h"
#include <time.h>
#include "RTC_SD3078.h"
#define RTC_SD3078_PRINT_NUM(info)  PrintS8int2String_(info)
#define RTC_SD3078_PRINT_R(info) debug_send_no_end(info);

/***************************************************************************************
**函数名:       u8 HexToBCD(u8 par)
**功能描述:     
把十六进制的转换为BCD
只转换低2位
例如：
十六进制数0X56，转换为BCD码，56.
**输入参数:     
**输出参数:     直接返回BCD编
**备注:         YY.c,2017-06-27,By Haixing.Huang
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
  uni_u8_tmp.stru_tmp.a = tmp_u8%10; //获得低位
  uni_u8_tmp.stru_tmp.b = tmp_u8/10; //获得高位
  return uni_u8_tmp.tmp;
}
/***************************************************************************************
**函数名:       u8 BCDToHex(u8 par)
**功能描述:     
把BCD转换为十六进制
只转换低2位
BCD码，56.，转换为十六进制数0X56
**输入参数:     
**输出参数:     直接返回BCD编
**备注:         YY.c,2017-06-27,By Haixing.Huang
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
**函数名:       SetTime
**功能描述:     外部接口，用于RTC的直接配置,可根据不同的RTC模块配置。
**输入参数:     RTC_DateTypeDef *tdata,RTC_TimeTypeDef *ttime
**输出参数:     0成功，1失败。
**备注:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
static u8 SetTimeFormRtc(RTC_DateTypeDef *tdata,RTC_TimeTypeDef *ttime)
{
  struRTC_SD3078_Time.year = HexToBCD(tdata->RTC_Year);
  struRTC_SD3078_Time.month = HexToBCD((u8)(tdata->RTC_Month));
  struRTC_SD3078_Time.day = (HexToBCD((tdata->RTC_Date)));  
  struRTC_SD3078_Time.week = HexToBCD((u8)(tdata->RTC_WeekDay));
  struRTC_SD3078_Time.hour = HexToBCD(ttime->RTC_Hours)|0x80;//12_24=1 24小时制
  struRTC_SD3078_Time.minute = HexToBCD(ttime->RTC_Minutes);
  struRTC_SD3078_Time.second = HexToBCD(ttime->RTC_Seconds);
  
  RTC_SD3078_SetTime((u8 *)(&struRTC_SD3078_Time));  //配置数据到指定的RTC模块中。
  return 0;
}
/***************************************************************************************
**函数名:       SetTime
**功能描述:     外部接口，用于RTC的直接配置,可根据修改对应的RTC模块。
**输入参数:     RTC_DateTypeDef *tdata,RTC_TimeTypeDef *ttime
**输出参数:     0成功，1失败。
**备注:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
static u8 GetTimeFormRtc(RTC_DateTypeDef *tdata,RTC_TimeTypeDef *ttime)
{
  RTC_SD3078_ReadTime((u8 *)(&struRTC_SD3078_Time));   //从RTC模块中获取数据。
  tdata->RTC_Year = BCDToHex(struRTC_SD3078_Time.year);
  tdata->RTC_Month = (RTC_Month_TypeDef)BCDToHex(struRTC_SD3078_Time.month);
  tdata->RTC_WeekDay = (RTC_Weekday_TypeDef)BCDToHex(struRTC_SD3078_Time.week);
  tdata->RTC_Date  = BCDToHex(struRTC_SD3078_Time.day);
  ttime->RTC_Hours = BCDToHex((struRTC_SD3078_Time.hour&~0x80)); //24小时制需要屏蔽12_24位
  ttime->RTC_Minutes = BCDToHex(struRTC_SD3078_Time.minute);
  ttime->RTC_Seconds = BCDToHex(struRTC_SD3078_Time.second);
  return 0;
}
/***************************************************************************************
**函数名:       GetTime
**功能描述:     根据时间戳获得时间,ps,需要把时间转换为stm8L的标准格式。
**输入参数:     int32_t tick
**输出参数:     0成功，1失败。
**备注:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
u8 TickGetTime(int32_t tick)
{
  RTC_TimeTypeDef   RTC_TimeStr;
  RTC_DateTypeDef   RTC_DateStr;
  
  tick = tick+28800;    //加8个小时.中国处于东8区。
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
**函数名:       TimeGetTick
**功能描述:     根据时间获得时间戳
**输入参数:     
**输出参数:     时间戳int32_t
**备注:         YY.c,2017-06-27,By Haixing.Huang
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
    RTC_SD3078_PRINT_R("年");
    RTC_SD3078_PRINT_NUM(RTC_DateStr.RTC_Month);
    RTC_SD3078_PRINT_R("月");
    RTC_SD3078_PRINT_NUM(RTC_DateStr.RTC_Date);
    RTC_SD3078_PRINT_R("日");
    RTC_SD3078_PRINT_NUM(RTC_TimeStr.RTC_Hours);
    RTC_SD3078_PRINT_R("时");
    RTC_SD3078_PRINT_NUM(RTC_TimeStr.RTC_Minutes);
    RTC_SD3078_PRINT_R("分");
    RTC_SD3078_PRINT_NUM(RTC_TimeStr.RTC_Seconds);
    RTC_SD3078_PRINT_R("秒");
    RTC_SD3078_PRINT_R("\n");
  }
  memset(&stm,0,sizeof(stm));  
  iY = RTC_DateStr.RTC_Year;  
  iM = RTC_DateStr.RTC_Month;  
  iD = RTC_DateStr.RTC_Date;  
  iH = RTC_TimeStr.RTC_Hours-8;//北京时间-8=UTC  
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

  