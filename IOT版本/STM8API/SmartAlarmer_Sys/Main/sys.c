#include "stm8l15x.h"
#include <string.h>
#include "config.h"


u8 sysFunState = 0;
u16 sysFunTimecount = 0;
//ADXL_XYZ_ANGLE temp_xyz_angle[10];

const char NO[16]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46};

//u8 getXyzZeroposFunState = 0;
//u8 getXyzOffsetAngleFunState = 0;
//u16 getXyzOffsetAngleTimecount = 0;
//u8 SetZeroPosFunState = 0;
//u16 SetZeroPosTimecount = 0;

//Liquid_Alarm LiquidAlarm;
ADXL362_PARA Adxl362Para;


u16 heart_timecount = 0;
_Bool heart_send_flag = 0;
char *tempstring = "";

u16 waitforconfig_timecount = 0;
_Bool para_config_flag = 0;

u16 sys_overtime_count = 0;//��ʱ����


//static u8 AnalyseData_State = 0;
ANALYSE_DATA_PARA Analyse_data_para = {
	20000,//��ʱʱ�� Ĭ��20S //over_time;//
	0,//���Ƕ� Ĭ��0�� //zero_angle;//
	6,//�趨�Ƕ� Ĭ��6�� //set_angle;//
	0,//��ǰ�Ƕ� Ĭ��0�� // u8 present_angle;//
	3,//��Ч�ǶȲ�ֵ Ĭ��2�� //valid_angle_d_value;//
	6,//�趨�����Ч�Ƕ�������Ŀ Ĭ��8�� set_valid_angle_unbroken_num;//
	0,//��Ч�Ƕ�������Ŀ Ĭ��0�� //valid_angle_unbroken_num;//
	0,//״̬��״ֵ̬ Ĭ��0 analyse_state;//
	0,//��̬��־ Ĭ��0 u8 inact_flag :1;//�ϵ�Ĭ��Ϊ��̬
	0,//������־ Ĭ��0 alarm_flag :1;//
        0,//����ȷ�ϱ�־ Ĭ��0 sencond_query_flag
        0,//����ȷ�Ͽ�ʼ���ͱ�־��Ĭ��0 sencond_query_start_flag
        0,//����ȷ�Ͻ������ͱ�־��Ĭ��0 sencond_query_finish_flag
        0,//����æ��־��Ĭ��0 send_busy_flag
};

#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
const SYS_KEYDATA Default_Sys_keydata =
{  /***************************����Ĳ�����Ҫ����Э�����õõ�******************************/

  DEVICE_ID,//u8 device_id[17];//�豸ID
  3,//u8 soft_version[2];//����汾//�޸�Э�飬������汾�̶���SOFTWARE_VER��������12
  2,//u8 harware_version[2];//Ӳ���汾 //�޸�Э�飬Ӳ���汾�����������ֽڣ���V12.34==0x0c,0x22
  0,//u8 status; //�豸״̬    Uchar    ���״̬ ����״̬ ��װ״̬ ����״̬
  0,//u8 SigStren;//�ź�ǿ��
  // ��ص�ѹ
  0,//u16 send_totalcount;//��ط����ܴ���
//  //�����¶�
  10,//u8 rotation_trigger_angle;//��ת�����Ƕ�
  10,//u8 rotation_trigger_sens;//��ת����������
  10,//u8 lean_trigger_angle;//��б�����Ƕ�
  10,//u8 lean_trigger_sens;//��б����������
  100,//u8 pressure_trigger_value;//ѹ��������ֵ
  10,//u8 pressure_interval;//ѹ�������
  0,//u16 Alarm;//�澯
  4,//u16 heartdata_sent_interval_hour_count;//�������ʱ��
  2,//u16 alarm_sent_interval_minute_count;//�������ʱ��
  30,//u16 trigger_interval_minute_count;//�������ʱ��
  DEFAULT_CLOUD_SERVER_ADD,//u8 default_cloud_server_add[22];//�ƶ˷�������ַ255.255.255.255,65535
  DEFAULT_CLOUD_SERVER_ADD,//u8 update_server_add[22];//������������ַ255.255.255.255,65535
  1,//u8 carrier_type;//��Ӫ��   �����޸� ��Ӳ���汾
  1,//u8 module_type;//ģ����ʽ �����޸� ��Ӳ���汾
  IOT_NUM,//u8 Num_Cmd[21];  //���������š�
//#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
//  0,//s32 TimesTamp;  //ʱ��� 
//  800,//pressure_upLimit; //ѹ������
//  100,//u16 pressure_downLimit; //ѹ������
//#endif  
//  /*************************************************************************/ 
  0,//u16 pressure_value;//ѹ��
  0,//u16 rotation_angle;//��ת�Ƕ�
  0,//u8 lean_angle;//��б�Ƕ�
//#endif
//#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
//  DEVICE_ID,//u8 device_id[17];//�豸ID
//#endif  
};
#endif 
#if APP_SELECT_COVER
SYS_KEYDATA Sys_keydata = 
{  /***************************����Ĳ�����Ҫ����Э�����õõ�******************************/
 //һ�´������Ƹǳ������еĴ���
  DEVICE_ID,//u8 device_id[17];//�豸ID
  3,//u8 soft_version[2];//����汾//�޸�Э�飬������汾�̶���SOFTWARE_VER��������12
  2,//u8 harware_version[2];//Ӳ���汾 //�޸�Э�飬Ӳ���汾�����������ֽڣ���V12.34==0x0c,0x22
  0,//u8 status; //�豸״̬    Uchar    ���״̬ ����״̬ ��װ״̬ ����״̬
  0,//u8 SigStren;//�ź�ǿ��
  // ��ص�ѹ
  0,//u16 send_totalcount;//��ط����ܴ���
//  //�����¶�
  10,//u8 rotation_trigger_angle;//��ת�����Ƕ�
  10,//u8 rotation_trigger_sens;//��ת����������
  10,//u8 lean_trigger_angle;//��б�����Ƕ�
  10,//u8 lean_trigger_sens;//��б����������
  100,//u8 pressure_trigger_value;//ѹ��������ֵ
  10,//u8 pressure_interval;//ѹ�������
  0,//u16 Alarm;//�澯
  4,//u16 heartdata_sent_interval_hour_count;//�������ʱ��
  2,//u16 alarm_sent_interval_minute_count;//�������ʱ��
  30,//u16 trigger_interval_minute_count;//�������ʱ��
  DEFAULT_CLOUD_SERVER_ADD,//u8 default_cloud_server_add[22];//�ƶ˷�������ַ255.255.255.255,65535
  DEFAULT_CLOUD_SERVER_ADD,//u8 update_server_add[22];//������������ַ255.255.255.255,65535
  1,//u8 carrier_type;//��Ӫ��   �����޸� ��Ӳ���汾
  1,//u8 module_type;//ģ����ʽ �����޸� ��Ӳ���汾
  IOT_NUM,//u8 Num_Cmd[21];  //���������š�
//  0,//s32 TimesTamp;  //ʱ��� 
//  800,//pressure_upLimit; //ѹ������
//  100,//u16 pressure_downLimit; //ѹ������
//  /*************************************************************************/ 
  0,//u16 pressure_value;//ѹ��
  0,//u16 rotation_angle;//��ת�Ƕ�
  0,//u8 lean_angle;//��б�Ƕ�

};
#endif
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
SYS_KEYDATA Sys_keydata=
{
  DEVICE_ID_PRESSURE, // u8 device_id[17];//�豸ID
  0x0C,
  0X22,//u8 harware_version[2];//Ӳ���汾 //�޸�Э�飬Ӳ���汾�����������ֽڣ���V12.34==0x0c,0x22
  0,//u8 status; //�豸״̬    Uchar    ���״̬ ����״̬ ��װ״̬ ����״̬
  0,//u8 SigStren;//�ź�ǿ��
  0,//u16 send_totalcount;//��ط����ܴ���
  0,//u16 pressure_value;//ѹ��
  0,//u16 SamplingValue; //����ԭʼֵ
  0,//u16 pressure_trigger_value;//ѹ���仯�ֱ���
  0, //u16 Alarm;//�澯
  24, //u16 heartdata_sent_interval_hour_count;//�������ʱ��
  0x00, //u16 AlarmEnableBit; //�澯ʹ��0ʧ�ܣ�1ʹ��
  120,//���޸澯���ʱ��
  120,//���޸澯���ʱ��
  120,//δ��⵽�������澯���
  120,//�ֱ����ϱ��澯���
  65535,//�˹������ϱ����
  30,//�������ʱ��
  DEFAULT_CLOUD_SERVER_ADD,//�ƶ˷�������ַ255.255.255.255,65535
  DEFAULT_CLOUD_SERVER_ADD,//������������ַ255.255.255.255,65535
  0,
  0,
  IOT_NUM,//���������š�
  0,//ʱ���
  600,//ѹ������
  300,//ѹ������
};

const SYS_KEYDATA Default_Sys_keydata=
{
  DEVICE_ID_PRESSURE, // u8 device_id[17];//�豸ID
  0x0C,
  0X22,//u8 harware_version[2];//Ӳ���汾 //�޸�Э�飬Ӳ���汾�����������ֽڣ���V12.34==0x0c,0x22
  0,//u8 status; //�豸״̬    Uchar    ���״̬ ����״̬ ��װ״̬ ����״̬
  0,//u8 SigStren;//�ź�ǿ��
  0,//u16 send_totalcount;//��ط����ܴ���
  0,//u16 pressure_value;//ѹ��
  0,//u16 SamplingValue; //����ԭʼֵ
  100,//u16 pressure_trigger_value;//ѹ���仯�ֱ���
  0, //u16 Alarm;//�澯
  24, //u16 heartdata_sent_interval_hour_count;//�������ʱ��
  0x00, //u16 AlarmEnableBit; //�澯ʹ��0ʧ�ܣ�1ʹ��
  120,//���޸澯���ʱ��
  120,//���޸澯���ʱ��
  120,//δ��⵽�������澯���
  120,//�ֱ����ϱ��澯���
  65535,//�˹������ϱ����
  10,//�������ʱ��
  DEFAULT_CLOUD_SERVER_ADD,//�ƶ˷�������ַ255.255.255.255,65535
  DEFAULT_CLOUD_SERVER_ADD,//������������ַ255.255.255.255,65535
  0,
  0,
  IOT_NUM,//���������š�
  0,//ʱ���
  600,//ѹ������
  300,//ѹ������
};
/***************************************************************************************
**������:       SamplingValue
**��������:     SamplingValue�ӿں�����
**�������:     --
**�������:     u16 *
**��ע:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
void SysKeydataInit()
{
//  ArrayToArray(Sys_keydata.device_id,DEVICE_ID,17); // u8 device_id[17];//�豸ID
  Sys_keydata.harware_version[0] = 0x0C;
  Sys_keydata.harware_version[1] = 0X22;//u8 harware_version[2];//Ӳ���汾 //�޸�Э�飬Ӳ���汾�����������ֽڣ���V12.34==0x0c,0x22
  Sys_keydata.status = 0;//u8 status; //�豸״̬    Uchar    ���״̬ ����״̬ ��װ״̬ ����״̬
  Sys_keydata.SigStren = 0;//u8 SigStren;//�ź�ǿ��
  Sys_keydata.send_totalcount = 0;//u16 send_totalcount;//��ط����ܴ���
  Sys_keydata.pressure_value = 0;//u16 pressure_value;//ѹ��
  Sys_keydata.SamplingValue = 0;//u16 SamplingValue; //����ԭʼֵ
  Sys_keydata.pressure_trigger_value = 0;//u16 pressure_trigger_value;//ѹ���仯�ֱ���
  Sys_keydata.Alarm = 0; //u16 Alarm;//�澯
  Sys_keydata.heartdata_sent_interval_hour_count = 24; //u16 heartdata_sent_interval_hour_count;//�������ʱ��
  Sys_keydata.AlarmEnableBit = 0xFF; //u16 AlarmEnableBit; //�澯ʹ��0ʧ�ܣ�1ʹ��
  Sys_keydata.PressureUpLimitAlarmInterval = 4;//���޸澯���ʱ��
  Sys_keydata.PressureDownLimitAlarmInterval = 4;//���޸澯���ʱ��
  Sys_keydata.NoSensorAlarmInterval =24;//δ��⵽�������澯���
  Sys_keydata.ResolutionAlarmInterval = 4;//�ֱ����ϱ��澯���
  Sys_keydata.TouchOffAlarmInterval = 65535;//�˹������ϱ����
  Sys_keydata.trigger_interval_minute_count = 30;//�������ʱ��
  ArrayToArray(Sys_keydata.default_cloud_server_add,DEFAULT_CLOUD_SERVER_ADD,22);//�ƶ˷�������ַ255.255.255.255,65535
  ArrayToArray(Sys_keydata.update_server_add,DEFAULT_CLOUD_SERVER_ADD,22);//������������ַ255.255.255.255,65535
  Sys_keydata.carrier_type = 0;
  Sys_keydata.module_type = 0;
  ArrayToArray(Sys_keydata.Num_Cmd,IOT_NUM,21);//���������š�
  Sys_keydata.TimesTamp = 0;//ʱ���
  Sys_keydata.pressure_upLimit = 600;//ѹ������
  Sys_keydata.pressure_downLimit = 300;//ѹ������
}
/***************************************************************************************
**������:       SamplingValue
**��������:     SamplingValue�ӿں�����
**�������:     --
**�������:     u16 *
**��ע:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getSamplingValue()
{
  return &Sys_keydata.SamplingValue;
}
/***************************************************************************************
**������:       AlarmEnableBit
**��������:     AlarmEnableBit�ӿں�����
**�������:     --
**�������:     u16 *
**��ע:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getAlarmEnableBit()
{
  return &Sys_keydata.AlarmEnableBit;
}

/***************************************************************************************
**������:       getPressureUpLimitAlarmInterval
**��������:     PressureUpLimitAlarmInterval�ӿں�����
**�������:     --
**�������:     u16 *
**��ע:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getPressureUpLimitAlarmInterval()
{
  return &Sys_keydata.PressureUpLimitAlarmInterval;
}
/***************************************************************************************
**������:       PressureDownLimitAlarmInterval
**��������:     PressureDownLimitAlarmInterval�ӿں�����
**�������:     --
**�������:     u16 *
**��ע:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getPressureDownLimitAlarmInterval()
{
  return &Sys_keydata.PressureDownLimitAlarmInterval;
}
/***************************************************************************************
**������:       NoSensorAlarmInterval
**��������:     NoSensorAlarmInterval�ӿں�����
**�������:     --
**�������:     u16 *
**��ע:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getNoSensorAlarmInterval()
{
  return &Sys_keydata.NoSensorAlarmInterval;
}

/***************************************************************************************
**������:       ResolutionAlarmInterval
**��������:     ResolutionAlarmInterval�ӿں�����
**�������:     --
**�������:     u16 *
**��ע:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getResolutionAlarmInterval()
{
  return &Sys_keydata.ResolutionAlarmInterval;
}

/***************************************************************************************
**������:       ResolutionAlarmInterval
**��������:     ResolutionAlarmInterval�ӿں�����
**�������:     --
**�������:     u16 *
**��ע:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getTouchOffAlarmInterval()
{
  return &Sys_keydata.TouchOffAlarmInterval;
}
/***************************************************************************************
**������:       getPressureUpLimit
**��������:     �ӿں�����
**�������:     --
**�������:     u16 *
**��ע:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getPressureUpLimit()
{
  return &Sys_keydata.pressure_upLimit;
}

/***************************************************************************************
**������:       getPressureDownLimit
**��������:     �ӿں�����
**�������:     --
**�������:     u16 *
**��ע:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getPressureDownLimit()
{
  return &Sys_keydata.pressure_downLimit;
}
#endif

//#define PRESENT_TEMP_ID '3','2','3','0','3','0','3','0','3','2','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'
//#define PRESENT_TEMP_ID '3','2','3','0','3','0','3','0','3','2','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'

#define FILTER_AT_OF_UART3 if(USARTx!=USART3)
//HEART_DATA Heart_data ={
//  0
//};
//union UNION_PACKET_DATA PacketData = {
//	'a', 't', '+', 'z', 'i', 'p', 's', 'e', 'n', 'd', '=', '0', ',', '2', '5', '0', 0x0D,
//	0x7E,//SOI
//	'1', '1', //VER
//	'0', '1', //ADR
//	'D', '0', //CID1
//	'1', 'A', //CID2
//	//'B', '0', 'B', 'A', //LEN
//        'A', '0', 'E', '8', //LEN
//	'0', '1', //��Ʒ���� PdType_Cmd
//	'0', '0', //�豸���� DvType_Cmd
//	'3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', //�豸��� Id_Cmd
//	'4', '3', '4', 'C', '4', '4', '5', 'A', //�ͻ��� Cstm_Cmd
//	'3', '1', '3', '3', '3', '7', '3', '3', '3', '6', '3', '7', '3',
//	'2', '3', '6', '3', '6', '3', '6', '3', '4', '3', '8', '3', '8', //���������� Num_Cmd
//	'3', '2', '3', '6', //�̼��汾 Firm_Cmd
//	'0', '0', //�豸״̬ State_Cmd
//	'1', '7', //�ź�ǿ�� SigStren_Cmd
//	'0', '0', //ʣ���ѹ ResV_Cmd
//	'1', '9', //�����¶� Temp_Cmd
//	'0', '9', //��б�Ƕ� Angle_Cmd
//	'0', '0', '0', '0', //X����ٶ� X_Value_Cmd
//	'0', '0', '0', '0', //Y����ٶ� Y_Value_Cmd
//	'0', '0', '0', '0', //Z����ٶ� Z_Value_Cmd
//	'0', '0', //��ѹ���� Low_Power_Alarm_Cmd
//	'0', '0', //���±��� Low_Temp_Alarm_Cmd
//	'0', '0', //�������� Trigger_Alarm_Cmd
//	'0', '0', //�����Ƕ� Trigger_Angle_Cmd
//	'0', '0', '0', '2', //������� Trigger_Interval_Cmd
//	'0', '0', '1', '8', //������� Alarm_Interval_Cmd(�������)
//	'0', '1', '2', 'C', //����ȷ�ϼ�� Sencond_Query_Interval_Cmd Ĭ��5���ӣ�300�룩
//	'0', '0', //������ Sens_Cmd
//	'0', '0', '0', '3', //�ۼƷ��ʹ��� Send_Totalcount_cmd //�������Rec_Interval_Cmd
//	'0', '3', //����������� Max_Count_Rec_Cmd
//	'0', '0', //��ǰ�������� Pre_Count_Rec_Cmd
//	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 
//	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', //��ǰ�ɼ���ַ
//	'0', '0', //��Ӫ�� Carrier_Cmd
//	'0', '0', //ģ����ʽ Mod_Type_Cmd
//	'0', '0', //MNC Mnc_Cmd
//	'0', '0', '0', '0', //LAC/TAC Lac_Tac_Cmd
//	'0', '0', '0', '0', '0', '0', '0', '0', //CI Ci_Cmd
//	'0', '0', '0', '0', //SID Sid_Cmd
//	'0', '0', '0', '0', //NID Nid_Cmd
//	'0', '0', '0', '0', //BID Bid_Cmd
//	'0', '0', //GPS ģ������ Gps_Type_Cmd
//	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', //GPS���� Gps_Long_Cmd
//	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', //GPSγ�� Gps_Lat_Cmd
//	'0', '0', '0', '0', //У���� Chksum_Cmd
//	0x0D,//EOI
//	'\r', '\n'
//};

//void TaskSchedule(void);//�����л�
//void Test_protocol(void);//Э�����
u16 task_schedule_timecount = 0;

u8 TashSchedule_State = 0;


//task scheduling
void TaskSchedule(void) 
{
  switch (TashSchedule_State) 
  {
  case 0:
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
        PressureSysFun();  //ˮѹҵ�����̳���
#endif
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
        CoverSysFun(); //�Ƹ�ҵ�����̳���
        NewInactAngle_Service();
#endif      
    break;
  case 1://����������
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
//       NewInactAngle_SetSamplingLockFlag();
#endif      
    PrintString_("Start to anlayse protol");
    task_schedule_timecount = 0 ;//����Э�������ʱʱ��
    TashSchedule_State = 2;
    DelayT_ms(20);
    break;
  case 2:
    if (task_schedule_timecount >= 3000) {
      PrintString_("Test_protocol overtime!");
      task_schedule_timecount = 0;
      TashSchedule_State = 3;
    }//��ʱ�˳�����
    else 
    {
      if (Select_protocol(&usart3Info)) //ѡ��Э�����
      { 
        TashSchedule_State = 3;
        PrintSysParaList();
      }
    }
    break;
  case 3:
    
    //GetNewInactAngle();//���»�ȡ��̬�Ƕ�
    Usart_RcvClr(&usart3Info);
    task_schedule_timecount = 0;
    TashSchedule_State = 0;//�˳�����Э�飬���½���������������
    PrintString_("back to lowpwer mode!");
//    IntoLowpowerMode(); //����͹���ģʽ//���½���͹���
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
   ENTER_LPM_STATUS;
#endif 
    break;
  case 4:
    break;
  case 5:
    break;
  default:
    break;
  }
}


u8 GetNewInactAngle_Fun_state = 3;
u16 GetNewInactAngle_timecount = 0;




//����һ�� ��ط����ܴ���
void Increase_send_totalcount(void) {
	Sys_keydata.send_totalcount ++;
	Write_sys_keydata_totalcount();
}

//��ӡ�̼��汾��
void PrintSysInfo(void) {
  debug_send(SOFT_VERSION);
  debug_send(HARDWARE_VERSION);
}
//��ӡ��������
void PrintSysDebugData(void){
//  sprintf( str, "MCU Software Update %s %s\r\n", __DATE__, __TIME__ );
//  debug_send(str);
  PrintString("MCU Software Update ");
  PrintString(__DATE__);
  PrintString("   ");
  PrintString_(__TIME__);
}

//��ӡϵͳ�����嵥
//��ӡ�����е�Sys_keydata�����б�  �Է���鿴
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���  
void PrintSysParaList(void) 
{
  PrintSysDebugData();
  PrintString_("================PrintSysParaList===================");
  PrintString_("Sys_keydata.device_id");
  PrintString_((char *)(&Sys_keydata.device_id));
  PrintString_("Sys_keydata.soft_version");
  PrintString_(SOFTWARE_VER);
  PrintString_("Sys_keydata.harware_version");
  //	PrintU8int2String_(Sys_keydata.harware_version);
  PrintString("V");
  printNumStr(Sys_keydata.harware_version[0]);
  PrintString(".");
  PrintU8int2String_(Sys_keydata.harware_version[1]);
  Watchdog_Feed();
  PrintString_("Sys_keydata.status");    //�豸״̬
  PrintU8int2String_(Sys_keydata.status);
  
  PrintString_("Sys_keydata.send_totalcount");    //���ʹ���
  PrintU16int2String_(Sys_keydata.send_totalcount);
  
  PrintString_("Sys_keydata.rotation_trigger_angle");    //��ת�仯�Ƕ�
  PrintU16int2String_(Sys_keydata.rotation_trigger_angle);
  
  PrintString_("Sys_keydata.rotation_trigger_sens");    //��ת������
  PrintU8int2String_(Sys_keydata.rotation_trigger_sens);
  
  PrintString_("Sys_keydata.lean_trigger_angle");    //��б�仯�Ƕ�
  PrintU8int2String_(Sys_keydata.lean_trigger_angle);
  
  PrintString_("Sys_keydata.lean_trigger_sens");    //��б������
  PrintU8int2String_(Sys_keydata.lean_trigger_sens);
  
  PrintString_("Sys_keydata.pressure_trigger_value");    //ѹ���仯��ֵ
  PrintU16int2String_(Sys_keydata.pressure_trigger_value);
  
  PrintString_("Sys_keydata.pressure_interval");    //ѹ�������
  PrintU16int2String_(Sys_keydata.pressure_interval); 
  
  PrintString_("Sys_keydata.heartdata_sent_interval_hour_count");    //�������ʱ��
  PrintU16int2String_(Sys_keydata.heartdata_sent_interval_hour_count);
  
  PrintString_("Sys_keydata.alarm_sent_interval_minute_count");    //�澯���ʱ��
  PrintU16int2String_(Sys_keydata.alarm_sent_interval_minute_count);
  
  PrintString_("Sys_keydata.trigger_interval_minute_count");    //�����ʱ��
  PrintU16int2String_(Sys_keydata.trigger_interval_minute_count);
  
  PrintString_("Sys_keydata.default_cloud_server_add");    //����ɼ���ַ
  PrintString_((char *)&Sys_keydata.default_cloud_server_add);
  
  PrintString_("Sys_keydata.update_server_add");    //������������ַ
  PrintString_((char *)&Sys_keydata.update_server_add);
  
  PrintString_("Sys_keydata.carrier_type");    //��Ӫ��
  PrintU8int2String_(Sys_keydata.carrier_type);
  
  PrintString_("Sys_keydata.module_type");    //������ʽ
  PrintU8int2String_(Sys_keydata.module_type);
  
  PrintString_("Sys_keydata.Num_Cmd");    //����������
  PrintString_((char *)Sys_keydata.Num_Cmd);
  
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���  
  PrintString_("Sys_keydata.TimesTamp");    //ʱ��
//  PrintU32int2String_(Sys_keydata.TimesTamp);
  
  PrintString_("Sys_keydata.pressure_upLimit");    //ѹ������    UINT    0~65535
//  PrintU16int2String_(Sys_keydata.pressure_upLimit);
  
  PrintString_("Sys_keydata.pressure_downLimit");    //ѹ������    UINT    0~65535
//  PrintU16int2String_(Sys_keydata.pressure_downLimit);
#endif
  PrintString_("******");
//  PrintString_("Sys_keydata.temp_offset");
  PrintString_("==============================================="); 
}

/***************************************************************************************
**������:       Parameter_Init
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
void Parameter_Init(void)
{
  Sys_keydata.Alarm =0;
  Sys_keydata.lean_angle = 0;
  Sys_keydata.rotation_angle=0;
}
#endif

#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
void PrintSysParaList(void) 
{
  PrintSysDebugData();
  PrintString_("================PrintSysParaList===================");
  PrintString_("Sys_keydata.device_id");
  PrintString_((char *)(&Sys_keydata.device_id));
  PrintString_("Sys_keydata.soft_version");
  PrintString_(SOFTWARE_VER);
  PrintString_("Sys_keydata.harware_version");
  //	PrintU8int2String_(Sys_keydata.harware_version);
  PrintString("V");
  printNumStr(Sys_keydata.harware_version[0]);
  PrintString(".");
  PrintU8int2String_(Sys_keydata.harware_version[1]);
  Watchdog_Feed();
  PrintString_("Sys_keydata.status");    //�豸״̬
  PrintU8int2String_(Sys_keydata.status);
  
  PrintString_("Sys_keydata.send_totalcount");    //���ʹ���
  PrintU16int2String_(Sys_keydata.send_totalcount);
  
//  PrintString_("Sys_keydata.pressure_value");    //ѹ��
//  PrintU16int2String_(Sys_keydata.pressure_value); 
//  
//  PrintString_("Sys_keydata.SamplingValue");    //����ԭʼֵ
//  PrintU16int2String_(Sys_keydata.SamplingValue);
//  
//  PrintString_("Sys_keydata.lean_trigger_angle");    //��б�仯�Ƕ�
////  PrintU16int2String_(Sys_keydata.lean_trigger_angle);
//  
//  PrintString_("Sys_keydata.lean_trigger_sens");    //��б������
////  PrintU16int2String_(Sys_keydata.lean_trigger_sens);
  
  PrintString_("Sys_keydata.pressure_trigger_value");    //ѹ���仯��ֵ
  PrintU16int2String_(Sys_keydata.pressure_trigger_value);
  
  
  PrintString_("Sys_keydata.heartdata_sent_interval_hour_count");    //�������ʱ��
  PrintU16int2String_(Sys_keydata.heartdata_sent_interval_hour_count);
  
  PrintString_("Sys_keydata.AlarmEnableBit");    //�澯ʹ��0ʧ�ܣ�1ʹ��
  PrintU16int2String_(Sys_keydata.AlarmEnableBit);
  
  PrintString_("Sys_keydata.PressureUpLimitAlarmInterval");    //���޸澯���ʱ��
  PrintU16int2String_(Sys_keydata.PressureUpLimitAlarmInterval);
  
  PrintString_("Sys_keydata.PressureDownLimitAlarmInterval");    //���޸澯���ʱ��
  PrintU16int2String_(Sys_keydata.PressureDownLimitAlarmInterval);
  
  PrintString_("Sys_keydata.NoSensorAlarmInterval");    //δ��⵽�������澯���
  PrintU16int2String_(Sys_keydata.NoSensorAlarmInterval);
  
  PrintString_("Sys_keydata.ResolutionAlarmInterval");     //�ֱ����ϱ��澯���
  PrintU16int2String_(Sys_keydata.ResolutionAlarmInterval);
  
  PrintString_("Sys_keydata.TouchOffAlarmInterval");     //�˹������ϱ����
  PrintU16int2String_(Sys_keydata.TouchOffAlarmInterval);
  
  PrintString_("Sys_keydata.trigger_interval_minute_count");    //�����ʱ��
  PrintU16int2String_(Sys_keydata.trigger_interval_minute_count);
  
  PrintString_("Sys_keydata.default_cloud_server_add");    //����ɼ���ַ
  PrintString_((char *)&Sys_keydata.default_cloud_server_add);
  
  PrintString_("Sys_keydata.update_server_add");    //������������ַ
  PrintString_((char *)&Sys_keydata.update_server_add);
  
  PrintString_("Sys_keydata.carrier_type");    //��Ӫ��
  PrintU8int2String_(Sys_keydata.carrier_type);
  
  PrintString_("Sys_keydata.module_type");    //������ʽ
  PrintU8int2String_(Sys_keydata.module_type);
  
  PrintString_("Sys_keydata.Num_Cmd");    //����������
  PrintString_((char *)Sys_keydata.Num_Cmd);
  
  PrintString_("Sys_keydata.TimesTamp");    //ʱ��
//  PrintU32int2String_(Sys_keydata.TimesTamp);
  
  PrintString_("Sys_keydata.pressure_upLimit");    //ѹ������    UINT    0~65535
  PrintU16int2String_(Sys_keydata.pressure_upLimit);
  
  PrintString_("Sys_keydata.pressure_downLimit");    //ѹ������    UINT    0~65535
  PrintU16int2String_(Sys_keydata.pressure_downLimit);
  PrintString_("******");
  PrintString_("===============================================");  
 
}

/***************************************************************************************
**������:       Parameter_Init
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
void Parameter_Init(void)
{
  Sys_keydata.Alarm =0;
  Sys_keydata.pressure_value = 0;
  Sys_keydata.SamplingValue = 0;
}
#endif 

#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
// **************************************************************
// ����:InitAdxl362Para
// ����1:��
// ����:��
// ����:��ʼ��adxl362ɨ�躯���Ĳ���
// **************************************************************
void InitAdxl362Para(void) 
{
	//Adxl362Para.Inteval_timecount = 0;
	Adxl362Para.Read_timecount = 0;
	//Adxl362Para.Max_readcount = 0;
	Adxl362Para.Readcount = 0;
	//Adxl362Para.Read_flag = 0;
	Adxl362Para.Busy_flag = 0;
	Adxl362Para.Scan_state = 0;
	SetAdxl362Interval_timeAndMax_count(20, 10);
}
#endif 
// **************************************************************
// ����:SetThresholdAngle
// ����1:x_angle  X����ֵ�Ƕ�
// ����2:y_angle  Y����ֵ�Ƕ�
// ����3:z_angle  Z����ֵ�Ƕ�
// ����:��
// ����:���ô�����ֵ�Ƕ�
// **************************************************************
//���ô�����ֵ�Ƕ�  threshold_angle
/*void SetThresholdAngle(s8 x_angle, s8 y_angle, s8 z_angle) {
	SysXyzData.xyz_angle.xyz_threshold_angle.x_angle = x_angle;
	SysXyzData.xyz_angle.xyz_threshold_angle.y_angle = y_angle;
	SysXyzData.xyz_angle.xyz_threshold_angle.z_angle = z_angle;
}*/
// **************************************************************
// ����:SetAdxl362Interval_timeAndMax_count
// ����1:interval_timecount ��ȡ�ļ��ʱ��
// ����2:max_readcount      ������ȡ�Ĵ���
// ����:��
// ����:����ADXL362�Ķ�ȡ���ʱ���������ȡ����
// **************************************************************
//����ADXL362�Ķ�ȡ���ʱ���������ȡ����
void SetAdxl362Interval_timeAndMax_count(u8 interval_timecount, u8 max_readcount) {
	Adxl362Para.Inteval_timecount = interval_timecount;
	Adxl362Para.Max_readcount = max_readcount;
}



//���عؼ�����(��������ʱ��)
// Sys_keydata ���뵽 Analyse_data_para (�Ƕ� ������)
// Sys_keydata ���뵽 PacketData (ID ����汾  Ӳ���汾  ���� ���� ��ط����ܴ��� ��Ӫ�� ģ������)
//(3����������ַ)
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
void Set_Key_Para(void){
	//����ID
//	SetTiggerAngleValue(Sys_keydata.trigger_angle);
//	SetSensitiveValue(Sys_keydata.trigger_sensitivity);
	/////////////////////////!!!!!//////////////////////
	//Analyse_data_para.valid_angle_d_value = Sys_keydata.trigger_angle;//���ô����Ƕ� 20170217 BUG
	/////////////////////////!!!!!//////////////////////
	//Analyse_data_para.set_valid_angle_unbroken_num = Sys_keydata.trigger_sensitivity;//���������� ������ֵԽСԽ����

  Analyse_data_para_XiaoFangZ.set_angle = Sys_keydata.lean_trigger_angle;//������б�����Ƕ� //by hx 2017-06-15
  Analyse_data_para_XiaoFangZ.set_valid_angle_unbroken_num = Sys_keydata.lean_trigger_sens; // ������б���������� //by hx 2017-06-15
  
  Analyse_data_para_XiaoFangX.set_angle = Sys_keydata.rotation_trigger_angle;//������ת�����Ƕ� //by hx 2017-06-15
  Analyse_data_para_XiaoFangX.set_valid_angle_unbroken_num = Sys_keydata.rotation_trigger_sens;// ������ת���������� //by hx 2017-06-15
}
#endif 
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
void Set_Key_Para(void){
	//����ID
//	SetTiggerAngleValue(Sys_keydata.trigger_angle);
//	SetSensitiveValue(Sys_keydata.trigger_sensitivity);
	/////////////////////////!!!!!//////////////////////
	//Analyse_data_para.valid_angle_d_value = Sys_keydata.trigger_angle;//���ô����Ƕ� 20170217 BUG
	/////////////////////////!!!!!//////////////////////
	//Analyse_data_para.set_valid_angle_unbroken_num = Sys_keydata.trigger_sensitivity;//���������� ������ֵԽСԽ����

//  Analyse_data_para_XiaoFangZ.set_angle = Sys_keydata.lean_trigger_angle;//������б�����Ƕ� //by hx 2017-06-15
//  Analyse_data_para_XiaoFangZ.set_valid_angle_unbroken_num = Sys_keydata.lean_trigger_sens; // ������б���������� //by hx 2017-06-15
//  
//  Analyse_data_para_XiaoFangX.set_angle = Sys_keydata.rotation_trigger_angle;//������ת�����Ƕ� //by hx 2017-06-15
//  Analyse_data_para_XiaoFangX.set_valid_angle_unbroken_num = Sys_keydata.rotation_trigger_sens;// ������ת���������� //by hx 2017-06-15
}
#endif



//���������������
void Pack_IotNum(char *num_string) {
//	PacketData.ZipsendData.TcpData_S.Data_Info.Num_Cmd = Hex2StrFun_13(num_string);
	//PacketData.ZipsendData.TcpData_S.Data_Info.SigStren_Cmd = Hex2StrFun_1(&sig_stren);
}


//�����ź�ǿ�� ����ź�ǿ��
void Pack_SigStren(u8 sig_stren) {
//	PacketData.ZipsendData.TcpData_S.Data_Info.SigStren_Cmd = Hex2StrFun_1(&sig_stren);
}

//void PacketSigStrenMsg(){
//	PacketData.ZipsendData.TcpData_S.Data_Info.SigStren_Cmd = Hex2StrFun_1(&tempdata);//��װ�¶���Ϣ
//}
/****************************************************************************************************
**��װҺ�汨������ʱռ�õ��±���
//20170630 ����Ϊ 0 ���� 1ˮλ���� 2��ˮ����
******************************************************************************************************/
void Pack_Low_Temp_Alarm(u8 tempflag) //by hx 2017-06-07
{
//	if (!tempflag) {
//		//��װ��ǰ��ֹ״̬�����Ƕ�
//		Pack_LeanAngle(Analyse_data_para.zero_angle);
//	}
//	PacketData.ZipsendData.TcpData_S.Data_Info.Low_Temp_Alarm_Cmd = Hex2StrFun_1(&tempflag);
//  Sys_keydata.Low_Temp_Alarm = tempflag;
}
//���ô�����־ ���������־
//20170425 �޸ı�־λ�趨 ԭ��־λ���� 0 ���� 1����
//����Ϊ 0���� 1���� 2�����ϱ�
void Pack_AlarmFlag(u8 tempflag) {
//        if (tempflag!=1) {//�����������  �ϴ���ֹ�Ƕ�
//		//��װ��ǰ��ֹ״̬�����Ƕ�
//		//Pack_LeanAngle(Analyse_data_para.zero_angle);
//	}
//        Sys_keydata.Trigger_Alarm = tempflag;
//	PacketData.ZipsendData.TcpData_S.Data_Info.Trigger_Alarm_Cmd = Hex2StrFun_1(&tempflag);//���������־
}

//����¶���Ϣ
//void Pack_TempValue(void) {
//
//	u8 tempdata = GetTemp();
//	//u8 tempdata = 88;
//	PacketData.ZipsendData.TcpData_S.Data_Info.Temp_Cmd = Hex2StrFun_1(&tempdata);//��װ�¶���Ϣ
//	////////ȡ�¶���Ϣ/////////
//
//	//debug_send("tempdata:");
//	//printNum2Str(tempdata);
//	///////////////////////////
//}

//void Pack_ADXL362_XYZ{
//  ACCEL_AD0 TempTran;
//  TempTran = ADXL362_Get_Value();
//  Sys_keydata.X_Value = TempTran.xadc;
//  Sys_keydata.Y_Value = TempTran.yadc;
//  Sys_keydata.Z_Value = TempTran.zadc;
//}
//�����б�Ƕ�
void Pack_LeanAngle(s8 lean_angle) {
	/*u8 temp_angle_u8;
        
	if (lean_angle <= 0) {
		temp_angle_u8 = (u8)(0 - lean_angle);
	} else {
		temp_angle_u8 = (u8)(lean_angle);
	}
	PacketData.ZipsendData.TcpData_S.Data_Info.Angle_Cmd = Hex2StrFun_1(&temp_angle_u8);//��װ��б�Ƕ�
        */
//        s8 temp_angle_s8;
//        temp_angle_s8 = 0 - lean_angle;
//        Sys_keydata.Angle=temp_angle_s8;
//        PacketData.ZipsendData.TcpData_S.Data_Info.Angle_Cmd = Hex2StrFun_1(&temp_angle_s8);//��װ��б�Ƕ�
}

////��������Ƕ�
//void Pack_TriggerAngle(void) {
////	PacketData.ZipsendData.TcpData_S.Data_Info.Trigger_Angle_Cmd = Hex2StrFun_1(&Sys_keydata.trigger_angle);//��װ�����Ƕ�
//}
////���������
//void Pack_TriggerSens(void) {
////	PacketData.ZipsendData.TcpData_S.Data_Info.Sens_Cmd = Hex2StrFun_1(&Sys_keydata.trigger_sensitivity);//��װ������
//}


////���ID��Ϣ
//void Pack_IdInfo(void) {
//	//PacketData.ZipsendData.TcpData_S.Data_Info.Id_Cmd = Hex2StrFun_16(DEVICE_ID);//��װ�豸ID
////	PacketData.ZipsendData.TcpData_S.Data_Info.Id_Cmd = Hex2StrFun_16(Sys_keydata.device_id);//��װ�豸ID
//}

////����̼�����汾  ��������汾��Ӳ���汾
//void Pack_Firm_Version(void) {
//	HEX2STR_1 firm_buf = {0, 0};
//	firm_buf.str1[0] = Sys_keydata.harware_version;
//	firm_buf.str1[1] = Sys_keydata.soft_version;
////	PacketData.ZipsendData.TcpData_S.Data_Info.Firm_Cmd = Hex2StrFun_2((u16 *)(&firm_buf));//��װ�̼��汾
//}

//�����Ӫ��
void Pack_Carrier_Type(void) {
//	PacketData.ZipsendData.TcpData_S.Data_Info.Carrier_Cmd = Hex2StrFun_1(&Sys_keydata.carrier_type);//��װ��Ӫ��
}
//���ģ������
void Pack_module_Type(void) {
//	PacketData.ZipsendData.TcpData_S.Data_Info.Mod_Type_Cmd = Hex2StrFun_1(&Sys_keydata.module_type);//��װģ������
}


//u16 heart_timecount = 0;
//_Bool heart_send_flag = 0;

// **************************************************************
// ����:SysParaInit
// ����1:��
// ����:��
// ����:ϵͳ״̬�л�������ʼ��
// **************************************************************
//void SysParaInit(void) {
//	Adxl362Para.Read_flag = 0;
//	EXTI_ClearITPendingBit(EXTI_IT_PortB);
//	EXTI_ClearITPendingBit(EXTI_IT_Pin3);
//	InitAdxl362Para();
//	Set_Key_Para();//���ýǶȺ������� 20170217�޸�  ֮ǰû������(BUG)
//}

/*
void printf_PackageMsg(void) {
	char *array_data = (char *)(&PacketData.ZipsendData.TcpData_S.Soi_Cmd);
	for (u8 i = 0; i < 204; ++i)
	{
		alarmer_uart3_send_byte(array_data[i]);
	}
}*/




//����У����
void CalcChecksum(HEX2STR_2 *checksum_out, char *array_data, u16 len) {
	u16 sumnum = 0;
	HEX2STR_2 checksum_out1;
	for (u16 i = 0; i < len; ++i)
	{
		sumnum += array_data[i];//ȥ��SOI EOI CHESUM
	}
	sumnum = (~(sumnum % 65535)) + 1;

	//sprintf(str, " sumnum:%d", sumnum);
	//debug_send(str);

	checksum_out1 = Hex2StrFun_2(&sumnum);

	//PrintString("CHESUM :");
	//PrintString_(checksum_out1.str2 + 0);

	ArrayCopy(checksum_out->str2, checksum_out1.str2, 4);

	ack_protocol_config_array[33] = 0x0D;
}





////�����װ
//void PackageMsg(void) {
//
//	//Pack_TempValue();//��װ�¶���Ϣ
//	Pack_IdInfo();//��װID��Ϣ
//	Pack_SendTotalcount();//��װ����ܵķ��ʹ���
//	Pack_TriggerAngle();//��װ�����Ƕ�
//	Pack_TriggerSens();//��װ����������
//	//��װ�����Ƕ� 
//	//��װ���� ����ŵ������Ǳ�
//
//	Pack_Second_Query_Intervalcount();//��װ����ȷ�ϼ��ʱ��
//	Pack_Cloud_Server_Add();//��װ��ǰ�ɼ���ַ
//	
//	Pack_Firm_Version();//��װ�̼��汾
//	Pack_Heartdata_Send_Intervalcount();//��װ����ʱ��
////	Pack_Alarm_Intervalcount();//�������ʱ�䣨���������
//	Pack_Carrier_Type();//�����Ӫ������
//	Pack_module_Type();//���ģ������
//
////        CalcChecksum((HEX2STR_2 *)(&PacketData.ZipsendData.TcpData_S.Chksum_Cmd), (char *)(&PacketData.ZipsendData.TcpData_S.Soi_Cmd + 1), 244);
//}


//Ŀǰ��Ҫͳ�Ƶ����ݰ�
//FifoData.BufArray ������X Y Z ������ת��Z�Ƕ�
//����ǰ�Ƕ��ж� FifoData.z_angle[i-1]
//������Ƕ��ж� SysBuf.z_angle[SysBuf.len]


//GetNewInactAngle ��ȡ��̬�µ��½Ƕ�


//���½��뾲̬״̬ǰ �����ɼ��Ƕ�
//��ʱʱ�� Ĭ��10s
_Bool GetAngleRealtimeBeforeInact(void) {

	return 1;
}

//����ǰ������ ����
//�Ƕȱ仯���� �仯�Ƕ�=ǰ�νǶ�-��νǶ�
//�м��������
void AnalyseDataBeforeTriggerFun(void) {

}

//����������ݷ���
//�Ƕ��뾲̬�ǶȶԱ� ���
//���½Ƕ�-��̬�Ƕȡݴ����Ƕ�
void AnalyseDataAfterTriggerFun(void) {

}


//��ӡ����ַ��� ���ڵ���ʹ��
void PrintString(char *temp_in_string) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
        Watchdog_Feed();
#endif
	debug_send_no_end(temp_in_string);
        Watchdog_Feed();
}

void PrintString_(char *temp_in_string) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
        Watchdog_Feed();
#endif
	debug_send(temp_in_string);
        Watchdog_Feed();
}

void PrintStringWithlen_(char *temp_in_string, u8 len) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
	while (len--) {
		debug_send(temp_in_string);
	}
}


/***************************************************************************************
**������:       printU32to2Str
**��������:     
��ӡС�ڵ���4�ֽڵ�ʮ���������
**�������:     --
**�������:     --
**��ע:         YY.c,2017-11-09,By Haixing.Huang
****************************************************************************************/
void printU32to2Str(u32 num) 
{
  u8 str_temp[11];
  u32 num_tmp;
  char c_tmp;
  num_tmp = num;
  u8 i=0;
  u8 cnt=0;
  while(num_tmp)
  {
    str_temp[i]=num_tmp%10+'0';
    i++;
    num_tmp = num_tmp/10;
    if(i>11)  //ǿ���˳���
    {
      break;
    }
    if(0 == num_tmp)
    {
      break;
    }
  }
  cnt = i-1;
  for(i=0;i<(cnt/2+1);i++) //����λ��
  {
    c_tmp=str_temp[i];
    str_temp[i]=str_temp[cnt-i];
    str_temp[cnt-i]= c_tmp;
  }
  str_temp[cnt+1] = '\0';  //ĩβ��ӽ�����
  PrintString((char*)str_temp);
}

//��ӡ��ͨ������ ��ʽΪʮ����
void PrintS8int2String(s8 print_num) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
        printNumStr(print_num);
//	sprintf(str, "%d", print_num);
//	debug_send_no_end(str);
}

void PrintS8int2String_(s8 print_num) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
        printNumStr(print_num);
//	sprintf(str, "%d", print_num);
//	debug_send(str);
}

void PrintU8int2String(u8 print_num) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
        printNum2Str(print_num);
//	sprintf(str, "%d", print_num);
//	debug_send_no_end(str);
}

void PrintU8int2String_(u8 print_num) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
        printNum2Str(print_num);
//	sprintf(str, "%d", print_num);
//	debug_send(str);
}

void PrintU16int2String(u16 print_num) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
        printUNum2Str(print_num);
//	sprintf(str, "%u", print_num);
//	debug_send_no_end(str);
}

void PrintU16int2String_(u16 print_num) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
        printUNum2Str(print_num);
//	sprintf(str, "%u", print_num);
//	debug_send(str);
}

void PrintU32int2String(u32 print_num) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
//        debug_send("����������");
        printU32to2Str(print_num);
//	sprintf(str, "%u", print_num);
//	debug_send_no_end(str);
}

void PrintU32int2String_(u32 print_num) 
{
#if defined(DISPLAY_DEBUG_EN)
      debug_send_no_end("DEBUG:");
#endif
//        debug_send("����������");
      printU32to2Str(print_num);
      debug_send("");
//	sprintf(str, "%u", print_num);
//	debug_send(str);
}

//void PrintU16int2HEX_(u16 print_num) {
//        //debug_send("����������");
//        for(u8 i = 0;i<4;i++){
//          Watchdog_Feed();
//          alarmer_uart3_send_byte(NO[(u8)(print_num>>((3-i)<<2))&0x0f]);
//        }
//}
//void PrintU32int2HEX_(u32 print_num) {
//#if defined(DISPLAY_DEBUG_EN)
//	debug_send_no_end("DEBUG:");
//#endif
//        //debug_send("����������");
//        for(u8 i = 0;i<8;i++){
//          Watchdog_Feed();
//          alarmer_uart3_send_byte(NO[(u8)(print_num>>((7-i)<<2))&0x0f]);
//        }
////	sprintf(str, "%u", print_num);
////	debug_send_no_end(str);
//}

//��ӡ�����ַ�
void PrintChinese(char *temp_in_string){
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
//	sprintf(str, "%s\n", temp_in_string);
//	debug_send(str);
        PrintString_(temp_in_string);
}


//�жϽǶ��Ƿ��ڷ�Χ��
_Bool IsZangleWithinRange(s8 input_Zangle) {
	if ((input_Zangle <= 0) && (input_Zangle >= -90)) {
		return 1;
	}
	return 0;
}

//�жϽǶ��Ƿ���ϴ����Ƕ�
_Bool IsZangleValid(s8 input_Zangle) {

	if (Analyse_data_para.zero_angle > (0 - Analyse_data_para.set_angle)) { //���Ƕ�С���趨�ĽǶ�
		//С�ǶȰڷŵ����
		if (input_Zangle >= Analyse_data_para.zero_angle) {
			//�Ƕ�û�г������Ƕȵķ�Χ
			//��Ϊ������
			return 0;
		} else {
			//�Ƕȳ������Ƕȵķ�Χ  ��Ҫ�ٽ����ж�
			if ((Analyse_data_para.zero_angle - input_Zangle) >= Analyse_data_para.set_angle) {
				//��ǰ�Ƕ�-���Ƕȡ��趨�Ƕ�
				//�ж�Ϊ��Ч�Ƕ�
				return 1;
			} else {
				//��ǰ�Ƕ�-���Ƕ�<�趨�Ƕ�
				//�ж�Ϊ��Ч�Ƕ�
				return 0;
			}
		}
	}
	else { //���Ƕȴ����趨�ĽǶ�
		if (input_Zangle >= Analyse_data_para.zero_angle) {
			//�Ƕ�û�г������Ƕȵķ�Χ
			//��Ҫ�ж��Ƿ��λ ������ĽǶȲ�ֵ
			if ((input_Zangle - Analyse_data_para.zero_angle) >= Analyse_data_para.set_angle) {
				//��ǰ�Ƕ�-���Ƕȡ��趨�Ƕ�
				//�ж�Ϊ��Ч�Ƕ�
				return 1;
			} else {
				//��ǰ�Ƕ�-���Ƕ�<�趨�Ƕ�
				//�ж�Ϊ��Ч�Ƕ�
				return 0;
			}
		} else {
			//�Ƕȳ������Ƕȵķ�Χ  ��Ҫ�ٽ����ж�
			if ((Analyse_data_para.zero_angle - input_Zangle) >= Analyse_data_para.set_angle) {
				//��ǰ�Ƕ�-���Ƕȡ��趨�Ƕ�
				//�ж�Ϊ��Ч�Ƕ�
				return 1;
			} else {
				//��ǰ�Ƕ�-���Ƕ�<�趨�Ƕ�
				//�ж�Ϊ��Ч�Ƕ�
				return 0;
			}
		}
	}
	//return 0;
}


_Bool IsDvalueWithinRange(s8 pre_angle, s8 new_angle, s8 d_value) {
	if (pre_angle >= new_angle) { //�Ƕȱ��
		if ((pre_angle - new_angle) <= d_value) {
			return 1;
		}
		else {
			return 0;
		}
	} else { //�Ƕȱ�С
		if ((new_angle - pre_angle) <= d_value) {
			return 1;
		}
		else {
			return 0;
		}
	}
	//return 0;
}

//���ô����Ƕ�
//�����Ƕ� ��ΧΪ3-40�� ��ƫ��0
void SetTiggerAngleValue(u8 tiggerangle_value) {

	Analyse_data_para.set_angle = tiggerangle_value;
}

//���÷�����������
//������Ĭ��Ϊ4 ��ƫ��4ת�����������ĿΪ8
//��ΧΪ1-7 ��ƫ�ƺ�ķ�ΧΪ5-11
void SetSensitiveValue(u8 sen_value) {

	Analyse_data_para.set_valid_angle_unbroken_num = sen_value; //�趨������Ϊ
}

void ClsAnalyseState(void) {
	Analyse_data_para.analyse_state = 0;
	Analyse_data_para.valid_angle_unbroken_num = 0;
	Analyse_data_para.alarm_flag = 0;
}


//�������� Analyse_data_para
_Bool AnalyseData(void) {
	//static s8 temp_z_angle = 0;//��ʱ���Z��Ƕ�
	//static s8 temp_d_value = 0;
	static s8 pre_temp_z_angle = 0;

	Analyse_data_para.present_angle = get_angle_Z(ADXL362_Get_Value());
	//if (IsZangleWithinRange(temp_z_angle))PrintS8int2String(temp_z_angle);//��Χ�ڵ�ֵ��ӡ����
	//temp_d_value = temp_z_angle - Analyse_data_para.zero_angle;

	//PrintS8int2String(Analyse_data_para.present_angle);//��ӡ������Ϣ
	PrintS8int2String(-55);//��ӡα������Ϣ
       /* if(Analyse_data_para.present_angle>0){
          PrintString("+");
        }else if(Analyse_data_para.present_angle<0){
          PrintString("-");
        }else{
          PrintString("0");
        }*/
        
	if (IsZangleWithinRange(Analyse_data_para.present_angle)) {
		//�Ƕȷ��Ϸ�Χ
		if (IsZangleValid(Analyse_data_para.present_angle)) {
			//�Ƕ��Ƿ�Ϸ�
			PrintString("?");
			if (IsDvalueWithinRange(pre_temp_z_angle, Analyse_data_para.present_angle, Analyse_data_para.valid_angle_d_value)) {
				//ǰһ��ֵ��ȥ���µ�ֵ
				//�ڲ�ֵ��Χ��
				PrintString("#");
			}
			//////////////20170117���� ����Ӱ����//////////////////////////////////////
			else {
				//ǰ���ֵ������  ���㲻�����Ŀ
				Analyse_data_para.valid_angle_unbroken_num = 0;
			}
			////////////////////////////////////////////////////
			if (++Analyse_data_para.valid_angle_unbroken_num >= Analyse_data_para.set_valid_angle_unbroken_num) {

				Analyse_data_para.valid_angle_unbroken_num = 0;
				debug_send("$$$");
				if ((Analyse_data_para.zero_angle <= 0)) {
					Analyse_data_para.alarm_flag = 1;//���ø澯��־
					//��װ��ǰ��б�Ƕ�
//                                        Pack_ADXL362_XYZ();//���XYZ����� ����ʱ��2017/10/26
                                        Analyse_data_para.present_angle = get_angle_Z(ADXL362_Get_Value());//
					Pack_LeanAngle(Analyse_data_para.present_angle);
					return 1;
				}
				else {
					PrintString_("zero_angle is >0,No new alarm!");
					Analyse_data_para.alarm_flag = 0;//����澯��־
				}
			}
			//if(pre_temp_z_angle)
		} else {
			//�ǶȲ��Ϸ�  ���㲻�����Ŀ
			Analyse_data_para.valid_angle_unbroken_num = 0;
		}
		pre_temp_z_angle = Analyse_data_para.present_angle;
	}
	else {
		//�ǶȲ����Ϸ�Χ  ���㲻�����Ŀ
		Analyse_data_para.valid_angle_unbroken_num = 0;
	}
	return 0;
}

_Bool SendHeartPacket(u16 heartcount) 
{
#ifdef HEART_REPORT_EN
//	if (!heart_send_flag) { //���û����������־
//		if (++heart_timecount >= (heartcount<<1)) { //����ʱ���ۼ� 8��Сʱ  1Сʱ=125����
//			heart_send_flag = 1;//��������λ
//			heart_timecount = 0;//ʱ������
//			return 1;
//		}
//	}
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
  if (!heart_send_flag) //���û����������־
  { 
    if (++heart_timecount >= heartcount) 
    { //����ʱ���ۼ� 8��Сʱ  1Сʱ=125����
      heart_send_flag = 1;//��������λ
      heart_timecount = 0;//ʱ������
      return 1;
    }
  }
#endif  
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
  if (!heart_send_flag) //���û����������־
  { 
    if (++heart_timecount >= heartcount) 
    { //����ʱ���ۼ� 8��Сʱ  1Сʱ=125����
      heart_send_flag = 1;//��������λ
      heart_timecount = 0;//ʱ������
      return 1;
    }
  }
#endif
#endif
	return 0;
}
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
/***************************************************************************************
**������:       ClearHeartTimeCount
**��������:     ��ȡ����
**�������:     --
**�������:     --
**��ע:         YY.c,2017-12-10,By Haixing.Huang
****************************************************************************************/
void ClearHeartTimeCount()
{
  heart_timecount = 0;//ʱ������
  heart_send_flag = 0; //�����־λ
  debug_send("��������������");
}
/***************************************************************************************
**������:       getheart_send_flag
**��������:     ��ȡ����
**�������:     --
**�������:     --
**��ע:         YY.c,2017-12-10,By Haixing.Huang
****************************************************************************************/
_Bool *getheart_send_flag()
{
  return &heart_send_flag;
}

u16 *getheart_timecount()
{
  return &heart_timecount;
}
#endif 
///***************************************************************************************
//**������:       PressureSysFun
//**��������:     ˮѹҵ�����̳���
//**�������:     --
//**�������:     --
//**��ע:         YY.c,2017-11-23,By Haixing.Huang
//****************************************************************************************/
//void PressureSysFun(void) 
//{
//  switch (sysFunState) 
//  {
//  case 0:
//    uniAlarm.AlarmAllData = 0X00; //������еĸ澯
//    Sys_keydata.Alarm=uniAlarm.AlarmAllData; 
//    POW24_ON();  //����ˮѹ���
//    //    POWSW1_ON();
//    DelayT_ms(500); //��500ms�������ϵ�ʱ��
//    AdcInit(); //��ʼ��ADC
//    Pressure_GatValue(&strPressureControl);  //��ȡ����
//    AdcClear();
//    POW24_OFF();  //�ر�ˮѹ���
//    //    POWSW1_OFF();
//    if(PressureKeyUpdataConfirm())
//    {
//      debug_send("���������ϱ�����!");
//      uniAlarm.struAlarmBit.KeyUpdata = 1; 
//      //      ENTER_SENDALARM_STATUS; //���뷢������; �Զ������
//      //      break;
//    }
//    if(1 == uniAlarm.struAlarmBit.WithoutPressureSensors  ) //��ӵ� δ��⵽�������澯�б�)
//    {
//      debug_send("δ��⵽������ ");
//    }
//    strPressureControl.CurrentValue = Sys_keydata.pressure_value;   //��ȡ���µ�ѹ��ֵ��
//    if(strPressureControl.CurrentValuePre >= strPressureControl.CurrentValue)   //���ѹ����ֵ��
//    {
//      strPressureControl.CurrentValueDifference = strPressureControl.CurrentValuePre- strPressureControl.CurrentValue;
//    }
//    else
//    {
//      strPressureControl.CurrentValueDifference = strPressureControl.CurrentValue- strPressureControl.CurrentValuePre;
//    }  
//    
//    if(AlarmSentIntervalCheck()) //û�и澯�������
//    {
//      if(uniAlarm.struAlarmBit.pressureAlarmUpDownLimit  == 1)
//      {
////        AlarmSentIntervalSet(Sys_keydata.alarm_sent_interval_minute_count);  //��Ӹ澯���
//        debug_send("��ʼѹ�����ޱ�����");
//        RESET_UPDATA_DISABLE; 
//        ENTER_SENDALARM_STATUS; //���뷢������; �Զ������
//        break;
//      }
//      else if(uniAlarm.struAlarmBit.pressureAlarmUpDownLimit  == 2)
//      {
////        AlarmSentIntervalSet(Sys_keydata.alarm_sent_interval_minute_count);  //��Ӹ澯���
//        debug_send("��ʼѹ�����ޱ�����");
//        RESET_UPDATA_DISABLE;
//        ENTER_SENDALARM_STATUS; //���뷢������; �Զ������
//        break;
//      }
//    }
//    else
//    {
//      debug_send("�ڸ澯����ڣ�");
//    }
//    
//    if(strPressureControl.CurrentValueDifference > Sys_keydata.pressure_trigger_value) //�仯ֵ����һ����Χ��
//    {
//      debug_send("�����仯��Χ������");
//      uniAlarm.struAlarmBit.pressureAlarm = 1;
//      RESET_UPDATA_DISABLE;
//      ENTER_SENDALARM_STATUS; //���뷢������; �Զ������
//      break;
//    }
//    
//    if(RESET_UPDATA_GET()) //��λ��������
//    {
//      debug_send("��λ��������");
//      RESET_UPDATA_DISABLE;
//      ENTER_SENDALARM_STATUS; //���뷢������; �Զ������
//      break;
//    }
//    if (heart_send_flag) 
//    {
//      heart_send_flag = 0;      //���������־��
//      sysFunState = 6;//����������
//      //      strPressureControl.startupdata = 1; //�ϴ����ݵ�ʱ�䵽�ˣ���ʼ�������ݡ� 
//      debug_send("��ʼ��������");
//      break;
//    } 
//    else 
//    {
//      
//    }
//    if(1 == uniAlarm.struAlarmBit.KeyUpdata)
//    {
//      //      debug_send("���������ϱ�����!");
//      ENTER_SENDALARM_STATUS; //���뷢������; �Զ������
//      break;
//    }
//    sysFunState = 4;  //�������ߣ�by hx 2017-06-17
//    break;
//  case 1:
//    sysFunState = 4;
//    break;
//  case 2:       //���ͱ���/����
//    Sys_keydata.Alarm=uniAlarm.AlarmAllData; 
//#if APP_CAN_UPDATA
//    iap_data.writeflag =MODE_ING_DOWNLOAD;
//#else
//    debug_send("δʹ��APP��������");
//#endif
//    {
//      sysFunState++;
//    }
//    break;
//  case 3://�ر�����
//    sysFunState++;
//    break;
//  case 4://�����־�����½���͹�������
//    Pressure_Start(&strPressureControl);  //���ѹ����־λ��
//    strPressureControl.CurrentValuePre = strPressureControl.CurrentValue; //���ϴε�ֵ 
//    sysFunState = 0;//���߻��Ѻ󣬴�ͷ��ʼִ��
//    debug_send("����͹���");
//    IntoLowpowerMode();
//    break;
//  case 5:// 
//    break;
//  case 6://
//    sysFunState = 2;
//    break;
//  case 10://�ȴ����ò�������  ��ʱʱ��Ϊ5S
//    break;
//  case 11:
//    break;
//  case 12://��ȡ���ò���  ID  У���¶�
//    //����ID��
//    break;
//  case 13://ˮλ���  �ַ���  ���л���
//    break;
//  case 14:
//    break;
//  case 15:
//    break;
//  case 16:
//    break;
//  default:
//    sysFunState = 0;
//    break;
//  }
//}

///***************************************************************************************
//**������:       CoverSysFun
//**��������:     �Ƹ�ҵ�����̳���
//**�������:     --
//**�������:     --
//**��ע:         YY.c,2017-11-23,By Haixing.Huang
//****************************************************************************************/
//void CoverSysFun(void) 
//{
//#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
// 
//   
//  switch (CoverSysFunState)
//  {
//  case CoverSysFunState_Detection://������ADXLģ������ݶ�ȡ
//    
//    WaterDetection_Service();
//    if (Adxl362Para.Read_flag == 1) //�������ֵ������־��λ
//    {
//      sysFunTimecount = 0;
//      CoverSysFunState = CoverSysFunState_AnalyzeACC; //������ٶȷ�����
//      PrintString_("��ʼ��ȡ��������!");
//      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//�������ģʽ������״̬
//      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//�������ģʽ������״̬
//      PrintString("Z�����Ƕ�:");
//      PrintS8int2String_(Analyse_data_para_XiaoFangZ.zero_angle);
//      PrintString("X�����Ƕ�:");
//      PrintS8int2String_(Analyse_data_para_XiaoFangX.zero_angle);
//      NewInactAngle_SetSamplingLockFlag();  //���붯���жϵ�ʱ�򣬲����о�ֹ�жϡ�
//    }
//    else        //���û����ֵ����
//    {
//      if (heart_send_flag) 
//      {
//        heart_send_flag = 0;   //���������
//        CoverSysFunState = CoverSysFunState_SendData;
//        debug_send("��ʼ����������");
//        break;
//      }
//      CoverSysFunState = CoverSysFunState_WaitInterLPM;
//    }
//    break;
//  case CoverSysFunState_SendData:       //���ͱ���/����
//    if(1 == struXiaoFangAlarmTimes.StratAlarm) //����Ҫģ�����һ�γ�ˮ�����澯����ֹ���ֶ����澯�������
//    {
//      struXiaoFangAlarmTimes.StratAlarm = 0;
//      struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
//      XiaoFang_ClearRtcWakeFlag();
//      debug_send("������ˮ������");
//    }
//#if APP_CAN_UPDATA
//    iap_data.writeflag =MODE_ING_DOWNLOAD;
//#else
//    debug_send("δʹ��APP��������");
//#endif   
//    CoverSysFunState = CoverSysFunState_WaitInterLPM;
//    break;
//  case CoverSysFunState_WaitInterLPM:
//    /**********************************************************************************************************************
//    ��ˮ����ˮʵʱ�����������澯�����ǡ���б������ϳ�ˮ�澯��
//    **********************************************************************************************************************/
//    if(1 == struXiaoFangAlarmTimes.StratAlarm)    
//    {
//      struXiaoFangAlarmTimes.StratAlarm = 0;
//      if((TRIGGER_FALLING == struXiaoFangAlarmTimes.TriggerStatus)||(XiaoFang_GetRtcWakeFlag())) 
//      {
//        struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
//        XiaoFang_ClearRtcWakeFlag();
//        debug_send("������ˮ������");
//        ENTER_SENDALARM_STATUS; //���뷢������
//      }
//    }
//    else
//    {
//      NewInactAngle_ClearSamplingLockFlag(); //�����ֹ���������
//      if(NewInactAngle_GetNewAngleEndFlag()) //�ȴ��ǶȾ�ֹ��ɡ�
//      {
//        Analyse_data_para.alarm_flag = 0;
//        Adxl362Para.Read_flag = 0;
//        uniAlarm.AlarmAllData = 0X00;     //������и澯
//        struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
////        XiaoFang_ClearRtcWakeFlag();
//        debug_send(">>>ϵͳ��������");
//        CoverSysFunState = CoverSysFunState_InterLPM;
//      }
//    }
//    break;
//  case CoverSysFunState_AnalyzeACC://
//    if (sysFunTimecount < 5000) //��ʱ�˳�
//    {
//      Analyse_data_para_XiaoFangZ.present_angle= get_angle_Z(ADXL362_Get_Value());//��ȡZ������ݡ�
//      XiaoFang_Analyse(&Analyse_data_para_XiaoFangZ);  //����Z������
//      Analyse_data_para_XiaoFangX.present_angle =get_angle_X(ADXL362_Get_Value());//��ȡX������ݡ�
//      XiaoFang_Analyse(&Analyse_data_para_XiaoFangX);  //����X������
//      //
//      if((1 == Analyse_data_para_XiaoFangZ.alarm_flag) || (1 == Analyse_data_para_XiaoFangX.alarm_flag))
//      {
////        Pack_Rotation_Angle(Analyse_data_para_XiaoFangX.present_angle); //�����ת�Ƕ�
////        Pack_LeanAngle(Analyse_data_para_XiaoFangZ.present_angle);//�����б�Ƕ�
//        if((1 == Analyse_data_para_XiaoFangZ.alarm_flag)) // ������б����
//        {
//          PrintString_("��ʼ��б�澯!");
//          uniAlarm.struAlarmBit.laenAlarm = 1;
//          //               Pack_Lean_Alarm(1);//�����б�澯��
//          ENTER_SENDALARM_STATUS;
//          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//�������ģʽ������״̬
//        }
//        
//        if((1 == Analyse_data_para_XiaoFangX.alarm_flag)) // ������ת����
//        {
//          PrintString_("��ʼ���Ǹ澯!");
//          //                Pack_Rotation_Alarm(1); //�����ת����
//          uniAlarm.struAlarmBit.openAlarm = 1;
//          ENTER_SENDALARM_STATUS;
//          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//�������ģʽ������״̬
//        }
//      }
//      else //�������δ�澯
//      {
//        if(NewInactAngle_GetStaticFlag()) //
//        {
////          sysFunState = 4;
//          CoverSysFunState = CoverSysFunState_WaitInterLPM;
//          //                ClsAnalyseState();//�������ģʽ������״̬
//          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//�������ģʽ������״̬
//          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//�������ģʽ������״̬
//          //          PrintString_("none valid angle!back to lowpwer mode!");
//          //          GetNewInactAngle();
//        }
//      }
//    }
//    else
//    {
//      //PrintString_("@");
//      //            ClsAnalyseState();//�������ģʽ������״̬
//      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//�������ģʽ������״̬
//      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//�������ģʽ������״̬
//      PrintString_("Exit!");
////      sysFunState = 4;//���½���͹�������
//      CoverSysFunState = CoverSysFunState_WaitInterLPM;
//      //Pack_AlarmFlag(1);//���ñ�����־
//      //PackageMsg();//�ȷ�װ���ٷ���
//      //sysFunState = 2;//������Ϣ
//    }
//    break;
//  case CoverSysFunState_InterLPM:
//    CoverSysFunState = CoverSysFunState_Detection;
//    IntoLowpowerMode();
//    break;
//  default:
//    CoverSysFunState = CoverSysFunState_Detection;
//    break;
//  }
//#endif
//}