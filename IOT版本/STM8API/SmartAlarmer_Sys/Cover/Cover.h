/***************************************************************************************
**�ļ���:     
**����:       Haixing.Huang
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   �汾     �޸���    ʱ��                     
**						
**��ע:       2016/3/23
****************************************************************************************/
#ifndef   __COVER_H__
#define   __COVER_H__

#include "config.h"
//#include "Cover.h"

#ifdef  __cplusplus
extern "C" {
#endif

#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
  typedef enum 
  {
    CoverSysFunState_Idle, //����
    CoverSysFunState_Initialize,//��ʼ��
    CoverSysFunState_Detection,//���
    CoverSysFunState_AnalyzeACC,//�������ٶ�
    CoverSysFunState_SendData,//��������
    CoverSysFunState_WaitInterLPM,//�ȴ�����͹���ģʽ
    CoverSysFunState_InterLPM,//����͹���ģʽ
    CoverSysFunState_Exit,//�˳�
  }CoverSysFunState_TypeDef;
  typedef enum 
  {
    CoverWaterType_None,
    CoverWaterType_Level,
    CoverWaterType_Trigger
  }CoverWaterType_TypeDef;
  
  typedef struct
  {
    CoverWaterType_TypeDef CoverWaterType; //��ˮ�澯����
    bool InterruptType; //�ж����ͣ�TRUE�½��أ�FALSE���½���
  }_struCoverControl;
  typedef struct  
{
  u16 openAlarm:1;
  u16 waterAlarm:1;
  u16 laenAlarm:1;
  u16 destroyAlarm:1;
  u16 pressureAlarm:1;
  u16 powershortageAlarm:1;
  u16 breakdownAlarm:1;
  u16 pressureAlarmUpDownLimit:2; //�����ޱ���
  u16 WithoutPressureSensors:1;
  u16 KeyUpdata:1;
  
}_struAlarmBit;

typedef union 
{
  _struAlarmBit struAlarmBit;
  u16 AlarmAllData;
}_uniAlarm;
  
#endif
  
#ifdef COVER_GLOBALS
#define COVER_EXT
#else
#define COVER_EXT extern
#endif
/*************************��������*************************/  
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
COVER_EXT _uniAlarm uniAlarm;
COVER_EXT _struCoverControl  struCoverControl;
COVER_EXT CoverSysFunState_TypeDef CoverSysFunState;
#endif
/*************************��������*************************/
COVER_EXT void Cover_WakeIntService(void);
COVER_EXT s8 WaterDetection_Service();
COVER_EXT void CoverInterrupt_Service();

COVER_EXT void SetHaveWaterAlarmFalg(void);
COVER_EXT void ReSetHaveWaterAlarmFalg(void);
COVER_EXT bool GetHaveWaterAlarmFalg();
#ifdef  __cplusplus
}
#endif
#endif