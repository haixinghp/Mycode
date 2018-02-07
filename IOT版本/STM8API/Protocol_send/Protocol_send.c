#include "config.h"


PROTOCOL_SEND_TYPE Net_Data_Send={0};

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : upload_link_send()
// 功能描述  : 升级握手
// 输入参数  : None
// 输出参数  : None
// 返回参数  : 无
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void upload_link_send_POST()
{
  //  u8 TempTran[10];
  PostSendHeadCmd(USART_FUNCTION_WCDMA,01,0xf8);
  //PostSendByteASC(USART_FUNCTION_WCDMA,0xf8);
  
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.device_id,16);
  //ID
  PostSend_HexStr(USART_FUNCTION_WCDMA,platform_soft_version,2);
  //平台约定固件版本
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.update_server_add,22);
  //采集IP
  PostSendDataLen(USART_FUNCTION_WCDMA);
  
  PostSendPostChksum(USART_FUNCTION_WCDMA);
  PostSendTail(USART_FUNCTION_WCDMA);
}
void SA_link_send_POST(void)
{
  PostSendHeadCmd(USART_FUNCTION_WCDMA,01,0x1A);  //打包命令头
  //  	0x7E,//SOI
  //	'1', '1', //VER
  //	'0', '1', //ADR
  //	'D', '0', //CID1
  //	'1', 'A', //CID2
  //	//'B', '0', 'B', 'A', //LEN
  //        'A', '0', 'E', '8', //LEN
  pack_heat();//把心跳数据内容注入缓存       
  PostSendDataLen(USART_FUNCTION_WCDMA);
  
  PostSendPostChksum(USART_FUNCTION_WCDMA);//'0', '0', '0', '0', //校验码 Chksum_Cmd
  PostSendTail(USART_FUNCTION_WCDMA); //0x0D,//EOI
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : SA_link_send()
// 功能描述  : 发送心跳
// 输入参数  : None
// 输出参数  : None
// 返回参数  : 无
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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
          SA_link_send_POST();//发送心跳
        }else{
          Down_Pro = PRO_END;
          PrintString_("发送失败，退出通讯");
        }
      }break;
    case WAIT_ACK:
      {
        if(Net_Data_Send.SA_Heartbeat.delays>Net_Data_Send.SA_Heartbeat.MAX_delays)
        {
          Net_Data_Send.SA_Heartbeat.delays=0;
          Net_Data_Send.SA_Heartbeat.send_state=SEND_READY;
          PrintString_("等待心跳回复超时");
          
        }
        if(Net_Data_Send.SA_Heartbeat.REC_state == REC_OK)
        {
          Net_Data_Send.SA_Heartbeat.TIMES=0;//发送应答成功后直接清零，不再重发
          Net_Data_Send.SA_Heartbeat.send_state=SEND_READY;
          Net_Data_Send.SA_Heartbeat.delays=0;
        }
      }break;
    default:break;
    }	
  }
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : upload_link_send()
// 功能描述  : 升级握手
// 输入参数  : None
// 输出参数  : None
// 返回参数  : 无
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void upload_link_send()
{
  if(Net_Data_Send.upload_handshake.REC_state != REC_OK)
  {
    switch(Net_Data_Send.upload_handshake.send_state )
    {
    case SEND_READY:
      {
        if(Net_Data_Send.upload_handshake.TIMES>0)//发送次数
        {
          if(Net_Data_Send.upload_handshake.TIMES!=0xff)
            Net_Data_Send.upload_handshake.TIMES--;
          
          Net_Data_Send.upload_handshake.delays=0;
          Net_Data_Send.upload_handshake.send_state=WAIT_ACK;
          upload_link_send_POST();
        }else{
          Down_Pro = PRO_END;
          PrintString_("发送失败，退出通讯");
        }
        
      }break;
    case WAIT_ACK:
      {
        if(Net_Data_Send.upload_handshake.delays>Net_Data_Send.upload_handshake.MAX_delays)
        {
          Net_Data_Send.upload_handshake.delays=0;
          Net_Data_Send.upload_handshake.send_state=SEND_READY;
          PrintString_("等待握手超时");
          
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
  PostSendPostChksum(USART_FUNCTION_WCDMA);//'0', '0', '0', '0', //校验码 Chksum_Cmd
  PostSendTail(USART_FUNCTION_WCDMA); //0x0D,//EOI
  return 1; //正确
}  
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : link_send()
// 功能描述  : 通用发送函数。
// 输入参数  : SEND_TYPE *sendtype,void (*fun)()
// 输出参数  : None
// 返回参数  : 无
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void link_send(SEND_TYPE *sendtype,s8 (*fun)())
{
  s8 s8_tmp; 
  switch(sendtype->send_state )
  {
  case SEND_READY:
    s8_tmp = fun();    //调用不同的心跳数据内容,直接打包数据到发送缓存。
    if((-1) == s8_tmp) 
    {
      //数据打包错误
      Down_Pro = PRO_END;
      PrintString_("数据打包错误，退出通讯");
      sendtype->send_state = EXIT;//退出发送过程
    }
    else if(0 == s8_tmp) //只发送一次命令
    {
      sendtype->send_control = SEND_STOP;//发送一次后就停止
      sendtype->send_state = SEND_ONE;//进入发送流程
    }
    else if(1 == s8_tmp) //重复发送一次命令
    {
      sendtype->send_control = SEND_CONTINUE;//重复发送
      sendtype->send_state = SEND_ONE;//进入发送流程
    }
    else
    {
      sendtype->send_state = EXIT;//退出发送过程
    }
    break;
  case SEND_ONE:
    link_send_POST(fun); //执行发送函数。
    sendtype->delays=0;//清除发送计时器
    sendtype->send_state=WAIT_ACK;//发送已经完成，进入等待回复状态。
    break;  
  case SEND_REPEATEDLY:
    if(sendtype->TIMES>0)//发送次数
    {
      if(sendtype->TIMES!=0xff)
      {
        sendtype->TIMES--;
      }
      sendtype->delays=0;
      CONST_REPEATEDLY_SEND_DATA; //重发一次数据。
      sendtype->delays=0;//清除发送计时器
      sendtype->send_state=WAIT_ACK;//发送已经完成，进入等待回复状态。
    }
    else
    {
      FLASH_Queue_Recover(); //恢复队列。
      sendtype->send_state=EXIT; //进入退出状态。
      PrintString_("发送失败，退出通讯");
    }
    break;
  case WAIT_ACK:
    {
      if(sendtype->delays>sendtype->MAX_delays)
      {
        sendtype->delays=0;
        sendtype->send_state=SEND_REPEATEDLY;
        PrintString_("等待握手超时");
        
      }
      if(REC_OK == sendtype->REC_state ) //成功收到回复
      {
        sendtype->REC_state = WAIT_REC; //清除成功标志。
        if(SEND_STOP == sendtype->send_control) //设置了发送停止。
        {
          sendtype->TIMES=0; //重复发送。
          sendtype->send_state=EXIT;
          sendtype->delays=0;
          PrintString_("发送成功，退出通讯");
        }
        else if(SEND_CONTINUE == sendtype->send_control)
        {
          sendtype->TIMES=6; //回填重试发送。
          sendtype->send_state=SEND_READY; //再次准备新的数据。
          sendtype->delays=0;//清除发送计时器
        }
      }
    }break;
  case EXIT:  
    sendtype->send_state = SEND_READY; //下次进入状态处于准备发送状态
    sendtype->TIMES=6; //回填重试发送。
    sendtype->delays=0;//清除发送计时器
    Down_Pro = PRO_END; //退出发送过程。
    break;
  default:break;
  }
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : link_send()
// 功能描述  : 通用发送函数。
// 输入参数  : SEND_TYPE *sendtype,void (*fun)()
// 输出参数  : None
// 返回参数  : 无
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※


//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : net_data_send_init()
// 功能描述  : 网络数据发送初始化参数
// 输入参数  : None
// 输出参数  : None
// 返回参数  :无
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void net_data_send_init()
{
  //Net_Data_Send.Link_Type = HEARTBEAT;
  //升级握手数据初始化
  Net_Data_Send.upload_handshake.delays =0;
  Net_Data_Send.upload_handshake.MAX_delays  =15;
  Net_Data_Send.upload_handshake.TIMES  =6;
  Net_Data_Send.upload_handshake.send_state  =SEND_READY;
  Net_Data_Send.upload_handshake.REC_state = WAIT_REC;
  
  
  Net_Data_Send.SA_Heartbeat.delays =0;
  Net_Data_Send.SA_Heartbeat.MAX_delays  = 15; //一次发送后等待应答时间5s
  Net_Data_Send.SA_Heartbeat.TIMES  =6;  //重复6次
  Net_Data_Send.SA_Heartbeat.send_state  =SEND_READY;
  Net_Data_Send.SA_Heartbeat.REC_state = WAIT_REC;
  
  Net_Data_Send.Updata.delays =0;
  Net_Data_Send.Updata.MAX_delays  = 15; //一次发送后等待应答时间5s
  Net_Data_Send.Updata.TIMES  =6;  //重复6次
  Net_Data_Send.Updata.send_state  =SEND_READY;
  Net_Data_Send.Updata.send_control = SEND_STOP;
  Net_Data_Send.Updata.REC_state = WAIT_REC;  
  
  Rec_Timeout.Post_REC_TIMEOUT.Rec_State = STOP;
  Rec_Timeout.Post_REC_TIMEOUT.delays = 0;
  Rec_Timeout.Post_REC_TIMEOUT.MAX_delays = 30;//初定30s超时结束
  
  Rec_Timeout.Download_REC_TIMEOUT.Rec_State = STOP;//下载时延时
  Rec_Timeout.Download_REC_TIMEOUT.delays = 0;
  Rec_Timeout.Download_REC_TIMEOUT.MAX_delays = 90;//初定90s超时结束
  
  Rec_Timeout.NET_STATE_TIMEOUT.Rec_State = RUN;//网络连接超时（默认打开）
  Rec_Timeout.NET_STATE_TIMEOUT.delays = 0;
  Rec_Timeout.NET_STATE_TIMEOUT.MAX_delays = 240;//初定4分钟超时结束
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : net_data_send_delay
// 功能描述  : 网络数据发送延时
// 输入参数  : None
// 输出参数  : None
// 返回参数  : 无
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void net_data_send_delay()
{
  Net_Data_Send.upload_handshake.delays++;
  Net_Data_Send.SA_Heartbeat.delays++;  
  Net_Data_Send.Updata.delays++;
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : net_data_send()
// 功能描述  : 网络数据发送
// 输入参数  : None
// 输出参数  : None
// 返回参数  : 无
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void net_data_send()
{
  if(net.reconnect_setp==LINK_OK)
  {
    switch(Net_Data_Send.Link_Type)
    {
    case UPLOAD:
      upload_link_send();//主动握手升级服务器
      break;
    case HEARTBEAT:
      SA_link_send();//主动发送心跳
      break;  
    case UPDATA:
      link_send(&Net_Data_Send.Updata,pack_updata);  //发送历史数据
      break;    
    default:break;   
    }
    
    //    if(Net_Data_Send.Link_Type == UPLOAD)
    //      upload_link_send();//主动握手升级服务器
    //    else
    //      SA_link_send();//主动发送心跳
  }
}



