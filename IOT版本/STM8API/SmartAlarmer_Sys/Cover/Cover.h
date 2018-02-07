/***************************************************************************************
**文件名:     
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   版本     修改者    时间                     
**						
**备注:       2016/3/23
****************************************************************************************/
#ifndef   __COVER_H__
#define   __COVER_H__

#include "config.h"
//#include "Cover.h"

#ifdef  __cplusplus
extern "C" {
#endif

#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
  typedef enum 
  {
    CoverSysFunState_Idle, //空闲
    CoverSysFunState_Initialize,//初始化
    CoverSysFunState_Detection,//检测
    CoverSysFunState_AnalyzeACC,//分析加速度
    CoverSysFunState_SendData,//发送数据
    CoverSysFunState_WaitInterLPM,//等待进入低功耗模式
    CoverSysFunState_InterLPM,//进入低功耗模式
    CoverSysFunState_Exit,//退出
  }CoverSysFunState_TypeDef;
  typedef enum 
  {
    CoverWaterType_None,
    CoverWaterType_Level,
    CoverWaterType_Trigger
  }CoverWaterType_TypeDef;
  
  typedef struct
  {
    CoverWaterType_TypeDef CoverWaterType; //出水告警类型
    bool InterruptType; //中断类型，TRUE下降沿，FALSE非下降沿
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
  u16 pressureAlarmUpDownLimit:2; //上下限报警
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
/*************************公共变量*************************/  
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
COVER_EXT _uniAlarm uniAlarm;
COVER_EXT _struCoverControl  struCoverControl;
COVER_EXT CoverSysFunState_TypeDef CoverSysFunState;
#endif
/*************************公共函数*************************/
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