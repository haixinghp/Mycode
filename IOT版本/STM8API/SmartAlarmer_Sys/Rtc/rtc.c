//#include "stm8l15x.h"
#include "config.h"
#include <time.h>
/***************************************************************************************
**函数名:       GetTime
**功能描述:     根据时间戳获得时间
**输入参数:     int32_t tick
**输出参数:     0成功，1失败。
**备注:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
u8 GetTime(int32_t tick)
{
  RTC_TimeTypeDef   RTC_TimeStr;
  RTC_DateTypeDef   RTC_DateStr;
  RTC_InitTypeDef   RTC_InitStr;
  tick = tick+28800;    //加8个小时.中国处于东8区。
  struct tm *stm;
  stm = localtime(&tick); 
  //直接配置到RTC时钟。先从时间开始。
  RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;
//  RTC_InitStr.RTC_AsynchPrediv = 0x7C;   
//  RTC_InitStr.RTC_SynchPrediv = 0x0004;  //38K/0X7C/132 大约等于1Hz
//#if RTC_USE_LSE  //使用外部时钟
//  RTC_InitStr.RTC_AsynchPrediv = 0x7C;   
//  RTC_InitStr.RTC_SynchPrediv = 0x0004;  //38K/0X7C/132 大约等于1Hz  
//#else  //使用内部时钟
//  RTC_InitStr.RTC_AsynchPrediv = 0x7C;   
//  RTC_InitStr.RTC_SynchPrediv = 0x0004;  //38K/0X7C/132 大约等于1Hz  
//#endif  
  RTC_InitStr.RTC_AsynchPrediv = 0x7C;   
  RTC_InitStr.RTC_SynchPrediv = 0x0004;  //38K/0X7C/132 大约等于1Hz  
  RTC_Init(&RTC_InitStr);
/******************RTC设置时间********************/  
  RTC_TimeStructInit(&RTC_TimeStr);  
  RTC_TimeStr.RTC_Hours = stm->tm_hour;
  RTC_TimeStr.RTC_Minutes = stm->tm_min;
  RTC_TimeStr.RTC_Seconds = stm->tm_sec;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr); 
/******************RTC设置日期********************/    
  RTC_DateStructInit(&RTC_DateStr); 
  RTC_DateStr.RTC_WeekDay = (RTC_Weekday_TypeDef)(stm->tm_wday);
  RTC_DateStr.RTC_Date = stm->tm_mday;
  RTC_DateStr.RTC_Month =(RTC_Month_TypeDef)(stm->tm_mon+1);
  RTC_DateStr.RTC_Year = stm->tm_year-100;
  RTC_SetDate(RTC_Format_BIN, &RTC_DateStr);  
//  while (RTC_WaitForSynchro() != SUCCESS);
//  RTC_SetDate(RTC_Format_BIN, &RTC_DateStr);
//  while (RTC_WaitForSynchro() != SUCCESS); 
//  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr);
  return 0;  
}
/***************************************************************************************
**函数名:       int32_t GetTick(void) 
**功能描述:     根据时间获得时间戳
**输入参数:     
**输出参数:     时间戳int32_t
**备注:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
int32_t GetTick(void)  
{  
  RTC_TimeTypeDef   RTC_TimeStr;
  RTC_DateTypeDef   RTC_DateStr;
  struct tm stm;  
  int iY, iM, iD, iH, iMin, iS;  
  while (RTC_WaitForSynchro() != SUCCESS);
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
  while (RTC_WaitForSynchro() != SUCCESS);
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
  debug_send_no_end("20");
  PrintU8int2String(RTC_DateStr.RTC_Year);
  debug_send_no_end("年");
  PrintU8int2String(RTC_DateStr.RTC_Month);
  debug_send_no_end("月");
  PrintU8int2String(RTC_DateStr.RTC_Date);
  debug_send_no_end("日");
  PrintU8int2String(RTC_TimeStr.RTC_Hours);
  debug_send_no_end("时");
  PrintU8int2String(RTC_TimeStr.RTC_Minutes);
  debug_send_no_end("分");
  PrintU8int2String(RTC_TimeStr.RTC_Seconds);
  debug_send("秒");
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


void CLK_Config(void)  
{
 // Select HSE as system clock source 
  CLK_SYSCLKSourceSwitchCmd(ENABLE);//使能时钟切换
  
  
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);//选择内部低速时钟作为时钟源
  //system clock prescaler: 1
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//设置系统时钟分频
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI)//等待时钟稳定
  {}
  CLK_HSICmd(DISABLE);                          //关闭原先的 CLK source
  CLK_HSEConfig(CLK_HSE_OFF);                   //关闭外部 CLK source
  
  
  
  /*
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);//选择内部低速时钟作为时钟源
  //system clock prescaler: 1
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//设置系统时钟分频
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_LSI)//等待时钟稳定
  {}
  CLK_HSICmd(DISABLE);                          //关闭原先的 CLK source
  CLK_HSEConfig(CLK_HSE_OFF);                   //关闭外部 CLK source
  */
}
//#if WATER_PRESSURE_EN
//void RTC_Config(void)
//{
//#if RTC_USE_LSE  //使用外部时钟
//  CLK_LSEConfig(CLK_LSE_ON);  
//  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
//  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//允许RTC时钟
//  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_64);
//  debug_send("RTC时钟：外部32.768KHz");
//#else  //使用内部时钟
//  CLK_LSICmd(ENABLE);
//  while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET); 
//  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//允许RTC时钟
//  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_64);
//  debug_send("RTC时钟：内部38KHz");
//#endif
//  RTC_WakeUpCmd(DISABLE);//the wake up Unit must be disabled (if enabled) using RTC_WakeUpCmd(Disable).
//  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//19k/16=1.1875KHz t=0.85ms
//#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
//  RTC_SetWakeUpCounter(XiaoFang_RtcSet(Sys_keydata.trigger_interval_minute_count));
//#endif
//#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
//  #if RTC_CAN_BE_SET
//    RTC_SetWakeUpCounter(XiaoFang_RtcSet(Sys_keydata.trigger_interval_minute_count));
//  #else
//    RTC_SetWakeUpCounter(RTC_MIN);
//  #endif
//#endif
//  
////  RTC_SetWakeUpCounter(800); //TEST
////  ITC_SetSoftwarePriority(RTC_IRQn, ITC_PriorityLevel_3);//优先线
//  ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_3);//注：由于选择芯片型号HD，底层指令符由RTC_IRQn改为RTC_CSSLSE_IRQn
//  RTC_ITConfig(RTC_IT_WUT, ENABLE);
//  RTC_WakeUpCmd(ENABLE);
//}
//#endif

#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
void RTC_Config(void)
{
#if RTC_USE_LSE  //使用外部时钟
  CLK_LSEConfig(CLK_LSE_ON);  
  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//允许RTC时钟
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_64);
  debug_send("RTC时钟：外部32.768KHz");
#else  //使用内部时钟
  CLK_LSICmd(ENABLE);
  while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET); 
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//允许RTC时钟
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_64);
  debug_send("RTC时钟：内部38KHz");
#endif
  RTC_WakeUpCmd(DISABLE);//the wake up Unit must be disabled (if enabled) using RTC_WakeUpCmd(Disable).
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//19k/16=1.1875KHz t=0.85ms

  #if RTC_CAN_BE_SET
    RTC_SetWakeUpCounter(XiaoFang_RtcSet(Sys_keydata.trigger_interval_minute_count));
  #else
    RTC_SetWakeUpCounter(RTC_MIN);
  #endif
//  RTC_SetWakeUpCounter(800); //TEST
//  ITC_SetSoftwarePriority(RTC_IRQn, ITC_PriorityLevel_3);//优先线
  ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_3);//注：由于选择芯片型号HD，底层指令符由RTC_IRQn改为RTC_CSSLSE_IRQn
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  RTC_WakeUpCmd(ENABLE);
}
#endif

#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
void RTC_Config(void)
{
  RTC_InitTypeDef   RTC_InitStr;
#if RTC_USE_LSE  //使用外部时钟
  CLK_LSEConfig(CLK_LSE_ON);  
  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//允许RTC时钟
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_64);// 0.512KHz
  debug_send("RTC时钟：外部32.768KHz");
#if SPEED_UP  
  RTC_InitStr.RTC_AsynchPrediv = 0x00;   
//  RTC_InitStr.RTC_SynchPrediv = 3072;  //6s的的频率为1/6HZ。  
  RTC_InitStr.RTC_SynchPrediv = 51;  //1s/10的的频率为1/10HZ。
#else
//  RTC_InitStr.RTC_AsynchPrediv = 0x00;   
//  RTC_InitStr.RTC_SynchPrediv = 0x0200;  //1s的的频率为1HZ。
  RTC_InitStr.RTC_AsynchPrediv = 0x00;   
//  RTC_InitStr.RTC_SynchPrediv = 30720;  //60s的的频率为1/60HZ。  
  RTC_InitStr.RTC_SynchPrediv = 512;  //1s的的频率为1HZ。 
#endif
  RTC_Init(&RTC_InitStr);
#else  //使用内部时钟
  CLK_LSICmd(ENABLE);
  while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET); 
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//允许RTC时钟
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_64);
  debug_send("RTC时钟：内部38KHz");
#endif
  RTC_WakeUpCmd(DISABLE);//the wake up Unit must be disabled (if enabled) using RTC_WakeUpCmd(Disable).
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//19k/16=1.1875KHz t=0.85ms
  RTC_SetWakeUpCounter(XiaoFang_RtcSet(Sys_keydata.trigger_interval_minute_count));
  ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_3);//注：由于选择芯片型号HD，底层指令符由RTC_IRQn改为RTC_CSSLSE_IRQn
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  RTC_WakeUpCmd(ENABLE);
}
#endif


void RTC_Config_4_secondquery(u16 interval_second_count)
{
  RTC_DeInit();
  CLK_LSICmd(ENABLE);
  while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);
  RTC_ClearITPendingBit(RTC_IT_WUT);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//
  
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_64);
  
  RTC_WakeUpCmd(DISABLE);//the wake up Unit must be disabled (if enabled) using RTC_WakeUpCmd(Disable).
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//19k/16=1.1875KHz t=0.85ms
  //RTC_ITConfig(RTC_IT_WUT, ENABLE);//
  
  //RTC_SetWakeUpCounter((63750/30)*interval_count);//按分计时
  RTC_SetWakeUpCounter((70125/1800)*interval_second_count);//按秒计时
  
  ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_3);//
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  RTC_WakeUpCmd(ENABLE);
}

