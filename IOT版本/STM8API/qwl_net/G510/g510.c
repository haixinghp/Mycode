
#include "config.h"
u8 AT_CPIN2[]	="AT+CPIN?";		//��ѯSIM��״̬
u8 AT_CIPSTART2[]="AT+MIPOPEN=1,,\"";//GPRS���ӷ�ʽ\GPRS���ӵ�ַ(�����û�ʵ��IP�޸�)\GPRS���ӵĶ˿�
u8 AT_CIPCLOSE2[]="AT+MIPCALL=0";		//TCP���ӹر�
//u8 AT_zpppopen2[]	="AT+MIPCALL=1,\"cmnet\",\"user\",\"pswd\"";		//������
u8 AT_zpppopen2[]	="AT+MIPCALL=1,\"cmnet\"";
//u8 AT_zpppstatus2[]="at+zpppstatus";//��ѯ����״̬
u8 AT_CIPSEND2[]	="AT+MIPSEND=1,";		//GPRS������Ϣ����
u8 AT_GETICCID2[]	="AT+CCID";		//��ѯSIM����
u8 AT_CIMI2[]	="AT+CIMI";		//��ѯSIM����
u8 AT_GTSET[]	="AT+GTSET=\"IPRFMT\",2";//���ý��ܸ�ʽ




u8 G510_RESET_flag=0;



void qwl_sendchar(u8 ch)
{
  USART3_sendchar(ch);
}


void qwl_sendstring(char *temp_in_string)
{
  PrintString_(temp_in_string);
}




/*****************************************************************************************************************
ʵ�ֹ���:��SIM800C��������,����SIM800C��������:��Ƭ�����Ǹ�SIM800C�Ĺܽ�(/IGT)��ʱ������Ϊ1s�ĵ͵�ƽ�ź�
*****************************************************************************************************************/
/*
void GSM_RESET(void)
{

GPIO_ResetBits(CDMA_IO,CDMA_RESET);
GPIO_SetBits(CDMA_IO,CDMA_ON_OFF);
Delay_MS(100);//�ֲ�ֻҪ20ms���Ҹ������ʱ��
GPIO_SetBits(CDMA_IO,CDMA_RESET);
GPIO_SetBits(CDMA_IO,CDMA_ON_OFF);
Delay_MS(50);//�ֲ�ֻҪ10ms���Ҹ������ʱ��
GPIO_SetBits(CDMA_IO,CDMA_RESET);
GPIO_ResetBits(CDMA_IO,CDMA_ON_OFF);

}
*/
/*****************************************************************************************************************
ʵ�ֹ���:��SIM800C��������,����SIM800C��������:��Ƭ�����Ǹ�SIM800C�Ĺܽ�(/IGT)��ʱ������Ϊ1s�ĵ͵�ƽ�ź�
*****************************************************************************************************************/
/*
void Start_GSM(void)
{
//	GSM_RESET();
//	Delay_MS(3000);
//  GPIO_SetBits(CDMA_IO,CDMA_ON_OFF);
//	GPIO_SetBits(CDMA_IO,CDMA_RESET);

}
*/
/*******************************************************************************
* �� �� ��         : G510_init()
* ��������		   : G510��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void G510_init()
{
  //u8 i;
  switch (net.setp_setp)
  {
  case 0:
    {
      clear_SystemBuf();
      qwl_net_sendstring("ATE0V1");//��ѯSIM��״̬
      net.delay=0;
      net.setp_setp=1;
      net.delay=0;
      net.delay_times=0;
    }break;
  case 1:
    {
      if(net.delay>2)//�ȴ���ʱ������������
      {net.delay=0;
      net.delay_times=0;
      net.setp_setp=0;
      net.reconnect_setp=MODE_CHOICE;//����������������
      net.delay=0;
      }
      if(strsearch_h("OK",SystemBuf)>0)
      { qwl_sendstring("���Թرճɹ�");
      net.delay_times=0;
      net.setp_setp=2;
      net.reconnect_setp=MODE_INIT;//����������������
      net.delay=0;
      }
    }break;
  case 2:
    {
      clear_SystemBuf();
      qwl_net_sendstring(AT_CPIN2);//��ѯSIM��״̬
      net.delay=0;
      net.setp_setp=3;
      net.delay=0;
      //net.delay_times=0;
    }break;
  case 3:
    {
      if(net.delay>2)//�ȴ���ʱ������������
      {net.delay=0;
      net.setp_setp=2;
      qwl_sendstring("��sim��״̬��ʱ");
      net.delay_times++;
      if(net.delay_times>12)
      {qwl_sendstring("sim��������");
      net.delay_times=0;
      net.setp_setp=0;
      net.reconnect_setp=MODE_CHOICE;//����������������
      net.delay=0;
      }
      }	
      if(strsearch_h("READY",SystemBuf)>0)
      { qwl_sendstring("��sim���ɹ�");
      net.delay_times=0;
      net.setp_setp=4;
      net.reconnect_setp=MODE_INIT;//������������
      net.delay=0;
//      Delay_MS(1000);
      }
      
      
    }break;
  case 4:
    {
      qwl_sendstring("��sim����");
      clear_SystemBuf();
      qwl_net_sendstring(AT_GETICCID2);//��ѯSIM������
      
      net.delay=0;
      net.setp_setp=5;
      net.delay=0;
      //net.delay_times=0;
    }break;
  case 5:
    {u16 j;
    u16 i;
    u8 last_setp=0;//��һ����������
    u8 next_setp=6;//��һ�������ĵ�ַ
    SETP_TYPE last_reconnect_setp=MODE_CHOICE;//��һ�����ӻ���
    SETP_TYPE next_reconnect_setp=MODE_INIT;//��һ�����ӻ���
    
    if(net.delay>2)//�ȴ���ʱ������������
    {net.delay=0;
    net.setp_setp=4;
    qwl_sendstring("��sim���ų�ʱ");
    net.delay_times++;
    if(net.delay_times>5)
    {qwl_sendstring("sim���Ų�����");
    net.delay_times=0;
    net.setp_setp=last_setp;
    net.reconnect_setp=last_reconnect_setp;//����������������
    net.delay=0;
    }
    }	
    if(g_flag==0)
      j=strsearch_h("+GETICCID:0x",SystemBuf);
    else
    {u8 qqq=0;
    j=strsearch_h("OK",SystemBuf);
    if(j>0)
    {
      for(i=0;i<Rx;i++)
      {
        if(SystemBuf[i]>=0x30)
          if(SystemBuf[i]<=0x39)
          {
            if(qqq>=5)
              break;
            qqq++;
          }
      }
      if(i>=5)
        j=i-5;
      else
        j=0;
    }
    
    }
    if(j>0)
    { qwl_sendstring("��sim���ųɹ�");
    for(i=0;i<20;i++)
    {SIM_ID[i]=SystemBuf[j+i];
    qwl_sendchar(SIM_ID[i]);
    }
    net.delay_times=0;
    net.setp_setp=next_setp;
    net.reconnect_setp=next_reconnect_setp;//������������
    net.delay=0;
//    Delay_MS(1000);
    }
    
    
    }break;
  case 6:
    {
      u8 next_setp=7;//��һ�������ĵ�ַ
      
      clear_SystemBuf();			
      qwl_net_sendstring(AT_GTSET);
      net.setp_setp=next_setp;
      net.delay=0;
      qwl_sendstring("���ý��ܸ�ʽ");
    }break;
  case 7:
    {
      u16 j;
      u8 last_setp=6;
//      SETP_TYPE next_reconnect_setp=OPEN_GPRS;//��һ�����ӻ���
      //next_reconnect_setp=OPEN_GPRS;//��һ�����ӻ���
      
      if(net.delay>3)//�ȴ���ʱ������������
      {net.delay=0;
      net.setp_setp=last_setp;//�ȴ�ʧ��
      
      qwl_sendstring("���ø�ʽ��ʱ");
      net.delay_times++;
      if(net.delay_times>3)
      {
        Delay_MS(1000);
        net.delay_times=0;
        net.setp_setp=0;
//        net.reconnect_setp=next_reconnect_setp;//����������������
        net.delay=0;
      }
      }
      
      
      j=strsearch_h("OK",SystemBuf);
      if(j>0)
      {
        
        qwl_sendstring("���ø�ʽ����");
        
        Delay_MS(1000);
        net.delay_times=0;
//        net.setp_setp=0;
//        net.reconnect_setp=next_reconnect_setp;//����������������
        net.setp_setp=8; //by hx 2017-12-22
        net.delay=0;
      }
      
      
    }break;
    case 8:
      {
      clear_SystemBuf();
      qwl_net_sendstring("AT+CGREG?");//��ѯGPRS״̬
      net.delay=0;
      net.setp_setp=9;
      net.delay=0;
      net.delay_times=0;
      qwl_sendstring("��ѯGPRS����ע��");
      Delay_MS(1000);
    }break;
  case 9:
    if(net.delay>2)//�ȴ���ʱ������������
      {
        net.delay=0;
        net.delay_times=0;
        net.setp_setp=8;
//        net.reconnect_setp=MODE_CHOICE;//����������������
        net.delay=0;
        net.delay_times++;
        if(net.delay_times>12)
        {
          qwl_sendstring("��ѯGPRS���糬ʱ");
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=MODE_CHOICE;//����������������
          net.delay=0;
        }
      }
      if(strsearch_h("+CGREG: 0,1",SystemBuf)>0)
      { 
        qwl_sendstring("GPRS����ע��ɹ�");
//        net.reconnect_setp= OPEN_GPRS;//��һ�����ӻ���
        net.delay_times=0;
        net.setp_setp=10;
        net.delay=0;
      }
    break;
   case 10:
      {
      clear_SystemBuf();
      qwl_net_sendstring("AT+CGATT?");//��ѯGPRS״̬
      net.delay=0;
      net.setp_setp=11;
      net.delay=0;
      net.delay_times=0;
      qwl_sendstring("��ѯGPRS���總��");
      Delay_MS(1000);
    }break;
  case 11:
    if(net.delay>2)//�ȴ���ʱ������������
      {
        net.delay=0;
        net.delay_times=0;
        net.setp_setp=10;
//        net.reconnect_setp=MODE_CHOICE;//����������������
        net.delay=0;
        net.delay_times++;
        if(net.delay_times>12)
        {
          qwl_sendstring("��ѯGPRS���總�ų�ʱ");
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=MODE_CHOICE;//����������������
          net.delay=0;
        }
      }
      if(strsearch_h("+CGATT: 1",SystemBuf)>0)
      { 
        qwl_sendstring("GPRS���總�ųɹ�");
        net.reconnect_setp= OPEN_GPRS;//��һ�����ӻ���
        net.delay_times=0;
        net.setp_setp=0;
        net.delay=0;
      }
    break;
  default:break;
  }
  
  
  
  
}
//*******************************************************
//TP����
//******************************************************
void TP_GPRS1(u8 *p,u8 *q)
{//u8 ip_init_word[]={"121.42.196.113,28070"};//112.74.98.39,8647����Ĭ��IP
  u8 i=0;
  while(*p)
  {
    qwl_net_sendchar(*p);
    qwl_sendchar(*p);
    p++;
    i++;
    if(i>20)
      break;
  }
  i=0;
  //q=&ip_init_word[0];
  while(*q)
  {
    if(*q==',')
    {
      qwl_net_sendchar('\"');
      qwl_sendchar('\"');
    }
    
    qwl_net_sendchar(*q);
    qwl_sendchar(*q);
    q++;
    i++;
    if(i>22)
      break;
  }
  qwl_net_sendchar(',');
  qwl_sendchar(',');
  qwl_net_sendchar(0x30);
  qwl_sendchar(0x30);
  qwl_sendchar(0x0D);
  qwl_sendchar(0x0A);
  qwl_net_sendchar(0x0D);
  qwl_net_sendchar(0x0A);
}
/*******************************************************************************
* �� �� ��         : void light_eth_reconnect()
* ��������		   : cdma��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void G510_reconnect()
{
  switch (net.setp_setp)
  {
  case 0:
    {
      qwl_sendstring("����IP");
      clear_SystemBuf();
      //if(exchange_platform.Platform_Choice == PLATFORM_CHOICE_PUBLIC)			
      //	TP_GPRS1(AT_CIPSTART2,light_TCP);//����IP
      //else
      //	TP_GPRS1(AT_CIPSTART2,update_ip);//��������IP
      
      if(Net_Data_Send.Link_Type == UPLOAD)
        TP_GPRS1(AT_CIPSTART2,Sys_keydata.update_server_add);
      else
        TP_GPRS1(AT_CIPSTART2,Sys_keydata.default_cloud_server_add);//����IP
//      Delay_MS(1000);  //by hx 2017.12.21
      net.setp_setp=1;
      net.delay=0;
      
      
      
    }break;
  case 1:
    {
      if(net.delay>=5)//�ȴ���ʱ������������
      {net.delay=0;
      net.setp_setp=0;//�ȴ�ʧ��
      qwl_sendstring("������ʱ");
      net.delay_times++;
      if(net.delay_times++>15)
      {net.delay_times=0;
      net.setp_setp=0;
      net.reconnect_setp=OPEN_GPRS;//����������������
      net.delay=0;
      }
      }
      
      if(strsearch_h("MIPOPEN",SystemBuf)>0)
      {
        net.reconnect_setp=LINK_OK;
        net.setp_setp=0;
        
        //					light_heart_time_flag=heart_max_time;
        qwl_sendstring("�����ɹ�1");
      }
      
      if(strsearch_h("ALREADY CONNECT",SystemBuf)>0)
      {net.reconnect_setp=LINK_OK;
      net.setp_setp=0;
      
      //					light_heart_time_flag=heart_max_time;
      qwl_sendstring("�����ɹ�1");
      }
      
      if(strsearch_h("CONNECT OK",SystemBuf)>0)
      {net.reconnect_setp=LINK_OK;
      net.setp_setp=0;
      
      //					light_heart_time_flag=heart_max_time;
      qwl_sendstring("�����ɹ�2");
      }
      
      if(strsearch_h("ERROR",SystemBuf)>0)
      {
        net.delay_times=0;
        net.setp_setp=0;
        net.reconnect_setp=OPEN_GPRS;//����������������
        net.delay=0;
        
        qwl_sendstring("��������");
        Delay_MS(1000);
      }
      
    }
  
    break;
  default:break;
  }
}
/*******************************************************************************
* �� �� ��         : void cdma_open_GPRS()
* ��������		   : ��������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void G510_open_GPRS()
{
  switch (net.setp_setp)
  {
  case 0:
    {
      
      G510_RESET_flag++;
      if(G510_RESET_flag>3)
      {
        net.delay_times=0;
        net.setp_setp=0;
        net.reconnect_setp=MODE_CHOICE;//����CDMA����
        net.delay=0;
        G510_RESET_flag=0;
        qwl_sendstring("����G510��λ");
      }
      clear_SystemBuf();
      qwl_net_sendstring(AT_CIPCLOSE2);
      net.setp_setp=1;
      net.delay=0;
      qwl_sendstring("�ر�����");
    }
    break;
  case 1:
    {
      if(net.delay>=6)//�ȴ���ʱ������������
      {
        net.delay=0;
        net.setp_setp=0;//�ȴ�ʧ��
        qwl_sendstring("����رճ�ʱ");
        net.delay_times++;
        if(net.delay_times>7)
        {
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=MODE_INIT;//����CDMA����
          net.delay=0;
          qwl_sendstring("����G510����");
        }
      }
      
      
      if(strsearch_h("+ZPPPSTATUS: CLOSED",SystemBuf)>0)
      {
        net.reconnect_setp=OPEN_GPRS;
        net.setp_setp=3;
        net.delay_times=0;
        net.delay=0;
        qwl_sendstring("����رճɹ�");
        Delay_MS(1000);
      }
      if(strsearch_h("ERROR",SystemBuf)>0)
      {
        net.reconnect_setp=OPEN_GPRS;
        net.setp_setp=3;
        net.delay_times=0;
        net.delay=0;
        
        qwl_sendstring("����رմ���");
        Delay_MS(1000);
      }
      
    }break;
  case 3:
    {
      clear_SystemBuf();			
      qwl_net_sendstring(AT_zpppopen2);
      Delay_MS(1000);  //by hx 2017.12.21
      net.setp_setp=4;
      net.delay=0;
      qwl_sendstring("����������");
    }break;
  case 4:
    {
      if(net.delay>=6)//�ȴ���ʱ������������
      {
        net.delay=0;
        net.setp_setp=0;//�ȴ�ʧ��
        qwl_sendstring("�������ӳ�ʱ");
        net.delay_times++;
        if(net.delay_times>7)
        {
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=MODE_INIT;//����CDMA����
          net.delay=0;
          qwl_sendstring("����CDMA����");
        }
      }
      
      
      if(strsearch_h("OK",SystemBuf)>0)
      {
//        net.reconnect_setp=LINK_IP;//����ϣ���������
        net.setp_setp=5;
        net.delay_times=0;
        net.delay=0;
        qwl_sendstring("���������ӳɹ�");
        Delay_MS(2000);//by hx 2017.12.21  �������Ҫ�ӳ٣���Ȼ���Ӳ������硣
      }
      if(strsearch_h("+ZPPPOPEN:CONNECTED",SystemBuf)>0)
      {net.reconnect_setp=LINK_IP;//����ϣ���������
      net.setp_setp=0;
      net.delay_times=0;
      net.delay=0;
      qwl_sendstring("���������ӳɹ�");
      Delay_MS(1000);
      }
      if(strsearch_h("ERROR",SystemBuf)>0)
      {
        net.setp_setp=0;//�ȴ�ʧ��					
        net.delay_times++;
        if(net.delay_times++>7)
        {
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=MODE_INIT;//����CDMA����
          net.delay=0;
          qwl_sendstring("����CDMA����");
        }
        
        qwl_sendstring("���������");
//        Delay_MS(1000);
      }
    }
    break;
  case 5:  //ȷ�������Ƿ��
    clear_SystemBuf();
    qwl_net_sendstring("AT+MIPCALL?");
    net.setp_setp=6;
    net.delay=0;
    qwl_sendstring("ȷ������");
    break;
  case 6:
    if(net.delay>=6)//�ȴ���ʱ������������
    {
      net.delay=0;
      net.setp_setp=0;//�ȴ�ʧ��
      qwl_sendstring("ȷ���������ӳ�ʱ");
      net.delay_times++;
      if(net.delay_times>7)
      {
        net.delay_times=0;
        net.setp_setp=0;
        net.reconnect_setp=MODE_INIT;//����CDMA����
        net.delay=0;
        qwl_sendstring("����CDMA����");
      }
    }
    if(strsearch_h("OK",SystemBuf)>0)
    {
      net.reconnect_setp=LINK_IP;//����ϣ���������
      G510_RESET_flag=0; //�����λ������by hx 2017.12.22
      net.setp_setp=0;
      //        net.setp_setp = 5;  //by hx 2017.12.22
      net.delay_times=0;
      net.delay=0;
      qwl_sendstring("ȷ���������ӳɹ�");
//      Delay_MS(2000);
      //      Delay_MS(1000);  //by hx 2017.12.21
    }
    if(strsearch_h("+ZPPPOPEN:CONNECTED",SystemBuf)>0)
    {
      net.reconnect_setp=LINK_IP;//����ϣ���������
      net.setp_setp=0;
      net.delay_times=0;
      net.delay=0;
      qwl_sendstring("���������ӳɹ�");
//      Delay_MS(1000);
    }
    if(strsearch_h("ERROR",SystemBuf)>0)
    {
      net.setp_setp=0;//�ȴ�ʧ��					
      net.delay_times++;
      if(net.delay_times++>7)
      {
        net.delay_times=0;
        net.setp_setp=0;
        net.reconnect_setp=MODE_INIT;//����CDMA����
        net.delay=0;
        qwl_sendstring("����G510����");
      }
      
      qwl_sendstring("ȷ���������Ӵ���");
//      Delay_MS(1000);
    }
    break;
  default:break;
  }
}

/*******************************************************************************
* �� �� ��         :G510_send(u8 *p,u16 len)
* ��������		   : 
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

u8 G510_send(u8 *p,u16 len)
{
  
  
  u16 j=0;
  u16 g_delay_us=0;
  u16 g_delay_ms=0;
 
  clear_SystemBuf();
  qwl_net_sendstring1(AT_CIPSEND2); 
  API_WatchDog_Feed();  //�豸�����������󣬼��幷����by hx 2017.12.22
  j=len;
  if(len>1000)
  {
    qwl_net_sendchar(NO[j/1000]);
    qwl_sendchar(NO[j/1000]);
    j=j%1000;
  }
  if(len>100)
  {
    qwl_net_sendchar(NO[j/100]);
    qwl_sendchar(NO[j/100]);
    j=j%100;
  }
  API_WatchDog_Feed();  //�豸�����������󣬼��幷����by hx 2017.12.22
  
  qwl_net_sendchar(NO[j/10]);
  qwl_net_sendchar(NO[j%10]);
  qwl_sendchar(NO[j/10]);
  qwl_sendchar(NO[j%10]);
  
  qwl_net_sendchar(0x0d);	
  qwl_net_sendchar(0x0A);
  g_delay_us=0;
  g_delay_ms=0;
  while(strsearch_h(">",SystemBuf)==0)
  {
    API_WatchDog_Feed();
    Delay_2us();
    g_delay_us++;
    if(g_delay_us>400)
    {
      g_delay_us=0;
      g_delay_ms++;
      if(g_delay_ms>1000)
      {
        
        break;
      }
    }
  }
  
  
  for(j=0;j<len;j++)
  {
    qwl_sendchar(p[j]);//������
    qwl_net_sendchar(p[j]);
  }
  qwl_sendchar(0x0D);
  qwl_sendchar(0x0A);
  qwl_net_sendchar(0x0D);
  qwl_net_sendchar(0x0A);
  
  
  return 0;
} 








