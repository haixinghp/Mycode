#include "config.h"

u8 SA_Pro=0;//功能进程
u8 Down_Pro=0;//通讯进程
u32 channel_en_bit = 0;//平台下发参数使能位

/*****************************************************************************************************************
实现功能:对SIM800C进行启动,开启SIM800C功能详述:单片机上是跟SIM800C的管脚(/IGT)加时长至少为1s的低电平信号
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

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : IOTMode_RESET
// 功能描述  : 物联模块重启
// 输入参数  : None
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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
  
  qwl_net_sendstring("AT+NRB");//复位
  qwl_net_sendstring("AT+CGMM");//G网复位
  
  Delay_16M_MS(1000);
  SET_MC_POWER_ONOFF;
  
  Delay_16M_MS(4000);
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : Get_SigStren
// 功能描述  : 获取信号强度
// 输入参数  : 保存信号强度的缓存数组下标
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void Get_SigStren(u8 TempVal)
{
  u8 Temp1,Temp2;
  
  Temp1=SystemBuf[TempVal];
  Temp2=SystemBuf[TempVal+1];
  
  PrintString("信号强度:");
  
  if(Temp2 != ','){//如果是两位数
    USART3_sendchar(Temp1);//打印信号强度
    USART3_sendchar(Temp2);
    Sys_keydata.SigStren = asc2hex(Temp1)*10+asc2hex(Temp2);
  }else{
    USART3_sendchar(Temp1);//打印信号强度
    Sys_keydata.SigStren = asc2hex(Temp1);
  }
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : Get_SimID
// 功能描述  : 获取13位卡号
// 输入参数  : 临时保存ID的起始地址
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void Get_SimID(u8 *TempPointer)//
{
  for(u8 i=0;i<13;i++)
  {
    Sys_keydata.Num_Cmd[i] = *TempPointer++;//获取、打包卡号
  }
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : Uploading_Send_12
// 功能描述  : 发送版本
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void Uploading_Send_12(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{
  u8 Temp_Tran[22];
  u8 Empty_SendAdd[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  PostSendHeadCmd(uf,01,0x00);//01 地址 00 cmd
  
  Temp_Tran[0] = '0';
  Temp_Tran[1] = '1';
  PostSend_HexStr(USART_FUNCTION_WCDMA,Temp_Tran,2);
  //本机地址	
  PostSend_HexStr(USART_FUNCTION_WCDMA,SOFTWARE_VER,6);
  //软件版本号，上位机不可配置
  Temp_Tran[0] = 'V';
  Temp_Tran[1] = '0';
  Temp_Tran[2] = Sys_keydata.harware_version[0] + '0';
  Temp_Tran[3] = '.';
  Temp_Tran[4] = Sys_keydata.harware_version[1] + '0';
  Temp_Tran[5] = '0';
  PostSend_HexStr(uf,Temp_Tran,6);
  //硬件版本号
  
  PostSend_HexStr(uf,COMPANY_NAME,20);
  //公司名字
  PostSend_HexStr(uf,Sys_keydata.device_id,16);
  PostSend_HexStr(uf,Empty_SendAdd,4);
  //设备序号 Id_Cmd
  PostSend_HexStr(uf,Empty_SendAdd,24);
  //序列号（备用）
  PostSend_HexStr(uf,__DATE__,11);
  PostSend_HexStr(uf,__TIME__,9);
  //编译时间
  PostSendDataLen(uf);
  PostSendPostChksum(uf);
  PostSendTail(uf);
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : pack_heat
// 功能描述  : 打包心跳
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void pack_heat(void)//把心跳数据内容注入缓存
{ 
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
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
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.device_id,16);//设备序号 Id_Cmd
  //  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,4);//客户码 Cstm_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.Num_Cmd,20);//物联网卡号 Num_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.harware_version,2);//硬件版本 
  TempTran[0] = (asc2hex(TempTran[1])*10)+(asc2hex(TempTran[2]));
  TempTran[1] = (asc2hex(TempTran[4])*10)+(asc2hex(TempTran[5]));
  PostSend_HexStr(USART_FUNCTION_WCDMA,TempTran,2);//软件版本 
  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,1);//设备状态 State_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,&Sys_keydata.SigStren,1);//信号强度 SigStren_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,1);//剩余电压 ResV_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.send_totalcount),2);//u16 send_totalcount;发送次数
  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,1);//环境温度 Temp_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_value),2);//u16 pressure_value;//压力
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.rotation_angle),2);//s16 rotation_angle;//旋转角度
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.lean_angle),1);//s8 lean_angle;//倾斜角度
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.rotation_trigger_angle),1);//u8 rotation_trigger_angle;//旋转触发角度
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.rotation_trigger_sens),1);//u8 rotation_trigger_sens;//旋转触发灵敏度
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.lean_trigger_angle),1);//u8 lean_trigger_angle;//倾斜触发角度
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.lean_trigger_sens),1);//u8 lean_trigger_sens;//倾斜触发灵敏度
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_trigger_value),2);//u16 pressure_trigger_value;//压力触发阈值
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_interval),2);//u16 pressure_interval;//压力检测间隔
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.Alarm),2); //u16 Alarm;//告警
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.heartdata_sent_interval_hour_count),2);//u16 heartdata_sent_interval_hour_count;//心跳间隔时间
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.alarm_sent_interval_minute_count),2);//u16 alarm_sent_interval_minute_count;//报警间隔时间
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.trigger_interval_minute_count),2);//u16 trigger_interval_minute_count;//触发间隔时间
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.default_cloud_server_add,21);//u8 default_cloud_server_add[22];//云端服务器地址255.255.255.255,65535
  PostSend_HexStr(USART_FUNCTION_WCDMA,&Sys_keydata.carrier_type,1);//u8 carrier_type;//运营商   不可修改 绑定硬件版本
  PostSend_HexStr(USART_FUNCTION_WCDMA,&Sys_keydata.module_type,1);//u8 module_type;//模块制式 不可修改 绑定硬件版本
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码  
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.TimesTamp),4);//s32 TimesTamp;  //时间戳 
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_upLimit),2);//u16 pressure_upLimit; //压力上限
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_downLimit),2);//u16 pressure_downLimit; //压力下限
#endif
#endif
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
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
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.device_id,16);//设备序号 Id_Cmd
  //  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,4);//客户码 Cstm_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.Num_Cmd,20);//物联网卡号 Num_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.harware_version,2);//硬件版本 
  TempTran[0] = (asc2hex(TempTran[1])*10)+(asc2hex(TempTran[2]));
  TempTran[1] = (asc2hex(TempTran[4])*10)+(asc2hex(TempTran[5]));
  PostSend_HexStr(USART_FUNCTION_WCDMA,TempTran,2);//软件版本 
  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,1);//设备状态 State_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,&Sys_keydata.SigStren,1);//信号强度 SigStren_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,1);//剩余电压 ResV_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.send_totalcount),2);//u16 send_totalcount;发送次数
  PostSend_HexStr(USART_FUNCTION_WCDMA,Empty_SendAdd,1);//环境温度 Temp_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_value),2);//u16 pressure_value;//压力
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)getSamplingValue(),2); //u16 SamplingValue; //采样原始值
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_trigger_value),2);//u16 pressure_trigger_value;//压力触发阈值
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.Alarm),2); //u16 Alarm;//告警
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.heartdata_sent_interval_hour_count),2);//u16 heartdata_sent_interval_hour_count;//心跳间隔时间
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)getAlarmEnableBit(),2); // u16 AlarmEnableBit; //告警使能0失能，1使能
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)getPressureUpLimitAlarmInterval(),2);//  u16 PressureUpLimitAlarmInterval; //上限告警间隔时间
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)getPressureDownLimitAlarmInterval(),2);//  u16 PressureDownLimitAlarmInterval; //下限告警间隔时间
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)getNoSensorAlarmInterval(),2);//  u16 NoSensorAlarmInterval; //未检测到传感器告警间隔
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)getResolutionAlarmInterval(),2);//  u16 ResolutionAlarmInterval; //分辨率上报告警间隔
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)getTouchOffAlarmInterval(),2);//  u16 TouchOffAlarmInterval;  //人工触发上报间隔
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.trigger_interval_minute_count),2);//u16 trigger_interval_minute_count;//触发间隔时间
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.default_cloud_server_add,21);//u8 default_cloud_server_add[22];//云端服务器地址255.255.255.255,65535
  PostSend_HexStr(USART_FUNCTION_WCDMA,&Sys_keydata.carrier_type,1);//u8 carrier_type;//运营商   不可修改 绑定硬件版本
  PostSend_HexStr(USART_FUNCTION_WCDMA,&Sys_keydata.module_type,1);//u8 module_type;//模块制式 不可修改 绑定硬件版本 
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.TimesTamp),4);//s32 TimesTamp;  //时间戳 
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_upLimit),2);//u16 pressure_upLimit; //压力上限
  PostSend_HexStr(USART_FUNCTION_WCDMA,(uchar *)(&Sys_keydata.pressure_downLimit),2);//u16 pressure_downLimit; //压力下限
#endif  
}
extern void PostSendFixupByte(USART_FUNCTION uf,uchar *TxBuf,uchar FixupNo,uchar valus);
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : PostSendUpDataLen
// 功能描述  : 邮电部协议发送历史数据长度
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : pack_heat
// 功能描述  : 把历史数据内容注入缓存
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
s8 pack_updata(void)
{
  int i;
  s8 s8_tmp =0;
  u8 infoLen =0x00;  //数据个数
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
  PostSend_HexStr(USART_FUNCTION_WCDMA,Sys_keydata.device_id,16);//设备序号 Id_Cmd
  PostSend_HexStr(USART_FUNCTION_WCDMA,&infoLen,1);//数值个数 虚拟填写
  if(1 == IsFLASH_QueueEmpty()) //检查队列是否为空
  {
    debug_send("QueueEmpty1!");
    return -1;  //强制退出发送
  }
  else  //读取数据到缓冲区。每次最大读取38组。需要写入FLASH缓存。
  {
    s8_tmp = 1;
    for(i=0;i<(CONST_PRESSUREDATA_MAX_LEN);i++) //一次最多缓存
    {
      Watchdog_Feed();  //测试发现，会有重启的现象，现在加入喂狗，进行测试。
      if(1 == IsFLASH_QueueEmpty()) //检查队列是否为空。
      {
        debug_send("QueueEmpty2!");
        s8_tmp = 0; //这里用于通知系统这次最后一次发送数据。
        break;  //数据列为空，强制退出
      }
      else
      {
        DeFLASH_Queue(&struPressureAData.TimesTamp,&struPressureAData.PressureData,&struPressureAData.AlarmList);
        PostSend_HexStr(USART_FUNCTION_WCDMA,(u8 *)(&struPressureAData.TimesTamp),4);//填写时间
        PostSend_HexStr(USART_FUNCTION_WCDMA,(u8 *)(&struPressureAData.PressureData),2);//填写压力数据
        PostSend_HexStr(USART_FUNCTION_WCDMA,(u8 *)(&struPressureAData.AlarmList),2);//填写告警表
      }
    }
    Watchdog_Feed();  //测试发现，会有重启的现象，现在加入喂狗，进行测试。
    PostSendUpDataLen(USART_FUNCTION_WCDMA,i);//填写 数值个数
  }
  return s8_tmp;
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : Get_DowndataFlag
// 功能描述  : 打包心跳
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void Get_DowndataFlag(uchar *DaPtr)
{
  //心跳返回56字节：
  //调试数据~1101D000802630303030303030303030303030303030000000F649(0x0D)
  //转换成hex（前面操作过）后，参数更新在DaPtr[24]
  if(DaPtr[25]==1){//升级更新开关
    PrintString_("进行固件升级");
    Rec_Timeout.Post_REC_TIMEOUT.Rec_State = RUN;//开启接收超时
    
  }else if(DaPtr[24]==1){//参数更新开关
    PrintString_("等待参数更新");
    Rec_Timeout.Post_REC_TIMEOUT.Rec_State = RUN;//开启接收超时
    
  }else{
    PrintString_("结束通讯");
    Down_Pro=PRO_END;//结束通讯，返回井盖程序
  }
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : Getparameters_Str
// 功能描述  : 平台下发参数 保存
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
uint16 Getparameters_Str(uchar *DaPtr,uchar *Par_Str,uint16 with)
{
  uint16 i;
  for(i=0; i<with; i++){
    *Par_Str++ = *DaPtr;
    DaPtr++;
  }
  
  if(*DaPtr==0x2C){
    with++;
    PrintString_("正确");
  }
  else
    PrintString_("错误");
  
  Watchdog_Feed();
  return with;
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : Getparameters_StrEN
// 功能描述  : 平台下发参数 保存，带通道使能
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
uint16 Getparameters_StrEN(uchar *DaPtr,uchar *Par_Str,uint16 with,u32 Enable)
{
  uint16 i;
  if(channel_en_bit & Enable){
    
    for(i=0; i<with; i++){
      *Par_Str++ = *DaPtr;
      DaPtr++;
    }
    
    if(*DaPtr==0x2C){
      PrintString_("正确");
    }
    else
      PrintString_("错误");
    
  }else{
    PrintString_("取消");
  }
  
  with++;
  Watchdog_Feed();
  return with;
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : Getparameters
// 功能描述  : 配置参数
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void Getparameters(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{

#define U32BIT1 ((u32)(1))
  u16 len_cur = 7;
  u32 Empty_Val = 0;
  u8  Empty_Str[22];
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
  //DaPtr += 7;//跳过头
  PrintString("配置通道个数");
  len_cur += Getparameters_Str(&DaPtr[len_cur],(uchar*)&Empty_Val,2);//通道个数
  PrintString("配置通道有效标志");
  len_cur += Getparameters_Str(&DaPtr[len_cur],(uchar*)&channel_en_bit,4);//通道有效标志
  PrintString("配置ID");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Empty_Str,16,U32BIT1<<0);//ID
  PrintString("配置软件版本");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,2,U32BIT1<<1);//软件版本
  PrintString("配置硬件版本");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,2,U32BIT1<<2);//硬件版本
  PrintString("配置设备状态");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.status,1,U32BIT1<<3);//设备状态
  PrintString("配置电池发送总数");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,2,U32BIT1<<4);//电池发送总数
  PrintString("配置旋转变化角度");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.rotation_trigger_angle,1,U32BIT1<<5);//旋转变化角度
  PrintString("配置旋转灵敏度");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.rotation_trigger_sens,1,U32BIT1<<6);//旋转灵敏度
  PrintString("配置倾斜变化角度");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.lean_trigger_angle,1,U32BIT1<<7);//倾斜变化角度
  PrintString("配置倾斜灵敏度");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.lean_trigger_sens,1,U32BIT1<<8);//倾斜灵敏度
  PrintString("配置压力变化阈值");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.lean_trigger_sens,2,U32BIT1<<9);//压力变化阈值
  PrintString("配置压力检测间隔");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.pressure_trigger_value,2,U32BIT1<<10);//压力检测间隔
  PrintString("配置心跳间隔时间");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.heartdata_sent_interval_hour_count,2,U32BIT1<<11);//告警间隔
  PrintString("配置告警间隔");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.alarm_sent_interval_minute_count,2,U32BIT1<<12);//告警间隔
  PrintString("配置检测间隔时间");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.trigger_interval_minute_count,2,U32BIT1<<13);//检测间隔时间
  PrintString("配置可配采集地址");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Sys_keydata.default_cloud_server_add,21,U32BIT1<<14);//可配采集地址
  PrintString("配置升级服务器地址");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Sys_keydata.update_server_add,21,U32BIT1<<15);//升级服务器地址
  PrintString("配置运营商");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,1,U32BIT1<<16);//运营商
  PrintString("配置模块类型");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,1,U32BIT1<<17);//模块类型
  PrintString("配置物联卡号");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Empty_Str,20,U32BIT1<<18);//物联卡号
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码  
  PrintString("配置时间戳");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.TimesTamp,4,U32BIT1<<19);//时间戳
  PrintString("配置压力上限");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.pressure_upLimit,2,U32BIT1<<20);//压力上限
  PrintString("配置压力下限");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.pressure_downLimit,2,U32BIT1<<21);//压力下限
#endif
  
#undef U32BIT1
  //  PrintString("配置触发灵敏度");
  ////  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.trigger_sensitivity,1,1<<4);//触发灵敏度
  //  PrintString("配置心跳间隔");
  ////  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.heartdata_sent_interval_hour_count,2,1<<5);//心跳间隔
  //  PrintString("配置报警间隔");
  ////  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.alarm_sent_interval_second_count,2,1<<6);//报警间隔
  //  PrintString("配置二次确认间隔");
  ////  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.second_query_interval_second_count,2,1<<7);//二次确认间隔
  //  
  //  PrintString("配置温度修正");
  //  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,1,1<<9);//温度修正
  //  PrintString("配置升级服务器地址");
  //  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Empty_Str,21,1<<10);//升级服务器地址
  //  PrintString("配置可配采集地址");
  //  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Empty_Str,21,1<<11);//可配采集地址
  //  PrintString("配置默认采集地址");
  ////  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Sys_keydata.default_cloud_server_add,21,1<<12);//默认采集地址
  //  PrintString("配置运营商");
  //  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,1,1<<13);//运营商
  //  PrintString("配置模块类型");
  //  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,1,1<<14);//模块类型
  
  /*~1101D0FB40EE00002C000010B82C000000000000000000000000000
  000002C00002C00002C062C062C00182C00002C012C2C00002C002C0
  000000000000000000000000000000000000000002C0000000000000
  000000000000000000000000000002C33392E3130382E39312E32303
  82C323030303800002C002C002CCE4E(0x0D)参数下发调试数据  */
  Watchdog_Feed();
  if (channel_en_bit & BIT_CONFIG_TRIGGER_INTERVAL)//检测间隔时间	UINT	0-65535秒  
  {
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
    RTC_Config();  //直接配置
#endif    
  }
  
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码  
  if (channel_en_bit & BIT_TIMESTAMP_ID)
  {
    TickGetTime(Sys_keydata.TimesTamp); //根据时间戳配置时间。
    TimeGetTick(TRUE);
#if SD3078_WIRTE_INIT_FLAG_EN
    RTC_SD3078_WriteInitFlag(); //写入标志;
#endif
  }
#endif  

  if(STATUS_CLEAR_FLASH == Sys_keydata.status) //检测到系统需要清除FLASH
  {
    debug_send("开始清除FLASH！");   
    ClearFLASH_Queue();
    Sys_keydata.status = STATUS_NORMAL; //恢复到正常状态。
  }
  Write_sys_keydata();
  
  Watchdog_Feed();
  Set_Key_Para();
  
  Watchdog_Feed();
  //  SysParaInit();//系统参数初始化
  PrintSysParaList();
  Watchdog_Feed();
#endif
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
   PrintString("配置通道个数");
  len_cur += Getparameters_Str(&DaPtr[len_cur],(uchar*)&Empty_Val,2);//通道个数
  PrintString("配置通道有效标志");
  len_cur += Getparameters_Str(&DaPtr[len_cur],(uchar*)&channel_en_bit,4);//通道有效标志
  PrintString("配置ID");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Empty_Str,16,U32BIT1<<0);//ID
  PrintString("配置软件版本");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,2,U32BIT1<<1);//软件版本
  PrintString("配置硬件版本");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,2,U32BIT1<<2);//硬件版本
  PrintString("配置设备状态");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.status,1,U32BIT1<<3);//设备状态
  PrintString("配置电池发送总数");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,2,U32BIT1<<4);//电池发送总数
  PrintString("配置压力变化分辨率");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.pressure_trigger_value,2,U32BIT1<<5);//压力变化分辨率
  PrintString("配置心跳间隔时间");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.heartdata_sent_interval_hour_count,2,U32BIT1<<6);//心跳间隔时间
  PrintString("配置告警使能");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)getAlarmEnableBit(),2,U32BIT1<<7);//告警使能
  PrintString("配置上限告警间隔时间");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)getPressureUpLimitAlarmInterval(),2,U32BIT1<<8);//上限告警间隔时间
  PrintString("配置下限告警间隔时间");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)getPressureDownLimitAlarmInterval(),2,U32BIT1<<9);//下限告警间隔时间
  PrintString("配置未检测到传感器告警间隔");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)getNoSensorAlarmInterval(),2,U32BIT1<<10);//未检测到传感器告警间隔
  PrintString("配置分辨率上报告警间隔");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)getResolutionAlarmInterval(),2,U32BIT1<<11);//分辨率上报告警间隔
  PrintString("配置人工触发上报间隔");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)getTouchOffAlarmInterval(),2,U32BIT1<<12);//人工触发上报间隔
  PrintString("配置检测间隔时间");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.trigger_interval_minute_count,2,U32BIT1<<13);//检测间隔时间
  PrintString("配置可配采集地址");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Sys_keydata.default_cloud_server_add,21,U32BIT1<<14);//可配采集地址
  PrintString("配置升级服务器地址");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Sys_keydata.update_server_add,21,U32BIT1<<15);//升级服务器地址
  PrintString("配置运营商");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,1,U32BIT1<<16);//运营商
  PrintString("配置模块类型");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Empty_Val,1,U32BIT1<<17);//模块类型
  PrintString("配置物联卡号");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],Empty_Str,20,U32BIT1<<18);//物联卡号
  PrintString("配置时间戳");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.TimesTamp,4,U32BIT1<<19);//时间戳
  PrintString("配置压力上限");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.pressure_upLimit,2,U32BIT1<<20);//压力上限
  PrintString("配置压力下限");
  len_cur += Getparameters_StrEN(&DaPtr[len_cur],(uchar*)&Sys_keydata.pressure_downLimit,2,U32BIT1<<21);//压力下限
#undef U32BIT1
  
  Watchdog_Feed();
  if (channel_en_bit & BIT_CONFIG_TRIGGER_INTERVAL)//检测间隔时间	UINT	0-65535秒  
  {
    RTC_Config();  //直接配置
  }
  if (channel_en_bit & BIT_TIMESTAMP_ID)
  {
    TickGetTime(Sys_keydata.TimesTamp); //根据时间戳配置时间。
    TimeGetTick(TRUE);
#if SD3078_WIRTE_INIT_FLAG_EN
    RTC_SD3078_WriteInitFlag(); //写入标志;
#endif
  }

  if(STATUS_CLEAR_FLASH == Sys_keydata.status) //检测到系统需要清除FLASH
  {
    debug_send("开始清除FLASH！");   
    //这里有重启的现象。
    Watchdog_Feed(); //这里比较废时间，要先清除看门狗
    ClearFLASH_Queue();
    Sys_keydata.status = STATUS_NORMAL; //恢复到正常状态。
  }
  Write_sys_keydata();
  
  Watchdog_Feed();
  Set_Key_Para();
  
  Watchdog_Feed();
  //  SysParaInit();//系统参数初始化
  PrintSysParaList();
  Watchdog_Feed();
#endif  
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : heartsend_Onehour
// 功能描述  : 升级时，心跳改为1小时，不存eeprom
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void heartsend_Onehour(void)
{
  Sys_keydata.heartdata_sent_interval_hour_count = 1;
}
