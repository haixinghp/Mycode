#include "config.h"



u8 cdma_or_nblot_flag=0;
u8 send_NSORF_flag=0;

CONNECT_TYPE net={0};

//ProtocolResource _qProtocol = {0};	/*Э�黺��*/

//u8 USART3_rec_start_flag=0;//����3���ܿ�ʼ��־
//u16 USART3_rec_count=0;
//u8 eth_rec_buf[1024];
//u8 eth_rec_buf_1[512];//ת����ֱ������
//u16 USART3_rec_count_1=0;//ֱ�����ݼ���
//u16 eth_rec_CHKSUM=0;
//u8 S1_deal_flag=0;
u8 connect_flag=0;

//u8 light_control_buf[512];//ת����ֱ������
//u16 light_control_len=0;

u8 nblot_or_cdma_flag=0;//������ת�ã�ʶ�����ģ��
//u8 nblot_NRB[]="AT+NRB";

u16 Rx=0;
u8 SystemBuf[RxIn];  //������ڽ�������

const u8 ASCII_hex[24]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};

u8 net_error_flag=0;//��������־
u8 deal_process=0;



void USART1_sendchar(u8 ch)
{
  USART_SendData8(USART1,ch);//ͨ������USARTx���͵�������
  
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
  
}

void USART3_sendchar(u8 ch)
{
  USART_SendData8(USART3,ch);//ͨ������USARTx���͵�������
  
  while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

void USART3_sendstring(u8 *p,u8 len)
{  
  while(len>0)
  {
    qwl_net_sendchar(*p);
    p++;
  }
}
/*******************************************************************************
* �� �� ��         : sendchar()
* ��������		   : ����1��������λ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

void qwl_net_sendchar(u8 ch)
{
  
  USART1_sendchar(ch);
}

/*******************************************************************************
* �� �� ��         : send�ַ���()
* ��������		   : ����1��������λ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void qwl_net_sendstring(u8 *p)
{
  while(*p)
  {
    qwl_net_sendchar(*p);
    p++;
  }
  qwl_net_sendchar(0x0d);
  qwl_net_sendchar(0x0a);
}
/*******************************************************************************
* �� �� ��         : send�ַ���()
* ��������		   : ����1��������λ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void qwl_net_sendstring1(u8 *p)
{
  while(*p)
  {
    qwl_net_sendchar(*p);
    p++;
  }
}

/*******************************************************************************
* �� �� ��         : net_state
* ��������		   	: 
* ��    ��         :
* ��    ��         :0��ʾ��������
*******************************************************************************/
void qwl_net_s_deal(void)
{
  // 			light_heart_time_flag++;
  net.delay++;
  //	nblot_read_flag1_delay++;
}

/*******************************************************************************
* �� �� ��         : net_state
* ��������		   	: 
* ��    ��         :
* ��    ��         :0��ʾ��������
*******************************************************************************/
u8 net_state()
{	
  
  if(net_error_flag==0)
  {
    send_NSORF_flag=0;
  }										
  
  if(net_error_flag>=3)
  {
    net_error_flag=0;
    //deal_process_overtimes=0;
    //deal_process=0;
    net.delay_times=0;
    if(net.mode_type ==MODE_TYPE_ZTE)
      net.reconnect_setp=OPEN_GPRS;
    else
      net.reconnect_setp=LINK_IP;
    net.delay=0;
    net.setp_setp =0;
  }
  else if(net_error_flag>0)
  {
    if(send_NSORF_flag!=net_error_flag)
    {
      send_NSORF_flag=net_error_flag;//����ÿ�γ�ʱ��һ��
      if(net.mode_type ==MODE_TYPE_BC95)
      {	
        if(net.reconnect_setp==LINK_OK)
        {
          qwl_net_sendstring1("AT+NSORF=0,");//�����	NBIOT��Ҫ									
          qwl_net_sendchar(0x35);
          qwl_net_sendchar(0x31);
          qwl_net_sendchar(0x32);
          qwl_net_sendchar(0x0d);
          qwl_net_sendchar(0x0a);
        }
      }
    }
  }
  if(net.reconnect_setp!=LINK_OK)//��û�����ɹ�ʱ���������������ϵı�־������������������
  {
    
    connect_flag=0;
  }
  switch(net.reconnect_setp)
  {
  case LINK_OK:
    {//net.delay_times=0;
      
      if(net.mode_type ==MODE_TYPE_BC95)
      {	
        
	NBIOT_SET_FALG=0;//�ɹ����ӣ�����Ҫ���л�����
        nblot_read();
        
        /*		if(nblot_read_flag1>0)
        {
        u8 k;
        if(nblot_read_flag1_delay>2)//�ȴ���ʱ������������
        {
        
        nblot_read_flag1_delay=0;//�������ͳɹ���ȷ�ϱ�־
        qwl_net_sendstring1("AT+NSORF=0,");//�����
        qwl_net_sendchar(0x35);
        qwl_net_sendchar(0x31);
        qwl_net_sendchar(0x32);
        qwl_net_sendchar(0x0d);
        qwl_net_sendchar(0x0a);
        
        
        qwl_sendstring("�ȴ���������ʱ4\r\n");
        nblot_read_flag1=0;//������һ�����̴���
        
      }
        
        //	k=nblot_group();
        if(k==0)
        nblot_read_flag1=0;
      }*/
        
        
        
      }
      
      if(connect_flag==0)//����ʱ�����ȷ�����������ֹ��������ʱ��̫�ã���Զ���Ӳ���
      {
        connect_flag=1;
        //upload_link_send();
        /*			if(heart_max_time<=3)
        light_heart_time_flag=0;//heart_max_time;
			else
        {
        light_heart_time_flag=heart_max_time-3;
      }*/
      }		
    }break;
  case LINK_IP:
    {
      if(net.mode_type ==MODE_TYPE_ZTE)
      {
        switch(g_flag)
        {
//        case 2:
//          {
//            M6312_reconnect();//�ƶ�ģ��ΪM6312
//          }
//          break;
        case 3:
          {
            G510_reconnect();//�ƶ�ģ��ΪG510
          }
          break;
        default:
          {
            cdma_reconnect();//�ƶ�ģ��ΪMG2618���ߵ���MC8618
          }break;
        }
        
      }
      else
        nblot_reconnect();
//      if(net.mode_type ==MODE_TYPE_ZTE)
//        cdma_reconnect();
//      else
//        nblot_reconnect();
      
    }break;
  case OPEN_GPRS:
    {
//      if(net.mode_type ==MODE_TYPE_ZTE)
//        cdma_open_GPRS();
//      else
//      {
//        nblot_open_GPRS();
//      }
      if(net.mode_type ==MODE_TYPE_ZTE)
      {
        
        switch(g_flag)
        {
//        case 2:
//          {
//            M6312_open_GPRS();//�ƶ�ģ��ΪM6312
//          }
//          break;
        case 3:
          {
            G510_open_GPRS();//�ƶ�ģ��ΪG510
          }
          break;
        default:
          {
            cdma_open_GPRS();//�ƶ�ģ��ΪMG2618���ߵ���MC8618
          }break;
        }
        
      }
      else
      {
        nblot_open_GPRS();
      }
    }break;
  case MODE_INIT:
    {
      
//      if(net.mode_type ==MODE_TYPE_ZTE)
//        cdma_init();
//      else
//        nblot_init();
      if(net.mode_type ==MODE_TYPE_ZTE)
      {
        switch(g_flag)
        {
//        case 2:
//          {
//            M6312_init();//�ƶ�ģ��ΪM6312
//          }
//          break;
        case 3:
          {
            G510_init();//�ƶ�ģ��ΪG510
          }
          break;
        default:
          {
            cdma_init();//�ƶ�ģ��ΪMG2618���ߵ���MC8618
          }break;
        }
        
      }
      else
      {
        nblot_init();
      } 
    }break;
  case MODE_CHOICE:
    {
      cdma_or_nblot();//ѡ����NBLOT����CDMA	
    }
  default:break;
  }
  return net.reconnect_setp;
}


//***************************************************
//��������
//*********************************************************
/*******************************************************************************
* �� �� ��         :net_send(u8 *p,u16 len)
* ��������		   : 
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

u8 net_send(u8 *p,u16 len)
{
  
//  if(net.mode_type ==MODE_TYPE_ZTE)
//  {
//    cdma_send(p,len);
//  }
//  else
//  {
//    nblot_send(p,len);
//    
//  }
    if(net.mode_type ==MODE_TYPE_ZTE)
  {
    
    switch(g_flag)
    {
//    case 2:
//      {
//        M6312_send(p,len);//�ƶ�ģ��ΪM6312
//      }
//      break;
    case 3:
      {
        G510_send(p,len);//�ƶ�ģ��ΪG510
      }
      break;
    default:
      {
        cdma_send(p,len);//�ƶ�ģ��ΪMG2618���ߵ���MC8618
      }break;
    }
    
    
  }
  else
  {
    nblot_send(p,len);
    
  }
  return 0;
} 




/*******************************************************************************
* �� �� ��         : net_rec
* ��������		   	: 
* ��    ��         :
* ��    ��         :
*******************************************************************************/
void net_rec(u8 k)
{
  //USART3_sendchar(k);
  if(Rx<RxIn)
  {
    SystemBuf[Rx]=k;
    Rx++;
    
  }
  
  if(net.mode_type ==MODE_TYPE_BC95)
  {
    if(net.reconnect_setp ==LINK_OK)
      nblot_rec(k);//��NB�Ƿ�����Ҫ��ȡ����
    
  }
  /*
  if(nblot_rec_times<500)
  {
  nblot_rec_buf[nblot_rec_times]=k;
  nblot_rec_times++;
}
  if(net.mode_type ==MODE_TYPE_BC95)
  {nblot_rec(k);
  
}
  else
  {
  //				if(qwl_peizhi==0)
  //				{
  eth_rec_deal(k);
  //				}
}
  */
  
  
}


/*******************************************************************************
* �� �� ��         : qwl_net_init()
* ��������		   	: 
* ��    ��         :
* ��    ��         :
*******************************************************************************/
void qwl_net_init()
{
  CDMA_IO_Init();
  nblot_init_0();
  
  API_SetTimer(1000,-1,TIMER_TYPE_QWL_NET_DEAL,qwl_net_s_deal,NULL);
}

/*******************************************************************************
* �� �� ��         : cdma_or_nblot()
* ��������		   : ��������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void cdma_or_nblot()
{
  switch (net.setp_setp)
  {
  case 0:
    {
      clear_SystemBuf();	
      net.setp_setp=1;
      net.delay=0;
      net.mode_type =MODE_TYPE_NONE;//?��?��?��?????���?���䨬?
      //qwl_sendstring("AT+CGMM");	
      qwl_net_sendstring("AT+NRB");//?��??	
      IOTMode_RESET();//?��??CDMA
      nblot_or_cdma_flag=0;
      g_flag=0;//g��?����????3y
    }break;
  case 1:
    {
      if(strsearch_h("ZTE",SystemBuf)>0)
      { 
        PrintString_("ģ����CDMA");
        net.mode_type =MODE_TYPE_ZTE;//��?CDMA
        
        net.delay_times=0;
        net.reconnect_setp=MODE_INIT;
        net.delay=0;
        net.setp_setp=0;
        
      }
      if(strsearch_h("MC",SystemBuf)>0)
      { 
        PrintString_("ģ����CDMA");
        net.mode_type =MODE_TYPE_ZTE;//��?CDMA
        
        net.delay_times=0;
        net.reconnect_setp=MODE_INIT;
        net.delay=0;
        net.setp_setp=0;
        
      }	
      if(strsearch_h("G510",SystemBuf)>0)
      { 
        PrintString_("ģ�����ƶ�G510");
        net.mode_type =MODE_TYPE_ZTE;//��CDMA
        g_flag=3;
        net.delay_times=0;
        net.reconnect_setp=MODE_INIT;
        net.delay=0;
        net.setp_setp=0;
//        cdma_or_nblot_setp_init();
      }					
      if(strsearch_h("MG",SystemBuf)>0)
      { 
        PrintString_("ģ����G��");
        net.mode_type =MODE_TYPE_ZTE;//��?CDMA
        g_flag=1;
        net.delay_times=0;
        net.reconnect_setp=MODE_INIT;
        net.delay=0;
        net.setp_setp=0;
      }	
      if(strsearch_h("REBOOT",SystemBuf)>0)
      { 
        PrintString_("ģ����NBLOT");
        net.mode_type =MODE_TYPE_BC95;//��?NBLOT
        /*	if(heart_max_time>170)
        {
        heart_max_time=170;//nb3?����3��??��?��?����??��2?��?��??����?��???3��??����??��
      }
        */
        net.delay_times=0;
        net.reconnect_setp=MODE_INIT;
        net.delay=0;
        net.setp_setp=0;
      }
      if(net.delay>=4)//�̨���y3?������?��t???��?����a
      {
        net.delay=0;
        PrintString_("ģ��ʶ��ʱ");
        
        if(nblot_or_cdma_flag==0)
          net.setp_setp=0;
        else
        {
          net.setp_setp=2;
        }
        
        net.delay_times++;
        if(net.delay_times>1)
        {
          net.delay_times=0;
          if(nblot_or_cdma_flag==0)
            net.setp_setp=2;
          else
          {
            PrintString_("AT+IPR=9600");
            uart1_init(115200);
            Delay_16M_MS(500);
            
            qwl_net_sendstring("AT+IPR=9600");
            Delay_16M_MS(500);
            
            uart1_init(9600);
            
            Delay_16M_MS(500);
            net.setp_setp=0;
          }
        }
      }
      
    }break;
  case 2:
    {
      /*				qwl_sendstring("?a??CDMA");
      Start_GSM();
      KickDog();
      delay_ms(1000);
      KickDog();
      qwl_net_sendstring("ATE0");//1?��?????����??DCE?aOK��?��?
      delay_ms(1000);
      KickDog();
      */
      //Start_GSM();
      clear_SystemBuf();	
      net.setp_setp=1;
      net.delay=0;
      net.mode_type =MODE_TYPE_NONE;//?��?��?��?????���?���䨬?
      qwl_net_sendstring("AT+CGMM");	
      //qwl_net_sendstring(nblot_NRB);//?��??	
      g_flag=0;//g��?����????3y
      nblot_or_cdma_flag=1;
    }break;
    //  switch (net.setp_setp)
    //  {
    //  case 0:
    //    {
    //      clear_SystemBuf();//�������	
    //      net.setp_setp=1;
    //      net.delay=0;
    //      net.mode_type =MODE_TYPE_NONE;//ģ��δѡ��ǰ��״̬
    //      IOTMode_RESET();//��λģ��
    //    }break;
    //  case 1:
    //    {	
    //      if(strsearch_h("+ZIN",SystemBuf)>0)
    //      { 
    //        PrintString_("ģ����CDMA");
    //        net.mode_type =MODE_TYPE_ZTE;//��CDMA
    //        
    //        net.delay_times=0;
    //        net.reconnect_setp=MODE_INIT;
    //        net.delay=0;
    //        net.setp_setp=0;
    //        
    //      }	
    //      if(strsearch_h("MG2",SystemBuf)>0)
    //      { 
    //        PrintString_("ģ����G��");
    //        net.mode_type =MODE_TYPE_ZTE;//��CDMA
    //        g_flag=1;
    //        net.delay_times=0;
    //        net.reconnect_setp=MODE_INIT;
    //        net.delay=0;
    //        net.setp_setp=0;
    //      }									
    //      if(strsearch_h("REBOOT",SystemBuf)>0)
    //      { 
    //        PrintString_("ģ����NBLOT");
    //        net.mode_type =MODE_TYPE_BC95;//��NBLOT
    //        net.delay_times=0;
    //        net.reconnect_setp=MODE_INIT;
    //        net.delay=0;
    //        net.setp_setp=0;
    //      }
    //      if(net.delay>=10)//�ȴ���ʱ�����¸�λ
    //      {
    //        net.delay=0;
    //        net.setp_setp=0;
    //        PrintString_("ʶ��ģ�鳬ʱ");
    //        
    //        net.delay_times++;
    //        if(net.delay_times>2)
    //        {
    //          net.delay_times=0;
    //          PrintString_("�л���9600");
    //          uart1_init(115200);
    //          Delay_MS(200);
    //          
    //          qwl_net_sendstring("AT+IPR=9600");
    //          Delay_MS(500);
    //          
    //          uart1_init(9600);
    //          Delay_MS(200);
    //          
    //          qwl_net_sendstring("AT");
    //          Delay_MS(200);          
    //          qwl_net_sendstring("AT");
    //          Delay_MS(200);
    //        }
    //      }
    //    }break;
  default:break;
  }
  
}

