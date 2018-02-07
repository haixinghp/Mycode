#include "config.h"



u8 cdma_or_nblot_flag=0;
u8 send_NSORF_flag=0;

CONNECT_TYPE net={0};

//ProtocolResource _qProtocol = {0};	/*协议缓存*/

//u8 USART3_rec_start_flag=0;//串口3接受开始标志
//u16 USART3_rec_count=0;
//u8 eth_rec_buf[1024];
//u8 eth_rec_buf_1[512];//转换成直接数据
//u16 USART3_rec_count_1=0;//直接数据计数
//u16 eth_rec_CHKSUM=0;
//u8 S1_deal_flag=0;
u8 connect_flag=0;

//u8 light_control_buf[512];//转换成直接数据
//u16 light_control_len=0;

u8 nblot_or_cdma_flag=0;//用于跳转用，识别各个模块
//u8 nblot_NRB[]="AT+NRB";

u16 Rx=0;
u8 SystemBuf[RxIn];  //储存出口接收数据

const u8 ASCII_hex[24]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};

u8 net_error_flag=0;//网络错误标志
u8 deal_process=0;



void USART1_sendchar(u8 ch)
{
  USART_SendData8(USART1,ch);//通过外设USARTx发送单个数据
  
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
  
}

void USART3_sendchar(u8 ch)
{
  USART_SendData8(USART3,ch);//通过外设USARTx发送单个数据
  
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
* 函 数 名         : sendchar()
* 函数功能		   : 串口1发送数据位
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void qwl_net_sendchar(u8 ch)
{
  
  USART1_sendchar(ch);
}

/*******************************************************************************
* 函 数 名         : send字符串()
* 函数功能		   : 串口1发送数据位
* 输    入         : 无
* 输    出         : 无
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
* 函 数 名         : send字符串()
* 函数功能		   : 串口1发送数据位
* 输    入         : 无
* 输    出         : 无
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
* 函 数 名         : net_state
* 函数功能		   	: 
* 输    入         :
* 输    出         :0表示网络正常
*******************************************************************************/
void qwl_net_s_deal(void)
{
  // 			light_heart_time_flag++;
  net.delay++;
  //	nblot_read_flag1_delay++;
}

/*******************************************************************************
* 函 数 名         : net_state
* 函数功能		   	: 
* 输    入         :
* 输    出         :0表示网络正常
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
      send_NSORF_flag=net_error_flag;//用于每次超时发一次
      if(net.mode_type ==MODE_TYPE_BC95)
      {	
        if(net.reconnect_setp==LINK_OK)
        {
          qwl_net_sendstring1("AT+NSORF=0,");//清队列	NBIOT需要									
          qwl_net_sendchar(0x35);
          qwl_net_sendchar(0x31);
          qwl_net_sendchar(0x32);
          qwl_net_sendchar(0x0d);
          qwl_net_sendchar(0x0a);
        }
      }
    }
  }
  if(net.reconnect_setp!=LINK_OK)//在没联网成功时，把设置心跳速上的标志启动，用于马上联网
  {
    
    connect_flag=0;
  }
  switch(net.reconnect_setp)
  {
  case LINK_OK:
    {//net.delay_times=0;
      
      if(net.mode_type ==MODE_TYPE_BC95)
      {	
        
	NBIOT_SET_FALG=0;//成功连接，不需要再切换扰码
        nblot_read();
        
        /*		if(nblot_read_flag1>0)
        {
        u8 k;
        if(nblot_read_flag1_delay>2)//等待超时，服务器问题
        {
        
        nblot_read_flag1_delay=0;//心跳发送成功否，确认标志
        qwl_net_sendstring1("AT+NSORF=0,");//清队列
        qwl_net_sendchar(0x35);
        qwl_net_sendchar(0x31);
        qwl_net_sendchar(0x32);
        qwl_net_sendchar(0x0d);
        qwl_net_sendchar(0x0a);
        
        
        qwl_sendstring("等待服务器超时4\r\n");
        nblot_read_flag1=0;//进行下一个进程处理
        
      }
        
        //	k=nblot_group();
        if(k==0)
        nblot_read_flag1=0;
      }*/
        
        
        
      }
      
      if(connect_flag==0)//联网时必须先发个心跳，防止心跳设置时间太久，永远连接不上
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
//            M6312_reconnect();//移动模块为M6312
//          }
//          break;
        case 3:
          {
            G510_reconnect();//移动模块为G510
          }
          break;
        default:
          {
            cdma_reconnect();//移动模块为MG2618或者电信MC8618
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
//            M6312_open_GPRS();//移动模块为M6312
//          }
//          break;
        case 3:
          {
            G510_open_GPRS();//移动模块为G510
          }
          break;
        default:
          {
            cdma_open_GPRS();//移动模块为MG2618或者电信MC8618
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
//            M6312_init();//移动模块为M6312
//          }
//          break;
        case 3:
          {
            G510_init();//移动模块为G510
          }
          break;
        default:
          {
            cdma_init();//移动模块为MG2618或者电信MC8618
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
      cdma_or_nblot();//选择是NBLOT还是CDMA	
    }
  default:break;
  }
  return net.reconnect_setp;
}


//***************************************************
//发送数据
//*********************************************************
/*******************************************************************************
* 函 数 名         :net_send(u8 *p,u16 len)
* 函数功能		   : 
* 输    入         : 无
* 输    出         : 无
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
//        M6312_send(p,len);//移动模块为M6312
//      }
//      break;
    case 3:
      {
        G510_send(p,len);//移动模块为G510
      }
      break;
    default:
      {
        cdma_send(p,len);//移动模块为MG2618或者电信MC8618
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
* 函 数 名         : net_rec
* 函数功能		   	: 
* 输    入         :
* 输    出         :
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
      nblot_rec(k);//读NB是否有需要读取的数
    
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
* 函 数 名         : qwl_net_init()
* 函数功能		   	: 
* 输    入         :
* 输    出         :
*******************************************************************************/
void qwl_net_init()
{
  CDMA_IO_Init();
  nblot_init_0();
  
  API_SetTimer(1000,-1,TIMER_TYPE_QWL_NET_DEAL,qwl_net_s_deal,NULL);
}

/*******************************************************************************
* 函 数 名         : cdma_or_nblot()
* 函数功能		   : 网络重连
* 输    入         : 无
* 输    出         : 无
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
      net.mode_type =MODE_TYPE_NONE;//?￡?é?′?????°μ?×′ì?
      //qwl_sendstring("AT+CGMM");	
      qwl_net_sendstring("AT+NRB");//?′??	
      IOTMode_RESET();//?′??CDMA
      nblot_or_cdma_flag=0;
      g_flag=0;//gí?±ê????3y
    }break;
  case 1:
    {
      if(strsearch_h("ZTE",SystemBuf)>0)
      { 
        PrintString_("模块是CDMA");
        net.mode_type =MODE_TYPE_ZTE;//ê?CDMA
        
        net.delay_times=0;
        net.reconnect_setp=MODE_INIT;
        net.delay=0;
        net.setp_setp=0;
        
      }
      if(strsearch_h("MC",SystemBuf)>0)
      { 
        PrintString_("模块是CDMA");
        net.mode_type =MODE_TYPE_ZTE;//ê?CDMA
        
        net.delay_times=0;
        net.reconnect_setp=MODE_INIT;
        net.delay=0;
        net.setp_setp=0;
        
      }	
      if(strsearch_h("G510",SystemBuf)>0)
      { 
        PrintString_("模块是移动G510");
        net.mode_type =MODE_TYPE_ZTE;//是CDMA
        g_flag=3;
        net.delay_times=0;
        net.reconnect_setp=MODE_INIT;
        net.delay=0;
        net.setp_setp=0;
//        cdma_or_nblot_setp_init();
      }					
      if(strsearch_h("MG",SystemBuf)>0)
      { 
        PrintString_("模块是G网");
        net.mode_type =MODE_TYPE_ZTE;//ê?CDMA
        g_flag=1;
        net.delay_times=0;
        net.reconnect_setp=MODE_INIT;
        net.delay=0;
        net.setp_setp=0;
      }	
      if(strsearch_h("REBOOT",SystemBuf)>0)
      { 
        PrintString_("模块是NBLOT");
        net.mode_type =MODE_TYPE_BC95;//ê?NBLOT
        /*	if(heart_max_time>170)
        {
        heart_max_time=170;//nb3?ê±3·??ó?í?áá??ó2?é?￡??ùò?×???3·??úá??ó
      }
        */
        net.delay_times=0;
        net.reconnect_setp=MODE_INIT;
        net.delay=0;
        net.setp_setp=0;
      }
      if(net.delay>=4)//μè′y3?ê±￡?·t???÷?êìa
      {
        net.delay=0;
        PrintString_("模块识别超时");
        
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
      qwl_net_sendstring("ATE0");//1?±?????éè??DCE?aOK·?ê?
      delay_ms(1000);
      KickDog();
      */
      //Start_GSM();
      clear_SystemBuf();	
      net.setp_setp=1;
      net.delay=0;
      net.mode_type =MODE_TYPE_NONE;//?￡?é?′?????°μ?×′ì?
      qwl_net_sendstring("AT+CGMM");	
      //qwl_net_sendstring(nblot_NRB);//?′??	
      g_flag=0;//gí?±ê????3y
      nblot_or_cdma_flag=1;
    }break;
    //  switch (net.setp_setp)
    //  {
    //  case 0:
    //    {
    //      clear_SystemBuf();//清除缓存	
    //      net.setp_setp=1;
    //      net.delay=0;
    //      net.mode_type =MODE_TYPE_NONE;//模块未选择前的状态
    //      IOTMode_RESET();//复位模块
    //    }break;
    //  case 1:
    //    {	
    //      if(strsearch_h("+ZIN",SystemBuf)>0)
    //      { 
    //        PrintString_("模块是CDMA");
    //        net.mode_type =MODE_TYPE_ZTE;//是CDMA
    //        
    //        net.delay_times=0;
    //        net.reconnect_setp=MODE_INIT;
    //        net.delay=0;
    //        net.setp_setp=0;
    //        
    //      }	
    //      if(strsearch_h("MG2",SystemBuf)>0)
    //      { 
    //        PrintString_("模块是G网");
    //        net.mode_type =MODE_TYPE_ZTE;//是CDMA
    //        g_flag=1;
    //        net.delay_times=0;
    //        net.reconnect_setp=MODE_INIT;
    //        net.delay=0;
    //        net.setp_setp=0;
    //      }									
    //      if(strsearch_h("REBOOT",SystemBuf)>0)
    //      { 
    //        PrintString_("模块是NBLOT");
    //        net.mode_type =MODE_TYPE_BC95;//是NBLOT
    //        net.delay_times=0;
    //        net.reconnect_setp=MODE_INIT;
    //        net.delay=0;
    //        net.setp_setp=0;
    //      }
    //      if(net.delay>=10)//等待超时，重新复位
    //      {
    //        net.delay=0;
    //        net.setp_setp=0;
    //        PrintString_("识别模块超时");
    //        
    //        net.delay_times++;
    //        if(net.delay_times>2)
    //        {
    //          net.delay_times=0;
    //          PrintString_("切换至9600");
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

