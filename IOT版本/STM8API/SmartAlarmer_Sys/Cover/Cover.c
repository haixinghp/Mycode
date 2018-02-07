#include "Cover.h"
#include "config.h"

#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
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
#warning �Ƹ�ģ��ʹ��!
CoverSysFunState_TypeDef CoverSysFunState;
_struCoverControl  struCoverControl;  //��Ҫ��ʼ��
#endif
/***************************************************************************************
**������:       Cover_WakeIntService
**��������:     �Ƹ��жϷ����ѷ������
**�������:     --
**�������:     --
**��ע:         YY.c,2017-11-24,By Haixing.Huang
****************************************************************************************/
void Cover_WakeIntService(void)
{
  if (Adxl362Para.Read_flag == 0) 
  {
    Adxl362Para.Read_flag = 1;
    //    Analyse_data_para.inact_flag = 0;//��̬��־����
    NewInactAngle_ClearStaticFlag();
    NewInactAngle_ClearNewAngleEndFlag(); //�����ֹ��־��
    debug_send("waked");
  }
}

/***************************************************************************************
**������:       WaterDetection_Service;
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-01,By Haixing.Huang
****************************************************************************************/
s8 WaterDetection_Service()
{
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
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
      uniAlarm.struAlarmBit.waterAlarm = 0;      //�رո澯
      debug_send("ȷ����ˮ");
      if(TRIGGER_RISING == struXiaoFangAlarmTimes.TriggerStatus)
      {
        if(GetHaveWaterAlarmFalg()) 
        { 
          ReSetHaveWaterAlarmFalg();
          struXiaoFangAlarmTimes.StratAlarm = 1;  //�����澯��
        }
        else
        {
          struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
          debug_send("û�п�ȡ���ĸ澯");
        }
      }
    } 
  }
  else
  {
    if(FALSE ==  tmp_bool)
    {
      uniAlarm.struAlarmBit.waterAlarm = 1;      //������ˮ����
      debug_send("ȷ����ˮ");
      if(TRIGGER_FALLING == struXiaoFangAlarmTimes.TriggerStatus)
      {
        if(!GetHaveWaterAlarmFalg()) 
        {
//          Reset_alarmtime(&alarmtime_all.waterover,Sys_keydata.alarm_sent_interval_minute_count);
          if(Is_flag_alarmtime(&alarmtime_all.waterover))
          {
            SetHaveWaterAlarmFalg();
            struXiaoFangAlarmTimes.StratAlarm = 1;  //�����澯��
          }
          else
          {
            struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
            debug_send("�ڸ澯����ڣ�");  //
          }
          
        }
        else
        {
          struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
          debug_send("��Ҫ����澯");
        }
      }
    }
  }
#else
  return 0;
#endif 
  
}
/***************************************************************************************
**������:       CoverInterrupt_Service
**��������:     �Ƹ���ˮ���io���жϷ������
**�������:     --
**�������:     --
**��ע:         YY.c,2017-11-23,By Haixing.Huang
****************************************************************************************/
void CoverInterrupt_Service()
{
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
  struCoverControl.InterruptType = TRUE;
    if(SJ_GET_OUTSTATUS)  //����״̬�������Ǹߵ�ƽ��Ϊ�������ء�
  {
    struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_RISING;
    debug_send("ˮ�������ж�");
  }
  else  //����״̬�������ǵ͵�ƽ��Ϊ���½��ء�
  {
    struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_FALLING;
    debug_send("ˮ�½����ж�");
  }
#endif  
}
/***************************************************************************************
**������:       CoverSysFun
**��������:     �Ƹ�ҵ�����̳���
**�������:     --
**�������:     --
**��ע:         YY.c,2017-11-23,By Haixing.Huang
****************************************************************************************/
void CoverSysFun(void) 
{
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
 
   
  switch (CoverSysFunState)
  {
  case CoverSysFunState_Detection://������ADXLģ������ݶ�ȡ
    
    WaterDetection_Service();
    if (Adxl362Para.Read_flag == 1) //�������ֵ������־��λ
    {
      sysFunTimecount = 0;
      CoverSysFunState = CoverSysFunState_AnalyzeACC; //������ٶȷ�����
      PrintString_("��ʼ��ȡ��������!");
      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//�������ģʽ������״̬
      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//�������ģʽ������״̬
      PrintString("Z�����Ƕ�:");
      PrintS8int2String_(Analyse_data_para_XiaoFangZ.zero_angle);
      PrintString("X�����Ƕ�:");
      PrintS8int2String_(Analyse_data_para_XiaoFangX.zero_angle);
      NewInactAngle_SetSamplingLockFlag();  //���붯���жϵ�ʱ�򣬲����о�ֹ�жϡ�
    }
    else        //���û����ֵ����
    {
      if (heart_send_flag) 
      {
        heart_send_flag = 0;   //���������
        CoverSysFunState = CoverSysFunState_SendData;
        debug_send("��ʼ����������");
        break;
      }
      CoverSysFunState = CoverSysFunState_WaitInterLPM;
    }
    break;
  case CoverSysFunState_SendData:       //���ͱ���/����
    if(CoverWaterType_None != struCoverControl.CoverWaterType) //�����˳�ˮ�澯
    {
      struCoverControl.CoverWaterType = CoverWaterType_None;
      
      debug_send("������ˮ������");
      //����澯�����
    }
#if APP_CAN_UPDATA
    Sys_keydata.Alarm = uniAlarm.AlarmAllData;
    iap_data.writeflag =MODE_ING_DOWNLOAD;
#else
    debug_send("δʹ��APP��������");
#endif   
    CoverSysFunState = CoverSysFunState_WaitInterLPM;
    break;
  case CoverSysFunState_WaitInterLPM:
    /**********************************************************************************************************************
    ��ˮ����ˮʵʱ�����������澯�����ǡ���б������ϳ�ˮ�澯��
    **********************************************************************************************************************/
//    if(CoverWaterType_None != struCoverControl.CoverWaterType)    
//    {
////      struXiaoFangAlarmTimes.StratAlarm = 0;
////      if((TRIGGER_FALLING == struXiaoFangAlarmTimes.TriggerStatus)||(XiaoFang_GetRtcWakeFlag())) 
////      {
////        struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
////        XiaoFang_ClearRtcWakeFlag();
////        debug_send("������ˮ������");
////        ENTER_SENDALARM_STATUS; //���뷢������
////      }
//      if(CoverWaterType_Trigger ==  struCoverControl.CoverWaterType) //��ˮ����ˮ����Ե
//      {
//        struCoverControl.CoverWaterType = CoverWaterType_None;
//        debug_send("������ˮ������");
//        ENTER_SENDALARM_STATUS; //���뷢������
//      }
//      else if(CoverWaterType_Level ==  struCoverControl.CoverWaterType) //��ƽ
//      {
//        struCoverControl.CoverWaterType = CoverWaterType_None;
////        if() //���澯�ļ�� 
//        {
//          //���뷢�����̡�
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
        debug_send("������ˮ������");
        ENTER_SENDALARM_STATUS; //���뷢������
      }
      else if(TRIGGER_RISING == struXiaoFangAlarmTimes.TriggerStatus)
      {
        struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
        XiaoFang_ClearRtcWakeFlag();
        debug_send("���������ˮ������");
        ENTER_SENDALARM_STATUS; //���뷢������
      }
    }
    else
    {
      if(TRIGGER_NOT != struXiaoFangAlarmTimes.TriggerStatus)  //����ڸ澯�����з����˶���
      {
        debug_send("���͹����з����˳�ˮ���������½��������̡�");
        CoverSysFunState = CoverSysFunState_Detection;  //�ص����¼�����̡�
//        sysFunState = 0;  //�ص����¼�����̡�
        break;
      }
      NewInactAngle_ClearSamplingLockFlag(); //�����ֹ���������
      if(NewInactAngle_GetNewAngleEndFlag()) //�ȴ��ǶȾ�ֹ��ɡ�
      {
        Analyse_data_para.alarm_flag = 0;
        Adxl362Para.Read_flag = 0;
        uniAlarm.AlarmAllData = 0X00;     //������и澯
        struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
//        XiaoFang_ClearRtcWakeFlag();
        debug_send(">>>ϵͳ��������");
        CoverSysFunState = CoverSysFunState_InterLPM;
      }
    }
    break;
  case CoverSysFunState_AnalyzeACC://
    if (sysFunTimecount < 5000) //��ʱ�˳�
    {
      Analyse_data_para_XiaoFangZ.present_angle= get_angle_Z(ADXL362_Get_Value());//��ȡZ������ݡ�
      XiaoFang_Analyse(&Analyse_data_para_XiaoFangZ);  //����Z������
      Analyse_data_para_XiaoFangX.present_angle =get_angle_X(ADXL362_Get_Value());//��ȡX������ݡ�
      XiaoFang_Analyse(&Analyse_data_para_XiaoFangX);  //����X������
      //
      if((1 == Analyse_data_para_XiaoFangZ.alarm_flag) || (1 == Analyse_data_para_XiaoFangX.alarm_flag))
      {
//        Pack_Rotation_Angle(Analyse_data_para_XiaoFangX.present_angle); //�����ת�Ƕ�
//        Pack_LeanAngle(Analyse_data_para_XiaoFangZ.present_angle);//�����б�Ƕ�
        Sys_keydata.rotation_angle= Analyse_data_para_XiaoFangX.present_angle;
        Sys_keydata.lean_angle =Analyse_data_para_XiaoFangZ.present_angle;
        
        if((1 == Analyse_data_para_XiaoFangZ.alarm_flag)) // ������б����
        {
          PrintString_("��ʼ��б�澯!");
          uniAlarm.struAlarmBit.laenAlarm = 1;
          //               Pack_Lean_Alarm(1);//�����б�澯��
          ENTER_SENDALARM_STATUS;
          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//�������ģʽ������״̬
        }
        
        if((1 == Analyse_data_para_XiaoFangX.alarm_flag)) // ������ת����
        {
          PrintString_("��ʼ���Ǹ澯!");
          //                Pack_Rotation_Alarm(1); //�����ת����
          uniAlarm.struAlarmBit.openAlarm = 1;
          ENTER_SENDALARM_STATUS;
          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//�������ģʽ������״̬
        }
      }
      else //�������δ�澯
      {
        if(NewInactAngle_GetStaticFlag()) //
        {
//          sysFunState = 4;
          CoverSysFunState = CoverSysFunState_WaitInterLPM;
          //                ClsAnalyseState();//�������ģʽ������״̬
          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//�������ģʽ������״̬
          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//�������ģʽ������״̬
          //          PrintString_("none valid angle!back to lowpwer mode!");
          //          GetNewInactAngle();
        }
      }
    }
    else
    {
      //PrintString_("@");
      //            ClsAnalyseState();//�������ģʽ������״̬
      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//�������ģʽ������״̬
      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//�������ģʽ������״̬
      PrintString_("Exit!");
//      sysFunState = 4;//���½���͹�������
      CoverSysFunState = CoverSysFunState_WaitInterLPM;
      //Pack_AlarmFlag(1);//���ñ�����־
      //PackageMsg();//�ȷ�װ���ٷ���
      //sysFunState = 2;//������Ϣ
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
**������:       Cover_Interrupt_Service;
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2017-12-07,By Haixing.Huang
****************************************************************************************/
//void Cover_Interrupt_Service()
//{
////  if(SJ_GET_OUTSTATUS)  //����״̬�������Ǹߵ�ƽ��Ϊ�������ء�
////  {
////    struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_RISING;
////    debug_send("ˮ�������ж�");
////  }
////  else  //����״̬�������ǵ͵�ƽ��Ϊ���½��ء�
////  {
////    struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_FALLING;
////    debug_send("ˮ�½����ж�");
////  }
//}
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
/***************************************************************************************
**������:       SetHaveWaterAlarmFalg
**��������:     ��λHaveWaterAlarmFalg
**�������:     --
**�������:     --
**��ע:         YY.c,2017-12-04,By Haixing.Huang
****************************************************************************************/
void SetHaveWaterAlarmFalg(void)
{
  struXiaoFangTimes.HaveWaterAlarmFalg = TRUE;
}
/***************************************************************************************
**������:       SetHaveWaterAlarmFalg
**��������:     ��λHaveWaterAlarmFalg
**�������:     --
**�������:     --
**��ע:         YY.c,2017-12-04,By Haixing.Huang
****************************************************************************************/
void ReSetHaveWaterAlarmFalg(void)
{
  struXiaoFangTimes.HaveWaterAlarmFalg = FALSE;
}

/***************************************************************************************
**������:       SetHaveWaterAlarmFalg
**��������:     ��λHaveWaterAlarmFalg
**�������:     --
**�������:     --
**��ע:         YY.c,2017-12-04,By Haixing.Huang
****************************************************************************************/
bool GetHaveWaterAlarmFalg()
{
  return struXiaoFangTimes.HaveWaterAlarmFalg;
}
#endif
