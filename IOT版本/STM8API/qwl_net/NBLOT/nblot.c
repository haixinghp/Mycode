#include "config.h"
u8 NB_ECL[4]={0};//信号覆盖等级
u8 NB_SNR[6]={0};//信噪比
u8 NB_Signal_power[5]={0};//信号强度
//u8 Ocean_ip[22]={"117.60.157.137,5683"};//华为iot平台ip
u8 Ocean_ip[22]={"180.101.147.115,5683"};//华为iot平台ip
//u8 nblot_CFUN[]="AT+CFUN=1";
//u8 nblot_NSOCL[]="AT+NSOCL=0"; 
//u8 nblot_CEREG[]="AT+CEREG=1";
//u8 nblot_CGDCONT[]="AT+CGDCONT=1,\"IP\",\"CTNB\"";
//u8 nblot_COPS[]="AT+COPS=1, 2,\"46011\"";
//u8 nblot_CGATT[]="AT+CGATT? ";
//u8 nblot_NSOST[]="AT+NSOST=0,";
//u8 nblot_NSOCR[]="AT+NSOCR= DGRAM,17,56852,1";
//u8 nblot_NSORF[]="AT+NSORF=0,";


//u8 nblot_NBAND[]="AT+NBAND?";
//u8 nblot_NBAND1[]="AT+NBAND=5";
//u8 nblot_NCONFIG[]="AT+NCONFIG?";

//u8 nblot_AUTOCONNECT[]="AT+NCONFIG=AUTOCONNECT,TRUE";

//u8 nblot_SCRAMBL_true[]="AT+NCONFIG=CR_0354_0338_SCRAMBLING,TRUE";
//u8 nblot_SCRAMBL_false[]="AT+NCONFIG=CR_0354_0338_SCRAMBLING,FALSE";
//u8 nblot_AVOID_true[]="AT+NCONFIG=CR_0859_SI_AVOID,TRUE";
//u8 nblot_AVOID_false[]="AT+NCONFIG=CR_0859_SI_AVOID,FALSE";
u8 nblot_rec_flag=0;
u8 nblot_read_flag=0;//nb读到需要接受的数据，需要主动读


u8 nblot_read_flag1=0;
u8 nblot_read_flag1_delay=0;

//u8 nblot_rec_buf[500];
//u16 nblot_rec_times=0;

u8 nblot_RESET_flag=0;


u8 nblot_overtime_flag=0;




//u8 light_UDP[22]={"39.108.148.248,26004"};;
u8 light_UDP[22]={"120.77.205.98,2555"};
u8 NBIOT_SET_FALG=0;

u8 NB_SET_FLAG=0;

u8 nblot_NCDP[]="AT+NCDP=";//连接华为IP
u8 nblot_NNMI[]="AT+NNMI=1";//开启下行数据通知功能
//u8 nblot_NNMI[]="AT";//开启下行数据通知功能
u8 nblot_NMGS[]="AT+NMGS=";//发送到华为ocean IOT

/*********************************************************************
** 函数名称: strsearch_hq ()
** 函数功能: 在指定的数组里连续找到相同的内容
** 入口参数: ptr2要查找的内容, ptr1当前数组
** 出口参数: 0-没有 找到   >1 查找到
*********************************************************************/
/*u16 strsearch_hq(u8 *ptr2,u8 *ptr1_at)//查字符串*ptr2在*ptr1中的位置
//本函数是用来检查字符串*ptr2是否完全包含在*ptr1中
//返回:  0  没有找到
//65535 从第N个字符开始相同
{
u16 i,j,k;


if(nblot_rec_times==0) return(0);

for(i=0,j=0;i<nblot_rec_times;i++)
{
if(ptr1_at[i]==ptr2[j])
{	//第一个字符相同
for(k=i;k<Rx+1;k++,j++)
{

if(ptr2[j]==0)//比较正确
{                return(k);               //返回值是整数，不含0
                                }
if(ptr1_at[k]!=ptr2[j]) break;

        		}
j=0;
        	}
	}
return(0);
}
*/
//*******************************************************
//TP链接
//******************************************************
void nblot_TP_GPRS(u8 *p,u8 *q)
{//u8 ip_init_word[]={"121.42.196.113,28070"};//112.74.98.39,8647出厂默认IP
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
}

//发送数据
//*********************************************************
///*******************************************************************************
//* 函 数 名         :qwl_net_sendstring1(u8 *p,u16 len)
//* 函数功能		   : 串口3发送数据位
//* 输    入         : 无
//* 输    出         : 无
//********************************************************************************/
//
//u8 nblot_send(u8 *p,u16 len)
//{	
//  u16 j=0;
//  clear_SystemBuf();
//  
//  if(Net_Data_Send.Link_Type == UPLOAD)//切换到升级地址
//    nblot_TP_GPRS("AT+NSOST=0,",Sys_keydata.update_server_add);
//  else
//    nblot_TP_GPRS("AT+NSOST=0,",Sys_keydata.default_cloud_server_add);
//  qwl_net_sendchar(0x2c);
//  USART3_sendchar(0x2c);
//  
//  ///	sprintf(i, "%d", (u8)len);
//  //	for(k=0;k<strlen(i);k++)
//  j=len;
//  if(len>99)
//  {	j=len/100;
//  qwl_net_sendchar(NO[j]);
//  USART3_sendchar(NO[j]);
//  j=len%100;
//  }
//  
//  qwl_net_sendchar(NO[j/10]);
//  USART3_sendchar(NO[j/10]);
//  qwl_net_sendchar(NO[j%10]);
//  USART3_sendchar(NO[j%10]);
//  
//  
//  
//  qwl_net_sendchar(0x2c);
//  USART3_sendchar(0x2c);
//  
//  for(j=0;j<len;j++)
//  {
//    USART3_sendchar(NO[p[j]>>4]);//测试用
//    USART3_sendchar(NO[p[j]&0x0f]);//测试用
//    //qwl_sendchar(p[j]);
//    qwl_net_sendchar(NO[p[j]>>4]);
//    qwl_net_sendchar(NO[p[j]&0x0f]);
//  }
//  
//  
//  USART3_sendchar(0x0D);
//  USART3_sendchar(0x0A);
//  qwl_net_sendchar(0x0D);
//  qwl_net_sendchar(0x0A);
//  return 1;
//} 

/*******************************************************************************
* 函 数 名         :nblot_send2(u8 *p,u16 len)
* 函数功能		   : 串口3发送数据位
* 输    入         : 无
* 输    出         : 无
********************************************************************************/
u8 nblot_send(u8 *p,u16 len)
{	
  u16 j=0;
  
  
  clear_SystemBuf();
  qwl_net_sendstring1(nblot_NMGS);
  j=len+14;
  if(len>1000)
  {
    qwl_net_sendchar(NO[j/1000]);
    USART3_sendchar(NO[j/1000]);
    j=j%1000;
  }
  if(len>100)
  {
    qwl_net_sendchar(NO[j/100]);
    USART3_sendchar(NO[j/100]);
    j=j%100;
  }
  
  qwl_net_sendchar(NO[j/10]);
  qwl_net_sendchar(NO[j%10]);
  USART3_sendchar(NO[j/10]);
  USART3_sendchar(NO[j%10]);
  qwl_net_sendstring1(",");
  
  for(j=0;j<4;j++)
  {
    USART3_sendchar(NO[NB_Signal_power[j]>>4]);//测试用
    USART3_sendchar(NO[NB_Signal_power[j]&0x0f]);//测试用
    //USART3_sendchar(p[j]);
    qwl_net_sendchar(NO[NB_Signal_power[j]>>4]);
    qwl_net_sendchar(NO[NB_Signal_power[j]&0x0f]);
  }
  for(j=0;j<4;j++)
  {
    USART3_sendchar(NO[NB_ECL[j]>>4]);//测试用
    USART3_sendchar(NO[NB_ECL[j]&0x0f]);//测试用
    //USART3_sendchar(p[j]);
    qwl_net_sendchar(NO[NB_ECL[j]>>4]);
    qwl_net_sendchar(NO[NB_ECL[j]&0x0f]);
  }
  for(j=0;j<6;j++)
  {
    USART3_sendchar(NO[NB_SNR[j]>>4]);//测试用
    USART3_sendchar(NO[NB_SNR[j]&0x0f]);//测试用
    //USART3_sendchar(p[j]);
    qwl_net_sendchar(NO[NB_SNR[j]>>4]);
    qwl_net_sendchar(NO[NB_SNR[j]&0x0f]);
  }
  for(j=0;j<len;j++)
  {
    USART3_sendchar(NO[p[j]>>4]);//测试用
    USART3_sendchar(NO[p[j]&0x0f]);//测试用
    //USART3_sendchar(p[j]);
    qwl_net_sendchar(NO[p[j]>>4]);
    qwl_net_sendchar(NO[p[j]&0x0f]);
  }
  
  
  USART3_sendchar(0x0D);
  USART3_sendchar(0x0A);
  qwl_net_sendchar(0x0D);
  qwl_net_sendchar(0x0A);
  return 1;
} 


/*******************************************************************************
* 函 数 名         : cdma_init()
* 函数功能		   : cdma初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void nblot_init()
{
  
  switch (net.setp_setp)
  {
    
  case 0:
    {
      PrintString_("查询频段");
      clear_SystemBuf();
      qwl_net_sendstring("AT+NBAND?");//复位
      net.delay=0;
      net.setp_setp=1;
    }break;
  case 1:
    {
      u8 addr=0;
      if(net.delay>2)//等待超时，服务器问题
      {net.delay=0;
      PrintString_("查询频段超时");
      net.setp_setp=0;
      net.delay=0;
      net.delay_times++;
      if(net.delay_times>5)
      {
        net.delay_times=0;
        net.setp_setp=0;
        net.reconnect_setp=MODE_CHOICE;//进入网络重新连接
        net.delay=0;
      }
      }	
      if(strsearch_h("OK",SystemBuf)>0){
        DelayT_ms(50);
        addr=strsearch_h("+NBAND:",SystemBuf);
        if(addr>0){
          if(SystemBuf[addr]=='5'){
            PrintString_("频段为5");
            NB_SET_FLAG=0;
          }else if(SystemBuf[addr]=='8'){
            PrintString_("频段为8");
            NB_SET_FLAG=0;
          }else{
            PrintString_("频段错误");
            NB_SET_FLAG=1;
          }
          net.delay_times=0;
          net.setp_setp=2;
          net.reconnect_setp=MODE_INIT;//进入网络重新连接
          net.delay=0;
        }
      }
      
    }break;
  case 2:
    {PrintString_("查询配置");
    clear_SystemBuf();
    qwl_net_sendstring("AT+NCONFIG?");//复位
    net.delay=0;
    net.setp_setp=3;
    }break;
  case 3:
    {
      u8 addr=0;
      if(net.delay>2)//等待超时，服务器问题
      {net.delay=0;
      PrintString_("查询配置超时");
      net.setp_setp=2;
      net.delay=0;
      net.delay_times++;
      if(net.delay_times>5)
      {
        net.delay_times=0;
        net.setp_setp=0;
        net.reconnect_setp=MODE_CHOICE;//进入网络重新连接
        net.delay=0;
      }	
      
      }
      addr=strsearch_h("AVOID,FALSE",SystemBuf);
      if(addr==0)
        addr=strsearch_h("AVOID,TRUE",SystemBuf);
      if(addr)
      {
        NB_SET_FLAG=0;
        addr=strsearch_h("AUTOCONNECT,FALSE",SystemBuf);
        if(addr>0)
        { NB_SET_FLAG=NB_SET_FLAG|0x02;
        PrintString_("自动连接FALSE");
        }
        
        addr=strsearch_h("SCRAMBLING,FALSE",SystemBuf);
        if(addr>0)
        { NB_SET_FLAG=NB_SET_FLAG|0x04;
        PrintString_("SCRAMB连接FALSE");
        }
        
        addr=strsearch_h("AVOID,FALSE",SystemBuf);
        if(addr>0)
        { NB_SET_FLAG=NB_SET_FLAG|0x08;
        PrintString_("AVOID连接FALSE");
        }
        
        if(NB_SET_FLAG&0x01)
        {
          net.delay_times=0;
          net.setp_setp=4;
          net.reconnect_setp=MODE_INIT;//进入网络重新连接
          net.delay=0;
        }
        else if(NB_SET_FLAG&0x02)
        {
          net.delay_times=0;
          net.setp_setp=6;
          net.reconnect_setp=MODE_INIT;//进入网络重新连接
          net.delay=0;
        }
        else if(1)
        {
          
          if(NBIOT_SET_FALG>3)//3次连接不上，就进行模式切换
          {
            
            net.delay_times=0;
            net.setp_setp=8;//0;//net.setp_setp=8;//饶玛自动识别关闭
            net.reconnect_setp=MODE_INIT;//2;//net_reconnection_flag=3;//进入网络重新连接
            net.delay=0;											
          }
          else
          {
            NBIOT_SET_FALG++;
            net.delay_times=0;
            net.setp_setp=0;
            net.reconnect_setp=OPEN_GPRS;//进入网络重新连接
            net.delay=0;		
          }
        }
      }
      
    }break;		
    
    
  case 4:
    {PrintString_("设置第五频段");
    clear_SystemBuf();
    qwl_net_sendstring("AT+NBAND=5");//设置频段为5
    net.delay=0;
    net.setp_setp=5;
    }break;
  case 5:
    {
      u8 addr=0;
      if(net.delay>2)//等待超时，服务器问题
      {net.delay=0;
      PrintString_("设置频段超时");
      net.setp_setp=4;
      net.delay=0;
      net.delay_times++;
      if(net.delay_times>5)
      {
        net.delay_times=0;
        net.setp_setp=0;
        net.reconnect_setp=MODE_CHOICE;//进入网络重新连接
        net.delay=0;
      }
      }
      
      addr=strsearch_h("OK",SystemBuf);
      if(addr>0)
      { 
        PrintString_("设置频段成功");
        if(NB_SET_FLAG&0x02)
        {
          net.delay_times=0;
          net.setp_setp=6;
          net.reconnect_setp=MODE_INIT;//进入网络重新连接
          net.delay=0;		
        }
        else if(NBIOT_SET_FALG>3)
        {
          net.delay_times=0;
          net.setp_setp=8;
          net.reconnect_setp=MODE_INIT;//进入网络重新连接
          net.delay=0;	
        }
        else if(1)
        {
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=MODE_CHOICE;//进入网络重新连接，设置完频段必须重启
          net.delay=0;	
        }
        
      }
      
    }break;
    
  case 6:
    {PrintString_("设置自动联网模式");
    clear_SystemBuf();
    qwl_net_sendstring("AT+NCONFIG=AUTOCONNECT,TRUE");//设置频段为5
    net.delay=0;
    net.setp_setp=7;
    }break;
  case 7:
    {
      u8 addr=0;
      if(net.delay>2)//等待超时，服务器问题
      {net.delay=0;
      PrintString_("设置自动联网超时");
      net.setp_setp=6;
      net.delay=0;
      net.delay_times++;
      if(net.delay_times>5)
      {
        net.delay_times=0;
        net.setp_setp=0;
        net.reconnect_setp=MODE_CHOICE;//进入网络重新连接
        net.delay=0;
      }
      }
      
      addr=strsearch_h("OK",SystemBuf);
      if(addr>0)
      { 
        PrintString_("设置自动联网成功");
        if(NBIOT_SET_FALG>3)
        {
          net.delay_times=0;
          net.setp_setp=0;
//          net.setp_setp=8;
          net.reconnect_setp=MODE_INIT;//进入网络重新连接
          net.delay=0;	
        }
        else if(1)
        {
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=MODE_CHOICE;//进入网络重新连接
          net.delay=0;	
        }
        
      }
      
    }break;	
    
//  case 8:
//    {
//      if((NB_SET_FLAG&0x0c)>0)
//        PrintString_("设置SCRAMBL=true");
//      else
//        PrintString_("设置SCRAMBL=false");	
//      clear_SystemBuf();
//      if((NB_SET_FLAG&0x0c)>0)
//        qwl_net_sendstring("AT+NCONFIG=CR_0354_0338_SCRAMBLING,TRUE");//设置为true
//      else
//        qwl_net_sendstring("AT+NCONFIG=CR_0354_0338_SCRAMBLING,FALSE");//设置为false
//      net.delay=0;
//      net.setp_setp=9;
//    }break;
//  case 9:
//    {
//      u8 addr=0;
//      if(net.delay>2)//等待超时，服务器问题
//      {net.delay=0;
//      PrintString_("设置SCRAMBL超时");
//      net.setp_setp=8;
//      net.delay=0;
//      net.delay_times++;
//      if(net.delay_times>5)
//      {
//        net.delay_times=0;
//        net.setp_setp=0;
//        net.reconnect_setp=MODE_CHOICE;//进入网络重新连接
//        net.delay=0;
//      }
//      }
//      
//      addr=strsearch_h("OK",SystemBuf);
//      if(addr>0)
//      { 
//        PrintString_("设置SCRAMBL成功");
//        net.delay_times=0;
//        net.setp_setp=10;
//        net.reconnect_setp=MODE_INIT;//进入网络重新连接
//        net.delay=0;	
//      }
//      
//    }break;	
//    
//    
//  case 10:
//    {
//      if(NB_SET_FLAG&0x0c)
//        PrintString_("设置AVOID=true");
//      else
//        PrintString_("设置AVOID=false");	
//      clear_SystemBuf();
//      if(NB_SET_FLAG&0x0c)
//        qwl_net_sendstring("AT+NCONFIG=CR_0859_SI_AVOID,TRUE");//设置为true
//      else
//        qwl_net_sendstring("AT+NCONFIG=CR_0859_SI_AVOID,FALSE");//设置为false
//      net.delay=0;
//      net.setp_setp=11;
//    }break;
//  case 11:
//    {
//      u8 addr=0;
//      if(net.delay>2)//等待超时，服务器问题
//      {net.delay=0;
//      PrintString_("设置AVOID超时");
//      net.setp_setp=10;
//      net.delay=0;
//      net.delay_times++;
//      if(net.delay_times>5)
//      {
//        net.delay_times=0;
//        net.setp_setp=0;
//        net.reconnect_setp=MODE_CHOICE;//进入网络重新连接
//        net.delay=0;
//      }
//      }
//      
//      addr=strsearch_h("OK",SystemBuf);
//      if(addr>0)
//      { 
//        NBIOT_SET_FALG=0;
//        PrintString_("设置AVOID成功");
//        net.delay_times=0;
//        net.setp_setp=0;
//        net.reconnect_setp=MODE_CHOICE;//进入网络重新连接,设置完需要重启
//        net.delay=0;	
//      }
//      
//    }break;	
  default:break;
  }
  
  
  
  
}

/*******************************************************************************
* 函 数 名         : void light_eth_reconnect()
* 函数功能		   : cdma初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
/*
void nblot_reconnect()
{

switch (net.setp_setp)
{

  case 0:
{
clear_SystemBuf();			
qwl_net_sendstring("AT+NSOCR= DGRAM,17,56852,1");
net.setp_setp=1;
net.delay=0;
PrintString_("创建包");
nblot_RESET_flag++;
if(nblot_RESET_flag>3)
{
net.delay_times=0;
net.setp_setp=0;
net.reconnect_setp=MODE_CHOICE;//进入CDMA重启
net.delay=0;
nblot_RESET_flag=0;
PrintString_("进入NBLOT复位");
      }
    }break;
  case 1:
{
if(net.delay>=6)//等待超时，服务器问题
{net.delay=0;
net.setp_setp=0;//等待失败
net.reconnect_setp=LINK_IP;


PrintString_("创建包超时");
net.delay_times++;
if(net.delay_times>7)
{
net.delay_times=0;
net.setp_setp=0;
net.reconnect_setp=OPEN_GPRS;//进入CDMA重启
net.delay=0;
PrintString_("进入NBLOT重启");
      }
      }


if(strsearch_h("OK",SystemBuf)>0)
{
net.reconnect_setp=LINK_OK;//打开完毕，进入联网
net.setp_setp=0;
net.delay_times=0;
net.delay=0;
deal_process=0;
PrintString_("创建包成功");
      }


    }break;	

default:break;
  }
}
*/
/*******************************************************************************
* 函 数 名         : void light_eth_reconnect2()
* 函数功能		   : cdma初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void nblot_reconnect()
{
  switch (net.setp_setp)
  {
    
  case 0:
    {
      clear_SystemBuf();			
//      TP_GPRS(nblot_NCDP,Ocean_ip);//连接IP
      nblot_TP_GPRS(nblot_NCDP,Ocean_ip);//连接IP
      USART3_sendchar(0x0D);
      USART3_sendchar(0x0A);
      qwl_net_sendchar(0x0D);
      qwl_net_sendchar(0x0A);
      net.setp_setp=1;
      net.delay=0;
      PrintString_("连接IP");
      nblot_RESET_flag++;
      if(nblot_RESET_flag>3)
      {
        net.delay_times=0;
        net.setp_setp=0;
        net.reconnect_setp=MODE_CHOICE;//进入CDMA重启
        net.delay=0;
        nblot_RESET_flag=0;
        PrintString_("进入NBLOT复位");
      }
    }break;
  case 1:
    {
      if(net.delay>=3)//等待超时，服务器问题
      {
        net.delay=0;
        net.setp_setp=0;//等待失败
        //net.reconnect_setp=LINK_IP;
        PrintString_("连接IP超时");
        net.delay_times++;
        if(net.delay_times>3)
        {
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=OPEN_GPRS;//重新附着
          net.delay=0;
          PrintString_("重新附着");
        }
      }
      
      
      if(strsearch_h("OK",SystemBuf)>0)
      {
        net.setp_setp=2;
        net.delay_times=0;
        net.delay=0;
        deal_process=0;
        PrintString_("连接IP成功");
      }
      
      
    }break;	
  case 2:
    {
      clear_SystemBuf();			
      qwl_net_sendstring(nblot_NNMI);
      net.setp_setp=3;
      net.delay=0;
      PrintString_("下行数据通知");
      
    }break;
  case 3:
    {
      if(net.delay>=3)//等待超时，服务器问题
      {net.delay=0;
      net.setp_setp=2;//等待失败
      //net.reconnect_setp=LINK_IP;
      
      
      PrintString_("开启下行数据通知超时");
      net.delay_times++;
      if(net.delay_times>3)
      {
        net.delay_times=0;
        net.setp_setp=0;
        net.reconnect_setp=OPEN_GPRS;//重新附着
        net.delay=0;
        PrintString_("重新附着");
      }
      }
      
      
      if(strsearch_h("OK",SystemBuf)>0)
      {//net.reconnect_setp=LINK_OK;//打开完毕，进入联网
        net.setp_setp=4;
        net.delay_times=0;
        net.delay=0;
        deal_process=0;
        PrintString_("连接IP成功");
      }
      
      
    }break;	
    
  case 4:
    {
      clear_SystemBuf();			
      qwl_net_sendstring("AT+NUESTATS");
      net.setp_setp=5;
      net.delay=0;
      PrintString_("读信号强度");
    }break;
  case 5:
    {
      u16 j;
      u16 k;
      u8 i;
      if(net.delay>3)//等待超时，服务器问题
      {net.delay=0;
      net.setp_setp=4;//等待失败
      
      PrintString_("读信号强度超时");
      net.delay_times++;
      if(net.delay_times>3)
      {
        net.reconnect_setp=LINK_OK;//打开完毕，进入联网
        net.setp_setp=0;
        net.delay_times=0;
        net.delay=0;
        deal_process=0;
      }
      }
      
      
      j=strsearch_h("RSRQ:",SystemBuf);
      if(j>0)
      {
        
        k=strsearch_h("Signal power:",SystemBuf);
        if(k)
        {
          PrintString_("信号强度：");
          for(i=0;i<5;i++)
          {
            NB_Signal_power[i]=0;
          }
          if(SystemBuf[k]>=0x30)
          {
            NB_Signal_power[0]=0x2b;
          }
          else
          {
            NB_Signal_power[0]=0x2d;
            k++;
          }
          USART3_sendchar(NB_Signal_power[0]);
          for(i=0;i<4;i++)
          {
            if(SystemBuf[k+i]==0x0d)
              break;
            
            NB_Signal_power[i+1]=SystemBuf[k+i];
            USART3_sendchar(NB_Signal_power[i+1]);
          }
          NB_Signal_power[i]=0;
          USART3_sendchar(0x0d);
          USART3_sendchar(0x0a);
        }
        
        k=strsearch_h("ECL:",SystemBuf);
        if(k)
        {PrintString_("信号覆盖等级：");
        for(i=0;i<4;i++)
        {
          NB_ECL[i]=0;
        }
        if(SystemBuf[k]>=0x30)
        {
          NB_ECL[0]=0x2b;
          
        }
        else
        {
          NB_ECL[0]=0x2d;
          k++;
        }
        
        USART3_sendchar(NB_ECL[0]);
        for(i=0;i<3;i++)
        {
          if(SystemBuf[k+i]==0x0d)
            break;
          
          NB_ECL[i+1]=SystemBuf[k+i];
          USART3_sendchar(NB_ECL[i+1]);
        }
        }
        USART3_sendchar(0x0d);
        USART3_sendchar(0x0a);
        
        k=strsearch_h("SNR:",SystemBuf);
        if(k)
        {
          PrintString_("信噪比：");
          for(i=0;i<6;i++)
          {
            NB_SNR[i]=0;
          }
          if(SystemBuf[k]>=0x30)
          {
            NB_SNR[0]=0x2b;
          }
          else
          {
            NB_SNR[0]=0x2d;
            k++;
          }
          USART3_sendchar(NB_SNR[0]);
          for(i=0;i<5;i++)
          {
            if(SystemBuf[k+i]==0x0d)
              break;
            
            NB_SNR[i+1]=SystemBuf[k+i];
            USART3_sendchar(NB_SNR[i+1]);
          }
          NB_SNR[i]=0;
          USART3_sendchar(0x0d);
          USART3_sendchar(0x0a);
        }
        
        
        
        net.reconnect_setp=LINK_OK;//打开完毕，进入联网
        net.setp_setp=0;
        net.delay_times=0;
        net.delay=0;
        deal_process=0;
      }
      
      
    }break;
  default:break;
  }
}
/*******************************************************************************
* 函 数 名         : void cdma_open_GPRS()
* 函数功能		   : 网络重连
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void nblot_open_GPRS()
{
  switch (net.setp_setp)
  {
    
  case 0:
    {
      clear_SystemBuf();			
      qwl_net_sendstring("AT+CGATT?");
      net.setp_setp=1;
      net.delay=0;
      PrintString_("附着查询");
    }break;
  case 1:
    {
      if(net.delay>6)//等待超时，服务器问题
      {
        net.delay=0;
        net.setp_setp=0;//等待失败
        net.reconnect_setp=OPEN_GPRS;
        
        
        PrintString_("附着超时");
        net.delay_times++;
        if(net.delay_times>15)
        {
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=MODE_CHOICE;//进入CDMA重启
          net.delay=0;
          PrintString_("进入NBLOT重启");
        }
      }
      
      
      if(strsearch_h("+CGATT:1",SystemBuf)>0)
      {
        net.setp_setp=2;
        net.delay_times=0;
        net.delay=0;
        PrintString_("NBLOT附着成功");
      }
      
      
    }break;
  case 2:
    {
      clear_SystemBuf();			
      qwl_net_sendstring("AT+NCCID");
      net.setp_setp=3;
      net.delay=0;
      PrintString_("查询卡号");
    }break;
  case 3:
    {
      u16 j;
      u8 i;
      if(net.delay>2)//等待超时，服务器问题
      {
        net.delay=0;
        net.setp_setp=4;
        PrintString_("查sim卡号超时");
        net.delay_times++;
        if(net.delay_times>5)
        {
          PrintString_("sim卡号不存在");
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=MODE_CHOICE;//进入网络重新连接
          net.delay=0;
        }
      }	
      if(g_flag==0)
        j=strsearch_h("+NCCID:",SystemBuf);
      else
      {
        j=strsearch_h("OK",SystemBuf);
        j=j+2;
      }
      if(j>0)
      { 
        PrintString_("查sim卡号成功");
        for(i=0;i<20;i++)
        {
          SIM_ID[i]=SystemBuf[j+i];
          if(i>5)
            Sys_keydata.Num_Cmd[0]=SystemBuf[j+i];
          USART3_sendchar(SIM_ID[i]);
        }
        for(i=0;i<13;i++)
        {
          Sys_keydata.Num_Cmd[i] = SIM_ID[i+6];//获取、打包卡号
        }
        
        net.delay_times=0;
        net.setp_setp=4;
        net.delay=0;
      }
    }
  case 4:
    {
      clear_SystemBuf();			
      qwl_net_sendstring("AT+CSQ");
      net.setp_setp=5;
      net.delay=0;
      PrintString_("查询信号强度");
    }break;
  case 5:
    {
      u8 TempTran = 0;
      if(net.delay>=5)//等待超时，服务器问题
      {
        net.delay=0;
        net.setp_setp=0;//等待失败
        PrintString_("查询超时");
        net.delay_times++;
        if(net.delay_times>7)
        {
          net.delay_times=0;
          net.setp_setp=0;
          net.reconnect_setp=MODE_INIT;//进入CDMA重启
          net.delay=0;
          PrintString_("进入CDMA重启");
        }
      }
      
      if(strsearch_h(",99",SystemBuf)>0)
      {
        net.reconnect_setp=LINK_IP;//进入网络重新连接
        net.setp_setp=0;
        net.delay_times=0;
        net.delay=0;
        TempTran = strsearch_h("+CSQ:",SystemBuf);
        Get_SigStren(TempTran);
      }
    }
    break;
  default:break;
  }
}
/*******************************************************************************
* 函 数 名         : cdma_init_0()
* 函数功能		   : 网络重连
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void nblot_init_0()
{
  net.delay_times=0;
  net.setp_setp=0;
  net.reconnect_setp=MODE_CHOICE;//进入网络重新连接
  net.reconnect_setp=MODE_CHOICE;
  net.delay=0;
}



/*******************************************************************************
* 函 数 名         : void nblot_rec(void)
* 函数功能		   : 网络重连
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void nblot_rec(u8 k)
{
  if(k=='+')
  {
    nblot_rec_flag=1;
  }
  switch(nblot_rec_flag)
  {
  case 1:
    {
      if(k=='N')
        nblot_rec_flag++;
    }
    break;
  case 2:
    {
      if(k=='S')
        nblot_rec_flag++;
    }
    break;
  case 3:
    {
      if(k=='O')
        nblot_rec_flag++;
    }
    break;
  case 4:
    {
      if(k=='N')
        nblot_rec_flag++;
    }
    break;
  case 5:
    {
      if(k=='M')
        nblot_rec_flag++;
    }
    break;
  case 6:
    {
      if(k=='I')
      {
        nblot_rec_flag=0;
        nblot_read_flag=1;
      }
    }
    break;
    
  default:break;
  }
  
}
/*******************************************************************************
* 函 数 名         : nblot_read()
* 函数功能		   : 
* 输    入         : 
* 输    出         : 
*******************************************************************************/
void nblot_read()
{
  
  if(nblot_read_flag>0)
  {
    
    nblot_read_flag1_delay=0;
    nblot_read_flag1=1;
    
    //		nblot_rec_times=0;
    
    PrintString_("AT+NSORF=0,512");//读清队列
    qwl_net_sendstring("AT+NSORF=0,512");//读清队列
    
    nblot_read_flag=0;
  }
}
/*******************************************************************************
* 函 数 名         : u8 nblot_group()
* 函数功能		   : 
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
/*u8 nblot_group(void)
{
u16 i=0;
u16 j=0;
u16 len=0;
u8 ch=0;
u8 k;
k=1;


len=nblot_rec_times;
Delay_MS(3);
if(len==nblot_rec_times);
{

qwl_sendchar(NO[nblot_rec_times>>4]);
qwl_sendchar(NO[nblot_rec_times&0x0f]);

i=strsearch_hq("7E",nblot_rec_buf);
if(i>1)
{
i=i-2;

for(j=i;j<nblot_rec_times;j++)
{
qwl_sendchar(nblot_rec_buf[j]);
			}				
for(j=i;j<nblot_rec_times;j++)
{ch=(ASCII_hex[nblot_rec_buf[j]-0x30]<<4)+ASCII_hex[nblot_rec_buf[j+1]-0x30];
j++;
eth_rec_deal(ch);
//				qwl_sendchar(ch);



			}


k=0;
nblot_rec_times=0;


			}

		}

//i=strsearch_h("+NSORF=0,",SystemBuf);
//{
//	len=
//nblot_read_flag=0;
//}

return k;
}

*/





