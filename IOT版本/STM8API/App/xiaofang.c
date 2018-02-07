#include "xiaofang.h"

_struXiaoFangTimes struXiaoFangTimes;
_struXiaoFangTimes struXiaoFangPressTimes;

#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���

_struXiaoFangTimes struXiaoFangAlarmTimes; //�������������
ANALYSE_DATA_PARA_XIAOFANG  Analyse_data_para_XiaoFangZ = 
{
  20000,//��ʱʱ�� Ĭ��20S //over_time;//
  0,//���Ƕ� Ĭ��0�� //zero_angle;//
  6,//�趨�Ƕ� Ĭ��6�� //set_angle;//
  0,//��ǰ�Ƕ� Ĭ��0�� // u8 present_angle;//
  3,//��Ч�ǶȲ�ֵ Ĭ��2�� //valid_angle_d_value;//
  6,//�趨�����Ч�Ƕ�������Ŀ Ĭ��8�� set_valid_angle_unbroken_num;//
  0,//��Ч�Ƕ�������Ŀ Ĭ��0�� //valid_angle_unbroken_num;//
  0,//״̬��״ֵ̬ Ĭ��0 analyse_state;//
  0,//��һ�εĽǶ���ֵ��
  0,//��̬��־ Ĭ��0 u8 inact_flag :1;//�ϵ�Ĭ��Ϊ��̬
  0,//������־ Ĭ��0 alarm_flag :1;//
};

ANALYSE_DATA_PARA_XIAOFANG  Analyse_data_para_XiaoFangX = 
{
  20000,//��ʱʱ�� Ĭ��20S //over_time;//
  0,//���Ƕ� Ĭ��0�� //zero_angle;//
  6,//�趨�Ƕ� Ĭ��6�� //set_angle;//
  0,//��ǰ�Ƕ� Ĭ��0�� // u8 present_angle;//
  3,//��Ч�ǶȲ�ֵ Ĭ��2�� //valid_angle_d_value;//
  6,//�趨�����Ч�Ƕ�������Ŀ Ĭ��8�� set_valid_angle_unbroken_num;//
  0,//��Ч�Ƕ�������Ŀ Ĭ��0�� //valid_angle_unbroken_num;//
  0,//״̬��״ֵ̬ Ĭ��0 analyse_state;//
  0,//��һ�εĽǶ���ֵ��
  0,//��̬��־ Ĭ��0 u8 inact_flag :1;//�ϵ�Ĭ��Ϊ��̬
  0,//������־ Ĭ��0 alarm_flag :1;//
};
#endif 

_Bool XiaoFang_IsDvalueWithinRange(s16 pre_angle, s16 new_angle, s8 d_value) 
{
  if (pre_angle >= new_angle) 
  { //�Ƕȱ��
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
  { //�Ƕȱ�С
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
**������:         --
**��������:     
��ȡ�㵹״̬��
�ȷ���Z��Ƕȡ�
��õ������Z�ᴦ��90�ȵ�λ�á�
�������������Z��ķ����ϣ���ôZ��ĽǶȾͿ��Ժ����Եػ�ȡ���㵹��״̬��
������������ڴ�ֱ��Z��ķ����ϣ�������Z���޷���ȡ���㵹״̬��������ҪX��Y��Э���жϡ�����Ǻ����������£��޷�����ת�������ֿ�����

��ת��ʵ����ת��ʱ��Z��������ᷢ���仯��Y���X��������ת�ķ����仯��
X������˨������װʱ˳ʱ�븺�Ƕ�����
��ʱ��ʱ�Ƕȼ�С��
���ٱ仯ʱ���ǶȻ�������������

X��Y��ĽǶȻ����仯��


**�������:     --
**�������:     --
**��ע:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
u8 XiaoFang_Analyse(ANALYSE_DATA_PARA_XIAOFANG *pAnalyse)
{
  //����Z��Ƕȡ�
  s16 angle_tmp_p = 0;
  s16 angle_tmp_z = 0;
  s16 angle_tmp_result=0;
//  pAnalyse->present_angle = get_angle_Z(ADXL362_Get_Value());  //��ȡZ������ݡ�
  PrintS8int2String(-55);//��ӡα������Ϣ
  //��ȡ��Z��ĽǶȺ������ǶȵĲ�ֵ��
  if(pAnalyse->present_angle<0)
  {
    angle_tmp_p =  180-pAnalyse->present_angle;  //ת��Ϊ0~180����
  }
  else
  {
    angle_tmp_p=pAnalyse->present_angle;
  }
  if(pAnalyse->zero_angle<0)
  {
    angle_tmp_z = 180-pAnalyse->zero_angle;//ת��Ϊ0~180����
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
  if(angle_tmp_result > pAnalyse->set_angle)            //���������ķ�Χ��������
  {
    if (XiaoFang_IsDvalueWithinRange(pAnalyse->pre_temp_z_angle,angle_tmp_p , pAnalyse->valid_angle_d_value)) 
    {
      //ǰһ��ֵ��ȥ���µ�ֵ
      //�ڲ�ֵ��Χ��
      PrintString("#");
    }
    //////////////20170117���� ����Ӱ����//////////////////////////////////////
    else 
    {
      //ǰ���ֵ������  ���㲻�����Ŀ
      pAnalyse->valid_angle_unbroken_num = 0;
    }
    ////////////////////////////////////////////////////
    if (++pAnalyse->valid_angle_unbroken_num >= pAnalyse->set_valid_angle_unbroken_num) 
    {
      
      pAnalyse->valid_angle_unbroken_num = 0;
      debug_send("$$$");
       pAnalyse->alarm_flag = 1;//���ø澯��־
        //��װ��ǰ��б�Ƕ�
//        Pack_LeanAngle(pAnalyse->present_angle);
        return 1;
//      if ((Analyse_data_para.zero_angle <= 0))  //
//      {
//        Analyse_data_para.alarm_flag = 1;//���ø澯��־
//        //��װ��ǰ��б�Ƕ�
//        Pack_LeanAngle(Analyse_data_para.present_angle);
//        return 1;
//      }
//      else 
//      {
//        PrintString_("zero_angle is >0,No new alarm!");
//        Analyse_data_para.alarm_flag = 0;//����澯��־
//      }
    }
  }
  else
  {
    pAnalyse->valid_angle_unbroken_num = 0;
  }
  pAnalyse->pre_temp_z_angle = angle_tmp_p;       //������һ�εĽǶ�ֵ��
  //����Y��Ƕȡ�
  //����Z��Ƕȡ�
  return 0;
}

/***************************************************************************************
**������:       XiaoFang_OpenDetection
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
void XiaoFang_OpenDetection(void)
{
//  SJ_SWITCH_OPEN;
//  SJ_DELAY; //�ȴ��缫�ȶ�
//  if(SJ_GET_OUTSTATUS)
//  {
//    uniAlarm.struAlarmBit.waterAlarm = 1;      //������ˮ����
//  }
//  SJ_SWITCH_CLOSE;
}

/***************************************************************************************
**������:       XiaoFang_TimerService;
**��������:     
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-17,By Haixing.Huang
****************************************************************************************/
u16 XiaoFang_RtcSet(u16 para)  
{
   if(para>XIAOFANG_RTC_MAX_TIME) //��������RTC��ʱ����ʱʱ��Ϊ�涨�Ķ�ʱʱ�䡣
   {
     para = XIAOFANG_RTC_MAX_TIME;      
   }
   return RTC_MIN*para;
}
/***************************************************************************************
**������:       XiaoFang_HeartdataSet
**��������:     ���ڼ�������ʱ�䡣RTC_HEARTΪ60���ӣ�trigger_interval_minute_countΪʱ�ӻ�׼
������Ҫ������������60/20 =3;
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-17,By Haixing.Huang
****************************************************************************************/
u16 XiaoFang_HeartdataSet(SYS_KEYDATA *parP)
{
  return (parP->heartdata_sent_interval_hour_count*RTC_HEART)/parP->trigger_interval_minute_count;
}
/***************************************************************************************
**������:       XiaoFang_TimerService;
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-17,By Haixing.Huang
****************************************************************************************/
void XiaoFang_TimerService(_struXiaoFangTimes *parP )
{
//  if(0 != parP->timesB_cnt)
//  {
//    if(parP->timesB_cnt == SJ_WAIT_TIME)  //��ʱ��һ��ʼ����ģ���ϵ�
//    {
//      SJ_SWITCH_OPEN;
//    }
//    else if(1 == parP->timesB_cnt)
//    {
//      parP->timesB_over_flag= 1;
//      if(SJ_GET_OUTSTATUS)
//      {
//        uniAlarm.struAlarmBit.waterAlarm = 1;      //������ˮ����
//      }
//      else
//      {
//        uniAlarm.struAlarmBit.waterAlarm = 0;      //�رճ�ˮ����
//      }
//      SJ_SWITCH_CLOSE;  //ֱ�Ӵ�ˮ��⿪�ء�  
//    }
//    parP->timesB_cnt --;
//  } 
}
/***************************************************************************************
**������:       XiaoFang_DetectionWait;
**��������:     �ȴ������ɡ�
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-17,By Haixing.Huang
****************************************************************************************/
u8 XiaoFang_DetectionWait(_struXiaoFangTimes *parP )
{
  if(0 == parP->timesB_over_flag) //��û�е����ʱ��
  {
    return 0;   
  }
  else
  {
    return 1; 
  }
}

/***************************************************************************************
**������:       XiaoFang_TimerInit;
**��������:     ��ʼ����ʱ
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-17,By Haixing.Huang
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
**������:       XiaoFang_TimerService;
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-17,By Haixing.Huang
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
**������:       XiaoFang_TimerRTC;
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-29,By Haixing.Huang
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
**������:       XiaoFang_TimerInit;
**��������:     ��ʼ����ʱ
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-29,By Haixing.Huang
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
**������:       XiaoFang_IT_Init;
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-01,By Haixing.Huang
****************************************************************************************/
void XiaoFang_Interrupt_Init()
{
  GPIO_Init(SJ_OUTSTATUS_GPIO,SJ_OUTSTATUS_PIN ,GPIO_Mode_In_FL_IT);
//  GPIO_Init(SJ_OUTSTATUS_GPIO,SJ_OUTSTATUS_PIN ,GPIO_Mode_In_PU_IT);    //by hx 2017-08-03
//  EXTI_SetPinSensitivity(SJ_OUTSTATUS_EXTI_PIN,EXTI_Trigger_Falling);   //ϵͳֻ����½���
  EXTI_SetPinSensitivity(SJ_OUTSTATUS_EXTI_PIN,EXTI_Trigger_Rising_Falling);

}


/***************************************************************************************
**������:       XiaoFang_TimerRTC;
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-29,By Haixing.Huang
****************************************************************************************/
void XiaoFang_SetRtcWakeFlag()
{
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ��� 
  struXiaoFangAlarmTimes.RtcWakeFlag = TRUE;
#endif   
}
/***************************************************************************************
**������:       XiaoFang_TimerRTC;
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-29,By Haixing.Huang
****************************************************************************************/
void XiaoFang_ClearRtcWakeFlag()
{
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ��� 
  struXiaoFangAlarmTimes.RtcWakeFlag = FALSE;
#endif 
}
/***************************************************************************************
**������:       bool XiaoFang_GetRtcWakeFlag()
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-29,By Haixing.Huang
****************************************************************************************/
bool XiaoFang_GetRtcWakeFlag()
{
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ��� 
  return struXiaoFangAlarmTimes.RtcWakeFlag; 
#else
  return FALSE;
#endif   

}