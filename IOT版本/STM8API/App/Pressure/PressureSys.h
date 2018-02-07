/***************************************************************************************
**�ļ���:     
**����:       Haixing.Huang
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   �汾     �޸���    ʱ��                     
**						
**��ע:       2016/3/23
****************************************************************************************/
#ifndef   __PRESSURESYS_H__
#define   __PRESSURESYS_H__
#include "config.h"
//#define PRESSURESYS_MODULE_EN (1) //ģ��ʹ��
#if  PRESSURESYS_MODULE_EN
#define ENTER_SENDALARM_STATUS_EN 1
#if ENTER_SENDALARM_STATUS_EN 
#define ENTER_SENDALARM_STATUS (PressureSysFunState = PressureSysFunState_SendData)
#else
#define ENTER_SENDALARM_STATUS 
#endif
typedef struct
{
  s32 LockTimeUpLimit;//Ŀ��ʱ�䡣
  s32 LockTimeDownLimit;//Ŀ��ʱ�䡣
  s32 LockTimeNoSensor;//Ŀ��ʱ�䡣
  s32 LockTimeResolution;//Ŀ��ʱ�䡣
  s32 LockTimeTouchOff;
}_strAlarmSentInterval;

typedef enum 
{
  PressureSysFunState_Idle, //����
  PressureSysFunState_Initialize,//��ʼ��
  PressureSysFunState_Detection,//���
  PressureSysFunState_KeyDetection,//�������
  PressureSysFunState_getPressure,//��ȡѹ����
  PressureSysFunState_Analyse,//��ѹ�����з�����
  PressureSysFunState_WaitSendData,//����ǰ��׼��
  PressureSysFunState_SendData,//��������
  PressureSysFunState_WaitInterLPM,//�ȴ�����͹���ģʽ
  PressureSysFunState_InterLPM,//����͹���ģʽ
  PressureSysFunState_Exit,//�˳�
}PressureSysFunState_TypeDef;

typedef struct  
{
  u16 pressureAlarmUpLimit:1; //���ޱ���
  u16 pressureAlarmDownLimit:1;//���ޱ���
  u16 WithoutPressureSensors:1; //δ���Ӵ������澯
  u16 KeyUpdata:1; //�˹������澯��
  u16 pressureResolutionAlarm:1; //�ֱ����ϱ�
  u16 resetAlarm:1;//��λ�ϱ���
}_struAlarmBit;

typedef union 
{
  _struAlarmBit struAlarmBit;
  u16 AlarmAllData;
}_uniAlarm;

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef PRESSURESYS_GLOBALS
#define PRESSURESYS_EXT
#else
#define PRESSURESYS_EXT extern
#endif
/*************************��������*************************/  
PRESSURESYS_EXT _uniAlarm uniAlarm;
/*************************��������*************************/
PRESSURESYS_EXT void ResetUpdataInit();
PRESSURESYS_EXT void PressureSysFunStateInit();
PRESSURESYS_EXT s8 AlarmSentIntervalSet(s32 *locktime,u16 time);
PRESSURESYS_EXT s8 AlarmSentIntervalCheck(s32 *locktime);
#ifdef  __cplusplus
}
#endif
#endif
#endif
/************************************Code End******************************************/