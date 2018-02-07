#include "config.h"

u8 SA_Pro=0;//���ܽ���
u8 Down_Pro=0;//ͨѶ����
u32 channel_en_bit = 0;//ƽ̨�·�����ʹ��λ

/*****************************************************************************************************************
ʵ�ֹ���:��SIM800C��������,����SIM800C��������:��Ƭ�����Ǹ�SIM800C�Ĺܽ�(/IGT)��ʱ������Ϊ1s�ĵ͵�ƽ�ź�
*****************************************************************************************************************/
#if CONF_HARDWARE_VERSION == HARDWARE_VERSION_V205
  #define POWERON_MC              GPIO_ResetBits(GPIOG,GPIO_Pin_3);GPIO_SetBits(GPIOG,GPIO_Pin_2)
  #define POWEROFF_MC             GPIO_SetBits(GPIOG,GPIO_Pin_3);GPIO_ResetBits(GPIOG,GPIO_Pin_2)

  #define RESET_MC_POWER_ONOFF    GPIO_SetBits(GPIOD,GPIO_Pin_6)
  #define SET_MC_POWER_ONOFF      GPIO_ResetBits(GPIOD,GPIO_Pin_6)

  #define RESET_MC_RESET          GPIO_SetBits(GPIOD,GPIO_Pin_7)
  #define SET_MC_RESET            GPIO_ResetBits (GPIOD,GPIO_Pin_7)
#endif
#if CONF_HARDWARE_VERSION == HARDWARE_VERSION_V105
  #define POWERON_MC              GPIO_ResetBits(GPIOD,GPIO_Pin_4);GPIO_SetBits(GPIOB,GPIO_Pin_1)
  #define POWEROFF_MC             GPIO_SetBits(GPIOD,GPIO_Pin_4);GPIO_ResetBits(GPIOB,GPIO_Pin_1)

  #define SET_MC_POWER_ONOFF      GPIO_SetBits(GPIOD,GPIO_Pin_6)
  #define RESET_MC_POWER_ONOFF    GPIO_ResetBits(GPIOD,GPIO_Pin_6)

  #define SET_MC_RESET            GPIO_SetBits(GPIOD,GPIO_Pin_7)
  #define RESET_MC_RESET          GPIO_ResetBits (GPIOD,GPIO_Pin_7)
#endif

//������������������������������������������������������������������������������
// ��������  : IOTMode_RESET
// ��������  : ����ģ������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void IOTMode_RESET(void)
{
  POWEROFF_MC;
  Delay_16M_MS(500);
  RESET_MC_RESET;
  RESET_MC_POWER_ONOFF;
  Delay_16M_MS(50);
  RESET_MC_RESET;
  POWERON_MC;
  SET_MC_POWER_ONOFF;
  Delay_16M_MS(40);
  
  SET_MC_RESET;
  Delay_16M_MS(20);
  RESET_MC_POWER_ONOFF;
  
  qwl_net_sendstring("AT+NRB");//��λ
  qwl_net_sendstring("AT+CGMM");//G����λ
  
  Delay_16M_MS(1000);
  SET_MC_POWER_ONOFF;
  
  Delay_16M_MS(4000);
}
//������������������������������������������������������������������������������
// ��������  : Get_SigStren
// ��������  : ��ȡ�ź�ǿ��
// �������  : �����ź�ǿ�ȵĻ��������±�
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void Get_SigStren(u8 TempVal)
{
  u8 Temp1,Temp2;
  
  Temp1=SystemBuf[TempVal];
  Temp2=SystemBuf[TempVal+1];
  
  PrintString("�ź�ǿ��:");
  
  if(Temp2 != ','){//�������λ��
    USART3_sendchar(Temp1);//��ӡ�ź�ǿ��
    USART3_sendchar(Temp2);
    Sys_keydata.SigStren = asc2hex(Temp1)*10+asc2hex(Temp2);
  }else{
    USART3_sendchar(Temp1);//��ӡ�ź�ǿ��
    Sys_keydata.SigStren = asc2hex(Temp1);
  }
}

//������������������������������������������������������������������������������
// ��������  : Get_SimID
// ��������  : ��ȡ13λ����
// �������  : ��ʱ����ID����ʼ��ַ
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void Get_SimID(u8 *TempPointer)//
{
  for(u8 i=0;i<13;i++)
  {
    Sys_keydata.Num_Cmd[i] = *TempPointer++;//��ȡ���������
  }
}
//������������������������������������������������������������������������������
// ��������  : Uploading_Send_12
// ��������  : ���Ͱ汾
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void Uploading_Send_12(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{
  u8 Temp_Tran[22];
  u8 Empty_SendAdd[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  PostSendHeadCmd(uf,01,0x00);//01 ��ַ 00 cmd
  
  Temp_Tran[0] = '0';
  Temp_Tran[1] = '1';
  PostSend_HexStr(USART_FUNCTION_WCDMA,Temp_Tran,2);
  //������ַ	
  PostSend_HexStr(USART_FUNCTION_WCDMA,SOFTWARE_VER,6);
  //����汾�ţ���λ����������
  Temp_Tran[0] = 'V';
  Temp_Tran[1] = '0';
  Temp_Tran[2] = Sys_keydata.harware_version[0] + '0';
  Temp_Tran[3] = '.';
  Temp_Tran[4] = Sys_keydata.harware_version[1] + '0';
  Temp_Tran[5] = '0';
  PostSend_HexStr(uf,Temp_Tran,6);
  //Ӳ���汾��
  
  PostSend_HexStr(uf,COMPANY_NAME,20);
  //��˾����
  PostSend_HexStr(uf,Sys_keydata.device_id,16);
  PostSend_HexStr(uf,Empty_SendAdd,4);
  //�豸��� Id_Cmd
  PostSend_HexStr(uf,Empty_SendAdd,24);
  //���кţ����ã�
  PostSend_HexStr(uf,__DATE__,11);
  PostSend_HexStr(uf,__TIME__,9);
  //����ʱ��
  PostSendDataLen(uf);
  PostSendPostChksum(uf);
  PostSendTail(uf);
}
//������������������������������������������������������������������������������
// ��������  : pack_heat
// ��������  : �������
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void pack_heat(void)//��������������ע�뻺��
{ 
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
  u8 Empty_SendAdd[10] = {0,0,0,0,0,0,0,0,0,0};
  u8 TempTran[6] = SOFTWARE_VER;
  PostSendHeadCmd(USART_FUNCTION_WCDMA,01,0x1A);
  //  	0x7E,//SOI
  //	'1', '1', //VER
  //	'0', '1', //ADR
  //	'D', '0', //CID1
  //	'1', 'A', //CID2
  //	//'B', '0', 'B', 'A', //LEN
  //        'A', '0', 'E', '8', //LEN
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.device_id,16);//�豸��� Id_Cmd
  //  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,4);//�ͻ��� Cstm_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.Num_Cmd,20);//���������� Num_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.harware_version,2);//Ӳ���汾 
  TempTran[0] = (asc2hex(TempTran[1])*10)+(asc2hex(TempTran[2]));
  TempTran[1] = (asc2hex(TempTran[4])*10)+(asc2hex(TempTran[5]));
  PostSend_HexStr(USART_FUNCTION_WCDMA,TempTran,2);//����汾 
  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,1);//�豸״̬ State_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,&Sys_keydata.SigStren,1);//�ź�ǿ�� SigStren_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,1);//ʣ���ѹ ResV_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.send_totalcount),2);//u16 send_totalcount;���ʹ���
  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,1);//�����¶� Temp_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_value),2);//u16 pressure_value;//ѹ��
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.rotation_angle),2);//s16 rotation_angle;//��ת�Ƕ�
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.lean_angle),1);//s8 lean_angle;//��б�Ƕ�
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.rotation_trigger_angle),1);//u8 rotation_trigger_angle;//��ת�����Ƕ�
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.rotation_trigger_sens),1);//u8 rotation_trigger_sens;//��ת����������
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.lean_trigger_angle),1);//u8 lean_trigger_angle;//��б�����Ƕ�
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.lean_trigger_sens),1);//u8 lean_trigger_sens;//��б����������
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_trigger_value),2);//u16 pressure_trigger_value;//ѹ��������ֵ
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_interval),2);//u16 pressure_interval;//ѹ�������
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.Alarm),2); //u16 Alarm;//�澯
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.heartdata_sent_interval_hour_count),2);//u16 heartdata_sent_interval_hour_count;//�������ʱ��
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.alarm_sent_interval_minute_count),2);//u16 alarm_sent_interval_minute_count;//�������ʱ��
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.trigger_interval_minute_count),2);//u16 trigger_interval_minute_count;//�������ʱ��
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.default_cloud_server_add,21);//u8 default_cloud_server_add[22];//�ƶ˷�������ַ255.255.255.255,65535
  PostSend_HexStr(USART_FUNCTION_WCDMA,&Sys_keydata.carrier_type,1);//u8 carrier_type;//��Ӫ��   �����޸� ��Ӳ���汾
  PostSend_HexStr(USART_FUNCTION_WCDMA,&Sys_keydata.module_type,1);//u8 module_type;//ģ����ʽ �����޸� ��Ӳ���汾
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���  
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.TimesTamp),4);//s32 TimesTamp;  //ʱ��� 
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_upLimit),2);//u16 pressure_upLimit; //ѹ������
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_downLimit),2);//u16 pressure_downLimit; //ѹ������
#endif
#endif
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
    u8 Empty_SendAdd[10] = {0,0,0,0,0,0,0,0,0,0};
  u8 TempTran[6] = SOFTWARE_VER;
  PostSendHeadCmd(USART_FUNCTION_WCDMA,01,0x1A);
  //  	0x7E,//SOI
  //	'1', '1', //VER
  //	'0', '1', //ADR
  //	'D', '0', //CID1
  //	'1', 'A', //CID2
  //	//'B', '0', 'B', 'A', //LEN
  //        'A', '0', 'E', '8', //LEN
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.device_id,16);//�豸��� Id_Cmd
  //  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,4);//�ͻ��� Cstm_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.Num_Cmd,20);//���������� Num_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.harware_version,2);//Ӳ���汾 
  TempTran[0] = (asc2hex(TempTran[1])*10)+(asc2hex(TempTran[2]));
  TempTran[1] = (asc2hex(TempTran[4])*10)+(asc2hex(TempTran[5]));
  PostSend_HexStr(USART_FUNCTION_WCDMA,TempTran,2);//����汾 
  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,1);//�豸״̬ State_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,&Sys_keydata.SigStren,1);//�ź�ǿ�� SigStren_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,1);//ʣ���ѹ ResV_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.send_totalcount),2);//u16 send_totalcount;���ʹ���
  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,1);//�����¶� Temp_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_value),2);//u16 pressure_value;//ѹ��
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)getSamplingValue(),2); //u16 SamplingValue; //����ԭʼֵ
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_trigger_value),2);//u16 pressure_trigger_value;//ѹ��������ֵ
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.Alarm),2); //u16 Alarm;//�澯
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.heartdata_sent_interval_hour_count),2);//u16 heartdata_sent_interval_hour_count;//�������ʱ��
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)getAlarmEnableBit(),2); // u16 AlarmEnableBit; //�澯ʹ��0ʧ�ܣ�1ʹ��
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)getPressureUpLimitAlarmInterval(),2);//  u16 PressureUpLimitAlarmInterval; //���޸澯���ʱ��
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)getPressureDownLimitAlarmInterval(),2);//  u16 PressureDownLimitAlarmInterval; //���޸澯���ʱ��
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)getNoSensorAlarmInterval(),2);//  u16 NoSensorAlarmInterval; //δ��⵽�������澯���
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)getResolutionAlarmInterval(),2);//  u16 ResolutionAlarmInterval; //�ֱ����ϱ��澯���
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)getTouchOffAlarmInterval(),2);//  u16 TouchOffAlarmInterval;  //�˹������ϱ����
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.trigger_interval_minute_count),2);//u16 trigger_interval_minute_count;//�������ʱ��
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.default_cloud_server_add,21);//u8 default_cloud_server_add[22];//�ƶ˷�������ַ255.255.255.255,65535
  PostSend_HexStr(USART_FUNCTION_WCDMA,&Sys_keydata.carrier_type,1);//u8 carrier_type;//��Ӫ��   �����޸� ��Ӳ���汾
  PostSend_HexStr(USART_FUNCTION_WCDMA,&Sys_keydata.module_type,1);//u8 module_type;//ģ����ʽ �����޸� ��Ӳ���汾 
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.TimesTamp),4);//s32 TimesTamp;  //ʱ��� 
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_upLimit),2);//u16 pressure_upLimit; //ѹ������
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_downLimit),2);//u16 pressure_downLimit; //ѹ������
#endif  
}
extern void PostSendFixupByte(USART_FUNCTION uf,uchar *TxBuf,uchar FixupNo,uchar valus);
//������������������������������������������������������������������������������
// ��������  : PostSendUpDataLen
// ��������  : �ʵ粿Э�鷢����ʷ���ݳ���
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void PostSendUpDataLen(USART_FUNCTION uf,u8 len)
{
  uchar *TxBuf;
  uint16 *Len;
//  Taglchksum  tmplen = {0};
//  unsigned int word;
//  unsigned char tempH;
//  unsigned char tempL;
  
  if(API_GetProtocolBufferOrLen(uf,&TxBuf,&Len)>-1)
  {
//    tmplen.word = *Len - 13;
//    tmplen.div.lChksum = tmplen.div.hLenth+tmplen.div.mLenth+tmplen.div.lLenth;
//    tmplen.div.lChksum = ~tmplen.div.lChksum+1;
//    word = tmplen.word;
//    tempH = word>>8;
//    tempL =	(uchar)word;
//    tempH = len;
    PostSendFixupByte(uf,TxBuf,45,len);	   
//    PostSendFixupByte(uf,TxBuf,11,tempL);
  }
}

//������������������������������������������������������������������������������
// ��������  : pack_heat
// ��������  : ����ʷ��������ע�뻺��
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
s8 pack_updata(void)
{
  int i;
  s8 s8_tmp =0;
  u8 infoLen =0x00;  //���ݸ���
  //  union _uni_u16_tmp
  //  {
  //    u16 tmp;
  //    u8 tmp_tab[2];
  //  };
  //  union _uni_u16_tmp uni_u16_tmp;
  
  PostSendHeadCmd(USART_FUNCTION_WCDMA,01,0x1B);
  //  	0x7E,//SOI
  //	'1', '1', //VER
  //	'0', '1', //ADR
  //	'D', '0', //CID1
  //	'1', 'B', //CID2
  //	//'B', '0', 'B', 'A', //LEN
  //        'A', '0', 'E', '8', //LEN 
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.device_id,16);//�豸��� Id_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,&infoLen,1);//��ֵ���� ������д
  if(1 == IsFLASH_QueueEmpty()) //�������Ƿ�Ϊ��
  {
    debug_send("QueueEmpty1!");
    return -1;  //ǿ���˳�����
  }
  else  //��ȡ���ݵ���������ÿ������ȡ38�顣��Ҫд��FLASH���档
  {
    s8_tmp = 1;
    for(i=0;i<(CONST_PRESSUREDATA_MAX_LEN);i++) //һ����໺��
    {
      Watchdog_Feed();  //���Է��֣������������������ڼ���ι�������в��ԡ�
      if(1 == IsFLASH_QueueEmpty()) //�������Ƿ�Ϊ�ա�
      {
        debug_send("QueueEmpty2!");
        s8_tmp = 0; //��������֪ͨϵͳ������һ�η������ݡ�
        break;  //������Ϊ�գ�ǿ���˳�
      }
      else
      {
        DeFLASH_Queue(&struPressureAData.TimesTamp,&struPressureAData.PressureData,&struPressureAData.AlarmList);
        PostSend_HexStr(USART_FUNCTION_WCDMA,(u8 *)(&struPressureAData.TimesTamp),4);//��дʱ��
        PostSend_HexStr(USART_FUNCTION_WCDMA,(u8 *)(&struPressureAData.PressureData),2);//��дѹ������
        PostSend_HexStr(USART_FUNCTION_WCDMA,(u8 *)(&struPressureAData.AlarmList),2);//��д�澯��
      }
    }
    Watchdog_Feed();  //���Է��֣������������������ڼ���ι�������в��ԡ�
    PostSendUpDataLen(USART_FUNCTION_WCDMA,i);//��д ��ֵ����
  }
  return s8_tmp;
}

//������������������������������������������������������������������������������
// ��������  : Get_DowndataFlag
// ��������  : �������
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void Get_DowndataFlag(uchar *DaPtr)
{
  //��������56�ֽڣ�
  //��������~1101D000802630303030303030303030303030303030000000F649(0x0D)
  //ת����hex��ǰ����������󣬲���������DaPtr[24]
  if(DaPtr[25]==1){//�������¿���
    PrintString_("���й̼�����");
    Rec_Timeout.Post_REC_TIMEOUT.Rec_State = RUN;//�������ճ�ʱ
    
  }else if(DaPtr[24]==1){//�������¿���
    PrintString_("�ȴ���������");
    Rec_Timeout.Post_REC_TIMEOUT.Rec_State = RUN;//�������ճ�ʱ
    
  }else{
    PrintString_("����ͨѶ");
    Down_Pro=PRO_END;//����ͨѶ�����ؾ��ǳ���
  }
}
//������������������������������������������������������������������������������
// ��������  : Getparameters_Str
// ��������  : ƽ̨�·����� ����
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
uint16 Getparameters_Str(uchar *DaPtr,uchar *Par_Str,uint16 with)
{
  uint16 i;
  for(i=0; i<with; i++){
    *Par_Str++ = *DaPtr;
    DaPtr++;
  }
  
  if(*DaPtr==0x2C){
    with++;
    PrintString_("��ȷ");
  }
  else
    PrintString_("����");
  
  Watchdog_Feed();
  return with;
}

//������������������������������������������������������������������������������
// ��������  : Getparameters_StrEN
// ��������  : ƽ̨�·����� ���棬��ͨ��ʹ��
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
uint16 Getparameters_StrEN(uchar *DaPtr,uchar *Par_Str,uint16 with,u32 Enable)
{
  uint16 i;
  if(channel_en_bit & Enable){
    
    for(i=0; i<with; i++){
      *Par_Str++ = *DaPtr;
      DaPtr++;
    }
    
    if(*DaPtr==0x2C){
      PrintString_("��ȷ");
    }
    else
      PrintString_("����");
    
  }else{
    PrintString_("ȡ��");
  }
  
  with++;
  Watchdog_Feed();
  return with;
}

//������������������������������������������������������������������������������
// ��������  : Getparameters
// ��������  : ���ò���
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void Getparameters(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{

#define U32BIT1 ((u32)(1))
  u16 len_cur = 7;
  u32 Empty_Val = 0;
  u8  Empty_Str[22];
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
  //DaPtr += 7;//����ͷ
  PrintString("����ͨ������");
  len_cur += Getparameters_Str(&DaPtr[len_cur],(uchar*)&Empty_Val,2);//ͨ������
  PrintString("����ͨ����Ч��־");
  len_cur += Getparameters_Str(&DaPtr[len_cur],(uchar*)&channel_en_bit,4);//ͨ����Ч��־
  PrintString("����ID");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Empty_Str,16,U32BIT1<<0);//ID
  PrintString("��������汾");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,2,U32BIT1<<1);//����汾
  PrintString("����Ӳ���汾");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,2,U32BIT1<<2);//Ӳ���汾
  PrintString("�����豸״̬");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.status,1,U32BIT1<<3);//�豸״̬
  PrintString("���õ�ط�������");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,2,U32BIT1<<4);//��ط�������
  PrintString("������ת�仯�Ƕ�");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.rotation_trigger_angle,1,U32BIT1<<5);//��ת�仯�Ƕ�
  PrintString("������ת������");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.rotation_trigger_sens,1,U32BIT1<<6);//��ת������
  PrintString("������б�仯�Ƕ�");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.lean_trigger_angle,1,U32BIT1<<7);//��б�仯�Ƕ�
  PrintString("������б������");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.lean_trigger_sens,1,U32BIT1<<8);//��б������
  PrintString("����ѹ���仯��ֵ");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.lean_trigger_sens,2,U32BIT1<<9);//ѹ���仯��ֵ
  PrintString("����ѹ�������");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.pressure_trigger_value,2,U32BIT1<<10);//ѹ�������
  PrintString("�����������ʱ��");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.heartdata_sent_interval_hour_count,2,U32BIT1<<11);//�澯���
  PrintString("���ø澯���");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.alarm_sent_interval_minute_count,2,U32BIT1<<12);//�澯���
  PrintString("���ü����ʱ��");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.trigger_interval_minute_count,2,U32BIT1<<13);//�����ʱ��
  PrintString("���ÿ���ɼ���ַ");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Sys_keydata.default_cloud_server_add,21,U32BIT1<<14);//����ɼ���ַ
  PrintString("����������������ַ");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Sys_keydata.update_server_add,21,U32BIT1<<15);//������������ַ
  PrintString("������Ӫ��");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,1,U32BIT1<<16);//��Ӫ��
  PrintString("����ģ������");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,1,U32BIT1<<17);//ģ������
  PrintString("������������");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Empty_Str,20,U32BIT1<<18);//��������
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���  
  PrintString("����ʱ���");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.TimesTamp,4,U32BIT1<<19);//ʱ���
  PrintString("����ѹ������");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.pressure_upLimit,2,U32BIT1<<20);//ѹ������
  PrintString("����ѹ������");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.pressure_downLimit,2,U32BIT1<<21);//ѹ������
#endif
  
#undef U32BIT1
  //  PrintString("���ô���������");
  ////  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.trigger_sensitivity,1,1<<4);//����������
  //  PrintString("�����������");
  ////  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.heartdata_sent_interval_hour_count,2,1<<5);//�������
  //  PrintString("���ñ������");
  ////  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.alarm_sent_interval_second_count,2,1<<6);//�������
  //  PrintString("���ö���ȷ�ϼ��");
  ////  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.second_query_interval_second_count,2,1<<7);//����ȷ�ϼ��
  //  
  //  PrintString("�����¶�����");
  //  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,1,1<<9);//�¶�����
  //  PrintString("����������������ַ");
  //  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Empty_Str,21,1<<10);//������������ַ
  //  PrintString("���ÿ���ɼ���ַ");
  //  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Empty_Str,21,1<<11);//����ɼ���ַ
  //  PrintString("����Ĭ�ϲɼ���ַ");
  ////  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Sys_keydata.default_cloud_server_add,21,1<<12);//Ĭ�ϲɼ���ַ
  //  PrintString("������Ӫ��");
  //  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,1,1<<13);//��Ӫ��
  //  PrintString("����ģ������");
  //  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,1,1<<14);//ģ������
  
  /*~1101D0FB40EE00002C000010B82C000000000000000000000000000
  000002C00002C00002C062C062C00182C00002C012C2C00002C002C0
  000000000000000000000000000000000000000002C0000000000000
  000000000000000000000000000002C33392E3130382E39312E32303
  82C323030303800002C002C002CCE4E(0x0D)�����·���������  */
  Watchdog_Feed();
  if (channel_en_bit & BIT_CONFIG_TRIGGER_INTERVAL)//�����ʱ��	UINT	0-65535��  
  {
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
    RTC_Config();  //ֱ������
#endif    
  }
  
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���  
  if (channel_en_bit & BIT_TIMESTAMP_ID)
  {
    TickGetTime(Sys_keydata.TimesTamp); //����ʱ�������ʱ�䡣
    TimeGetTick(TRUE);
#if SD3078_WIRTE_INIT_FLAG_EN
    RTC_SD3078_WriteInitFlag(); //д���־;
#endif
  }
#endif  

  if(STATUS_CLEAR_FLASH == Sys_keydata.status) //��⵽ϵͳ��Ҫ���FLASH
  {
    debug_send("��ʼ���FLASH��");   
    ClearFLASH_Queue();
    Sys_keydata.status = STATUS_NORMAL; //�ָ�������״̬��
  }
  Write_sys_keydata();
  
  Watchdog_Feed();
  Set_Key_Para();
  
  Watchdog_Feed();
  //  SysParaInit();//ϵͳ������ʼ��
  PrintSysParaList();
  Watchdog_Feed();
#endif
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
   PrintString("����ͨ������");
  len_cur += Getparameters_Str(&DaPtr[len_cur],(uchar*)&Empty_Val,2);//ͨ������
  PrintString("����ͨ����Ч��־");
  len_cur += Getparameters_Str(&DaPtr[len_cur],(uchar*)&channel_en_bit,4);//ͨ����Ч��־
  PrintString("����ID");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Empty_Str,16,U32BIT1<<0);//ID
  PrintString("��������汾");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,2,U32BIT1<<1);//����汾
  PrintString("����Ӳ���汾");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,2,U32BIT1<<2);//Ӳ���汾
  PrintString("�����豸״̬");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.status,1,U32BIT1<<3);//�豸״̬
  PrintString("���õ�ط�������");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,2,U32BIT1<<4);//��ط�������
  PrintString("����ѹ���仯�ֱ���");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.pressure_trigger_value,2,U32BIT1<<5);//ѹ���仯�ֱ���
  PrintString("�����������ʱ��");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.heartdata_sent_interval_hour_count,2,U32BIT1<<6);//�������ʱ��
  PrintString("���ø澯ʹ��");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)getAlarmEnableBit(),2,U32BIT1<<7);//�澯ʹ��
  PrintString("�������޸澯���ʱ��");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)getPressureUpLimitAlarmInterval(),2,U32BIT1<<8);//���޸澯���ʱ��
  PrintString("�������޸澯���ʱ��");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)getPressureDownLimitAlarmInterval(),2,U32BIT1<<9);//���޸澯���ʱ��
  PrintString("����δ��⵽�������澯���");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)getNoSensorAlarmInterval(),2,U32BIT1<<10);//δ��⵽�������澯���
  PrintString("���÷ֱ����ϱ��澯���");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)getResolutionAlarmInterval(),2,U32BIT1<<11);//�ֱ����ϱ��澯���
  PrintString("�����˹������ϱ����");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)getTouchOffAlarmInterval(),2,U32BIT1<<12);//�˹������ϱ����
  PrintString("���ü����ʱ��");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.trigger_interval_minute_count,2,U32BIT1<<13);//�����ʱ��
  PrintString("���ÿ���ɼ���ַ");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Sys_keydata.default_cloud_server_add,21,U32BIT1<<14);//����ɼ���ַ
  PrintString("����������������ַ");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Sys_keydata.update_server_add,21,U32BIT1<<15);//������������ַ
  PrintString("������Ӫ��");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,1,U32BIT1<<16);//��Ӫ��
  PrintString("����ģ������");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,1,U32BIT1<<17);//ģ������
  PrintString("������������");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Empty_Str,20,U32BIT1<<18);//��������
  PrintString("����ʱ���");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.TimesTamp,4,U32BIT1<<19);//ʱ���
  PrintString("����ѹ������");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.pressure_upLimit,2,U32BIT1<<20);//ѹ������
  PrintString("����ѹ������");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.pressure_downLimit,2,U32BIT1<<21);//ѹ������
#undef U32BIT1
  
  Watchdog_Feed();
  if (channel_en_bit & BIT_CONFIG_TRIGGER_INTERVAL)//�����ʱ��	UINT	0-65535��  
  {
    RTC_Config();  //ֱ������
  }
  if (channel_en_bit & BIT_TIMESTAMP_ID)
  {
    TickGetTime(Sys_keydata.TimesTamp); //����ʱ�������ʱ�䡣
    TimeGetTick(TRUE);
#if SD3078_WIRTE_INIT_FLAG_EN
    RTC_SD3078_WriteInitFlag(); //д���־;
#endif
  }

  if(STATUS_CLEAR_FLASH == Sys_keydata.status) //��⵽ϵͳ��Ҫ���FLASH
  {
    debug_send("��ʼ���FLASH��");   
    //����������������
    Watchdog_Feed(); //����ȽϷ�ʱ�䣬Ҫ��������Ź�
    ClearFLASH_Queue();
    Sys_keydata.status = STATUS_NORMAL; //�ָ�������״̬��
  }
  Write_sys_keydata();
  
  Watchdog_Feed();
  Set_Key_Para();
  
  Watchdog_Feed();
  //  SysParaInit();//ϵͳ������ʼ��
  PrintSysParaList();
  Watchdog_Feed();
#endif  
}
//������������������������������������������������������������������������������
// ��������  : heartsend_Onehour
// ��������  : ����ʱ��������Ϊ1Сʱ������eeprom
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void heartsend_Onehour(void)
{
  Sys_keydata.heartdata_sent_interval_hour_count = 1;
}
