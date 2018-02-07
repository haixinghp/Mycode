/***************************************************************************************
**�ļ���:     
**����:       Haixing.Huang
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   �汾     �޸���    ʱ��                     
**
**�ļ�ʹ��˵����
��ʼ����NewInactAngle_InitPara();
����NewInactAngle_Service();
��Ҫ���� NewInactAngle_ClearSamplingLockFlag(); //�����ֹ���������
�ڷ������ݣ��������ض�������׶ε��� NewInactAngle_SetSamplingLockFlag();  //���붯���жϵ�ʱ�򣬲����о�ֹ�жϡ�

���ھ�̬��־��λ���ⲿ�ӿڡ�
NEWINACTANGLE_EXT void NewInactAngle_SetStaticFlag();  
����������� stm8l15x_it.c �� INTERRUPT_HANDLER(EXTI2_IRQHandler, 10) �ж��������ж����ᾲֹ�ж��Ƿ������
  NEWINACTANGLE_EXT void NewInactAngle_ClearStaticFlag();
��void Cover_WakeIntService(void) ���� stm8l15x_it.c �� INTERRUPT_HANDLER(EXTI3_IRQHandler, 11) ���ᷢ��������
  NEWINACTANGLE_EXT bool NewInactAngle_GetStaticFlag();
sys.c ���ã����ڻ�ȡ����ֹ��־��

  NEWINACTANGLE_EXT void NewInactAngle_ClearNewAngleEndFlag();
  NEWINACTANGLE_EXT bool NewInactAngle_GetNewAngleEndFlag();
  
**��ע:       2017-08-09
****************************************************************************************/
#ifndef   __NEWINACTANGLE_H__
#define   __NEWINACTANGLE_H__
//	#include "NewInactAngle.h"
#include "stm8l15x.h"

#define ANGLESAMECNT_LIMIT (10) //��ͬ�Ƕȴ���
#define TRYCNT_LIMIT (10)        //���Դ���
#define INIT_VALUE (0) //��ʼֵ
#define ANGLE_CHANGE_LIMIT (1)
#define TASHSCHEDULE_RUN (0) //��Э��״̬
#define SYSFUNSTATE_HALT (4)    //����״̬
#define SAMPLINGTIMECNT_LIMIT (101) //������ʱ��
typedef enum AngleState
{
  NewInactAngleState_Idle,
  NewInactAngleState_Initialize,
  NewInactAngleState_FirstAwake,
  NewInactAngleState_GetPreNewAngleTmp,
  NewInactAngleState_VerifyNewAngle,
  NewInactAngleState_Exit,
}NewInactAngleState_TypeDef;
typedef struct 
{
  NewInactAngleState_TypeDef NewInactAngleState;
  s8 PreNewAngleTmpZ;  //��ǰһ���Ƕȵ�
  s8 NewAngleTmpZ;  //��ǰһ���Ƕȵ�
  s8 PreNewAngleTmpX;  //��ǰһ���Ƕȵ�
  s8 NewAngleTmpX;  //��ǰһ���Ƕȵ�
  s8 PreNewAngleTmpY;  //��ǰһ���Ƕȵ�
  s8 NewAngleTmpY;  //��ǰһ���Ƕȵ�
  u16 SamplingTimeCnt; //�ǶȲ���ʱ�������
  u8  AngleSameCnt;     //��ͬ�Ƕȼ�����
  u8 TryCnt;    //���Դ���������
  bool StaticFlag; //����������־��
  bool NewAngleEndFlag; //��ֹ�ǶȻ�ȡ״̬������־��
  bool SamplingTimeOverFlag; //�ǶȲ���ʱ�䳬ʱ
  bool SamplingLockFlag; //����������־��
}_struNewInactAngleControl;

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef NEWINACTANGLE_GLOBALS
#define NEWINACTANGLE_EXT
#else
#define NEWINACTANGLE_EXT extern
#endif
/*************************��������*************************/  
  NEWINACTANGLE_EXT _struNewInactAngleControl struNewInactAngleControl;
//  NEWINACTANGLE_EXT u8 NewInactAngle_GetTashSchedule_State(void);
//  NEWINACTANGLE_EXT u8 NewInactAngle_GetsysFunState(void);
//  NEWINACTANGLE_EXT void NewInactAngle_SetZeroAngle(u8 praA);
/*************************��������*************************/    
  NEWINACTANGLE_EXT void NewInactAngle_InitPara();
  
  NEWINACTANGLE_EXT void NewInactAngle_SetStaticFlag();
  NEWINACTANGLE_EXT void NewInactAngle_ClearStaticFlag();
  NEWINACTANGLE_EXT bool NewInactAngle_GetStaticFlag();
  
  NEWINACTANGLE_EXT void NewInactAngle_SetSamplingLockFlag();
  NEWINACTANGLE_EXT void NewInactAngle_ClearSamplingLockFlag();
  
  NEWINACTANGLE_EXT void NewInactAngle_ClearNewAngleEndFlag();
  NEWINACTANGLE_EXT bool NewInactAngle_GetNewAngleEndFlag();
  
  NEWINACTANGLE_EXT void NewInactAngle_TimerService();
  NEWINACTANGLE_EXT void NewInactAngle_TimerSet();
  
  NEWINACTANGLE_EXT void NewInactAngle_ClearNewAngleEndFlag();
  NEWINACTANGLE_EXT bool NewInactAngle_GetNewAngleEndFlag();
  
  NEWINACTANGLE_EXT void NewInactAngle_Service(void);
#ifdef  __cplusplus
}
#endif
#endif
/************************************Code End******************************************/