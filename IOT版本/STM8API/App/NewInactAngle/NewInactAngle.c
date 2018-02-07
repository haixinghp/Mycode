#include "NewInactAngle.h"
#include "config.h"

_struNewInactAngleControl struNewInactAngleControl;


/***************************************************************************************
**函数名:       NewInactAngle_InitPara()
**功能描述:     初始化静止数据状态机 
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_InitPara()
{
  struNewInactAngleControl.NewInactAngleState = NewInactAngleState_Initialize;
}
/***************************************************************************************
**函数名:       NewInactAngle_GetTashSchedule_State
**功能描述:     获取当前的任务工作状态。
**输入参数:     --
**输出参数:     任务工作状态。
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
u8 NewInactAngle_GetTashSchedule_State(void)
{
  return TashSchedule_State;
}

/***************************************************************************************
**函数名:       NewInactAngle_GetsysFunState
**功能描述:     获取sysFunState状态
**输入参数:     --
**输出参数:     sysFunState状态
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
u8 NewInactAngle_GetsysFunState(void)
{
  return sysFunState;
}

/***************************************************************************************
**函数名:       NewInactAngle_SetZeroAngle 
**功能描述:     设置零点角度
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_SetZeroAngle(u8 praZ,u8 praX,u8 praY)
{
//  Analyse_data_para.zero_angle = praA;
  Analyse_data_para_XiaoFangZ.zero_angle = praZ;
  Analyse_data_para_XiaoFangX.zero_angle = praX;
  
}

/***************************************************************************************
**函数名:       NewInactAngle_SetStaticFlag
**功能描述:     静态标志置位，外部接口。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_SetStaticFlag()
{
  struNewInactAngleControl.StaticFlag =TRUE;
}

/***************************************************************************************
**函数名:       NewInactAngle_ClearStaticFlag
**功能描述:     静态标志复位，外部接口。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_ClearStaticFlag()
{
  struNewInactAngleControl.StaticFlag =FALSE;
}
/***************************************************************************************
**函数名:       NewInactAngle_GetStaticFlag
**功能描述:     静态标志获取，外部接口。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
bool NewInactAngle_GetStaticFlag()
{
  return struNewInactAngleControl.StaticFlag;
}
/***************************************************************************************
**函数名:       
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_ClearNewAngleEndFlag()
{
  struNewInactAngleControl.NewAngleEndFlag = FALSE;
}
/***************************************************************************************
**函数名:       
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
bool NewInactAngle_GetNewAngleEndFlag()
{
  return struNewInactAngleControl.NewAngleEndFlag;
}
/***************************************************************************************
**函数名:       NewInactAngle_TimerService
**功能描述:     定时器服务程序，使用时要添加到相应的定时器中断服务程序中。 
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_TimerService()
{
  if(0 != struNewInactAngleControl.SamplingTimeCnt)
  {
    if(1 == struNewInactAngleControl.SamplingTimeCnt)
    {
      struNewInactAngleControl.SamplingTimeOverFlag= TRUE;  
    }
    struNewInactAngleControl.SamplingTimeCnt --;
  } 
}
/***************************************************************************************
**函数名:       
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_TimerSet()
{
  struNewInactAngleControl.SamplingTimeCnt = SAMPLINGTIMECNT_LIMIT; 
}

/***************************************************************************************
**函数名:         --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
bool NewInactAngle_WhetherWake(void)
{
  if(TASHSCHEDULE_RUN == NewInactAngle_GetTashSchedule_State()) //在非协议分析状态        
  {
    if(SYSFUNSTATE_HALT != NewInactAngle_GetsysFunState())      //在非休眠状态时
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
    
  }
  else
  {
    return FALSE;
  }
}
/***************************************************************************************
**函数名:         --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_ClearPara(void)
{
  struNewInactAngleControl.PreNewAngleTmpX = INIT_VALUE;
  struNewInactAngleControl.NewAngleTmpX = INIT_VALUE;
  struNewInactAngleControl.PreNewAngleTmpZ = INIT_VALUE;
  struNewInactAngleControl.NewAngleTmpZ = INIT_VALUE;
  struNewInactAngleControl.PreNewAngleTmpY = INIT_VALUE;
  struNewInactAngleControl.NewAngleTmpY = INIT_VALUE;
  struNewInactAngleControl.AngleSameCnt = INIT_VALUE;
  struNewInactAngleControl.TryCnt = INIT_VALUE;
}
/***************************************************************************************
**函数名:       NewInactAngle_ClearSamplingLockFlag
**功能描述:     置位SamplingLockFlag
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_SetSamplingLockFlag()
{
  struNewInactAngleControl.SamplingLockFlag = TRUE;
}
/***************************************************************************************
**函数名:       NewInactAngle_ClearSamplingLockFlag
**功能描述:     清除SamplingLockFlag
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_ClearSamplingLockFlag()
{
  struNewInactAngleControl.SamplingLockFlag = FALSE;
}
/***************************************************************************************
**函数名:       NewInactAngle_Service
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_Service(void)
{
  if(FALSE == struNewInactAngleControl.SamplingLockFlag) //在锁定的条件下不进行数据静止的判断。
  {
    if ((ADXL362_ReadReg(ADXL362_REG_STATUS)&ADXL362_STATUS_AWAKE)) //主动查询模块的方式
    {
      struNewInactAngleControl.NewInactAngleState = NewInactAngleState_Initialize; //进入初始阶段
    }
    if(FALSE == struNewInactAngleControl.StaticFlag)
    {
      struNewInactAngleControl.NewInactAngleState = NewInactAngleState_Initialize; //进入初始阶段
    }
    switch( struNewInactAngleControl.NewInactAngleState)
    {
    case NewInactAngleState_Idle:  //空闲
      break;
    case NewInactAngleState_Initialize: //初始化
      struNewInactAngleControl.NewInactAngleState = NewInactAngleState_FirstAwake;
      NewInactAngle_ClearPara();
      break;
    case NewInactAngleState_FirstAwake: //从传感器中断获取到静止中断信号标志。
      if(TRUE == struNewInactAngleControl.StaticFlag)
      {
        struNewInactAngleControl.NewInactAngleState = NewInactAngleState_GetPreNewAngleTmp; //进入获取新角度状态
      }
      break;
    case NewInactAngleState_GetPreNewAngleTmp:
      if (!(ADXL362_ReadReg(ADXL362_REG_STATUS)&ADXL362_STATUS_AWAKE))  //检查是否是静止状态
      {
        struNewInactAngleControl.PreNewAngleTmpZ = Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_Z);
        struNewInactAngleControl.PreNewAngleTmpX = Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_X);
        PrintString("PreNewAngleTmpZ:");
        printNum2Str(struNewInactAngleControl.PreNewAngleTmpZ);
        PrintString("PreNewAngleTmpX:");
        printNum2Str(struNewInactAngleControl.PreNewAngleTmpX);
        struNewInactAngleControl.NewInactAngleState = NewInactAngleState_VerifyNewAngle;
        //这里开启软件定时器
        NewInactAngle_TimerSet(); //开启软件定时器。
      }
      else
      {
        struNewInactAngleControl.NewInactAngleState = NewInactAngleState_Initialize; //进入初始阶段
      }
      break;  
    case NewInactAngleState_VerifyNewAngle:
//      if(NewInactAngle_WhetherWake()) //获取工作状态,如果处于其他工作状态，清除所有的状态。
//      {
//        NewInactAngle_ClearPara();
//        break;
//      }
      if(TRUE == struNewInactAngleControl.SamplingTimeOverFlag )  //检测时间超时标志
      {
        struNewInactAngleControl.SamplingTimeOverFlag = FALSE; //清除时间超时标志
        struNewInactAngleControl.NewAngleTmpZ = Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_Z);
        struNewInactAngleControl.NewAngleTmpX = Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_X);
        if((abs(struNewInactAngleControl.NewAngleTmpZ - struNewInactAngleControl.PreNewAngleTmpZ) <=ANGLE_CHANGE_LIMIT)&&
           (abs(struNewInactAngleControl.NewAngleTmpX - struNewInactAngleControl.PreNewAngleTmpX) <=ANGLE_CHANGE_LIMIT)
             )
        {
          struNewInactAngleControl.AngleSameCnt++;
          PrintString(">>>AngleSameCnt:");
          printNum2Str(struNewInactAngleControl.AngleSameCnt);
          PrintString(">>>NewAngleTmpZ:");
          printNum2Str(struNewInactAngleControl.NewAngleTmpZ);
          PrintString(">>>NewAngleTmpX:");
          printNum2Str(struNewInactAngleControl.NewAngleTmpX);
          if(struNewInactAngleControl.AngleSameCnt >= ANGLESAMECNT_LIMIT) //连续ANGLESAMECNT_LIMIT次 都取到一样的角度值 说明角度切换正常
          {
            PrintString("!!!!!!!!!!!New AgnleZ:");
            printNum2Str(struNewInactAngleControl.NewAngleTmpZ);
            PrintString("!!!!!!!!!!!New AgnleX:");
            printNum2Str(struNewInactAngleControl.NewAngleTmpX); 
            NewInactAngle_SetZeroAngle(struNewInactAngleControl.NewAngleTmpZ,struNewInactAngleControl.NewAngleTmpX,0); //配置零点
            NewInactAngle_ClearPara();
            struNewInactAngleControl.NewInactAngleState = NewInactAngleState_Exit; //退出
            break; //强制退出
          }
        }
        else
        {
          struNewInactAngleControl.PreNewAngleTmpZ  = struNewInactAngleControl.NewAngleTmpZ;
          struNewInactAngleControl.PreNewAngleTmpX  = struNewInactAngleControl.NewAngleTmpX;
          PrintString("PreNewAngleTmpZ:");
          printNum2Str(struNewInactAngleControl.PreNewAngleTmpZ);
          PrintString("PreNewAngleTmpX:");
          printNum2Str(struNewInactAngleControl.PreNewAngleTmpX);
          struNewInactAngleControl.AngleSameCnt = INIT_VALUE;
          struNewInactAngleControl.TryCnt ++;
          if(struNewInactAngleControl.TryCnt > TRYCNT_LIMIT)
          {
            NewInactAngle_ClearPara();
            struNewInactAngleControl.NewInactAngleState = NewInactAngleState_Exit;//退出
            break; //强制退出
          }
          PrintString("<<<<<<<<<<TryCnt:");
          printNum2Str(struNewInactAngleControl.TryCnt);
        }
        NewInactAngle_TimerSet(); //开启软件定时器。
        //时间重填
      }
      break;    
    case NewInactAngleState_Exit:
      struNewInactAngleControl.NewAngleEndFlag = TRUE;
      struNewInactAngleControl.NewInactAngleState = NewInactAngleState_Idle;
      break;     
    default:
      
      break;
    }
  }
  else
  {
    NewInactAngle_ClearPara();
  }
}