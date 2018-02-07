#include "config.h"

PROTOCOL_REC_TYPE Rec_Timeout;
uchar platform_soft_version[2]={0,0};
//������������������������������������������������������������������������������
// ��������  : Uploading_Send_50
// ��������  : ��ȡ��ַ
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void Uploading_Send_50(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{	
  PostSendHead(uf);
  PostSend_Word(uf,ModAddress);     // ��Э���ַ		
  PostSendDataLen(uf);
  PostSendPostChksum(uf);
  PostSendTail(uf);
}
//������������������������������������������������������������������������������
// ��������  : Uploading_Send_F8
// ��������  : ��ȡ��ַ
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void Uploading_Send_F8(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{

Run_User_Program_Falg = true;
switch (uf)
{
case USART_FUNCTION_DEBUG:
  {
#ifdef STM32_BOOT
    iap_data.copyflag = 1;
    iap_data.Address = ModAddress;
    API_goto_iap();
#else
    PostSendHeadCmd(uf,01,0x00);
    //PostSendByteASC(USART_FUNCTION_WCDMA,0xf8);
    
    PostSend_HexStr(uf,Sys_keydata.device_id,16);
    //ID
    PostSend_HexStr(uf,platform_soft_version,2);
    //ƽ̨Լ���̼��汾
    PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.update_server_add,22);
    //�ɼ�IP
    //    PostSendByteASC(uf,Cmd);		
    PostSendDataLen(uf);
    PostSendPostChksum(uf);
    PostSendTail(uf);
#endif
  }break;
case USART_FUNCTION_WCDMA:
  {
    PostSendHeadCmd(uf,01,0x00);
    //PostSendByteASC(USART_FUNCTION_WCDMA,0xf8);
    
    PostSend_HexStr(uf,Sys_keydata.device_id,16);
    //ID
    PostSend_HexStr(uf,platform_soft_version,2);
    //ƽ̨Լ���̼��汾
    PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.update_server_add,22);
    //�ɼ�IP
    PostSendDataLen(uf);
    PostSendPostChksum(uf);
    PostSendTail(uf);
  }break;
default:break;
}

}
//������������������������������������������������������������������������������
// ��������  : Uploading_Pro
// ��������  : �ϱ���Э�鴦��
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void Uploading_Pro(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{
  uchar  Cmd = DaPtr[4];
  switch(Cmd)
  {	
  case 0x50://��ȡ��ַ
    Run_User_Program_Falg = true;
    Uploading_Send_50(uf,DaPtr,len);
    break;
  case 0xF8:
    {	
      if(Net_Data_Send.upload_handshake.REC_state == WAIT_REC)//�ж��״����֣��ȴ���������
      {
        Rec_Timeout.Post_REC_TIMEOUT.delays = 0;
        Uploading_Send_F8(uf,DaPtr,len);
        
        if(Net_Data_Send.Link_Type != UPLOAD)
        {
#if CONF_EX_MEMORY_EN == UPLOAD_EEPROM//����ǰ����Ӳ��
          if(EEPROM_CheckOk()==1){
#else
            if(W25QXX_Test() == 1){
#endif
              PrintString_("�洢оƬOK");
              Net_Data_Send.Link_Type = UPLOAD;//һ��Ӧ����л����̼�����ģʽ
              net.reconnect_setp=LINK_IP;//IP��������
              heartsend_Onehour();//������ΪһСʱ
              //�ӿ�����Ƶ�ʣ���ֹ���������ж��������������������ܴ�eeprom�����ظ�
              Rec_Timeout.Post_REC_TIMEOUT.Rec_State = STOP;//�л�IP�رճ�ʱ
              Delay_MS(1000);//�л�ƽ̨ǰ����ʱ1sȷ�����һ�����ݷ��ͳɹ�
              Getparameters_Str(&DaPtr[25],(uchar*)Sys_keydata.update_server_add,21);
              Getparameters_Str(&DaPtr[23],(uchar*)platform_soft_version,2);
            }else{
              PrintString_("�洢оƬerror,����ͨѶ");
              Down_Pro=PRO_END;//����ͨѶ�����ؾ��ǳ���
            }
            break;
          }
          Net_Data_Send.upload_handshake.REC_state=REC_OK;//
        }		
      }
      break;
    case 0x00:
      switch(Net_Data_Send.Link_Type)
      {
      case HEARTBEAT:
        if(Net_Data_Send.SA_Heartbeat.REC_state == WAIT_REC)
        {
          //��������56�ֽڣ�
          //��������~1101D000802630303030303030303030303030303030000000F649(0x0D)
          //ת����hex��ǰ����������󣬲���������DaPtr[24]
          if(DaPtr[25]==1)//�������¿���
          {
            PrintString_("���й̼�����");
            Rec_Timeout.Post_REC_TIMEOUT.Rec_State = RUN;//�������ճ�ʱ
            
          }
          else if(DaPtr[24]==1)//�������¿���
          {
            PrintString_("�ȴ���������");
            Rec_Timeout.Post_REC_TIMEOUT.Rec_State = RUN;//�������ճ�ʱ
            
          }
          else
          {
            //        PrintString_("����ͨѶ");
            //        Down_Pro=PRO_END;//����ͨѶ�����ؾ��ǳ���
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
            PrintString_("�����ϱ��ɹ�");
            Net_Data_Send.Link_Type = UPDATA;  //������ʷ�ϱ�����
            PrintString_("��ʼ��ʷ�����ϱ�");
#endif            
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
            PrintString_("����ͨѶ");
            Down_Pro=PRO_END;//����ͨѶ�����ؾ��ǳ���
#endif             
          }
          Net_Data_Send.SA_Heartbeat.REC_state=REC_OK;//
        }
        break;
      case UPDATA:
        if(Net_Data_Send.Updata.REC_state == WAIT_REC)
        {
          Net_Data_Send.Updata.REC_state=REC_OK;
          PrintString_("��ʷ�����ϱ��ɹ�");
        }
        break;  
      default:break;
      }
      
      break;
    case 0x12:
      PrintString_("�ظ����汾");
      Uploading_Send_12(uf,DaPtr,len);//
      Rec_Timeout.Post_REC_TIMEOUT.delays = 0;//�ȴ�F8
      
      break;
    case 0xFB:
      //��У�����ֱ������
      PostSendDefault(uf);//�ظ�//~1001D0000000FDAA
      PrintString_("�����·�����");
      //      Watchdog_Feed();
      Getparameters(uf,DaPtr,len);
      Watchdog_Feed();
      
//      Down_Pro=PRO_END;//����ͨѶ�����ؾ��ǳ���
      
      Rec_Timeout.Post_REC_TIMEOUT.Rec_State = STOP;//���������곬ʱ�ر�
      Rec_Timeout.Post_REC_TIMEOUT.delays = 0;
      
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���      
      Net_Data_Send.Link_Type = UPDATA;  //������ʷ�ϱ�����
      PrintString_("��ʼ��ʷ�����ϱ�");
#endif
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
            PrintString_("����ͨѶ");
            Down_Pro=PRO_END;//����ͨѶ�����ؾ��ǳ���
#endif             
      /*~1101D0FB40EE00002C000010B82C000000000000000000000000000
      000002C00002C00002C062C062C00182C00002C012C2C00002C002C0
      000000000000000000000000000000000000000002C0000000000000
      000000000000000000000000000002C33392E3130382E39312E32303
      82C323030303800002C002C002CCE4E(0x0D)�����·���������  */
      break;
    default:
      RTN= 0x04;//��Ч����
      PostSendDefault(uf);
      break;
    }
}
void post_data_rec_delay(void)
{
  if(net.reconnect_setp!=LINK_OK){//�������ӳ�ʱ
    if(Rec_Timeout.NET_STATE_TIMEOUT.Rec_State == RUN){
      if(Rec_Timeout.NET_STATE_TIMEOUT.delays++>Rec_Timeout.NET_STATE_TIMEOUT.MAX_delays){
        Down_Pro=PRO_END;//����ͨѶ�����ؾ��ǳ���
        PrintString_("�޷���������");
      }
    }
  }else{
    Rec_Timeout.NET_STATE_TIMEOUT.delays = 0;//����
  }
  
  if(Rec_Timeout.Post_REC_TIMEOUT.Rec_State == RUN){//�ʵ粿Э����ճ�ʱ
    PrintString_("�ȴ�����");
    if(Rec_Timeout.Post_REC_TIMEOUT.delays++>Rec_Timeout.Post_REC_TIMEOUT.MAX_delays){
      Down_Pro=PRO_END;//����ͨѶ�����ؾ��ǳ���
      PrintString_("�ȴ���ʱ");
    }
  }
    
  if(Rec_Timeout.Download_REC_TIMEOUT.Rec_State == RUN){//���ؽ��ճ�ʱ
    PrintString_("�ȴ�����");
    if(Rec_Timeout.Download_REC_TIMEOUT.delays++>Rec_Timeout.Download_REC_TIMEOUT.MAX_delays){
      Down_Pro=PRO_END;//����ͨѶ�����ؾ��ǳ���
      PrintString_("�ȴ���ʱ");
    }
  }
}
//������������������������������������������������������������������������������
//                          �������(END)
//������������������������������������������������������������������������������


