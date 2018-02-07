//#include "stm8l15x.h"
#include "config.h"
#include <time.h>
/***************************************************************************************
**������:       GetTime
**��������:     ����ʱ������ʱ��
**�������:     int32_t tick
**�������:     0�ɹ���1ʧ�ܡ�
**��ע:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
u8 GetTime(int32_t tick)
{
  RTC_TimeTypeDef   RTC_TimeStr;
  RTC_DateTypeDef   RTC_DateStr;
  RTC_InitTypeDef   RTC_InitStr;
  tick = tick+28800;    //��8��Сʱ.�й����ڶ�8����
  struct tm *stm;
  stm = localtime(&tick); 
  //ֱ�����õ�RTCʱ�ӡ��ȴ�ʱ�俪ʼ��
  RTC_InitStr.RTC_HourFormat = RTC_HourFormat_24;
//  RTC_InitStr.RTC_AsynchPrediv = 0x7C;   
//  RTC_InitStr.RTC_SynchPrediv = 0x0004;  //38K/0X7C/132 ��Լ����1Hz
//#if RTC_USE_LSE  //ʹ���ⲿʱ��
//  RTC_InitStr.RTC_AsynchPrediv = 0x7C;   
//  RTC_InitStr.RTC_SynchPrediv = 0x0004;  //38K/0X7C/132 ��Լ����1Hz  
//#else  //ʹ���ڲ�ʱ��
//  RTC_InitStr.RTC_AsynchPrediv = 0x7C;   
//  RTC_InitStr.RTC_SynchPrediv = 0x0004;  //38K/0X7C/132 ��Լ����1Hz  
//#endif  
  RTC_InitStr.RTC_AsynchPrediv = 0x7C;   
  RTC_InitStr.RTC_SynchPrediv = 0x0004;  //38K/0X7C/132 ��Լ����1Hz  
  RTC_Init(&RTC_InitStr);
/******************RTC����ʱ��********************/  
  RTC_TimeStructInit(&RTC_TimeStr);  
  RTC_TimeStr.RTC_Hours = stm->tm_hour;
  RTC_TimeStr.RTC_Minutes = stm->tm_min;
  RTC_TimeStr.RTC_Seconds = stm->tm_sec;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStr); 
/******************RTC��������********************/    
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
**������:       int32_t GetTick(void) 
**��������:     ����ʱ����ʱ���
**�������:     
**�������:     ʱ���int32_t
**��ע:         YY.c,2017-06-27,By Haixing.Huang
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
  debug_send_no_end("��");
  PrintU8int2String(RTC_DateStr.RTC_Month);
  debug_send_no_end("��");
  PrintU8int2String(RTC_DateStr.RTC_Date);
  debug_send_no_end("��");
  PrintU8int2String(RTC_TimeStr.RTC_Hours);
  debug_send_no_end("ʱ");
  PrintU8int2String(RTC_TimeStr.RTC_Minutes);
  debug_send_no_end("��");
  PrintU8int2String(RTC_TimeStr.RTC_Seconds);
  debug_send("��");
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


void CLK_Config(void)  
{
 // Select HSE as system clock source 
  CLK_SYSCLKSourceSwitchCmd(ENABLE);//ʹ��ʱ���л�
  
  
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);//ѡ���ڲ�����ʱ����Ϊʱ��Դ
  //system clock prescaler: 1
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//����ϵͳʱ�ӷ�Ƶ
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI)//�ȴ�ʱ���ȶ�
  {}
  CLK_HSICmd(DISABLE);                          //�ر�ԭ�ȵ� CLK source
  CLK_HSEConfig(CLK_HSE_OFF);                   //�ر��ⲿ CLK source
  
  
  
  /*
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);//ѡ���ڲ�����ʱ����Ϊʱ��Դ
  //system clock prescaler: 1
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//����ϵͳʱ�ӷ�Ƶ
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_LSI)//�ȴ�ʱ���ȶ�
  {}
  CLK_HSICmd(DISABLE);                          //�ر�ԭ�ȵ� CLK source
  CLK_HSEConfig(CLK_HSE_OFF);                   //�ر��ⲿ CLK source
  */
}
//#if WATER_PRESSURE_EN
//void RTC_Config(void)
//{
//#if RTC_USE_LSE  //ʹ���ⲿʱ��
//  CLK_LSEConfig(CLK_LSE_ON);  
//  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
//  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//����RTCʱ��
//  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_64);
//  debug_send("RTCʱ�ӣ��ⲿ32.768KHz");
//#else  //ʹ���ڲ�ʱ��
//  CLK_LSICmd(ENABLE);
//  while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET); 
//  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//����RTCʱ��
//  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_64);
//  debug_send("RTCʱ�ӣ��ڲ�38KHz");
//#endif
//  RTC_WakeUpCmd(DISABLE);//the wake up Unit must be disabled (if enabled) using RTC_WakeUpCmd(Disable).
//  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//19k/16=1.1875KHz t=0.85ms
//#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
//  RTC_SetWakeUpCounter(XiaoFang_RtcSet(Sys_keydata.trigger_interval_minute_count));
//#endif
//#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
//  #if RTC_CAN_BE_SET
//    RTC_SetWakeUpCounter(XiaoFang_RtcSet(Sys_keydata.trigger_interval_minute_count));
//  #else
//    RTC_SetWakeUpCounter(RTC_MIN);
//  #endif
//#endif
//  
////  RTC_SetWakeUpCounter(800); //TEST
////  ITC_SetSoftwarePriority(RTC_IRQn, ITC_PriorityLevel_3);//������
//  ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_3);//ע������ѡ��оƬ�ͺ�HD���ײ�ָ�����RTC_IRQn��ΪRTC_CSSLSE_IRQn
//  RTC_ITConfig(RTC_IT_WUT, ENABLE);
//  RTC_WakeUpCmd(ENABLE);
//}
//#endif

#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
void RTC_Config(void)
{
#if RTC_USE_LSE  //ʹ���ⲿʱ��
  CLK_LSEConfig(CLK_LSE_ON);  
  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//����RTCʱ��
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_64);
  debug_send("RTCʱ�ӣ��ⲿ32.768KHz");
#else  //ʹ���ڲ�ʱ��
  CLK_LSICmd(ENABLE);
  while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET); 
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//����RTCʱ��
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_64);
  debug_send("RTCʱ�ӣ��ڲ�38KHz");
#endif
  RTC_WakeUpCmd(DISABLE);//the wake up Unit must be disabled (if enabled) using RTC_WakeUpCmd(Disable).
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//19k/16=1.1875KHz t=0.85ms

  #if RTC_CAN_BE_SET
    RTC_SetWakeUpCounter(XiaoFang_RtcSet(Sys_keydata.trigger_interval_minute_count));
  #else
    RTC_SetWakeUpCounter(RTC_MIN);
  #endif
//  RTC_SetWakeUpCounter(800); //TEST
//  ITC_SetSoftwarePriority(RTC_IRQn, ITC_PriorityLevel_3);//������
  ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_3);//ע������ѡ��оƬ�ͺ�HD���ײ�ָ�����RTC_IRQn��ΪRTC_CSSLSE_IRQn
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  RTC_WakeUpCmd(ENABLE);
}
#endif

#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
void RTC_Config(void)
{
  RTC_InitTypeDef   RTC_InitStr;
#if RTC_USE_LSE  //ʹ���ⲿʱ��
  CLK_LSEConfig(CLK_LSE_ON);  
  while (CLK_GetFlagStatus(CLK_FLAG_LSERDY) == RESET);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//����RTCʱ��
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_64);// 0.512KHz
  debug_send("RTCʱ�ӣ��ⲿ32.768KHz");
#if SPEED_UP  
  RTC_InitStr.RTC_AsynchPrediv = 0x00;   
//  RTC_InitStr.RTC_SynchPrediv = 3072;  //6s�ĵ�Ƶ��Ϊ1/6HZ��  
  RTC_InitStr.RTC_SynchPrediv = 51;  //1s/10�ĵ�Ƶ��Ϊ1/10HZ��
#else
//  RTC_InitStr.RTC_AsynchPrediv = 0x00;   
//  RTC_InitStr.RTC_SynchPrediv = 0x0200;  //1s�ĵ�Ƶ��Ϊ1HZ��
  RTC_InitStr.RTC_AsynchPrediv = 0x00;   
//  RTC_InitStr.RTC_SynchPrediv = 30720;  //60s�ĵ�Ƶ��Ϊ1/60HZ��  
  RTC_InitStr.RTC_SynchPrediv = 512;  //1s�ĵ�Ƶ��Ϊ1HZ�� 
#endif
  RTC_Init(&RTC_InitStr);
#else  //ʹ���ڲ�ʱ��
  CLK_LSICmd(ENABLE);
  while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET); 
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//����RTCʱ��
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_64);
  debug_send("RTCʱ�ӣ��ڲ�38KHz");
#endif
  RTC_WakeUpCmd(DISABLE);//the wake up Unit must be disabled (if enabled) using RTC_WakeUpCmd(Disable).
  RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//19k/16=1.1875KHz t=0.85ms
  RTC_SetWakeUpCounter(XiaoFang_RtcSet(Sys_keydata.trigger_interval_minute_count));
  ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_3);//ע������ѡ��оƬ�ͺ�HD���ײ�ָ�����RTC_IRQn��ΪRTC_CSSLSE_IRQn
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
  
  //RTC_SetWakeUpCounter((63750/30)*interval_count);//���ּ�ʱ
  RTC_SetWakeUpCounter((70125/1800)*interval_second_count);//�����ʱ
  
  ITC_SetSoftwarePriority(RTC_CSSLSE_IRQn, ITC_PriorityLevel_3);//
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  RTC_WakeUpCmd(ENABLE);
}

