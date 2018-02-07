/***************************************************************************************
**文件名:     
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   版本     修改者    时间                     
**
**文件使用说明：
初始化：NewInactAngle_InitPara();
启用NewInactAngle_Service();
需要调用 NewInactAngle_ClearSamplingLockFlag(); //清除禁止检查锁定。
在发送数据，或者在特定的任务阶段调用 NewInactAngle_SetSamplingLockFlag();  //进入动作判断的时候，不进行静止判断。

用于静态标志置位，外部接口。
NEWINACTANGLE_EXT void NewInactAngle_SetStaticFlag();  
这个函数用于 stm8l15x_it.c 中 INTERRUPT_HANDLER(EXTI2_IRQHandler, 10) 中断中用于判断三轴静止中断是否产生。
  NEWINACTANGLE_EXT void NewInactAngle_ClearStaticFlag();
被void Cover_WakeIntService(void) 调用 stm8l15x_it.c 中 INTERRUPT_HANDLER(EXTI3_IRQHandler, 11) 三轴发生动作后，
  NEWINACTANGLE_EXT bool NewInactAngle_GetStaticFlag();
sys.c 调用，用于获取到静止标志。

  NEWINACTANGLE_EXT void NewInactAngle_ClearNewAngleEndFlag();
  NEWINACTANGLE_EXT bool NewInactAngle_GetNewAngleEndFlag();
  
**备注:       2017-08-09
****************************************************************************************/
#ifndef   __NEWINACTANGLE_H__
#define   __NEWINACTANGLE_H__
//	#include "NewInactAngle.h"
#include "stm8l15x.h"

#define ANGLESAMECNT_LIMIT (10) //相同角度次数
#define TRYCNT_LIMIT (10)        //尝试次数
#define INIT_VALUE (0) //初始值
#define ANGLE_CHANGE_LIMIT (1)
#define TASHSCHEDULE_RUN (0) //非协议状态
#define SYSFUNSTATE_HALT (4)    //休眠状态
#define SAMPLINGTIMECNT_LIMIT (101) //采样定时。
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
  s8 PreNewAngleTmpZ;  //以前一个角度的
  s8 NewAngleTmpZ;  //以前一个角度的
  s8 PreNewAngleTmpX;  //以前一个角度的
  s8 NewAngleTmpX;  //以前一个角度的
  s8 PreNewAngleTmpY;  //以前一个角度的
  s8 NewAngleTmpY;  //以前一个角度的
  u16 SamplingTimeCnt; //角度采样时间计数器
  u8  AngleSameCnt;     //相同角度计数器
  u8 TryCnt;    //尝试次数计数器
  bool StaticFlag; //产生动作标志。
  bool NewAngleEndFlag; //静止角度获取状态结束标志。
  bool SamplingTimeOverFlag; //角度采样时间超时
  bool SamplingLockFlag; //采样锁定标志。
}_struNewInactAngleControl;

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef NEWINACTANGLE_GLOBALS
#define NEWINACTANGLE_EXT
#else
#define NEWINACTANGLE_EXT extern
#endif
/*************************公共变量*************************/  
  NEWINACTANGLE_EXT _struNewInactAngleControl struNewInactAngleControl;
//  NEWINACTANGLE_EXT u8 NewInactAngle_GetTashSchedule_State(void);
//  NEWINACTANGLE_EXT u8 NewInactAngle_GetsysFunState(void);
//  NEWINACTANGLE_EXT void NewInactAngle_SetZeroAngle(u8 praA);
/*************************公共函数*************************/    
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