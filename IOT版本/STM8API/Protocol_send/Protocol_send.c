#include "config.h"


PROTOCOL_SEND_TYPE Net_Data_Send={0};

//������������������������������������������������������������������������������
// ��������  : upload_link_send()
// ��������  : ��������
// �������  : None
// �������  : None
// ���ز���  : ��
//������������������������������������������������������������������������������
void upload_link_send_POST()
{
  //  u8 TempTran[10];
  PostSendHeadCmd(USART_FUNCTION_WCDMA,01,0xf8);
  //PostSendByteASC(USART_FUNCTION_WCDMA,0xf8);
  
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.device_id,16);
  //ID
  PostSend_HexStr(USART_FUNCTION_WCDMA,platform_soft_version,2);
  //ƽ̨Լ���̼��汾
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.update_server_add,22);
  //�ɼ�IP
  PostSendDataLen(USART_FUNCTION_WCDMA);
  
  PostSendPostChksum(USART_FUNCTION_WCDMA);
  PostSendTail(USART_FUNCTION_WCDMA);
}
void SA_link_send_POST(void)
{
  PostSendHeadCmd(USART_FUNCTION_WCDMA,01,0x1A);  //�������ͷ
  //  	0x7E,//SOI
  //	'1', '1', //VER
  //	'0', '1', //ADR
  //	'D', '0', //CID1
  //	'1', 'A', //CID2
  //	//'B', '0', 'B', 'A', //LEN
  //        'A', '0', 'E', '8', //LEN
  pack_heat();//��������������ע�뻺��       
  PostSendDataLen(USART_FUNCTION_WCDMA);
  
  PostSendPostChksum(USART_FUNCTION_WCDMA);//'0', '0', '0', '0', //У���� Chksum_Cmd
  PostSendTail(USART_FUNCTION_WCDMA); //0x0D,//EOI
}
//������������������������������������������������������������������������������
// ��������  : SA_link_send()
// ��������  : ��������
// �������  : None
// �������  : None
// ���ز���  : ��
//������������������������������������������������������������������������������
void SA_link_send(void)
{
  if(Net_Data_Send.SA_Heartbeat.REC_state != REC_OK)
  {
    switch(Net_Data_Send.SA_Heartbeat.send_state )
    {
    case SEND_READY:
      {
        if(Net_Data_Send.SA_Heartbeat.TIMES>0){
          if(Net_Data_Send.SA_Heartbeat.TIMES!=0xff)
            Net_Data_Send.SA_Heartbeat.TIMES--;
          
          Net_Data_Send.SA_Heartbeat.delays=0;
          Net_Data_Send.SA_Heartbeat.send_state=WAIT_ACK;
          SA_link_send_POST();//��������
        }else{
          Down_Pro = PRO_END;
          PrintString_("����ʧ�ܣ��˳�ͨѶ");
        }
      }break;
    case WAIT_ACK:
      {
        if(Net_Data_Send.SA_Heartbeat.delays>Net_Data_Send.SA_Heartbeat.MAX_delays)
        {
          Net_Data_Send.SA_Heartbeat.delays=0;
          Net_Data_Send.SA_Heartbeat.send_state=SEND_READY;
          PrintString_("�ȴ������ظ���ʱ");
          
        }
        if(Net_Data_Send.SA_Heartbeat.REC_state == REC_OK)
        {
          Net_Data_Send.SA_Heartbeat.TIMES=0;//����Ӧ��ɹ���ֱ�����㣬�����ط�
          Net_Data_Send.SA_Heartbeat.send_state=SEND_READY;
          Net_Data_Send.SA_Heartbeat.delays=0;
        }
      }break;
    default:break;
    }	
  }
}
//������������������������������������������������������������������������������
// ��������  : upload_link_send()
// ��������  : ��������
// �������  : None
// �������  : None
// ���ز���  : ��
//������������������������������������������������������������������������������
void upload_link_send()
{
  if(Net_Data_Send.upload_handshake.REC_state != REC_OK)
  {
    switch(Net_Data_Send.upload_handshake.send_state )
    {
    case SEND_READY:
      {
        if(Net_Data_Send.upload_handshake.TIMES>0)//���ʹ���
        {
          if(Net_Data_Send.upload_handshake.TIMES!=0xff)
            Net_Data_Send.upload_handshake.TIMES--;
          
          Net_Data_Send.upload_handshake.delays=0;
          Net_Data_Send.upload_handshake.send_state=WAIT_ACK;
          upload_link_send_POST();
        }else{
          Down_Pro = PRO_END;
          PrintString_("����ʧ�ܣ��˳�ͨѶ");
        }
        
      }break;
    case WAIT_ACK:
      {
        if(Net_Data_Send.upload_handshake.delays>Net_Data_Send.upload_handshake.MAX_delays)
        {
          Net_Data_Send.upload_handshake.delays=0;
          Net_Data_Send.upload_handshake.send_state=SEND_READY;
          PrintString_("�ȴ����ֳ�ʱ");
          
        }
        if(Net_Data_Send.upload_handshake.REC_state == REC_OK)
        {
          Net_Data_Send.upload_handshake.TIMES=0;
          Net_Data_Send.upload_handshake.send_state=SEND_READY;
          Net_Data_Send.upload_handshake.delays=0;
          
        }
      }break;
    default:break;
    }
  }
}	

s8 link_send_POST(s8 (*fun)())
{
  
  PostSendDataLen(USART_FUNCTION_WCDMA);
  PostSendPostChksum(USART_FUNCTION_WCDMA);//'0', '0', '0', '0', //У���� Chksum_Cmd
  PostSendTail(USART_FUNCTION_WCDMA); //0x0D,//EOI
  return 1; //��ȷ
}  
//������������������������������������������������������������������������������
// ��������  : link_send()
// ��������  : ͨ�÷��ͺ�����
// �������  : SEND_TYPE *sendtype,void (*fun)()
// �������  : None
// ���ز���  : ��
//������������������������������������������������������������������������������
void link_send(SEND_TYPE *sendtype,s8 (*fun)())
{
  s8 s8_tmp; 
  switch(sendtype->send_state )
  {
  case SEND_READY:
    s8_tmp = fun();    //���ò�ͬ��������������,ֱ�Ӵ�����ݵ����ͻ��档
    if((-1) == s8_tmp) 
    {
      //���ݴ������
      Down_Pro = PRO_END;
      PrintString_("���ݴ�������˳�ͨѶ");
      sendtype->send_state = EXIT;//�˳����͹���
    }
    else if(0 == s8_tmp) //ֻ����һ������
    {
      sendtype->send_control = SEND_STOP;//����һ�κ��ֹͣ
      sendtype->send_state = SEND_ONE;//���뷢������
    }
    else if(1 == s8_tmp) //�ظ�����һ������
    {
      sendtype->send_control = SEND_CONTINUE;//�ظ�����
      sendtype->send_state = SEND_ONE;//���뷢������
    }
    else
    {
      sendtype->send_state = EXIT;//�˳����͹���
    }
    break;
  case SEND_ONE:
    link_send_POST(fun); //ִ�з��ͺ�����
    sendtype->delays=0;//������ͼ�ʱ��
    sendtype->send_state=WAIT_ACK;//�����Ѿ���ɣ�����ȴ��ظ�״̬��
    break;  
  case SEND_REPEATEDLY:
    if(sendtype->TIMES>0)//���ʹ���
    {
      if(sendtype->TIMES!=0xff)
      {
        sendtype->TIMES--;
      }
      sendtype->delays=0;
      CONST_REPEATEDLY_SEND_DATA; //�ط�һ�����ݡ�
      sendtype->delays=0;//������ͼ�ʱ��
      sendtype->send_state=WAIT_ACK;//�����Ѿ���ɣ�����ȴ��ظ�״̬��
    }
    else
    {
      FLASH_Queue_Recover(); //�ָ����С�
      sendtype->send_state=EXIT; //�����˳�״̬��
      PrintString_("����ʧ�ܣ��˳�ͨѶ");
    }
    break;
  case WAIT_ACK:
    {
      if(sendtype->delays>sendtype->MAX_delays)
      {
        sendtype->delays=0;
        sendtype->send_state=SEND_REPEATEDLY;
        PrintString_("�ȴ����ֳ�ʱ");
        
      }
      if(REC_OK == sendtype->REC_state ) //�ɹ��յ��ظ�
      {
        sendtype->REC_state = WAIT_REC; //����ɹ���־��
        if(SEND_STOP == sendtype->send_control) //�����˷���ֹͣ��
        {
          sendtype->TIMES=0; //�ظ����͡�
          sendtype->send_state=EXIT;
          sendtype->delays=0;
          PrintString_("���ͳɹ����˳�ͨѶ");
        }
        else if(SEND_CONTINUE == sendtype->send_control)
        {
          sendtype->TIMES=6; //�������Է��͡�
          sendtype->send_state=SEND_READY; //�ٴ�׼���µ����ݡ�
          sendtype->delays=0;//������ͼ�ʱ��
        }
      }
    }break;
  case EXIT:  
    sendtype->send_state = SEND_READY; //�´ν���״̬����׼������״̬
    sendtype->TIMES=6; //�������Է��͡�
    sendtype->delays=0;//������ͼ�ʱ��
    Down_Pro = PRO_END; //�˳����͹��̡�
    break;
  default:break;
  }
}
//������������������������������������������������������������������������������
// ��������  : link_send()
// ��������  : ͨ�÷��ͺ�����
// �������  : SEND_TYPE *sendtype,void (*fun)()
// �������  : None
// ���ز���  : ��
//������������������������������������������������������������������������������


//������������������������������������������������������������������������������
// ��������  : net_data_send_init()
// ��������  : �������ݷ��ͳ�ʼ������
// �������  : None
// �������  : None
// ���ز���  :��
//������������������������������������������������������������������������������
void net_data_send_init()
{
  //Net_Data_Send.Link_Type = HEARTBEAT;
  //�����������ݳ�ʼ��
  Net_Data_Send.upload_handshake.delays =0;
  Net_Data_Send.upload_handshake.MAX_delays  =15;
  Net_Data_Send.upload_handshake.TIMES  =6;
  Net_Data_Send.upload_handshake.send_state  =SEND_READY;
  Net_Data_Send.upload_handshake.REC_state = WAIT_REC;
  
  
  Net_Data_Send.SA_Heartbeat.delays =0;
  Net_Data_Send.SA_Heartbeat.MAX_delays  = 15; //һ�η��ͺ�ȴ�Ӧ��ʱ��5s
  Net_Data_Send.SA_Heartbeat.TIMES  =6;  //�ظ�6��
  Net_Data_Send.SA_Heartbeat.send_state  =SEND_READY;
  Net_Data_Send.SA_Heartbeat.REC_state = WAIT_REC;
  
  Net_Data_Send.Updata.delays =0;
  Net_Data_Send.Updata.MAX_delays  = 15; //һ�η��ͺ�ȴ�Ӧ��ʱ��5s
  Net_Data_Send.Updata.TIMES  =6;  //�ظ�6��
  Net_Data_Send.Updata.send_state  =SEND_READY;
  Net_Data_Send.Updata.send_control = SEND_STOP;
  Net_Data_Send.Updata.REC_state = WAIT_REC;  
  
  Rec_Timeout.Post_REC_TIMEOUT.Rec_State = STOP;
  Rec_Timeout.Post_REC_TIMEOUT.delays = 0;
  Rec_Timeout.Post_REC_TIMEOUT.MAX_delays = 30;//����30s��ʱ����
  
  Rec_Timeout.Download_REC_TIMEOUT.Rec_State = STOP;//����ʱ��ʱ
  Rec_Timeout.Download_REC_TIMEOUT.delays = 0;
  Rec_Timeout.Download_REC_TIMEOUT.MAX_delays = 90;//����90s��ʱ����
  
  Rec_Timeout.NET_STATE_TIMEOUT.Rec_State = RUN;//�������ӳ�ʱ��Ĭ�ϴ򿪣�
  Rec_Timeout.NET_STATE_TIMEOUT.delays = 0;
  Rec_Timeout.NET_STATE_TIMEOUT.MAX_delays = 240;//����4���ӳ�ʱ����
}
//������������������������������������������������������������������������������
// ��������  : net_data_send_delay
// ��������  : �������ݷ�����ʱ
// �������  : None
// �������  : None
// ���ز���  : ��
//������������������������������������������������������������������������������
void net_data_send_delay()
{
  Net_Data_Send.upload_handshake.delays++;
  Net_Data_Send.SA_Heartbeat.delays++;  
  Net_Data_Send.Updata.delays++;
}
//������������������������������������������������������������������������������
// ��������  : net_data_send()
// ��������  : �������ݷ���
// �������  : None
// �������  : None
// ���ز���  : ��
//������������������������������������������������������������������������������
void net_data_send()
{
  if(net.reconnect_setp==LINK_OK)
  {
    switch(Net_Data_Send.Link_Type)
    {
    case UPLOAD:
      upload_link_send();//������������������
      break;
    case HEARTBEAT:
      SA_link_send();//������������
      break;  
    case UPDATA:
      link_send(&Net_Data_Send.Updata,pack_updata);  //������ʷ����
      break;    
    default:break;   
    }
    
    //    if(Net_Data_Send.Link_Type == UPLOAD)
    //      upload_link_send();//������������������
    //    else
    //      SA_link_send();//������������
  }
}



