#include "NewInactAngle.h"
#include "config.h"

_struNewInactAngleControl struNewInactAngleControl;


/***************************************************************************************
**������:       NewInactAngle_InitPara()
**��������:     ��ʼ����ֹ����״̬�� 
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_InitPara()
{
  struNewInactAngleControl.NewInactAngleState = NewInactAngleState_Initialize;
}
/***************************************************************************************
**������:       NewInactAngle_GetTashSchedule_State
**��������:     ��ȡ��ǰ��������״̬��
**�������:     --
**�������:     ������״̬��
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
u8 NewInactAngle_GetTashSchedule_State(void)
{
  return TashSchedule_State;
}

/***************************************************************************************
**������:       NewInactAngle_GetsysFunState
**��������:     ��ȡsysFunState״̬
**�������:     --
**�������:     sysFunState״̬
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
u8 NewInactAngle_GetsysFunState(void)
{
  return sysFunState;
}

/***************************************************************************************
**������:       NewInactAngle_SetZeroAngle 
**��������:     �������Ƕ�
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_SetZeroAngle(u8 praZ,u8 praX,u8 praY)
{
//  Analyse_data_para.zero_angle = praA;
  Analyse_data_para_XiaoFangZ.zero_angle = praZ;
  Analyse_data_para_XiaoFangX.zero_angle = praX;
  
}

/***************************************************************************************
**������:       NewInactAngle_SetStaticFlag
**��������:     ��̬��־��λ���ⲿ�ӿڡ�
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_SetStaticFlag()
{
  struNewInactAngleControl.StaticFlag =TRUE;
}

/***************************************************************************************
**������:       NewInactAngle_ClearStaticFlag
**��������:     ��̬��־��λ���ⲿ�ӿڡ�
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_ClearStaticFlag()
{
  struNewInactAngleControl.StaticFlag =FALSE;
}
/***************************************************************************************
**������:       NewInactAngle_GetStaticFlag
**��������:     ��̬��־��ȡ���ⲿ�ӿڡ�
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
bool NewInactAngle_GetStaticFlag()
{
  return struNewInactAngleControl.StaticFlag;
}
/***************************************************************************************
**������:       
**��������:     
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_ClearNewAngleEndFlag()
{
  struNewInactAngleControl.NewAngleEndFlag = FALSE;
}
/***************************************************************************************
**������:       
**��������:     
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
bool NewInactAngle_GetNewAngleEndFlag()
{
  return struNewInactAngleControl.NewAngleEndFlag;
}
/***************************************************************************************
**������:       NewInactAngle_TimerService
**��������:     ��ʱ���������ʹ��ʱҪ��ӵ���Ӧ�Ķ�ʱ���жϷ�������С� 
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
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
**������:       
**��������:     
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_TimerSet()
{
  struNewInactAngleControl.SamplingTimeCnt = SAMPLINGTIMECNT_LIMIT; 
}

/***************************************************************************************
**������:         --
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
bool NewInactAngle_WhetherWake(void)
{
  if(TASHSCHEDULE_RUN == NewInactAngle_GetTashSchedule_State()) //�ڷ�Э�����״̬        
  {
    if(SYSFUNSTATE_HALT != NewInactAngle_GetsysFunState())      //�ڷ�����״̬ʱ
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
**������:         --
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
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
**������:       NewInactAngle_ClearSamplingLockFlag
**��������:     ��λSamplingLockFlag
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_SetSamplingLockFlag()
{
  struNewInactAngleControl.SamplingLockFlag = TRUE;
}
/***************************************************************************************
**������:       NewInactAngle_ClearSamplingLockFlag
**��������:     ���SamplingLockFlag
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_ClearSamplingLockFlag()
{
  struNewInactAngleControl.SamplingLockFlag = FALSE;
}
/***************************************************************************************
**������:       NewInactAngle_Service
**��������:     
**�������:     --
**�������:     --
**��ע:         YY.c,2017-08-09 ,By Haixing.Huang
****************************************************************************************/
void NewInactAngle_Service(void)
{
  if(FALSE == struNewInactAngleControl.SamplingLockFlag) //�������������²��������ݾ�ֹ���жϡ�
  {
    if ((ADXL362_ReadReg(ADXL362_REG_STATUS)&ADXL362_STATUS_AWAKE)) //������ѯģ��ķ�ʽ
    {
      struNewInactAngleControl.NewInactAngleState = NewInactAngleState_Initialize; //�����ʼ�׶�
    }
    if(FALSE == struNewInactAngleControl.StaticFlag)
    {
      struNewInactAngleControl.NewInactAngleState = NewInactAngleState_Initialize; //�����ʼ�׶�
    }
    switch( struNewInactAngleControl.NewInactAngleState)
    {
    case NewInactAngleState_Idle:  //����
      break;
    case NewInactAngleState_Initialize: //��ʼ��
      struNewInactAngleControl.NewInactAngleState = NewInactAngleState_FirstAwake;
      NewInactAngle_ClearPara();
      break;
    case NewInactAngleState_FirstAwake: //�Ӵ������жϻ�ȡ����ֹ�ж��źű�־��
      if(TRUE == struNewInactAngleControl.StaticFlag)
      {
        struNewInactAngleControl.NewInactAngleState = NewInactAngleState_GetPreNewAngleTmp; //�����ȡ�½Ƕ�״̬
      }
      break;
    case NewInactAngleState_GetPreNewAngleTmp:
      if (!(ADXL362_ReadReg(ADXL362_REG_STATUS)&ADXL362_STATUS_AWAKE))  //����Ƿ��Ǿ�ֹ״̬
      {
        struNewInactAngleControl.PreNewAngleTmpZ = Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_Z);
        struNewInactAngleControl.PreNewAngleTmpX = Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_X);
        PrintString("PreNewAngleTmpZ:");
        printNum2Str(struNewInactAngleControl.PreNewAngleTmpZ);
        PrintString("PreNewAngleTmpX:");
        printNum2Str(struNewInactAngleControl.PreNewAngleTmpX);
        struNewInactAngleControl.NewInactAngleState = NewInactAngleState_VerifyNewAngle;
        //���￪�������ʱ��
        NewInactAngle_TimerSet(); //���������ʱ����
      }
      else
      {
        struNewInactAngleControl.NewInactAngleState = NewInactAngleState_Initialize; //�����ʼ�׶�
      }
      break;  
    case NewInactAngleState_VerifyNewAngle:
//      if(NewInactAngle_WhetherWake()) //��ȡ����״̬,���������������״̬��������е�״̬��
//      {
//        NewInactAngle_ClearPara();
//        break;
//      }
      if(TRUE == struNewInactAngleControl.SamplingTimeOverFlag )  //���ʱ�䳬ʱ��־
      {
        struNewInactAngleControl.SamplingTimeOverFlag = FALSE; //���ʱ�䳬ʱ��־
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
          if(struNewInactAngleControl.AngleSameCnt >= ANGLESAMECNT_LIMIT) //����ANGLESAMECNT_LIMIT�� ��ȡ��һ���ĽǶ�ֵ ˵���Ƕ��л�����
          {
            PrintString("!!!!!!!!!!!New AgnleZ:");
            printNum2Str(struNewInactAngleControl.NewAngleTmpZ);
            PrintString("!!!!!!!!!!!New AgnleX:");
            printNum2Str(struNewInactAngleControl.NewAngleTmpX); 
            NewInactAngle_SetZeroAngle(struNewInactAngleControl.NewAngleTmpZ,struNewInactAngleControl.NewAngleTmpX,0); //�������
            NewInactAngle_ClearPara();
            struNewInactAngleControl.NewInactAngleState = NewInactAngleState_Exit; //�˳�
            break; //ǿ���˳�
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
            struNewInactAngleControl.NewInactAngleState = NewInactAngleState_Exit;//�˳�
            break; //ǿ���˳�
          }
          PrintString("<<<<<<<<<<TryCnt:");
          printNum2Str(struNewInactAngleControl.TryCnt);
        }
        NewInactAngle_TimerSet(); //���������ʱ����
        //ʱ������
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