#include "Cover.h"
#include "config.h"

#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
_uniAlarm uniAlarm=
{
  0,
  0,
  0,
  0,
  0,
  0,
  0,
};

_uniAlarm uniAlarmBackup=
{
  0,
  0,
  0,
  0,
  0,
  0,
  0,
};
#warning 闷盖模块使能!
CoverSysFunState_TypeDef CoverSysFunState;
_struCoverControl  struCoverControl;  //需要初始化
#endif
/***************************************************************************************
**函数名:       Cover_WakeIntService
**功能描述:     闷盖中断服务唤醒服务程序。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-11-24,By Haixing.Huang
****************************************************************************************/
void Cover_WakeIntService(void)
{
  if (Adxl362Para.Read_flag == 0) 
  {
    Adxl362Para.Read_flag = 1;
    //    Analyse_data_para.inact_flag = 0;//静态标志置零
    NewInactAngle_ClearStaticFlag();
    NewInactAngle_ClearNewAngleEndFlag(); //清除静止标志。
    debug_send("waked");
  }
}

/***************************************************************************************
**函数名:       WaterDetection_Service;
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-01,By Haixing.Huang
****************************************************************************************/
s8 WaterDetection_Service()
{
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
  bool tmp_bool;
  if(SJ_GET_OUTSTATUS)
  {
    tmp_bool = TRUE;
  }
  else
  {
    tmp_bool = FALSE;
  }
  DelayT_ms(10);
  if(SJ_GET_OUTSTATUS)  
  {
    if(TRUE ==  tmp_bool)
    {
      uniAlarm.struAlarmBit.waterAlarm = 0;      //关闭告警
      debug_send("确认无水");
      if(TRIGGER_RISING == struXiaoFangAlarmTimes.TriggerStatus)
      {
        if(GetHaveWaterAlarmFalg()) 
        { 
          ReSetHaveWaterAlarmFalg();
          struXiaoFangAlarmTimes.StratAlarm = 1;  //发生告警。
        }
        else
        {
          struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
          debug_send("没有可取消的告警");
        }
      }
    } 
  }
  else
  {
    if(FALSE ==  tmp_bool)
    {
      uniAlarm.struAlarmBit.waterAlarm = 1;      //开启出水报警
      debug_send("确认有水");
      if(TRIGGER_FALLING == struXiaoFangAlarmTimes.TriggerStatus)
      {
        if(!GetHaveWaterAlarmFalg()) 
        {
//          Reset_alarmtime(&alarmtime_all.waterover,Sys_keydata.alarm_sent_interval_minute_count);
          if(Is_flag_alarmtime(&alarmtime_all.waterover))
          {
            SetHaveWaterAlarmFalg();
            struXiaoFangAlarmTimes.StratAlarm = 1;  //发生告警。
          }
          else
          {
            struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
            debug_send("在告警间隔内！");  //
          }
          
        }
        else
        {
          struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
          debug_send("需要清除告警");
        }
      }
    }
  }
#else
  return 0;
#endif 
  
}
/***************************************************************************************
**函数名:       CoverInterrupt_Service
**功能描述:     闷盖有水检测io口中断服务程序。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-11-23,By Haixing.Huang
****************************************************************************************/
void CoverInterrupt_Service()
{
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
  struCoverControl.InterruptType = TRUE;
    if(SJ_GET_OUTSTATUS)  //这里状态检测如何是高电平认为是上升沿。
  {
    struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_RISING;
    debug_send("水上升沿中断");
  }
  else  //这里状态检测如何是低电平认为是下降沿。
  {
    struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_FALLING;
    debug_send("水下降沿中断");
  }
#endif  
}
/***************************************************************************************
**函数名:       CoverSysFun
**功能描述:     闷盖业务流程程序
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-11-23,By Haixing.Huang
****************************************************************************************/
void CoverSysFun(void) 
{
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
 
   
  switch (CoverSysFunState)
  {
  case CoverSysFunState_Detection://进入检测ADXL模块的数据读取
    
    WaterDetection_Service();
    if (Adxl362Para.Read_flag == 1) //如果有阈值触发标志置位
    {
      sysFunTimecount = 0;
      CoverSysFunState = CoverSysFunState_AnalyzeACC; //进入加速度分析。
      PrintString_("开始获取三轴数据!");
      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//清除分析模式的所有状态
      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//清除分析模式的所有状态
      PrintString("Z轴零点角度:");
      PrintS8int2String_(Analyse_data_para_XiaoFangZ.zero_angle);
      PrintString("X轴零点角度:");
      PrintS8int2String_(Analyse_data_para_XiaoFangX.zero_angle);
      NewInactAngle_SetSamplingLockFlag();  //进入动作判断的时候，不进行静止判断。
    }
    else        //如果没有阈值触发
    {
      if (heart_send_flag) 
      {
        heart_send_flag = 0;   //清除心跳包
        CoverSysFunState = CoverSysFunState_SendData;
        debug_send("开始发送心跳包");
        break;
      }
      CoverSysFunState = CoverSysFunState_WaitInterLPM;
    }
    break;
  case CoverSysFunState_SendData:       //发送报警/心跳
    if(CoverWaterType_None != struCoverControl.CoverWaterType) //发生了出水告警
    {
      struCoverControl.CoverWaterType = CoverWaterType_None;
      
      debug_send("被动出水报警！");
      //加入告警间隔。
    }
#if APP_CAN_UPDATA
    Sys_keydata.Alarm = uniAlarm.AlarmAllData;
    iap_data.writeflag =MODE_ING_DOWNLOAD;
#else
    debug_send("未使能APP程序上网");
#endif   
    CoverSysFunState = CoverSysFunState_WaitInterLPM;
    break;
  case CoverSysFunState_WaitInterLPM:
    /**********************************************************************************************************************
    无水到有水实时触发。其他告警：开盖、倾斜、会带上出水告警。
    **********************************************************************************************************************/
//    if(CoverWaterType_None != struCoverControl.CoverWaterType)    
//    {
////      struXiaoFangAlarmTimes.StratAlarm = 0;
////      if((TRIGGER_FALLING == struXiaoFangAlarmTimes.TriggerStatus)||(XiaoFang_GetRtcWakeFlag())) 
////      {
////        struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
////        XiaoFang_ClearRtcWakeFlag();
////        debug_send("主动出水报警！");
////        ENTER_SENDALARM_STATUS; //进入发射流程
////      }
//      if(CoverWaterType_Trigger ==  struCoverControl.CoverWaterType) //无水到有水，边缘
//      {
//        struCoverControl.CoverWaterType = CoverWaterType_None;
//        debug_send("主动出水报警！");
//        ENTER_SENDALARM_STATUS; //进入发射流程
//      }
//      else if(CoverWaterType_Level ==  struCoverControl.CoverWaterType) //电平
//      {
//        struCoverControl.CoverWaterType = CoverWaterType_None;
////        if() //检查告警的间隔 
//        {
//          //进入发送流程。
//        }
//      }
//      
//      
//    }
    if(1 == struXiaoFangAlarmTimes.StratAlarm)    
    {
      struXiaoFangAlarmTimes.StratAlarm = 0;
      if(TRIGGER_FALLING == struXiaoFangAlarmTimes.TriggerStatus) //||(XiaoFang_GetRtcWakeFlag()
      {
        struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
        Reset_alarmtime(&alarmtime_all.waterover,Sys_keydata.alarm_sent_interval_minute_count);
        XiaoFang_ClearRtcWakeFlag();
        debug_send("主动出水报警！");
        ENTER_SENDALARM_STATUS; //进入发射流程
      }
      else if(TRIGGER_RISING == struXiaoFangAlarmTimes.TriggerStatus)
      {
        struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
        XiaoFang_ClearRtcWakeFlag();
        debug_send("清除主动出水报警！");
        ENTER_SENDALARM_STATUS; //进入发射流程
      }
    }
    else
    {
      if(TRIGGER_NOT != struXiaoFangAlarmTimes.TriggerStatus)  //如果在告警过程中发生了动作
      {
        debug_send("发送过程中发生了出水动作，重新进入检测流程。");
        CoverSysFunState = CoverSysFunState_Detection;  //回到重新检测流程。
//        sysFunState = 0;  //回到重新检测流程。
        break;
      }
      NewInactAngle_ClearSamplingLockFlag(); //清除禁止检查锁定。
      if(NewInactAngle_GetNewAngleEndFlag()) //等待角度静止完成。
      {
        Analyse_data_para.alarm_flag = 0;
        Adxl362Para.Read_flag = 0;
        uniAlarm.AlarmAllData = 0X00;     //清除所有告警
        struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
//        XiaoFang_ClearRtcWakeFlag();
        debug_send(">>>系统进入休眠");
        CoverSysFunState = CoverSysFunState_InterLPM;
      }
    }
    break;
  case CoverSysFunState_AnalyzeACC://
    if (sysFunTimecount < 5000) //超时退出
    {
      Analyse_data_para_XiaoFangZ.present_angle= get_angle_Z(ADXL362_Get_Value());//获取Z轴的数据。
      XiaoFang_Analyse(&Analyse_data_para_XiaoFangZ);  //分析Z轴数据
      Analyse_data_para_XiaoFangX.present_angle =get_angle_X(ADXL362_Get_Value());//获取X轴的数据。
      XiaoFang_Analyse(&Analyse_data_para_XiaoFangX);  //分析X轴数据
      //
      if((1 == Analyse_data_para_XiaoFangZ.alarm_flag) || (1 == Analyse_data_para_XiaoFangX.alarm_flag))
      {
//        Pack_Rotation_Angle(Analyse_data_para_XiaoFangX.present_angle); //打包旋转角度
//        Pack_LeanAngle(Analyse_data_para_XiaoFangZ.present_angle);//打包倾斜角度
        Sys_keydata.rotation_angle= Analyse_data_para_XiaoFangX.present_angle;
        Sys_keydata.lean_angle =Analyse_data_para_XiaoFangZ.present_angle;
        
        if((1 == Analyse_data_para_XiaoFangZ.alarm_flag)) // 发生倾斜报警
        {
          PrintString_("开始倾斜告警!");
          uniAlarm.struAlarmBit.laenAlarm = 1;
          //               Pack_Lean_Alarm(1);//打包倾斜告警。
          ENTER_SENDALARM_STATUS;
          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//清除分析模式的所有状态
        }
        
        if((1 == Analyse_data_para_XiaoFangX.alarm_flag)) // 发生旋转报警
        {
          PrintString_("开始开盖告警!");
          //                Pack_Rotation_Alarm(1); //打包旋转报警
          uniAlarm.struAlarmBit.openAlarm = 1;
          ENTER_SENDALARM_STATUS;
          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//清除分析模式的所有状态
        }
      }
      else //如果发生未告警
      {
        if(NewInactAngle_GetStaticFlag()) //
        {
//          sysFunState = 4;
          CoverSysFunState = CoverSysFunState_WaitInterLPM;
          //                ClsAnalyseState();//清除分析模式的所有状态
          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//清除分析模式的所有状态
          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//清除分析模式的所有状态
          //          PrintString_("none valid angle!back to lowpwer mode!");
          //          GetNewInactAngle();
        }
      }
    }
    else
    {
      //PrintString_("@");
      //            ClsAnalyseState();//清除分析模式的所有状态
      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//清除分析模式的所有状态
      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//清除分析模式的所有状态
      PrintString_("Exit!");
//      sysFunState = 4;//重新进入低功耗休眠
      CoverSysFunState = CoverSysFunState_WaitInterLPM;
      //Pack_AlarmFlag(1);//设置报警标志
      //PackageMsg();//先封装，再发送
      //sysFunState = 2;//发送消息
    }
    break;
  case CoverSysFunState_InterLPM:
    CoverSysFunState = CoverSysFunState_Detection;
    IntoLowpowerMode();
    break;
  default:
    CoverSysFunState = CoverSysFunState_Detection;
    break;
  }
#endif
}
/***************************************************************************************
**函数名:       Cover_Interrupt_Service;
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-12-07,By Haixing.Huang
****************************************************************************************/
//void Cover_Interrupt_Service()
//{
////  if(SJ_GET_OUTSTATUS)  //这里状态检测如何是高电平认为是上升沿。
////  {
////    struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_RISING;
////    debug_send("水上升沿中断");
////  }
////  else  //这里状态检测如何是低电平认为是下降沿。
////  {
////    struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_FALLING;
////    debug_send("水下降沿中断");
////  }
//}
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
/***************************************************************************************
**函数名:       SetHaveWaterAlarmFalg
**功能描述:     置位HaveWaterAlarmFalg
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-12-04,By Haixing.Huang
****************************************************************************************/
void SetHaveWaterAlarmFalg(void)
{
  struXiaoFangTimes.HaveWaterAlarmFalg = TRUE;
}
/***************************************************************************************
**函数名:       SetHaveWaterAlarmFalg
**功能描述:     复位HaveWaterAlarmFalg
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-12-04,By Haixing.Huang
****************************************************************************************/
void ReSetHaveWaterAlarmFalg(void)
{
  struXiaoFangTimes.HaveWaterAlarmFalg = FALSE;
}

/***************************************************************************************
**函数名:       SetHaveWaterAlarmFalg
**功能描述:     复位HaveWaterAlarmFalg
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-12-04,By Haixing.Huang
****************************************************************************************/
bool GetHaveWaterAlarmFalg()
{
  return struXiaoFangTimes.HaveWaterAlarmFalg;
}
#endif
