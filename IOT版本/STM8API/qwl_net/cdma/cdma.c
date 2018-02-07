
#include "config.h"

#define AT_CPIN	        "AT+CPIN?"		//��ѯSIM��״̬
#define AT_CIPSTART     "at+zipsetup=0,"//GPRS���ӷ�ʽ\GPRS���ӵ�ַ(�����û�ʵ��IP�޸�)\GPRS���ӵĶ˿�
#define AT_CIPCLOSE     "at+zpppclose"	//TCP���ӹر�
#define AT_zpppopen     "at+zpppopen"		//������
#define AT_zpppstatus   "at+zpppstatus"//��ѯ����״̬
#define AT_CIPSEND      "at+zipsend=0,"		//GPRS������Ϣ����
#define AT_GETICCID     "AT+GETICCID"		//��ѯSIM����
#define AT_CIMI         "AT+CIMI"		//��ѯSIM����



u8 SIM_ID[20]={0};
u8 cdma_RESET_flag=0;

u8 g_flag=0;//G����־
/*******************************************************************************
* �� �� ��         : CDMA_IO_Init
* ��������		   : LED��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void CDMA_IO_Init()	  //�˿ڳ�ʼ��
{
  //	GPIO_InitTypeDef GPIO_InitStructure;	//����һ���ṹ�������������ʼ��GPIO
  //
  //
  //	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE); /* ����GPIOʱ�� */
  //
  //	/*  ����GPIO��ģʽ��IO�� */
  //	GPIO_InitStructure.GPIO_Pin=CDMA_RESET|CDMA_ON_OFF;	 //ѡ����Ҫ���õ�IO��
  //	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	  //�����������ģʽ
  //	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
  //	GPIO_Init(CDMA_IO,&GPIO_InitStructure); /* ��ʼ��GPIO */	
  //	GPIO_ResetBits(CDMA_IO,CDMA_RESET);
  //	GPIO_SetBits(CDMA_IO,CDMA_ON_OFF);
  
  GPIO_Init(GPIOE, CDMA_RESET|CDMA_ON_OFF, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_ResetBits(CDMA_IO,CDMA_RESET);
  GPIO_SetBits(CDMA_IO,CDMA_ON_OFF);
  
}
/*********************************************************************
** ��������: strsearch_h ()
** ��������: ��ָ���������������ҵ���ͬ������
** ��ڲ���: ptr2Ҫ���ҵ�����, ptr1��ǰ����
** ���ڲ���: 0-û�� �ҵ�   >1 ���ҵ�
*********************************************************************/
u16 strsearch_h(u8 *ptr2,u8 *ptr1_at)//���ַ���*ptr2��*ptr1�е�λ��
//����������������ַ���*ptr2�Ƿ���ȫ������*ptr1��
//����:  0  û���ҵ�
//65535 �ӵ�N���ַ���ʼ��ͬ
{
  u16 i,j,k;
  
  
  if(Rx==0) return(0);
  
  for(i=0,j=0;i<Rx;i++)
  {
    if(ptr1_at[i]==ptr2[j])
    {	//��һ���ַ���ͬ
      for(k=i;k<Rx+1;k++,j++)
      {
        
        if(ptr2[j]==0)//�Ƚ���ȷ
        {                return(k);               //����ֵ������������0
        }
        if(ptr1_at[k]!=ptr2[j]) break;
        
      }
      j=0;
    }
  }
  return(0);
}

/****************************************************************************************************************/
///////////////////////////////////////void clear_SystemBuf();������ڽ��ջ�����/////////////////////////////////
/****************************************************************************************************************/
void clear_SystemBuf()
{
  
  Rx=0;
}



//*******************************************************
//TP����
//******************************************************
void TP_GPRS(u8 *p,u8 *q)
{//u8 ip_init_word[]={"121.42.196.113,28070"};//112.74.98.39,8647����Ĭ��IP
  u8 i=0;
  while(*p)
  {
    qwl_net_sendchar(*p);
    USART3_sendchar(*p);
    p++;
    i++;
    if(i>20)
      break;
  }
  i=0;
  //q=&ip_init_word[0];
  while(*q)
  {
    qwl_net_sendchar(*q);
    USART3_sendchar(*q);
    q++;
    i++;
    if(i>22)
      break;
  }
  USART3_sendchar(0x0D);
  USART3_sendchar(0x0A);
  qwl_net_sendchar(0x0D);
  qwl_net_sendchar(0x0A);
}
//*******************************************************
//SIM800C��ʼ��
//*********************************************************
/*u8 GSM_init()
{	u8 i=0;
qwl_net_sendstring("ATE0V1");//�رջ�������DCEΪOK��ʽ
Delay_MS(1000);

while(1)
{

led_reversal();
clear_SystemBuf();
qwl_net_sendstring(AT_CPIN);//��ѯSIM��״̬
Delay_MS(1000);
if(strsearch_h("READY",SystemBuf)==0)
{ 
i++;
if(i>5)
return 1;
		}
		else
{break;
		}
	}
clear_SystemBuf();
qwl_net_sendstring(AT_CIPCLOSE);
i=0;
led_reversal();
while(1)
{
Delay_MS(1000);
if(strsearch_h("ERROR",SystemBuf)==0)
{ 
i++;
if(i>5)
return 2;
					}
					else
{break;
					}

if(strsearch_h("+ZPPPSTATUS: CLOSED",SystemBuf)==0)
{ 
i++;
if(i>5)
return 2;
					}
					else
{break;
					}
			}
clear_SystemBuf();			
qwl_net_sendstring(AT_zpppopen);	
i=0;
led_reversal();
while(1)
{
Delay_MS(1000);
if(strsearch_h("OPENED",SystemBuf)==0)
{ 
i++;
if(i>8)
return 2;
					}
					else
{break;
					}
			}	 
clear_SystemBuf();
TP_GPRS(AT_CIPSTART,light_ip);//����IP
i=0;
led_reversal();
while(1)
{
Delay_MS(1000);
if(strsearch_h("+ZTCPESTABLISHED:0",SystemBuf)==0)
{ 
i++;
if(i>5)
return 2;
					}
					else
{break;
					}

if(strsearch_h("ERROR",SystemBuf)==0)
{ 
i++;
if(i>5)
return 2;
					}
					else
{return 2;
					}
			}	 
return 0;
}

*/

/*****************************************************************************************************************
ʵ�ֹ���:��SIM800C��������,����SIM800C��������:��Ƭ�����Ǹ�SIM800C�Ĺܽ�(/IGT)��ʱ������Ϊ1s�ĵ͵�ƽ�ź�
*****************************************************************************************************************/

void Start_GSM(void)
{
  //	GSM_RESET();
  //	Delay_MS(3000);
  //  GPIO_SetBits(CDMA_IO,CDMA_ON_OFF);
  //	GPIO_SetBits(CDMA_IO,CDMA_RESET);
  
}

/*******************************************************************************
* �� �� ��         : cdma_init()
* ��������		   : cdma��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void cdma_init()
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
      {
        net.delay=0;
        net.delay_times=0;
        net.setp_setp=0;
        net.reconnect_setp=MODE_CHOICE;//����������������
        net.delay=0;
      }
      if(strsearch_h("OK",SystemBuf)>0)
      { 
        PrintString_("���Թرճɹ�");
        net.delay_times=0;
        net.setp_setp=2;
        net.reconnect_setp=MODE_INIT;//����������������
        net.delay=0;
      }
    }break;
  case 2:
    {
      clear_SystemBuf();
      qwl_net_sendstring(AT_CPIN);//��ѯSIM��״̬
      net.delay=0;
      net.setp_setp=3;
      net.delay=0;
      //net.delay_times=0;
    }break;
  case 3:
    {
      if(net.delay>2)//�ȴ���ʱ������������
      {
        net.delay=0;
      net.setp_setp=2;
      PrintString_("��sim��״̬��ʱ");
      net.delay_times++;
      if(net.delay_times>5)
      {PrintString_("sim��������");
      net.delay_times=0;
      net.setp_setp=0;
      net.reconnect_setp=MODE_CHOICE;//����������������
      net.delay=0;
      }
      }	
      if(strsearch_h("READY",SystemBuf)>0)
      { 
        PrintString_("��sim���ɹ�");
        net.delay_times=0;
        net.setp_setp=4;
        net.reconnect_setp=MODE_INIT;//������������
        net.delay=0;
      }
      
      
    }break;
  case 4:
    {
      PrintString_("��sim����");
      clear_SystemBuf();
      if(g_flag==0)
        qwl_net_sendstring(AT_GETICCID);//��ѯSIM������
      else
        qwl_net_sendstring(AT_CIMI);
      net.delay=0;
      net.setp_setp=5;
      net.delay=0;
      //net.delay_times=0;
    }break;
  case 5:
    {
      u16 j;
      u8 i;
      if(net.delay>2)//�ȴ���ʱ������������
      {
        net.delay=0;
        net.setp_setp=4;
        PrintString_("��sim���ų�ʱ");
        net.delay_times++;
        if(net.delay_times>5)
        {
          PrintString_("sim���Ų�����");
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=MODE_CHOICE;//����������������
          net.delay=0;
        }
      }	
      if(g_flag==0)
        j=strsearch_h("+GETICCID:0x",SystemBuf);
      else
      {
        j=strsearch_h("OK",SystemBuf);
        j=j+2;
      }
      if(j>0)
      { 
        PrintString_("��sim���ųɹ�");
        for(i=0;i<20;i++)
        {
          SIM_ID[i]=SystemBuf[j+i];
          if(i>5)
            Sys_keydata.Num_Cmd[0]=SystemBuf[j+i];
          USART3_sendchar(SIM_ID[i]);
        }
        for(i=0;i<13;i++)
        {
          Sys_keydata.Num_Cmd[i] = SIM_ID[i+6];//��ȡ���������
        }
        
        net.delay_times=0;
        net.setp_setp=0;
        net.reconnect_setp=OPEN_GPRS;//����������������
        net.delay=0;
      }
      
      
    }break;
    
  default:break;
  }
  
  
  /*
  Start_GSM();
  i=GSM_init();
  if(i==1)
  PrintString_("��ѯSIM��״̬ʧ��");
	else if(i==2)
  PrintString_("��������ʧ��");
	else if(i==0)
  PrintString_("�ɹ�����");	
  */
  //send_data();//ip_init_word);
  
}

/*******************************************************************************
* �� �� ��         : void light_eth_reconnect()
* ��������		   : cdma��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void cdma_reconnect()
{
  switch (net.setp_setp)
  {
  case 0:
    {
      PrintString_("����IP");
      clear_SystemBuf();
      
  if(Net_Data_Send.Link_Type == UPLOAD)
    TP_GPRS(AT_CIPSTART,Sys_keydata.update_server_add);
  else
      TP_GPRS(AT_CIPSTART,Sys_keydata.default_cloud_server_add);//����IP
      net.setp_setp=1;
      net.delay=0;
      
      
      
    }break;
  case 1:
    {
      if(net.delay>=5)//�ȴ���ʱ������������
      {
        net.delay=0;
        net.setp_setp=0;//�ȴ�ʧ��
        PrintString_("������ʱ");
        net.delay_times++;
        if(net.delay_times++>15)
        {
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=OPEN_GPRS;//����������������
          net.delay=0;
        }
      }
      
      
      if(strsearch_h("+ZTCPESTABLISHED:0",SystemBuf)>0)
      {
        net.reconnect_setp=LINK_OK;
        net.setp_setp=0;
        
        //					light_heart_time_flag=heart_max_time;
        PrintString_("�����ɹ�1");
      }
      
      if(strsearch_h("+ZIPSETUP:CONNECTED",SystemBuf)>0)
      {
        net.reconnect_setp=LINK_OK;
        net.setp_setp=0;
        
        //					light_heart_time_flag=heart_max_time;
        PrintString_("�����ɹ�2");
      }
      
      if(strsearch_h("ERROR",SystemBuf)>0)
      {
        net.delay_times=0;
        net.setp_setp=0;
        net.reconnect_setp=OPEN_GPRS;//����������������
        net.delay=0;
        
        PrintString_("��������");
      }
      
    }break;
  default:break;
  }
}
/*******************************************************************************
* �� �� ��         : void cdma_open_GPRS()
* ��������		   : ��������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

void cdma_open_GPRS()
{
  u8 TempTran = 0;
  switch (net.setp_setp)
  {
  case 0:
    {
      
      cdma_RESET_flag++;
      if(cdma_RESET_flag>3)
      {
        net.delay_times=0;
        net.setp_setp=0;
        net.reconnect_setp=MODE_CHOICE;//����CDMA����
        net.delay=0;
        cdma_RESET_flag=0;
        PrintString_("����cdma��λ");
      }
      clear_SystemBuf();
      qwl_net_sendstring(AT_CIPCLOSE);
      net.setp_setp=1;
      net.delay=0;
      PrintString_("�ر�����");
    }break;
  case 1:
    {
      if(net.delay>=8)//�ȴ���ʱ������������
      {
        net.delay=0;
        net.setp_setp=0;//�ȴ�ʧ��
        PrintString_("����رճ�ʱ");
        net.delay_times++;
        if(net.delay_times>7)
        {
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=MODE_INIT;//����CDMA����
          net.delay=0;
          PrintString_("����CDMA����");
        }
      }
      
      
      if(strsearch_h("+ZPPPSTATUS: CLOSED",SystemBuf)>0)
      {
        net.reconnect_setp=OPEN_GPRS;
        net.setp_setp=3;
        net.delay_times=0;
        net.delay=0;
        PrintString_("����رճɹ�");
      }
      if(strsearch_h("ERROR",SystemBuf)>0)
      {
        net.reconnect_setp=OPEN_GPRS;
        net.setp_setp=3;
        net.delay_times=0;
        net.delay=0;
        
        PrintString_("����رմ���");
      }
      
    }break;
  case 3:
    {
      clear_SystemBuf();			
      qwl_net_sendstring(AT_zpppopen);
      net.setp_setp=4;
      net.delay=0;
      PrintString_("����������");
    }break;
  case 4:
    {
      if(net.delay>=15)//�ȴ���ʱ������������
      {
        net.delay=0;
        net.setp_setp=0;//�ȴ�ʧ��
        PrintString_("�������ӳ�ʱ");
        net.delay_times++;
        if(net.delay_times>5)
        {
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=MODE_INIT;//����CDMA����
          net.delay=0;
          PrintString_("����CDMA����");
        }
      }
      
      
      if(strsearch_h("OPENED",SystemBuf)>0)
      {
        net.reconnect_setp=OPEN_GPRS;//����ϣ���������
        net.setp_setp=5;
        net.delay_times=0;
        net.delay=0;
        PrintString_("���������ӳɹ�");
      }
      if(strsearch_h("+ZPPPOPEN:CONNECTED",SystemBuf)>0)
      {
        net.reconnect_setp=OPEN_GPRS;//����ϣ���������
        net.setp_setp=5;
        net.delay_times=0;
        net.delay=0;
        PrintString_("���������ӳɹ�");
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
          PrintString_("����CDMA����");
        }
        
        PrintString_("���������");
      }
      
    }break;
  case 5:
    {
      clear_SystemBuf();			
      qwl_net_sendstring("AT+CSQ");
      net.setp_setp=6;
      net.delay=0;
      PrintString_("��ѯ�ź�ǿ��");
    }break;
  case 6:
      if(net.delay>=5)//�ȴ���ʱ������������
      {
        net.delay=0;
        net.setp_setp=0;//�ȴ�ʧ��
        PrintString_("��ѯ��ʱ");
        net.delay_times++;
        if(net.delay_times>7)
        {
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=MODE_INIT;//����CDMA����
          net.delay=0;
          PrintString_("����CDMA����");
        }
      }
      
      
      if(strsearch_h(", 99",SystemBuf)>0)
      {
        net.reconnect_setp=LINK_IP;
        net.setp_setp=0;
        net.delay_times=0;
        net.delay=0;
        TempTran = strsearch_h("+CSQ: ",SystemBuf);
        Get_SigStren(TempTran);
      }
    break;
  default:break;
  }
}
/*******************************************************************************
* �� �� ��         : cdma_init_0()
* ��������		   : ��������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void cdma_init_0()
{
  net.delay_times=0;
  net.setp_setp=0;
  net.reconnect_setp=MODE_CHOICE;//�Ƚ���NBLOT����CDMAѡ��
  net.delay=0;
}
/*******************************************************************************
* �� �� ��         :cdma_send(u8 *p,u16 len)
* ��������		   : 
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

u8 cdma_send(u8 *p,u16 len)
{
  
  
  u16 j=0;
  u16 g_delay_us=0;
  u16 g_delay_ms=0;
  clear_SystemBuf();
  qwl_net_sendstring1(AT_CIPSEND);   //���� "at+zipsend=0,"		//GPRS������Ϣ����
  j=len;
  if(len>1000) 
  {
    qwl_net_sendchar(NO[j/1000]);
    USART3_sendchar(NO[j/1000]);
    j=j%1000;
  }
  if(len>100)
  {
    u8 k= NO[j/100];
    qwl_net_sendchar(k);
    USART3_sendchar(k);
    j=j%100;
  }
  
  qwl_net_sendchar(NO[j/10]);
  qwl_net_sendchar(NO[j%10]);
  USART3_sendchar(NO[j/10]);
  USART3_sendchar(NO[j%10]);
  if(g_flag==0)
  {
    qwl_net_sendchar(0x20);	
    qwl_net_sendchar(0x0d);	
  }
  else
  {
    qwl_net_sendchar(0x0d);	
    qwl_net_sendchar(0x0A);
    g_delay_us=0;
    g_delay_ms=0;
    while(strsearch_h(">",SystemBuf)==0)
    {
      Watchdog_Feed();
      Delay_2us();
      g_delay_us++;
      if(g_delay_us>500)
      {
        g_delay_us=0;
        g_delay_ms++;
        if(g_delay_ms>3000)
        {
          
          break;
        }
      }
    }
  }
  
  for(j=0;j<len;j++)
  {
    USART3_sendchar(p[j]);//������
    qwl_net_sendchar(p[j]);
  }
  USART3_sendchar(0x0D);
  USART3_sendchar(0x0A);
  qwl_net_sendchar(0x0D);
  qwl_net_sendchar(0x0A);
  
  
  return 0;
} 








