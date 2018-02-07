#include "PressureSys.h"
#if  PRESSURESYS_MODULE_EN //ģ��ʹ��
#define PRESSURESYS_PRINT_16NUM_R(info) PrintU16int2String_(info)
#define PRESSURESYS_PRINT_R(info) debug_send_no_end(info)

#warning ˮѹģ��ʹ��!
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
**������:       ResetUpdataInit;
**��������:     ��λʱǿ���ϱ�����
**�������:     --
**�������:     --
**��ע:         YY.c,2017-12-09,By Haixing.Huang
****************************************************************************************/
void ResetUpdataInit()
{
  uniAlarm.struAlarmBit.resetAlarm = 1;
}
/***************************************************************************************
**������:       PressureSysFunStateInit;
**��������:     ��ʼ���豸״̬��
**�������:     --
**�������:     --
**��ע:         YY.c,2017-12-09,By Haixing.Huang
****************************************************************************************/
void PressureSysFunStateInit()
{
  PressureSysFunState = PressureSysFunState_KeyDetection;
}
/***************************************************************************************
**������:       AlarmSentIntervalCheck;
**��������:     ��ʼ����ʱ,���ʱ������Ϊ65535��0XFFFF����ʾȡ��
**�������:     --
**�������:     --
**��ע:         YY.c,2017-11-15,By Haixing.Huang
****************************************************************************************/
s8 AlarmSentIntervalSet(s32 *locktime,u16 time)
{
  if(0XFFFF != time)
  {
    *locktime = TimeGetTick(FALSE);//��ȡʱ��
#if SPEED_UP
    *locktime += (time*6);//�õ�Ŀ��ʱ��� ���԰汾
#else 
    *locktime += (time*60);//�õ�Ŀ��ʱ��� ��ʽ�汾
#endif 
    return 1;
  }
  else
  {
    return 0;
  }
}
/***************************************************************************************
**������:       AlarmSentIntervalCheck;
**��������:     ���ʱ���Ƿ񵽴�
**�������:     --
**�������:     --
**��ע:         YY.c,2017-11-15,By Haixing.Huang
****************************************************************************************/
s8 AlarmSentIntervalCheck(s32 *locktime)
{
  s32 tmp_s32;
  tmp_s32 = TimeGetTick(FALSE);
  if(tmp_s32 > *locktime) //�������������ʱ��
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/***************************************************************************************
**������:       AlarmPrintf;
**��������:     �澯��ӡ��
**�������:     u16 value
**�������:     --
**��ע:         YY.c,2017-12-09,By Haixing.Huang
****************************************************************************************/
s8 AlarmPrintf(u16 value)
{
  _uniAlarm uniAlarmTmp;
  uniAlarmTmp.AlarmAllData = value;
  
  if(1 == uniAlarmTmp.struAlarmBit.pressureAlarmUpLimit)
  {
    debug_send("���޸澯"); 
  }
  if(1 == uniAlarmTmp.struAlarmBit.pressureAlarmDownLimit)
  {
    debug_send("���޸澯");
  }
  if(1 == uniAlarmTmp.struAlarmBit.WithoutPressureSensors)
  {
    debug_send("δ���Ӵ������澯");
  }
  if(1 == uniAlarmTmp.struAlarmBit.KeyUpdata)
  {
    debug_send("�˹������澯");
  }
  
  if(1 == uniAlarmTmp.struAlarmBit.pressureResolutionAlarm)
  {
    debug_send("�ֱ����ϱ�");
  }
  if(1 == uniAlarmTmp.struAlarmBit.resetAlarm)
  {
    debug_send("��λ�ϱ�");
  }
  return 0;
}
/***************************************************************************************
**������:       AlarmEnableBitcheck;
**��������:     ���澯��Чλ�����θ澯λ��
**�������:     u16 *uniAlarm,u16 value
**�������:     --
**��ע:         YY.c,2017-12-09,By Haixing.Huang
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
    debug_send("���޸澯������");
    uniAlarmpTmp->struAlarmBit.pressureAlarmUpLimit = 0; //��Ӧ�澯λ�ø澯��� 
  }
  if(1 == uniAlarmTmp.struAlarmBit.pressureAlarmDownLimit)
  {
    debug_send("���޸澯������");
    uniAlarmpTmp->struAlarmBit.pressureAlarmDownLimit = 0;
  }
  if(1 == uniAlarmTmp.struAlarmBit.WithoutPressureSensors)
  {
    debug_send("δ���Ӵ������澯������");
    uniAlarmpTmp->struAlarmBit.WithoutPressureSensors = 0; //��Ӧ�澯λ�ø澯��� 
  }
  if(1 == uniAlarmTmp.struAlarmBit.KeyUpdata)
  {
    debug_send("�˹������澯������");
    uniAlarmpTmp->struAlarmBit.KeyUpdata = 0; //��Ӧ�澯λ�ø澯��� 
  }
  
  if(1 == uniAlarmTmp.struAlarmBit.pressureResolutionAlarm)
  {
    debug_send("�ֱ����ϱ�������");
    uniAlarmpTmp->struAlarmBit.pressureResolutionAlarm = 0; //��Ӧ�澯λ�ø澯��� 
  }
  return 0;
}

/***************************************************************************************
**������:       AlarmValidityCheck
**��������:     �澯����Ч�Լ��
**�������:     u16 *uniAlarm,u16 value
**�������:     --
**��ע:         YY.c,2017-12-09,By Haixing.Huang
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
    debug_send("δ���Ӵ��������޸澯��Ч");
    debug_send("δ���Ӵ��������޸澯��Ч"); 
    debug_send("δ���Ӵ������ֱ����ϱ��澯��Ч"); 
  }
  return 1;
}
///***************************************************************************************
//**������:       AlarmIntervalCheck
//**��������:     ���澯���
//**�������:     u16 *uniAlarm
//**�������:     --
//**��ע:         YY.c,2017-12-09,By Haixing.Huang
//****************************************************************************************/
//s8 AlarmIntervalCheck(u16 value)
//{
//  _uniAlarm uniAlarmTmp;
//  uniAlarmTmp.AlarmAllData = value;
//  s8 s8_tmp =0;
//  bool ValueValidFlag = TRUE;
//  if(1 == uniAlarmTmp.struAlarmBit.WithoutPressureSensors)
//  {
//    ValueValidFlag = FALSE; //�����޸澯��ѹ���仯�ʸ澯ȫ����Ч��
//    if(Is_flag_alarmtime(getAlarmTimeNoSensor()))
//    {
//      Reset_alarmtime(getAlarmTimeNoSensor(),
//                      *getNoSensorAlarmInterval());   //��д�澯���
//      s8_tmp =1;
//      
//    }
//    else
//    {
//      debug_send("δ���Ӵ������澯�ڼ����"); 
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
//        debug_send("���޸澯�ڼ����"); 
//      }
//    }
//    else
//    {
//      debug_send("δ���Ӵ��������޸澯��Ч"); 
//    }
//  }
//  if(1 == uniAlarmTmp.struAlarmBit.pressureAlarmDownLimit)
//  {
//    if(ValueValidFlag)
//    {
//      if(Is_flag_alarmtime(getAlarmTimePressureDownLimit()))
//      {
//        Reset_alarmtime(getAlarmTimePressureDownLimit(),
//                        *getPressureDownLimitAlarmInterval());  //��д�澯���
//        s8_tmp =1;
//      }
//      else
//      {
//        debug_send("���޸澯�ڼ����"); 
//      }
//    }
//    else
//    {
//      uniAlarmTmp.struAlarmBit.pressureAlarmDownLimit = 1;
//      debug_send("δ���Ӵ��������޸澯��Ч"); 
//    }
//  }
//  
//  if(1 == uniAlarmTmp.struAlarmBit.KeyUpdata)
//  {
//    if(Is_flag_alarmtime(getAlarmTimeTouchOff()))
//    {
//      Reset_alarmtime(getAlarmTimeTouchOff(),
//                      *getTouchOffAlarmInterval());  //��д�澯���
//      s8_tmp =1;
//    }
//    else
//    {
//      debug_send("�˹������澯�ڼ����"); 
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
//                        *getResolutionAlarmInterval());  //��д�澯���
//        s8_tmp =1;
//      }
//      else
//      {
//        debug_send("�ֱ����ϱ��澯�ڼ����"); 
//      }
//    }
//    else
//    {
//      debug_send("δ���Ӵ������ֱ����ϱ��澯��Ч"); 
//    }
//  }
//  if(1 == uniAlarmTmp.struAlarmBit.resetAlarm)
//  {
//    s8_tmp =1;
//    debug_send("��λ�ϱ�"); 
//  }
////  ShowAlarmTimer();
//  return s8_tmp;
//}
/***************************************************************************************
**������:       AlarmIntervalCheck
**��������:     ���澯���
**�������:     u16 *uniAlarm
**�������:     --
**��ע:         YY.c,2017-12-09,By Haixing.Huang
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
                      *getNoSensorAlarmInterval());   //��д�澯���
      s8_tmp =1;
      
    }
    else
    {
      debug_send("δ���Ӵ������澯�ڼ����"); 
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
        debug_send("���޸澯�ڼ����"); 
      }
  }
  if(1 == uniAlarmTmp.struAlarmBit.pressureAlarmDownLimit)
  {
      if(Is_flag_alarmtime(getAlarmTimePressureDownLimit()))
      {
        Reset_alarmtime(getAlarmTimePressureDownLimit(),
                        *getPressureDownLimitAlarmInterval());  //��д�澯���
        s8_tmp =1;
      }
      else
      {
        debug_send("���޸澯�ڼ����"); 
      }
  }
  
  if(1 == uniAlarmTmp.struAlarmBit.KeyUpdata)
  {
    if(Is_flag_alarmtime(getAlarmTimeTouchOff()))
    {
      Reset_alarmtime(getAlarmTimeTouchOff(),
                      *getTouchOffAlarmInterval());  //��д�澯���
      s8_tmp =1;
    }
    else
    {
      debug_send("�˹������澯�ڼ����"); 
    }
  }
  
  if(1 == uniAlarmTmp.struAlarmBit.pressureResolutionAlarm)
  {
      if(Is_flag_alarmtime(getAlarmTimeResolutionAlarm()))
      {
        Reset_alarmtime(getAlarmTimeResolutionAlarm(),
                        *getResolutionAlarmInterval());  //��д�澯���
        s8_tmp =1;
      }
      else
      {
        debug_send("�ֱ����ϱ��澯�ڼ����"); 
      }
  }
  return s8_tmp;
}


void ShowHeartCount()
{
  PRESSURESYS_PRINT_R("����������:");
  PRESSURESYS_PRINT_16NUM_R(*getheart_timecount());
  PRESSURESYS_PRINT_R("����Ŀ��ֵ:");
  PRESSURESYS_PRINT_16NUM_R(XiaoFang_HeartdataSet(&Sys_keydata));
}
/***************************************************************************************
**������:       PressureSysFun
**��������:     ˮѹҵ�����̳���
**�������:     --
**�������:     --
**��ע:         YY.c,2017-11-23,By Haixing.Huang
****************************************************************************************/
void PressureSysFun(void) 
{
  u16 tmp_u16;
  switch (PressureSysFunState) 
  {
  case PressureSysFunState_Initialize:
    uniAlarm.AlarmAllData = 0X00; //������еĸ澯
    Sys_keydata.Alarm=uniAlarm.AlarmAllData; 
    PressureSysFunState = PressureSysFunState_KeyDetection;
    break;
  case  PressureSysFunState_KeyDetection:
    if(PressureKeyUpdataConfirm())
    {
      debug_send("���������ϱ�����!");
      uniAlarm.struAlarmBit.KeyUpdata = 1; 
    }
    PressureSysFunState = PressureSysFunState_getPressure;
    break;
  case PressureSysFunState_getPressure:
    POW24_ON();  //����ˮѹ���
    DelayT_ms(500); //��500ms�������ϵ�ʱ��
    AdcInit(); //��ʼ��ADC
    tmp_u16 = GetADCValue();
    Sys_keydata.SamplingValue = tmp_u16;
    AdcClear();
    POW24_OFF();  //�ر�ˮѹ���
    
    if(tmp_u16 > 250 ) //����0.2V 
    {
      *getPressureData() = GetPressureValue(tmp_u16);//��ȡ��ѹ��ֵ��
      Sys_keydata.pressure_value = *getPressureData();
    }
    else //δ���Ӵ�������
    {
      Sys_keydata.pressure_value = 0; //��дһ��α����Ϊ0
      PRESSURESYS_PRINT_R(" ����������δ���Ӵ�����������");
      PRESSURESYS_PRINT_R("\n");
      uniAlarm.struAlarmBit.WithoutPressureSensors = 1; //��ӵ� δ��⵽�������澯�б�
    }
    PRESSURESYS_PRINT_R("_=====>ѹ��ֵΪ��");
    PRESSURESYS_PRINT_16NUM_R(Sys_keydata.pressure_value);
    *getTimesTamp() = TimeGetTick(TRUE);//��ȡʱ��
    Sys_keydata.TimesTamp = *getTimesTamp();
    PressureSysFunState = PressureSysFunState_Analyse;
    break;
  case PressureSysFunState_Analyse:
    strPressureControl.CurrentValue = Sys_keydata.pressure_value;   //��ȡ���µ�ѹ��ֵ��
    if(strPressureControl.CurrentValuePre >= strPressureControl.CurrentValue)   //���ѹ����ֵ��
    {
      strPressureControl.CurrentValueDifference = strPressureControl.CurrentValuePre- strPressureControl.CurrentValue;
    }
    else
    {
      strPressureControl.CurrentValueDifference = strPressureControl.CurrentValue- strPressureControl.CurrentValuePre;
    }  
    if(strPressureControl.CurrentValueDifference > Sys_keydata.pressure_trigger_value) //�仯ֵ����һ����Χ��
    {
      debug_send("�����仯��Χ������");
      uniAlarm.struAlarmBit.pressureResolutionAlarm = 1;
    }
    if(strPressureControl.CurrentValue > (*getPressureUpLimit()))
    {
      uniAlarm.struAlarmBit.pressureAlarmUpLimit = 1; //���޸澯
    }
    else if(strPressureControl.CurrentValue < (*getPressureDownLimit()))
    {
      uniAlarm.struAlarmBit.pressureAlarmDownLimit = 1;// ���޸澯
    }
    else
    {
      uniAlarm.struAlarmBit.pressureAlarmDownLimit = 0; //û�и澯
      uniAlarm.struAlarmBit.pressureAlarmUpLimit = 0;//û�и澯
    }
    PressureSysFunState = PressureSysFunState_WaitSendData;
    break;
  case PressureSysFunState_WaitSendData:
    if(0 != uniAlarm.AlarmAllData)
    {
      AlarmPrintf(uniAlarm.AlarmAllData);//��ӡ�澯����
      AlarmEnableBitCheck(&uniAlarm,*getAlarmEnableBit());//���澯��Чλ��
      AlarmValidityCheck(&uniAlarm);
      if(AlarmIntervalCheck(uniAlarm.AlarmAllData)) //���澯�����
      {
        PressureSysFunState = PressureSysFunState_SendData; // ���뷢������
        ClearHeartTimeCount(); //�������������
      }
      else //û�����澯
      {
        
        PressureSysFunState = PressureSysFunState_WaitInterLPM;  //�����Ϸ���������������������
      }
      
    }
    else //û�и澯����������
    {
      PressureSysFunState = PressureSysFunState_WaitInterLPM;
    }
    if(*getheart_send_flag()) //���������־λ
    {
      PRESSURESYS_PRINT_R("��������"); 
      PRESSURESYS_PRINT_R("\n");
      *getheart_send_flag() = 0;  
      PressureSysFunState = PressureSysFunState_SendData; // ���뷢������
    }
    Refresh_all_alarmtime(); //ˢ�¼������
    ShowHeartCount();
    ShowAlarmTimer();
    //������Ҫ��FLASHд�����ݡ�
    EnFLASH_Queue(Sys_keydata.TimesTamp,Sys_keydata.pressure_value,Sys_keydata.Alarm);
//    DeFLASH_Queue(&struPressureAData.TimesTamp,&struPressureAData.PressureData,&struPressureAData.AlarmList);
    break;
  case PressureSysFunState_SendData:       //���ͱ���/����
    Sys_keydata.Alarm=uniAlarm.AlarmAllData; 
#if APP_CAN_UPDATA
    iap_data.writeflag =MODE_ING_DOWNLOAD;
#else
    debug_send("δʹ��APP��������");
#endif
    PressureSysFunState = PressureSysFunState_WaitInterLPM;
    break;
  case PressureSysFunState_WaitInterLPM:
    PressureSysFunState = PressureSysFunState_InterLPM;
    break;
  case PressureSysFunState_InterLPM://�����־�����½���͹�������
    strPressureControl.CurrentValuePre = strPressureControl.CurrentValue; //���ϴε�ֵ 
    PressureSysFunState = PressureSysFunState_Initialize;//���߻��Ѻ󣬴�ͷ��ʼִ��
    debug_send("����͹���");
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