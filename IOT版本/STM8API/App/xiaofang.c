#include "xiaofang.h"

_struXiaoFangTimes struXiaoFangTimes;
_struXiaoFangTimes struXiaoFangPressTimes;

#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码

_struXiaoFangTimes struXiaoFangAlarmTimes; //报警间隔控制器
ANALYSE_DATA_PARA_XIAOFANG  Analyse_data_para_XiaoFangZ = 
{
  20000,//超时时间 默认20S //over_time;//
  0,//零点角度 默认0度 //zero_angle;//
  6,//设定角度 默认6度 //set_angle;//
  0,//当前角度 默认0度 // u8 present_angle;//
  3,//有效角度差值 默认2度 //valid_angle_d_value;//
  6,//设定的最长有效角度连续数目 默认8个 set_valid_angle_unbroken_num;//
  0,//有效角度连续数目 默认0个 //valid_angle_unbroken_num;//
  0,//状态机状态值 默认0 analyse_state;//
  0,//上一次的角度数值。
  0,//静态标志 默认0 u8 inact_flag :1;//上电默认为动态
  0,//报警标志 默认0 alarm_flag :1;//
};

ANALYSE_DATA_PARA_XIAOFANG  Analyse_data_para_XiaoFangX = 
{
  20000,//超时时间 默认20S //over_time;//
  0,//零点角度 默认0度 //zero_angle;//
  6,//设定角度 默认6度 //set_angle;//
  0,//当前角度 默认0度 // u8 present_angle;//
  3,//有效角度差值 默认2度 //valid_angle_d_value;//
  6,//设定的最长有效角度连续数目 默认8个 set_valid_angle_unbroken_num;//
  0,//有效角度连续数目 默认0个 //valid_angle_unbroken_num;//
  0,//状态机状态值 默认0 analyse_state;//
  0,//上一次的角度数值。
  0,//静态标志 默认0 u8 inact_flag :1;//上电默认为动态
  0,//报警标志 默认0 alarm_flag :1;//
};
#endif 

_Bool XiaoFang_IsDvalueWithinRange(s16 pre_angle, s16 new_angle, s8 d_value) 
{
  if (pre_angle >= new_angle) 
  { //角度变大
    if ((pre_angle - new_angle) <= d_value) 
    {
      return 1;
    }
    else 
    {
      return 0;
    }
  } 
  else 
  { //角度变小
    if ((new_angle - pre_angle) <= d_value) 
    {
      return 1;
    }
    else 
    {
      return 0;
    }
  }
  //return 0;
}

void XiaoFang_ClsAnalyseState(ANALYSE_DATA_PARA_XIAOFANG *pAnalyse) 
{
	pAnalyse->analyse_state = 0;
	pAnalyse->valid_angle_unbroken_num = 0;
	pAnalyse->alarm_flag = 0;
}
/***************************************************************************************
**函数名:         --
**功能描述:     
获取倾倒状态。
先分析Z轴角度。
最好的情况是Z轴处在90度的位置。
如果外力作用在Z轴的方向上，那么Z轴的角度就可以很明显地获取到倾倒的状态。
最坏的外力作用在垂直于Z轴的方向上，理论上Z轴无法获取到倾倒状态。所以需要X和Y轴协助判断。如果是很理想的情况下，无法和旋转警报区分开来。

旋转，实测旋转的时候Z轴基本不会发生变化。Y轴和X会随着旋转的发生变化。
X轴消防栓正常安装时顺时针负角度增大。
逆时针时角度减小。
快速变化时，角度会有跳变的情况。

X轴Y轴的角度互补变化。


**输入参数:     --
**输出参数:     --
**备注:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
u8 XiaoFang_Analyse(ANALYSE_DATA_PARA_XIAOFANG *pAnalyse)
{
  //分析Z轴角度。
  s16 angle_tmp_p = 0;
  s16 angle_tmp_z = 0;
  s16 angle_tmp_result=0;
//  pAnalyse->present_angle = get_angle_Z(ADXL362_Get_Value());  //获取Z轴的数据。
  PrintS8int2String(-55);//打印伪数据信息
  //获取到Z轴的角度和与零点角度的差值。
  if(pAnalyse->present_angle<0)
  {
    angle_tmp_p =  180-pAnalyse->present_angle;  //转换为0~180度数
  }
  else
  {
    angle_tmp_p=pAnalyse->present_angle;
  }
  if(pAnalyse->zero_angle<0)
  {
    angle_tmp_z = 180-pAnalyse->zero_angle;//转换为0~180度数
  }
  else
  {
    angle_tmp_z = pAnalyse->zero_angle;
  }
  if(angle_tmp_p >= angle_tmp_z)
  {
    angle_tmp_result = angle_tmp_p-angle_tmp_z;
  }
  else
  {
    angle_tmp_result = angle_tmp_z-angle_tmp_p;
  }
  if(angle_tmp_result > pAnalyse->set_angle)            //超出了最大的范围，报警。
  {
    if (XiaoFang_IsDvalueWithinRange(pAnalyse->pre_temp_z_angle,angle_tmp_p , pAnalyse->valid_angle_d_value)) 
    {
      //前一个值减去最新的值
      //在差值范围内
      PrintString("#");
    }
    //////////////20170117增加 可能影响监测//////////////////////////////////////
    else 
    {
      //前后差值不合理  清零不间断数目
      pAnalyse->valid_angle_unbroken_num = 0;
    }
    ////////////////////////////////////////////////////
    if (++pAnalyse->valid_angle_unbroken_num >= pAnalyse->set_valid_angle_unbroken_num) 
    {
      
      pAnalyse->valid_angle_unbroken_num = 0;
      debug_send("$$$");
       pAnalyse->alarm_flag = 1;//设置告警标志
        //封装当前倾斜角度
//        Pack_LeanAngle(pAnalyse->present_angle);
        return 1;
//      if ((Analyse_data_para.zero_angle <= 0))  //
//      {
//        Analyse_data_para.alarm_flag = 1;//设置告警标志
//        //封装当前倾斜角度
//        Pack_LeanAngle(Analyse_data_para.present_angle);
//        return 1;
//      }
//      else 
//      {
//        PrintString_("zero_angle is >0,No new alarm!");
//        Analyse_data_para.alarm_flag = 0;//清除告警标志
//      }
    }
  }
  else
  {
    pAnalyse->valid_angle_unbroken_num = 0;
  }
  pAnalyse->pre_temp_z_angle = angle_tmp_p;       //保存上一次的角度值。
  //分析Y轴角度。
  //分析Z轴角度。
  return 0;
}

/***************************************************************************************
**函数名:       XiaoFang_OpenDetection
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
void XiaoFang_OpenDetection(void)
{
//  SJ_SWITCH_OPEN;
//  SJ_DELAY; //等待电极稳定
//  if(SJ_GET_OUTSTATUS)
//  {
//    uniAlarm.struAlarmBit.waterAlarm = 1;      //开启出水报警
//  }
//  SJ_SWITCH_CLOSE;
}

/***************************************************************************************
**函数名:       XiaoFang_TimerService;
**功能描述:     
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-17,By Haixing.Huang
****************************************************************************************/
u16 XiaoFang_RtcSet(u16 para)  
{
   if(para>XIAOFANG_RTC_MAX_TIME) //超过最大的RTC定时，定时时间为规定的定时时间。
   {
     para = XIAOFANG_RTC_MAX_TIME;      
   }
   return RTC_MIN*para;
}
/***************************************************************************************
**函数名:       XiaoFang_HeartdataSet
**功能描述:     用于计算心跳时间。RTC_HEART为60分钟，trigger_interval_minute_count为时钟基准
，尽量要做到整除。如60/20 =3;
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-17,By Haixing.Huang
****************************************************************************************/
u16 XiaoFang_HeartdataSet(SYS_KEYDATA *parP)
{
  return (parP->heartdata_sent_interval_hour_count*RTC_HEART)/parP->trigger_interval_minute_count;
}
/***************************************************************************************
**函数名:       XiaoFang_TimerService;
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-17,By Haixing.Huang
****************************************************************************************/
void XiaoFang_TimerService(_struXiaoFangTimes *parP )
{
//  if(0 != parP->timesB_cnt)
//  {
//    if(parP->timesB_cnt == SJ_WAIT_TIME)  //定时的一开始就是模块上电
//    {
//      SJ_SWITCH_OPEN;
//    }
//    else if(1 == parP->timesB_cnt)
//    {
//      parP->timesB_over_flag= 1;
//      if(SJ_GET_OUTSTATUS)
//      {
//        uniAlarm.struAlarmBit.waterAlarm = 1;      //开启出水报警
//      }
//      else
//      {
//        uniAlarm.struAlarmBit.waterAlarm = 0;      //关闭出水报警
//      }
//      SJ_SWITCH_CLOSE;  //直接打开水检测开关。  
//    }
//    parP->timesB_cnt --;
//  } 
}
/***************************************************************************************
**函数名:       XiaoFang_DetectionWait;
**功能描述:     等待检测完成。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-17,By Haixing.Huang
****************************************************************************************/
u8 XiaoFang_DetectionWait(_struXiaoFangTimes *parP )
{
  if(0 == parP->timesB_over_flag) //还没有到检测时间
  {
    return 0;   
  }
  else
  {
    return 1; 
  }
}

/***************************************************************************************
**函数名:       XiaoFang_TimerInit;
**功能描述:     初始化定时
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-17,By Haixing.Huang
****************************************************************************************/
void XiaoFang_TimerInit(_struXiaoFangTimes *parP )
{
//  parP ->timesA_over_flag =0;
//  parP ->timesB_over_flag = 0;
  if(0 == parP->times_EnableFlag)
  {
    parP->times_EnableFlag = 1;
    parP->timesB_over_flag = 0;
    parP->timesB_cnt= SJ_WAIT_TIME;  // 
    
  }
  
}
/***************************************************************************************
**函数名:       XiaoFang_TimerService;
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-17,By Haixing.Huang
****************************************************************************************/
void XiaoFang_TimerOverFlagClear(_struXiaoFangTimes *parP )
{
//  parP ->timesA_over_flag =0;
  parP->timesB_over_flag = 0;
  parP->timesB_cnt = 0;
  parP->times_Read = 0;
  parP->times_EnableFlag = 0;
}

/***************************************************************************************
**函数名:       XiaoFang_TimerRTC;
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-29,By Haixing.Huang
****************************************************************************************/
void XiaoFang_RTCService(_struXiaoFangTimes *parP )
{
  if(0 != parP->timesB_cnt)
  {
    if(1 == parP->timesB_cnt)
    {
      parP->timesB_over_flag= 1;  
    }
    parP->timesB_cnt --;
  } 
}

/***************************************************************************************
**函数名:       XiaoFang_TimerInit;
**功能描述:     初始化定时
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-29,By Haixing.Huang
****************************************************************************************/
void XiaoFang_RTCInit(_struXiaoFangTimes *parP,u16 parTime)
{
//  parP ->timesA_over_flag =0;
//  parP ->timesB_over_flag = 0;
  if(0 == parP->times_EnableFlag)
  {
    parP->times_EnableFlag = 1;
    parP->timesB_over_flag = 0;
    if((0XFFFF) != parTime)
    {
      parP->timesB_cnt= parTime+1;  // 
    } 
    
  }
}



/***************************************************************************************
**函数名:       XiaoFang_IT_Init;
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-08-01,By Haixing.Huang
****************************************************************************************/
void XiaoFang_Interrupt_Init()
{
  GPIO_Init(SJ_OUTSTATUS_GPIO,SJ_OUTSTATUS_PIN ,GPIO_Mode_In_FL_IT);
//  GPIO_Init(SJ_OUTSTATUS_GPIO,SJ_OUTSTATUS_PIN ,GPIO_Mode_In_PU_IT);    //by hx 2017-08-03
//  EXTI_SetPinSensitivity(SJ_OUTSTATUS_EXTI_PIN,EXTI_Trigger_Falling);   //系统只检测下降沿
  EXTI_SetPinSensitivity(SJ_OUTSTATUS_EXTI_PIN,EXTI_Trigger_Rising_Falling);

}


/***************************************************************************************
**函数名:       XiaoFang_TimerRTC;
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-29,By Haixing.Huang
****************************************************************************************/
void XiaoFang_SetRtcWakeFlag()
{
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码 
  struXiaoFangAlarmTimes.RtcWakeFlag = TRUE;
#endif   
}
/***************************************************************************************
**函数名:       XiaoFang_TimerRTC;
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-29,By Haixing.Huang
****************************************************************************************/
void XiaoFang_ClearRtcWakeFlag()
{
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码 
  struXiaoFangAlarmTimes.RtcWakeFlag = FALSE;
#endif 
}
/***************************************************************************************
**函数名:       bool XiaoFang_GetRtcWakeFlag()
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-29,By Haixing.Huang
****************************************************************************************/
bool XiaoFang_GetRtcWakeFlag()
{
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码 
  return struXiaoFangAlarmTimes.RtcWakeFlag; 
#else
  return FALSE;
#endif   

}