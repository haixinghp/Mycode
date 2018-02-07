#include "config.h"

PROTOCOL_REC_TYPE Rec_Timeout;
uchar platform_soft_version[2]={0,0};
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : Uploading_Send_50
// 功能描述  : 获取地址
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
static void Uploading_Send_50(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{	
  PostSendHead(uf);
  PostSend_Word(uf,ModAddress);     // 送协议地址		
  PostSendDataLen(uf);
  PostSendPostChksum(uf);
  PostSendTail(uf);
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : Uploading_Send_F8
// 功能描述  : 获取地址
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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
    //平台约定固件版本
    PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.update_server_add,22);
    //采集IP
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
    //平台约定固件版本
    PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.update_server_add,22);
    //采集IP
    PostSendDataLen(uf);
    PostSendPostChksum(uf);
    PostSendTail(uf);
  }break;
default:break;
}

}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : Uploading_Pro
// 功能描述  : 上报口协议处理
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void Uploading_Pro(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{
  uchar  Cmd = DaPtr[4];
  switch(Cmd)
  {	
  case 0x50://获取地址
    Run_User_Program_Falg = true;
    Uploading_Send_50(uf,DaPtr,len);
    break;
  case 0xF8:
    {	
      if(Net_Data_Send.upload_handshake.REC_state == WAIT_REC)//判断首次握手，等待接受数据
      {
        Rec_Timeout.Post_REC_TIMEOUT.delays = 0;
        Uploading_Send_F8(uf,DaPtr,len);
        
        if(Net_Data_Send.Link_Type != UPLOAD)
        {
#if CONF_EX_MEMORY_EN == UPLOAD_EEPROM//升级前测试硬件
          if(EEPROM_CheckOk()==1){
#else
            if(W25QXX_Test() == 1){
#endif
              PrintString_("存储芯片OK");
              Net_Data_Send.Link_Type = UPLOAD;//一次应答后，切换到固件升级模式
              net.reconnect_setp=LINK_IP;//IP重新设置
              heartsend_Onehour();//心跳改为一小时
              //加快心跳频率，防止升级过程中断连接慢，更新重启后能从eeprom读出回复
              Rec_Timeout.Post_REC_TIMEOUT.Rec_State = STOP;//切换IP关闭超时
              Delay_MS(1000);//切换平台前，延时1s确保最后一条数据发送成功
              Getparameters_Str(&DaPtr[25],(uchar*)Sys_keydata.update_server_add,21);
              Getparameters_Str(&DaPtr[23],(uchar*)platform_soft_version,2);
            }else{
              PrintString_("存储芯片error,结束通讯");
              Down_Pro=PRO_END;//结束通讯，返回井盖程序
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
          //心跳返回56字节：
          //调试数据~1101D000802630303030303030303030303030303030000000F649(0x0D)
          //转换成hex（前面操作过）后，参数更新在DaPtr[24]
          if(DaPtr[25]==1)//升级更新开关
          {
            PrintString_("进行固件升级");
            Rec_Timeout.Post_REC_TIMEOUT.Rec_State = RUN;//开启接收超时
            
          }
          else if(DaPtr[24]==1)//参数更新开关
          {
            PrintString_("等待参数更新");
            Rec_Timeout.Post_REC_TIMEOUT.Rec_State = RUN;//开启接收超时
            
          }
          else
          {
            //        PrintString_("结束通讯");
            //        Down_Pro=PRO_END;//结束通讯，返回井盖程序
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
            PrintString_("数据上报成功");
            Net_Data_Send.Link_Type = UPDATA;  //进入历史上报进程
            PrintString_("开始历史数据上报");
#endif            
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
            PrintString_("结束通讯");
            Down_Pro=PRO_END;//结束通讯，返回井盖程序
#endif             
          }
          Net_Data_Send.SA_Heartbeat.REC_state=REC_OK;//
        }
        break;
      case UPDATA:
        if(Net_Data_Send.Updata.REC_state == WAIT_REC)
        {
          Net_Data_Send.Updata.REC_state=REC_OK;
          PrintString_("历史数据上报成功");
        }
        break;  
      default:break;
      }
      
      break;
    case 0x12:
      PrintString_("回复读版本");
      Uploading_Send_12(uf,DaPtr,len);//
      Rec_Timeout.Post_REC_TIMEOUT.delays = 0;//等待F8
      
      break;
    case 0xFB:
      //已校验过，直接配置
      PostSendDefault(uf);//回复//~1001D0000000FDAA
      PrintString_("处理下发配置");
      //      Watchdog_Feed();
      Getparameters(uf,DaPtr,len);
      Watchdog_Feed();
      
//      Down_Pro=PRO_END;//结束通讯，返回井盖程序
      
      Rec_Timeout.Post_REC_TIMEOUT.Rec_State = STOP;//参数配置完超时关闭
      Rec_Timeout.Post_REC_TIMEOUT.delays = 0;
      
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码      
      Net_Data_Send.Link_Type = UPDATA;  //进入历史上报进程
      PrintString_("开始历史数据上报");
#endif
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
            PrintString_("结束通讯");
            Down_Pro=PRO_END;//结束通讯，返回井盖程序
#endif             
      /*~1101D0FB40EE00002C000010B82C000000000000000000000000000
      000002C00002C00002C062C062C00182C00002C012C2C00002C002C0
      000000000000000000000000000000000000000002C0000000000000
      000000000000000000000000000002C33392E3130382E39312E32303
      82C323030303800002C002C002CCE4E(0x0D)参数下发调试数据  */
      break;
    default:
      RTN= 0x04;//无效命令
      PostSendDefault(uf);
      break;
    }
}
void post_data_rec_delay(void)
{
  if(net.reconnect_setp!=LINK_OK){//网络连接超时
    if(Rec_Timeout.NET_STATE_TIMEOUT.Rec_State == RUN){
      if(Rec_Timeout.NET_STATE_TIMEOUT.delays++>Rec_Timeout.NET_STATE_TIMEOUT.MAX_delays){
        Down_Pro=PRO_END;//结束通讯，返回井盖程序
        PrintString_("无法连接网络");
      }
    }
  }else{
    Rec_Timeout.NET_STATE_TIMEOUT.delays = 0;//清零
  }
  
  if(Rec_Timeout.Post_REC_TIMEOUT.Rec_State == RUN){//邮电部协议接收超时
    PrintString_("等待接收");
    if(Rec_Timeout.Post_REC_TIMEOUT.delays++>Rec_Timeout.Post_REC_TIMEOUT.MAX_delays){
      Down_Pro=PRO_END;//结束通讯，返回井盖程序
      PrintString_("等待超时");
    }
  }
    
  if(Rec_Timeout.Download_REC_TIMEOUT.Rec_State == RUN){//下载接收超时
    PrintString_("等待接收");
    if(Rec_Timeout.Download_REC_TIMEOUT.delays++>Rec_Timeout.Download_REC_TIMEOUT.MAX_delays){
      Down_Pro=PRO_END;//结束通讯，返回井盖程序
      PrintString_("等待超时");
    }
  }
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
//                          程序结束(END)
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※


