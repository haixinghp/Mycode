#include "PressureSys.h"
#if  PRESSURESYS_MODULE_EN //模块使能
#define PRESSURESYS_PRINT_16NUM_R(info) PrintU16int2String_(info)
#define PRESSURESYS_PRINT_R(info) debug_send_no_end(info)

#warning 水压模块使能!
_uniAlarm uniAlarm=
{
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

};
static _strAlarmSentInterval strAlarmSentInterval;
static PressureSysFunState_TypeDef PressureSysFunState;
s32 *getLockTimeUpLimit()
{
  return &strAlarmSentInterval.LockTimeUpLimit;
}
s32 *getLockTimeDownLimit()
{
  return &strAlarmSentInterval.LockTimeDownLimit;
}
s32 *getLockTimeNoSensor()
{
  return &strAlarmSentInterval.LockTimeNoSensor;
}
s32 *getLockTimeResolution()
{
  return &strAlarmSentInterval.LockTimeResolution;
}
s32 *getLockTimeTouchOff()
{
  return &strAlarmSentInterval.LockTimeTouchOff;
}

/***************************************************************************************
**函数名:       ResetUpdataInit;
**功能描述:     复位时强制上报数据
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-12-09,By Haixing.Huang
****************************************************************************************/
void ResetUpdataInit()
{
  uniAlarm.struAlarmBit.resetAlarm = 1;
}
/***************************************************************************************
**函数名:       PressureSysFunStateInit;
**功能描述:     初始化设备状态机
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-12-09,By Haixing.Huang
****************************************************************************************/
void PressureSysFunStateInit()
{
  PressureSysFunState = PressureSysFunState_KeyDetection;
}
/***************************************************************************************
**函数名:       AlarmSentIntervalCheck;
**功能描述:     初始化定时,如果时间设置为65535（0XFFFF）表示取消
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-11-15,By Haixing.Huang
****************************************************************************************/
s8 AlarmSentIntervalSet(s32 *locktime,u16 time)
{
  if(0XFFFF != time)
  {
    *locktime = TimeGetTick(FALSE);//获取时间
#if SPEED_UP
    *locktime += (time*6);//得到目标时间戳 测试版本
#else 
    *locktime += (time*60);//得到目标时间戳 正式版本
#endif 
    return 1;
  }
  else
  {
    return 0;
  }
}
/***************************************************************************************
**函数名:       AlarmSentIntervalCheck;
**功能描述:     检查时间是否到达
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-11-15,By Haixing.Huang
****************************************************************************************/
s8 AlarmSentIntervalCheck(s32 *locktime)
{
  s32 tmp_s32;
  tmp_s32 = TimeGetTick(FALSE);
  if(tmp_s32 > *locktime) //如果超过了锁定时间
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/***************************************************************************************
**函数名:       AlarmPrintf;
**功能描述:     告警打印。
**输入参数:     u16 value
**输出参数:     --
**备注:         YY.c,2017-12-09,By Haixing.Huang
****************************************************************************************/
s8 AlarmPrintf(u16 value)
{
  _uniAlarm uniAlarmTmp;
  uniAlarmTmp.AlarmAllData = value;
  
  if(1 == uniAlarmTmp.struAlarmBit.pressureAlarmUpLimit)
  {
    debug_send("上限告警"); 
  }
  if(1 == uniAlarmTmp.struAlarmBit.pressureAlarmDownLimit)
  {
    debug_send("下限告警");
  }
  if(1 == uniAlarmTmp.struAlarmBit.WithoutPressureSensors)
  {
    debug_send("未连接传感器告警");
  }
  if(1 == uniAlarmTmp.struAlarmBit.KeyUpdata)
  {
    debug_send("人工触发告警");
  }
  
  if(1 == uniAlarmTmp.struAlarmBit.pressureResolutionAlarm)
  {
    debug_send("分辨率上报");
  }
  if(1 == uniAlarmTmp.struAlarmBit.resetAlarm)
  {
    debug_send("复位上报");
  }
  return 0;
}
/***************************************************************************************
**函数名:       AlarmEnableBitcheck;
**功能描述:     检查告警有效位并屏蔽告警位。
**输入参数:     u16 *uniAlarm,u16 value
**输出参数:     --
**备注:         YY.c,2017-12-09,By Haixing.Huang
****************************************************************************************/
s8 AlarmEnableBitCheck(_uniAlarm *uniAlarmpTmp,u16 value)
{
  _uniAlarm uniAlarmTmp;
//  _uniAlarm *uniAlarmpTmp;
//  _struAlarmBit *struAlarmBittmp;
//  struAlarmBittmp = (_struAlarmBit)(uniAlarm);
//  uniAlarmpTmp = uniAlarm;
  uniAlarmTmp.AlarmAllData = value;
  
  if(1 == uniAlarmTmp.struAlarmBit.pressureAlarmUpLimit)
  {
    debug_send("上限告警被屏蔽");
    uniAlarmpTmp->struAlarmBit.pressureAlarmUpLimit = 0; //对应告警位置告警清除 
  }
  if(1 == uniAlarmTmp.struAlarmBit.pressureAlarmDownLimit)
  {
    debug_send("下限告警被屏蔽");
    uniAlarmpTmp->struAlarmBit.pressureAlarmDownLimit = 0;
  }
  if(1 == uniAlarmTmp.struAlarmBit.WithoutPressureSensors)
  {
    debug_send("未连接传感器告警被屏蔽");
    uniAlarmpTmp->struAlarmBit.WithoutPressureSensors = 0; //对应告警位置告警清除 
  }
  if(1 == uniAlarmTmp.struAlarmBit.KeyUpdata)
  {
    debug_send("人工触发告警被屏蔽");
    uniAlarmpTmp->struAlarmBit.KeyUpdata = 0; //对应告警位置告警清除 
  }
  
  if(1 == uniAlarmTmp.struAlarmBit.pressureResolutionAlarm)
  {
    debug_send("分辨率上报被屏蔽");
    uniAlarmpTmp->struAlarmBit.pressureResolutionAlarm = 0; //对应告警位置告警清除 
  }
  return 0;
}

/***************************************************************************************
**函数名:       AlarmValidityCheck
**功能描述:     告警的有效性检查
**输入参数:     u16 *uniAlarm,u16 value
**输出参数:     --
**备注:         YY.c,2017-12-09,By Haixing.Huang
****************************************************************************************/
s8 AlarmValidityCheck(_uniAlarm *uniAlarmpTmp)
{
  _uniAlarm *p;
  p = uniAlarmpTmp;
//  bool ValueValidFlag = TRUE;
  if(1 == p->struAlarmBit.WithoutPressureSensors)
  {
    p->struAlarmBit.pressureAlarmDownLimit = 0;
    p->struAlarmBit.pressureAlarmUpLimit = 0;
    p->struAlarmBit.pressureResolutionAlarm =0;
    debug_send("未连接传感器上限告警无效");
    debug_send("未连接传感器下限告警无效"); 
    debug_send("未连接传感器分辨率上报告警无效"); 
  }
  return 1;
}
///***************************************************************************************
//**函数名:       AlarmIntervalCheck
//**功能描述:     检查告警间隔
//**输入参数:     u16 *uniAlarm
//**输出参数:     --
//**备注:         YY.c,2017-12-09,By Haixing.Huang
//****************************************************************************************/
//s8 AlarmIntervalCheck(u16 value)
//{
//  _uniAlarm uniAlarmTmp;
//  uniAlarmTmp.AlarmAllData = value;
//  s8 s8_tmp =0;
//  bool ValueValidFlag = TRUE;
//  if(1 == uniAlarmTmp.struAlarmBit.WithoutPressureSensors)
//  {
//    ValueValidFlag = FALSE; //上下限告警，压力变化率告警全部无效。
//    if(Is_flag_alarmtime(getAlarmTimeNoSensor()))
//    {
//      Reset_alarmtime(getAlarmTimeNoSensor(),
//                      *getNoSensorAlarmInterval());   //填写告警间隔
//      s8_tmp =1;
//      
//    }
//    else
//    {
//      debug_send("未连接传感器告警在间隔内"); 
//    }
//  }
//  
//  if(1 == uniAlarmTmp.struAlarmBit.pressureAlarmUpLimit)
//  {
//    if(ValueValidFlag)
//    {
//      if(Is_flag_alarmtime(getAlarmTimePressureUpLimit()))
//      {
//        Reset_alarmtime(getAlarmTimePressureUpLimit(),
//                        *getPressureUpLimitAlarmInterval());
//        s8_tmp = 1;
//      }
//      else
//      {
//        uniAlarmTmp.struAlarmBit.pressureAlarmUpLimit = 0;
//        debug_send("上限告警在间隔内"); 
//      }
//    }
//    else
//    {
//      debug_send("未连接传感器上限告警无效"); 
//    }
//  }
//  if(1 == uniAlarmTmp.struAlarmBit.pressureAlarmDownLimit)
//  {
//    if(ValueValidFlag)
//    {
//      if(Is_flag_alarmtime(getAlarmTimePressureDownLimit()))
//      {
//        Reset_alarmtime(getAlarmTimePressureDownLimit(),
//                        *getPressureDownLimitAlarmInterval());  //填写告警间隔
//        s8_tmp =1;
//      }
//      else
//      {
//        debug_send("下限告警在间隔内"); 
//      }
//    }
//    else
//    {
//      uniAlarmTmp.struAlarmBit.pressureAlarmDownLimit = 1;
//      debug_send("未连接传感器下限告警无效"); 
//    }
//  }
//  
//  if(1 == uniAlarmTmp.struAlarmBit.KeyUpdata)
//  {
//    if(Is_flag_alarmtime(getAlarmTimeTouchOff()))
//    {
//      Reset_alarmtime(getAlarmTimeTouchOff(),
//                      *getTouchOffAlarmInterval());  //填写告警间隔
//      s8_tmp =1;
//    }
//    else
//    {
//      debug_send("人工触发告警在间隔内"); 
//    }
//  }
//  
//  if(1 == uniAlarmTmp.struAlarmBit.pressureResolutionAlarm)
//  {
//    if(ValueValidFlag)
//    {
//      if(Is_flag_alarmtime(getAlarmTimeResolutionAlarm()))
//      {
//        Reset_alarmtime(getAlarmTimeResolutionAlarm(),
//                        *getResolutionAlarmInterval());  //填写告警间隔
//        s8_tmp =1;
//      }
//      else
//      {
//        debug_send("分辨率上报告警在间隔内"); 
//      }
//    }
//    else
//    {
//      debug_send("未连接传感器分辨率上报告警无效"); 
//    }
//  }
//  if(1 == uniAlarmTmp.struAlarmBit.resetAlarm)
//  {
//    s8_tmp =1;
//    debug_send("复位上报"); 
//  }
////  ShowAlarmTimer();
//  return s8_tmp;
//}
/***************************************************************************************
**函数名:       AlarmIntervalCheck
**功能描述:     检查告警间隔
**输入参数:     u16 *uniAlarm
**输出参数:     --
**备注:         YY.c,2017-12-09,By Haixing.Huang
****************************************************************************************/
s8 AlarmIntervalCheck(u16 value)
{
  _uniAlarm uniAlarmTmp;
  uniAlarmTmp.AlarmAllData = value;
  s8 s8_tmp =0;
  if(1 == uniAlarmTmp.struAlarmBit.WithoutPressureSensors)
  {
    if(Is_flag_alarmtime(getAlarmTimeNoSensor()))
    {
      Reset_alarmtime(getAlarmTimeNoSensor(),
                      *getNoSensorAlarmInterval());   //填写告警间隔
      s8_tmp =1;
      
    }
    else
    {
      debug_send("未连接传感器告警在间隔内"); 
    }
  }
  
  if(1 == uniAlarmTmp.struAlarmBit.pressureAlarmUpLimit)
  {
      if(Is_flag_alarmtime(getAlarmTimePressureUpLimit()))
      {
        Reset_alarmtime(getAlarmTimePressureUpLimit(),
                        *getPressureUpLimitAlarmInterval());
        s8_tmp = 1;
      }
      else
      {
        uniAlarmTmp.struAlarmBit.pressureAlarmUpLimit = 0;
        debug_send("上限告警在间隔内"); 
      }
  }
  if(1 == uniAlarmTmp.struAlarmBit.pressureAlarmDownLimit)
  {
      if(Is_flag_alarmtime(getAlarmTimePressureDownLimit()))
      {
        Reset_alarmtime(getAlarmTimePressureDownLimit(),
                        *getPressureDownLimitAlarmInterval());  //填写告警间隔
        s8_tmp =1;
      }
      else
      {
        debug_send("下限告警在间隔内"); 
      }
  }
  
  if(1 == uniAlarmTmp.struAlarmBit.KeyUpdata)
  {
    if(Is_flag_alarmtime(getAlarmTimeTouchOff()))
    {
      Reset_alarmtime(getAlarmTimeTouchOff(),
                      *getTouchOffAlarmInterval());  //填写告警间隔
      s8_tmp =1;
    }
    else
    {
      debug_send("人工触发告警在间隔内"); 
    }
  }
  
  if(1 == uniAlarmTmp.struAlarmBit.pressureResolutionAlarm)
  {
      if(Is_flag_alarmtime(getAlarmTimeResolutionAlarm()))
      {
        Reset_alarmtime(getAlarmTimeResolutionAlarm(),
                        *getResolutionAlarmInterval());  //填写告警间隔
        s8_tmp =1;
      }
      else
      {
        debug_send("分辨率上报告警在间隔内"); 
      }
  }
  return s8_tmp;
}


void ShowHeartCount()
{
  PRESSURESYS_PRINT_R("心跳计数器:");
  PRESSURESYS_PRINT_16NUM_R(*getheart_timecount());
  PRESSURESYS_PRINT_R("心跳目标值:");
  PRESSURESYS_PRINT_16NUM_R(XiaoFang_HeartdataSet(&Sys_keydata));
}
/***************************************************************************************
**函数名:       PressureSysFun
**功能描述:     水压业务流程程序
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-11-23,By Haixing.Huang
****************************************************************************************/
void PressureSysFun(void) 
{
  u16 tmp_u16;
  switch (PressureSysFunState) 
  {
  case PressureSysFunState_Initialize:
    uniAlarm.AlarmAllData = 0X00; //清除所有的告警
    Sys_keydata.Alarm=uniAlarm.AlarmAllData; 
    PressureSysFunState = PressureSysFunState_KeyDetection;
    break;
  case  PressureSysFunState_KeyDetection:
    if(PressureKeyUpdataConfirm())
    {
      debug_send("按键触发上报数据!");
      uniAlarm.struAlarmBit.KeyUpdata = 1; 
    }
    PressureSysFunState = PressureSysFunState_getPressure;
    break;
  case PressureSysFunState_getPressure:
    POW24_ON();  //开启水压检测
    DelayT_ms(500); //给500ms的外设上电时间
    AdcInit(); //初始化ADC
    tmp_u16 = GetADCValue();
    Sys_keydata.SamplingValue = tmp_u16;
    AdcClear();
    POW24_OFF();  //关闭水压检测
    
    if(tmp_u16 > 250 ) //大于0.2V 
    {
      *getPressureData() = GetPressureValue(tmp_u16);//获取到压力值。
      Sys_keydata.pressure_value = *getPressureData();
    }
    else //未连接传感器。
    {
      Sys_keydata.pressure_value = 0; //填写一个伪数据为0
      PRESSURESYS_PRINT_R(" ！！！错误，未连接传感器！！！");
      PRESSURESYS_PRINT_R("\n");
      uniAlarm.struAlarmBit.WithoutPressureSensors = 1; //添加到 未检测到传感器告警列表
    }
    PRESSURESYS_PRINT_R("_=====>压力值为：");
    PRESSURESYS_PRINT_16NUM_R(Sys_keydata.pressure_value);
    *getTimesTamp() = TimeGetTick(TRUE);//获取时间
    Sys_keydata.TimesTamp = *getTimesTamp();
    PressureSysFunState = PressureSysFunState_Analyse;
    break;
  case PressureSysFunState_Analyse:
    strPressureControl.CurrentValue = Sys_keydata.pressure_value;   //获取最新的压力值。
    if(strPressureControl.CurrentValuePre >= strPressureControl.CurrentValue)   //检查压力差值。
    {
      strPressureControl.CurrentValueDifference = strPressureControl.CurrentValuePre- strPressureControl.CurrentValue;
    }
    else
    {
      strPressureControl.CurrentValueDifference = strPressureControl.CurrentValue- strPressureControl.CurrentValuePre;
    }  
    if(strPressureControl.CurrentValueDifference > Sys_keydata.pressure_trigger_value) //变化值超过一定范围。
    {
      debug_send("超过变化范围报警！");
      uniAlarm.struAlarmBit.pressureResolutionAlarm = 1;
    }
    if(strPressureControl.CurrentValue > (*getPressureUpLimit()))
    {
      uniAlarm.struAlarmBit.pressureAlarmUpLimit = 1; //上限告警
    }
    else if(strPressureControl.CurrentValue < (*getPressureDownLimit()))
    {
      uniAlarm.struAlarmBit.pressureAlarmDownLimit = 1;// 下限告警
    }
    else
    {
      uniAlarm.struAlarmBit.pressureAlarmDownLimit = 0; //没有告警
      uniAlarm.struAlarmBit.pressureAlarmUpLimit = 0;//没有告警
    }
    PressureSysFunState = PressureSysFunState_WaitSendData;
    break;
  case PressureSysFunState_WaitSendData:
    if(0 != uniAlarm.AlarmAllData)
    {
      AlarmPrintf(uniAlarm.AlarmAllData);//打印告警类型
      AlarmEnableBitCheck(&uniAlarm,*getAlarmEnableBit());//检查告警有效位。
      AlarmValidityCheck(&uniAlarm);
      if(AlarmIntervalCheck(uniAlarm.AlarmAllData)) //检查告警间隔。
      {
        PressureSysFunState = PressureSysFunState_SendData; // 进入发送流程
        ClearHeartTimeCount(); //清除心跳计数器
      }
      else //没发生告警
      {
        
        PressureSysFunState = PressureSysFunState_WaitInterLPM;  //不符合发送数据条件，进入休眠
      }
      
    }
    else //没有告警，进入休眠
    {
      PressureSysFunState = PressureSysFunState_WaitInterLPM;
    }
    if(*getheart_send_flag()) //检查心跳标志位
    {
      PRESSURESYS_PRINT_R("发送心跳"); 
      PRESSURESYS_PRINT_R("\n");
      *getheart_send_flag() = 0;  
      PressureSysFunState = PressureSysFunState_SendData; // 进入发送流程
    }
    Refresh_all_alarmtime(); //刷新检测间隔。
    ShowHeartCount();
    ShowAlarmTimer();
    //这里需要向FLASH写入数据。
    EnFLASH_Queue(Sys_keydata.TimesTamp,Sys_keydata.pressure_value,Sys_keydata.Alarm);
//    DeFLASH_Queue(&struPressureAData.TimesTamp,&struPressureAData.PressureData,&struPressureAData.AlarmList);
    break;
  case PressureSysFunState_SendData:       //发送报警/心跳
    Sys_keydata.Alarm=uniAlarm.AlarmAllData; 
#if APP_CAN_UPDATA
    iap_data.writeflag =MODE_ING_DOWNLOAD;
#else
    debug_send("未使能APP程序上网");
#endif
    PressureSysFunState = PressureSysFunState_WaitInterLPM;
    break;
  case PressureSysFunState_WaitInterLPM:
    PressureSysFunState = PressureSysFunState_InterLPM;
    break;
  case PressureSysFunState_InterLPM://清除标志，重新进入低功耗休眠
    strPressureControl.CurrentValuePre = strPressureControl.CurrentValue; //被上次的值 
    PressureSysFunState = PressureSysFunState_Initialize;//休眠唤醒后，从头开始执行
    debug_send("进入低功耗");
#if ENTER_LPW_ENABLE
    IntoLowpowerMode();
#endif
    break;
  default:
    break;
  }
}
#endif
/************************************Code End******************************************/