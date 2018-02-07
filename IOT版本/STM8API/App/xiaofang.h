//.hͷ�ļ�
/***************************************************************************************
**�ļ���:     
**����:       Haixing.Huang
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   �汾     �޸���    ʱ��                     
**						
**��ע:       2016/3/23
****************************************************************************************/
#ifndef   __XIAOFANG_H__
#define   __XIAOFANG_H__
#include "config.h"

#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
  typedef struct
  {
  //  u16 timesA_cnt;  //��������
  //  u16 timesA_limit;      //��ʱʱ�䡣
    u16 timesB_cnt;  //���м�����
    u16 timesB_limit;      //��ʱʱ�䡣
  //  u8 timesA_over_flag;   //��ʱ��ʱ��־��
    u8 timesB_over_flag:1;   //��ʱ��ʱ��־��
    u8 times_Read:1;      //��ʱ���Ѷ�ȡ��־�� 
    u8 times_EnableFlag:1;      //��ʱ��ʹ�ܱ�־��
    u8 StratAlarm:1;  //��ʼ������־
    u8 TriggerStatus:2; //��Ե��״̬��0��û�б�Ե������1�����أ�2�½���
    bool RtcWakeFlag; //������RTC���ѱ�־��
    /*��ˮ����ˮ�澯����λ����ˮ����ˮ������������ǹ��˸澯����������־��
  ��λ���޷����ͳ�ˮ�澯����λ���޷����������ˮ�澯*/
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
//  u16 pressureAlarmUpDownLimit:2; //�����ޱ���
//}_struAlarmBit;
//
//union _uniAlarm
//{
//  _struAlarmBit struAlarmBit;
//  u16 AlarmAllData;
//};



#define XIAOFANG_RTC_MAX_TIME (30) // RTC���ʱʱ��

//#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���

#define SJ_SWITCH_GPIO (GPIOA)
#define SJ_SWITCH_PIN (GPIO_Pin_5)

#define SJ_OUTSTATUS_GPIO (GPIOA)
#define SJ_OUTSTATUS_PIN (GPIO_Pin_6)
#define SJ_OUTSTATUS_EXTI_PIN (EXTI_Pin_6)

#define SJ_DELAY (DelayT_ms(500)) 
#define SJ_WAIT_TIME (500)
#define SJ_SWITCH_OPEN  (GPIO_Init(GPIOA,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Slow),GPIO_ResetBits(GPIOA,GPIO_Pin_5))
#define SJ_SWITCH_CLOSE  (GPIO_Init(GPIOA,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Slow),GPIO_SetBits(GPIOA,GPIO_Pin_5))

#define SJ_GET_OUTSTATUS (RESET != GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)) //�ߵ�ƽ��ʱ���ʾ��ˮ��

#define SJ_DELAY (DelayT_ms(500)) 
#define SJ_WAIT_TIME (500)

//#endif 


//#include "xiaofang.h"
typedef struct 
{
  u16 over_time;//��ʱʱ��
  s8 zero_angle;//���Ƕ�
  u8 set_angle;//�趨�Ƕ�
  s8 present_angle;//��ǰ�Ƕ�
  s8 valid_angle_d_value;//��Ч�Ƕȵ��໥��ֵ
  u8 set_valid_angle_unbroken_num;//�趨�����Ч�Ƕ�������Ŀ
  u8 valid_angle_unbroken_num;//���Ч�Ƕ�������Ŀ
  u8 analyse_state;//״̬��״ֵ̬
  s16 pre_temp_z_angle; //��һ�εĽǶ���ֵ�� 
  u8 inact_flag : 1; //��̬��־
  u8 alarm_flag : 1; //������־
} ANALYSE_DATA_PARA_XIAOFANG;

#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
typedef struct
{
//  u16 timesA_cnt;  //��������
//  u16 timesA_limit;      //��ʱʱ�䡣
  u16 timesB_cnt;  //���м�����
  u16 timesB_limit;      //��ʱʱ�䡣
//  u8 timesA_over_flag;   //��ʱ��ʱ��־��
  u8 timesB_over_flag;   //��ʱ��ʱ��־��
  u8 times_Read;      //��ʱ���Ѷ�ȡ��־�� 
  u8 times_EnableFlag;      //��ʱ��ʹ�ܱ�־��
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
  XIAOFANG_EXT _struXiaoFangTimes struXiaoFangAlarmTimes; //�������������
  
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