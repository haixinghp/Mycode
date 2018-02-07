//.h头文件
/***************************************************************************************
**文件名:     
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   版本     修改者    时间                     
**						
**备注:       2016/3/23
****************************************************************************************/
#ifndef   __XIAOFANG_H__
#define   __XIAOFANG_H__
#include "config.h"

#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
  typedef struct
  {
  //  u16 timesA_cnt;  //检测计数器
  //  u16 timesA_limit;      //定时时间。
    u16 timesB_cnt;  //运行计数器
    u16 timesB_limit;      //定时时间。
  //  u8 timesA_over_flag;   //定时超时标志。
    u8 timesB_over_flag:1;   //定时超时标志。
    u8 times_Read:1;      //定时器已读取标志。 
    u8 times_EnableFlag:1;      //定时器使能标志。
    u8 StratAlarm:1;  //开始报警标志
    u8 TriggerStatus:2; //边缘的状态，0，没有边缘触发，1上升沿，2下降沿
    bool RtcWakeFlag; //来自与RTC唤醒标志。
    /*无水到有水告警后置位，有水到无水后清除，或者是过了告警间隔后清除标志。
  置位后无法发送出水告警，复位后无法发送清除出水告警*/
  bool HaveWaterAlarmFalg; 
  }_struXiaoFangTimes;

#define TRIGGER_NOT (0)
#define TRIGGER_RISING (1)
#define TRIGGER_FALLING  (2)

#endif 

#define SJ_SWITCH_GPIO (GPIOA)
#define SJ_SWITCH_PIN (GPIO_Pin_5)

#define SJ_OUTSTATUS_GPIO (GPIOA)
#define SJ_OUTSTATUS_PIN (GPIO_Pin_6)
#define SJ_OUTSTATUS_EXTI_PIN (EXTI_Pin_6)

#define SJ_DELAY (DelayT_ms(500)) 
#define SJ_WAIT_TIME (500)

//typedef struct
//{
//  u16 openAlarm:1;
//  u16 waterAlarm:1;
//  u16 laenAlarm:1;
//  u16 destroyAlarm:1;
//  u16 pressureAlarm:1;
//  u16 powershortageAlarm:1;
//  u16 breakdownAlarm:1;
//  u16 pressureAlarmUpDownLimit:2; //上下限报警
//}_struAlarmBit;
//
//union _uniAlarm
//{
//  _struAlarmBit struAlarmBit;
//  u16 AlarmAllData;
//};



#define XIAOFANG_RTC_MAX_TIME (30) // RTC最大定时时间

//#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码

#define SJ_SWITCH_GPIO (GPIOA)
#define SJ_SWITCH_PIN (GPIO_Pin_5)

#define SJ_OUTSTATUS_GPIO (GPIOA)
#define SJ_OUTSTATUS_PIN (GPIO_Pin_6)
#define SJ_OUTSTATUS_EXTI_PIN (EXTI_Pin_6)

#define SJ_DELAY (DelayT_ms(500)) 
#define SJ_WAIT_TIME (500)
#define SJ_SWITCH_OPEN  (GPIO_Init(GPIOA,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Slow),GPIO_ResetBits(GPIOA,GPIO_Pin_5))
#define SJ_SWITCH_CLOSE  (GPIO_Init(GPIOA,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Slow),GPIO_SetBits(GPIOA,GPIO_Pin_5))

#define SJ_GET_OUTSTATUS (RESET != GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)) //高电平的时候表示有水。

#define SJ_DELAY (DelayT_ms(500)) 
#define SJ_WAIT_TIME (500)

//#endif 


//#include "xiaofang.h"
typedef struct 
{
  u16 over_time;//超时时间
  s8 zero_angle;//零点角度
  u8 set_angle;//设定角度
  s8 present_angle;//当前角度
  s8 valid_angle_d_value;//有效角度的相互差值
  u8 set_valid_angle_unbroken_num;//设定的最长有效角度连续数目
  u8 valid_angle_unbroken_num;//最长有效角度连续数目
  u8 analyse_state;//状态机状态值
  s16 pre_temp_z_angle; //上一次的角度数值。 
  u8 inact_flag : 1; //静态标志
  u8 alarm_flag : 1; //报警标志
} ANALYSE_DATA_PARA_XIAOFANG;

#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
typedef struct
{
//  u16 timesA_cnt;  //检测计数器
//  u16 timesA_limit;      //定时时间。
  u16 timesB_cnt;  //运行计数器
  u16 timesB_limit;      //定时时间。
//  u8 timesA_over_flag;   //定时超时标志。
  u8 timesB_over_flag;   //定时超时标志。
  u8 times_Read;      //定时器已读取标志。 
  u8 times_EnableFlag;      //定时器使能标志。
}_struXiaoFangTimes; 

#endif


#ifdef  __cplusplus
extern "C" {
#endif

#ifdef XIAOFANG_GLOBALS
#define XIAOFANG_EXT
#else
#define XIAOFANG_EXT extern
#endif
  XIAOFANG_EXT ANALYSE_DATA_PARA_XIAOFANG  Analyse_data_para_XiaoFangZ;
  XIAOFANG_EXT ANALYSE_DATA_PARA_XIAOFANG  Analyse_data_para_XiaoFangX;
  XIAOFANG_EXT _struXiaoFangTimes struXiaoFangTimes;
  XIAOFANG_EXT _struXiaoFangTimes struXiaoFangPressTimes;
  XIAOFANG_EXT _struXiaoFangTimes struXiaoFangAlarmTimes; //报警间隔控制器
  
  XIAOFANG_EXT u8 XiaoFang_Analyse(ANALYSE_DATA_PARA_XIAOFANG *pAnalyse);
  XIAOFANG_EXT void XiaoFang_ClsAnalyseState(ANALYSE_DATA_PARA_XIAOFANG *pAnalyse);
  XIAOFANG_EXT void XiaoFang_OpenDetection(void);
  XIAOFANG_EXT void XiaoFang_TimerService(_struXiaoFangTimes *parP );
  XIAOFANG_EXT u8 XiaoFang_DetectionWait(_struXiaoFangTimes *parP );
  XIAOFANG_EXT void XiaoFang_TimerOverFlagClear(_struXiaoFangTimes *parP );
  XIAOFANG_EXT void XiaoFang_OpenDetection(void);
  XIAOFANG_EXT u16 XiaoFang_HeartdataSet(SYS_KEYDATA *parP);
  XIAOFANG_EXT u16 XiaoFang_RtcSet(u16 para);
  XIAOFANG_EXT void XiaoFang_TimerInit(_struXiaoFangTimes *parP );
  XIAOFANG_EXT void XiaoFang_TimerOverFlagClear(_struXiaoFangTimes *parP);
  XIAOFANG_EXT void XiaoFang_RTCService(_struXiaoFangTimes *parP );
  XIAOFANG_EXT void XiaoFang_RTCInit(_struXiaoFangTimes *parP,u16 parTime);
  
  
  XIAOFANG_EXT void XiaoFang_Interrupt_Init();
//  XIAOFANG_EXT s8 WaterDetection_Service();
  
  XIAOFANG_EXT void XiaoFang_SetRtcWakeFlag();
  XIAOFANG_EXT void XiaoFang_ClearRtcWakeFlag();
  XIAOFANG_EXT bool XiaoFang_GetRtcWakeFlag();
  //XIAOFANG_EXT void YY_Init(void);
//XIAOFANG_EXT void YY_Service(void);
//XIAOFANG_EXT void YY_UART_Service(void);
//XIAOFANG_EXT void YY_Timer_Service(void);
#ifdef  __cplusplus
}
#endif
#endif
/************************************Code End******************************************/