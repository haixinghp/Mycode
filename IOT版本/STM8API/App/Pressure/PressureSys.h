/***************************************************************************************
**文件名:     
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   版本     修改者    时间                     
**						
**备注:       2016/3/23
****************************************************************************************/
#ifndef   __PRESSURESYS_H__
#define   __PRESSURESYS_H__
#include "config.h"
//#define PRESSURESYS_MODULE_EN (1) //模块使能
#if  PRESSURESYS_MODULE_EN
#define ENTER_SENDALARM_STATUS_EN 1
#if ENTER_SENDALARM_STATUS_EN 
#define ENTER_SENDALARM_STATUS (PressureSysFunState = PressureSysFunState_SendData)
#else
#define ENTER_SENDALARM_STATUS 
#endif
typedef struct
{
  s32 LockTimeUpLimit;//目标时间。
  s32 LockTimeDownLimit;//目标时间。
  s32 LockTimeNoSensor;//目标时间。
  s32 LockTimeResolution;//目标时间。
  s32 LockTimeTouchOff;
}_strAlarmSentInterval;

typedef enum 
{
  PressureSysFunState_Idle, //空闲
  PressureSysFunState_Initialize,//初始化
  PressureSysFunState_Detection,//检测
  PressureSysFunState_KeyDetection,//按键检测
  PressureSysFunState_getPressure,//获取压力。
  PressureSysFunState_Analyse,//对压力进行分析。
  PressureSysFunState_WaitSendData,//发送前的准备
  PressureSysFunState_SendData,//发送数据
  PressureSysFunState_WaitInterLPM,//等待进入低功耗模式
  PressureSysFunState_InterLPM,//进入低功耗模式
  PressureSysFunState_Exit,//退出
}PressureSysFunState_TypeDef;

typedef struct  
{
  u16 pressureAlarmUpLimit:1; //上限报警
  u16 pressureAlarmDownLimit:1;//下限报警
  u16 WithoutPressureSensors:1; //未连接传感器告警
  u16 KeyUpdata:1; //人工触发告警。
  u16 pressureResolutionAlarm:1; //分辨率上报
  u16 resetAlarm:1;//复位上报。
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
/*************************公共变量*************************/  
PRESSURESYS_EXT _uniAlarm uniAlarm;
/*************************公共函数*************************/
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