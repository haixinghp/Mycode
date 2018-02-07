//#include "stm8l15x.h"
#include "config.h"
u8 Protocol_state = 0;

#define PROTOCOL_UPDATA_LEN 12
//const u8 protocol_updata_array[PROTOCOL_UPDATA_LEN] = {0xAA, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xF8, 0xCC, 0x33, 0xC3, 0x3C};
#define PROTOCOL_CONFIG_LEN 9
const u8 protocol_config_array[PROTOCOL_CONFIG_LEN] =
{0x7E, '1', '0', '0', '1', 'D', '0', 'F', 'B'};

//#define PROTOCOL_CONFIG_TOTAL_LEN 250
//#define PROTOCOL_CONFIG_TOTAL_LEN 256

u8 ack_protocol_config_array[37] =
{	'a', 't', '+', 'z', 'i', 'p', 's', 'e', 'n', 'd', '=', '0', ',', '1', '8', 0x0D,
	0x7E,
	'1', '0',
	'0', '1',
	'D', '0',
	'0', '0',
	'0', '0', '0', '0',
	'0', '0', '0', '0',
	0x0D,
	'\r', '\n',
	'\0'
};

u8 result_u16[22] = {"000000000000000000000"};

u8 Para_config_protocol_state = 0;


//检查字符串是否符合，并且替换掉符号
_Bool Check_Sever_Addr(char *sever_addr) {
	if (strrchr(sever_addr, ':')) {
		char *temp_string = strrchr(sever_addr, ':');
		temp_string[0] = ',';
	}
	//PrintString_("sever_addr");
	//PrintString_(sever_addr);
	return 0;
}

//配置协议固定长度256字节（包含头尾）
//配置协议分析
_Bool Para_config_protocol(char *input_protocol_string) 
{
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
  static u32 channel_en_bit = 0;
  u16 tmp_u16 =0;
  PrintString_("start to get parameter!");
  //PrintU32int2String_(channel_en_bit);
  
  //获取通道有效标志位
  channel_en_bit = *((u32 *)(Str2HexFun(input_protocol_string,CONFIG_BIT_FLAG_ADDR, 4)));
  PrintU32int2String_(channel_en_bit);
  //获取ID
  if (channel_en_bit & BIT_CONFIG_ID)
  {
    ArrayCopy(Sys_keydata.device_id, 
              Str2HexFun(input_protocol_string, CONFIG_ID_ADDR, CONFIG_ID_LEN/2), 
              CONFIG_ID_LEN/2);//ID号	字符串	16位
    Sys_keydata.device_id[16] = '\0';
    //ArrayCopy(Sys_keydata.device_id,Str2HexFun_16(input_protocol_string,CONFIG_ID_ADDR),16);
  }	
  
  //获取软件版本
  if (channel_en_bit & BIT_CONFIG_SOFT_VER)
  {
    //ArrayCopy(Sys_keydata.soft_version, Str2HexFun(input_protocol_string, CONFIG_SOFT_VER_ADDR, 2), 2);
    ArrayCopy(&Sys_keydata.harware_version[0], Str2HexFun(input_protocol_string, CONFIG_SOFT_VER_ADDR, 2) + 1, 1);//软件版本	字符串	2位，如“10”  
    
  }
  
  //获取硬件版本
  if (channel_en_bit & BIT_CONFIG_HARDWARE_VER)
  {
    //ArrayCopy(Sys_keydata.harware_version, Str2HexFun(input_protocol_string, CONFIG_HARDWARE_VER_ADDR, 2), 2);
    ArrayCopy(&Sys_keydata.harware_version[1], Str2HexFun(input_protocol_string, CONFIG_HARDWARE_VER_ADDR, 2) + 1, 1);//硬件版本	字符串	2位，如“10”
  }	
  //消防栓设备状态
  if (channel_en_bit & BIT_CONFIG_STATUS)
  {
    ArrayCopy(&Sys_keydata.status, 
              Str2HexFun(input_protocol_string, CONFIG_STATUS_ADDR, CONFIG_STATUS_LEN/2), 
              CONFIG_STATUS_LEN/2);     //设备状态	Uchar	库存状态 运输状态 待装状态 正常状态 
  }
  
  //
  if (channel_en_bit & BIT_CONFIG_SEND_TOTALCOUNT)
  {
    ArrayCopy((u8 *)&Sys_keydata.send_totalcount, 
              Str2HexFun(input_protocol_string, CONFIG_SEND_TOTALCOUNT_ADDR, CONFIG_SEND_TOTALCOUNT_LEN/2),
              CONFIG_SEND_TOTALCOUNT_LEN/2);  //Bit4	发送次数	UINT	0-65535  
  }	
  //
  if (channel_en_bit & BIT_CONFIG_ROTATION_TRIGGER_ANGLE)//旋转触发角度
  {
    ArrayCopy((u8 *)&Sys_keydata.rotation_trigger_angle,
              Str2HexFun(input_protocol_string, CONFIG_ROTATION_TRIGGER_ANGLE_ADDR, CONFIG_ROTATION_TRIGGER_ANGLE_LEN/2),
              CONFIG_ROTATION_TRIGGER_ANGLE_LEN/2);
    //Sys_keydata.heartdata_sent_interval_minute_count = (u16)(Sys_keydata.heartdata_sent_interval_minute_count / 30);
  }
  
  if (channel_en_bit & BIT_CONFIG_ROTATION_TRIGGER_SENS)//旋转灵敏度    Uchar    0-99   
  {
    ArrayCopy((u8 *)&Sys_keydata.rotation_trigger_sens,
              Str2HexFun(input_protocol_string, CONFIG_ROTATION_TRIGGER_SENS_ADDR, CONFIG_ROTATION_TRIGGER_SENS_LEN/2),
              CONFIG_ROTATION_TRIGGER_SENS_LEN/2);
  }
  if (channel_en_bit & BIT_CONFIG_LEAN_TRIGGER_ANGLE)//倾斜变化角度    Uchar    -90~90
  {
    ArrayCopy((u8 *)&Sys_keydata.lean_trigger_angle,
              Str2HexFun(input_protocol_string,CONFIG_LEAN_TRIGGER_ANGLE_ADDR, CONFIG_LEAN_TRIGGER_ANGLE_LEN/2),
              CONFIG_LEAN_TRIGGER_ANGLE_LEN/2);
  }	
  
  if (channel_en_bit & BIT_CONFIG_LEAN_TRIGGER_SENS )//倾斜灵敏度    Uchar    0-99      
  {
    ArrayCopy((u8 *)&Sys_keydata.lean_trigger_sens,
              Str2HexFun(input_protocol_string, CONFIG_LEAN_TRIGGER_SENS_ADDR, CONFIG_LEAN_TRIGGER_SENS_LEN/2),
              CONFIG_LEAN_TRIGGER_SENS_LEN/2);
  }	
  
  //
  if (channel_en_bit & BIT_CONFIG_PRESSURE_TRIGGER_VALUE)//压力变化阈值    UINT    1-65535 
  {
    ArrayCopy((u8 *)&Sys_keydata.pressure_trigger_value,
              Str2HexFun(input_protocol_string,CONFIG_PRESSURE_TRIGGER_VALUE_ADDR, CONFIG_PRESSURE_TRIGGER_VALUE_LEN/2),
              CONFIG_PRESSURE_TRIGGER_VALUE_LEN/2);
  }
  
  //
  if (channel_en_bit & BIT_CONFIG_PRESSURE_INTERVAL)//压力检测间隔    UINT    0-65535
  {
    ArrayCopy((u8 *)&Sys_keydata.pressure_interval, 
              Str2HexFun(input_protocol_string, CONFIG_PRESSURE_INTERVAL_ADDR, CONFIG_PRESSURE_INTERVAL_LEN/2), 
              CONFIG_PRESSURE_INTERVAL_LEN/2);
  }
  
//  if (channel_en_bit & BIT_CONFIG_HEART_INTERVAL)//心跳间隔时间	UINT	0-65535时 
//  {
//    ArrayCopy((u8 *)&Sys_keydata.heartdata_sent_interval_hour_count, 
//              Str2HexFun(input_protocol_string, CONFIG_HEART_INTERVAL_ADDR, CONFIG_HEART_INTERVAL_LEN/2), 
//              CONFIG_HEART_INTERVAL_LEN/2);
//  }
  if (channel_en_bit & BIT_CONFIG_HEART_INTERVAL)//心跳间隔时间	UINT	0-65535时 
  {
    tmp_u16 = *((u16 *)(Str2HexFun(input_protocol_string, CONFIG_HEART_INTERVAL_ADDR, CONFIG_HEART_INTERVAL_LEN/2)));
    if(0 != tmp_u16) //一定要保证写入的数据不为0
    {
      ArrayCopy((u8 *)&Sys_keydata.heartdata_sent_interval_hour_count, 
              (u8 *)(&tmp_u16), 
              CONFIG_HEART_INTERVAL_LEN/2);
    }
//    ArrayCopy((u8 *)&Sys_keydata.heartdata_sent_interval_minute_count, 
//              Str2HexFun(input_protocol_string, CONFIG_HEART_INTERVAL_ADDR, CONFIG_HEART_INTERVAL_LEN/2), 
//              CONFIG_HEART_INTERVAL_LEN/2);
  }  
  if (channel_en_bit & BIT_CONFIG_ALARM_INTERVAL)//心跳间隔时间	UINT	0-65535时 
  {
    ArrayCopy((u8 *)&Sys_keydata.alarm_sent_interval_minute_count, 
              Str2HexFun(input_protocol_string, CONFIG_ALARM_INTERVAL_ADDR, CONFIG_ALARM_INTERVAL_LEN/2), 
              CONFIG_ALARM_INTERVAL_LEN/2);
  }
  
//  if (channel_en_bit & BIT_CONFIG_TRIGGER_INTERVAL)//检测间隔时间	UINT	0-65535秒  
//  {
//    ArrayCopy((u8 *)&Sys_keydata.trigger_interval_minute_count, 
//              Str2HexFun(input_protocol_string, CONFIG_TRIGGER_INTERVAL_ADDR, CONFIG_TRIGGER_INTERVAL_LEN/2), 
//              CONFIG_TRIGGER_INTERVAL_LEN/2);
//    RTC_Config();  //直接配置
//  }
   if (channel_en_bit & BIT_CONFIG_TRIGGER_INTERVAL)//检测间隔时间	UINT	0-65535秒  
  {
    tmp_u16 = *((u16 *)(Str2HexFun(input_protocol_string, CONFIG_TRIGGER_INTERVAL_ADDR, CONFIG_TRIGGER_INTERVAL_LEN/2)));
    if(0 != tmp_u16) //一定要保证写入的数据不为0
    {
      ArrayCopy((u8 *)&Sys_keydata.trigger_interval_minute_count, 
              (u8 *)(&tmp_u16), 
              CONFIG_TRIGGER_INTERVAL_LEN/2);
      RTC_Config();  //直接配置
    }
    
//    ArrayCopy((u8 *)&Sys_keydata.trigger_interval_minute_count, 
//              Str2HexFun(input_protocol_string, CONFIG_TRIGGER_INTERVAL_ADDR, CONFIG_TRIGGER_INTERVAL_LEN/2), 
//              CONFIG_TRIGGER_INTERVAL_LEN/2);
    
  } 
  
  //获取可配采集地址
  if (channel_en_bit & BIT_CONFIG_CLOUD_SERVER_ADD) {
    ArrayCopy(Sys_keydata.default_cloud_server_add,
              Str2HexFun(input_protocol_string, CONFIG_CLOUD_SERVER_ADD_ADDR, CONFIG_CLOUD_SERVER_ADD_LEN/2),
              CONFIG_CLOUD_SERVER_ADD_LEN/2);
    Check_Sever_Addr((char *)Sys_keydata.default_cloud_server_add);
    Sys_keydata.default_cloud_server_add[CONFIG_CLOUD_SERVER_ADD_LEN/2] = '\0';
  }
  //获取升级服务器地址
  if (channel_en_bit & BIT_CONFIG_UPDATE_SERVER) 
  {
    ArrayCopy(Sys_keydata.update_server_add, 
              Str2HexFun(input_protocol_string, CONFIG_UPDATE_SERVER_ADDR, CONFIG_UPDATE_SERVER_LEN/2), 
              CONFIG_UPDATE_SERVER_LEN/2);
    //if(strpos(Sys_keydata.update_server_add,':') !== false)//替换冒号为逗号
    Check_Sever_Addr((char *)Sys_keydata.update_server_add);
    Sys_keydata.update_server_add[CONFIG_UPDATE_SERVER_LEN/2] = '\0';
  }
  
  //	//获取默认采集地址
  //	if (channel_en_bit & BIT_CONFIG_DEFAULT_CLOUD_SERVER) {
  //		ArrayCopy(Sys_keydata.default_cloud_server_add, Str2HexFun(input_protocol_string, CONFIG_DEFAULT_CLOUD_SERVER_ADDR, 21), 21);
  //		Check_Sever_Addr((char *)Sys_keydata.default_cloud_server_add);
  //		Sys_keydata.default_cloud_server_add[21] = '\0';
  //	}
  
  
  //获取运营商类型
  if (channel_en_bit & BIT_CONFIG_CARRIER_TYPE)
  {
    ArrayCopy(&Sys_keydata.carrier_type, 
              Str2HexFun(input_protocol_string,CONFIG_CARRIER_TYPE_ADDR, CONFIG_CARRIER_TYPE_LEN/2), 
              CONFIG_CARRIER_TYPE_LEN/2);
  }
  
  //获取模块类型
  if (channel_en_bit & BIT_CONFIG_MODULE_TYPE)
  {
    ArrayCopy(&Sys_keydata.module_type,
              Str2HexFun(input_protocol_string, CONFIG_MODULE_TYPE_ADDR, CONFIG_MODULE_TYPE_LEN/2), 
              CONFIG_MODULE_TYPE_LEN/2);
  }
  //物联网卡号
  if (channel_en_bit & BIT_CONFIG_CARD_ID)
  {
    ArrayCopy(Sys_keydata.Num_Cmd, 
              Str2HexFun(input_protocol_string, CONFIG_CARD_ID_ADDR, CONFIG_CARD_ID_LEN/2), 
              CONFIG_CARD_ID_LEN/2);//ID号	字符串	16位
    //ArrayCopy(Sys_keydata.device_id,Str2HexFun_16(input_protocol_string,CONFIG_ID_ADDR),16);
  }
  
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
  //时间戳
  if (channel_en_bit & BIT_TIMESTAMP_ID)
  {
    ArrayCopy((u8 *)&Sys_keydata.TimesTamp, 
              Str2HexFun(input_protocol_string, CONFIG_TIMESTAMP_ADDR, CONFIG_TIMESTAMP_LEN/2), 
              CONFIG_TIMESTAMP_LEN/2);//
    TickGetTime(Sys_keydata.TimesTamp); //根据时间戳配置时间。
#if SD3078_WIRTE_INIT_FLAG_EN
    RTC_SD3078_WriteInitFlag(); //写入标志
#endif
//    TimeGetTick();
  }
  
  if (channel_en_bit & BIT_PRESSURE_UPLIMIT_ID)//压力上限    UINT    0~65535
  {
    ArrayCopy((u8 *)&Sys_keydata.pressure_upLimit, 
              Str2HexFun(input_protocol_string, CONFIG_PRESSURE_UPLIMIT_ADDR, CONFIG_PRESSURE_UPLIMIT_LEN/2), 
              CONFIG_PRESSURE_UPLIMIT_LEN/2);//
  }
  if (channel_en_bit & BIT_PRESSURE_DOWNLIMIT_ID)//压力上限    UINT    0~65535
  {
    ArrayCopy((u8 *)&Sys_keydata.pressure_downLimit, 
              Str2HexFun(input_protocol_string, CONFIG_PRESSURE_DOWNLIMIT_ADDR, CONFIG_PRESSURE_DOWNLIMIT_LEN/2), 
              CONFIG_PRESSURE_DOWNLIMIT_LEN/2);//
  }
#endif  
  
  if(STATUS_CLEAR_FLASH == Sys_keydata.status) //检测到系统需要清除FLASH
  {
    debug_send("开始清除FLASH！");   
    ClearFLASH_Queue();
    Sys_keydata.status = STATUS_NORMAL; //恢复到正常状态。
  }
  //这里的告警位需要清除，不然在参数更新后保存告警值。
  Write_sys_keydata();
  
  Set_Key_Para();
  
  Usart3_RcvClr();
#endif
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
   static u32 channel_en_bit = 0;
  u16 tmp_u16 =0;
  PrintString_("start to get parameter!");
  //PrintU32int2String_(channel_en_bit);
  
  //获取通道有效标志位
  channel_en_bit = *((u32 *)(Str2HexFun(input_protocol_string,CONFIG_BIT_FLAG_ADDR, 4)));
  PrintU32int2String_(channel_en_bit);
  //获取ID
  if (channel_en_bit & BIT_CONFIG_ID)
  {
    ArrayCopy(Sys_keydata.device_id, 
              Str2HexFun(input_protocol_string, CONFIG_ID_ADDR, 16), 
              16);//ID号	字符串	16位
    //ArrayCopy(Sys_keydata.device_id,Str2HexFun_16(input_protocol_string,CONFIG_ID_ADDR),16);
  }	
  
  //获取软件版本
  if (channel_en_bit & BIT_CONFIG_SOFT_VER)
  {
    //ArrayCopy(Sys_keydata.soft_version, Str2HexFun(input_protocol_string, CONFIG_SOFT_VER_ADDR, 2), 2);
    ArrayCopy(&Sys_keydata.harware_version[0], Str2HexFun(input_protocol_string, CONFIG_SOFT_VER_ADDR, 2) + 1, 1);//软件版本	字符串	2位，如“10”  
    
  }
  
  //获取硬件版本
  if (channel_en_bit & BIT_CONFIG_HARDWARE_VER)
  {
    //ArrayCopy(Sys_keydata.harware_version, Str2HexFun(input_protocol_string, CONFIG_HARDWARE_VER_ADDR, 2), 2);
    ArrayCopy(&Sys_keydata.harware_version[1], Str2HexFun(input_protocol_string, CONFIG_HARDWARE_VER_ADDR, 2) + 1, 1);//硬件版本	字符串	2位，如“10”
  }	
  //设备状态
  if (channel_en_bit & BIT_CONFIG_STATUS)
  {
    ArrayCopy(&Sys_keydata.status, 
              Str2HexFun(input_protocol_string, CONFIG_STATUS_ADDR, 1), 
              1);     //设备状态	Uchar	库存状态 运输状态 待装状态 正常状态 
  }
  
  //发送次数
  if (channel_en_bit & BIT_CONFIG_SEND_TOTALCOUNT)
  {
    ArrayCopy((u8 *)&Sys_keydata.send_totalcount, 
              Str2HexFun(input_protocol_string, CONFIG_SEND_TOTALCOUNT_ADDR, 2),
              2);  //Bit4	发送次数	UINT	0-65535  
  }	
  
  if (channel_en_bit & BIT_CONFIG_PRESSURE_TRIGGER_VALUE)//Bit5	压力变化分辨率	UINT	1-65535
  {
    ArrayCopy((u8 *)&Sys_keydata.pressure_trigger_value,
              Str2HexFun(input_protocol_string,CONFIG_PRESSURE_TRIGGER_VALUE_ADDR, 2),
              2); 
  }

  if (channel_en_bit & BIT_CONFIG_HEART_INTERVAL)//心跳间隔时间	UINT	0-65535时 
  {
    tmp_u16 = *((u16 *)(Str2HexFun(input_protocol_string, CONFIG_HEART_INTERVAL_ADDR, 2)));
    if(0 != tmp_u16) //一定要保证写入的数据不为0
    {
      ArrayCopy((u8 *)&Sys_keydata.heartdata_sent_interval_hour_count, 
              (u8 *)(&tmp_u16), 
              2);
    }
  }  
  //Bit7	告警使能 2 0失能，1使能，失能，对应的告警标志和告警间隔会失效。
  if (channel_en_bit & BIT_CONFIG_ALARM_ENABLE_BIT)
  {
    ArrayCopy((u8 *)getAlarmEnableBit(),
              Str2HexFun(input_protocol_string,CONFIG_ALARM_ENABLE_BIT_ADDR, 2),
              2); 
  }
  //Bit8	上限告警间隔时间	2	1~65534 分钟65535表示取消
  if (channel_en_bit & BIT_CONFIG_PRESSURE_UPLIMIT_ALARM_INTERVAL)
  {
    ArrayCopy((u8 *)getPressureUpLimitAlarmInterval(),
              Str2HexFun(input_protocol_string,CONFIG_PRESSURE_UPLIMIT_ALARM_INTERVAL_ADDR, 2),
              2); 
  }
  //Bit9	下限告警间隔时间	2	1~65534 分钟65535表示取消
  if (channel_en_bit & BIT_CONFIG_PRESSURE_DOWN_LIMIT_ALARM_INTERVAL)
  {
    ArrayCopy((u8 *)getPressureDownLimitAlarmInterval(),
              Str2HexFun(input_protocol_string,CONFIG_PRESSURE_DOWN_LIMIT_ALARM_INTERVAL_ADDR, 2),
              2); 
  }
  //Bit10	未检测到传感器告警间隔。	2	1~65534 分钟65535表示取消
  if (channel_en_bit & BIT_CONFIG_NO_SENSOR_ALARM_INTERVAL)
  {
    ArrayCopy((u8 *)getNoSensorAlarmInterval(),
              Str2HexFun(input_protocol_string,CONFIG_NO_SENSOR_ALARM_INTERVAL_ADDR, 2),
              2); 
  }
  //Bit11	分辨率上报告警间隔	2	1~65534 分钟65535表示取消
  if (channel_en_bit & BIT_CONFIG_RESOLUTION_ALARM_INTERVAL)
  {
    ArrayCopy((u8 *)getResolutionAlarmInterval(),
              Str2HexFun(input_protocol_string,CONFIG_RESOLUTION_ALARM_INTERVAL_ADDR, 2),
              2); 
  }
  //Bit12	人工触发上报间隔	2	1~65534 分钟65535表示取消
  if (channel_en_bit & BIT_CONFIG_TOUCHOFF_ALARM_INTERVAL)
  {
    ArrayCopy((u8 *)getTouchOffAlarmInterval(),
              Str2HexFun(input_protocol_string,CONFIG_TOUCHOFF_ALARM_INTERVAL_ADDR, 2),
              2); 
  }
  
  if (channel_en_bit & BIT_CONFIG_TRIGGER_INTERVAL)//检测间隔时间	UINT	0-65535秒  
  {
    tmp_u16 = *((u16 *)(Str2HexFun(input_protocol_string, CONFIG_TRIGGER_INTERVAL_ADDR, 2)));
    if(0 != tmp_u16) //一定要保证写入的数据不为0
    {
      ArrayCopy((u8 *)&Sys_keydata.trigger_interval_minute_count, 
              (u8 *)(&tmp_u16), 
              2);
      RTC_Config();  //直接配置
    } 
  } 
  
  //获取可配采集地址
  if (channel_en_bit & BIT_CONFIG_CLOUD_SERVER_ADD) {
    ArrayCopy(Sys_keydata.default_cloud_server_add,
              Str2HexFun(input_protocol_string, CONFIG_CLOUD_SERVER_ADD_ADDR, 21),
              21);
    Check_Sever_Addr((char *)Sys_keydata.default_cloud_server_add);
    Sys_keydata.default_cloud_server_add[21] = '\0';
  }
  //获取升级服务器地址
  if (channel_en_bit & BIT_CONFIG_UPDATE_SERVER) 
  {
    ArrayCopy(Sys_keydata.update_server_add, 
              Str2HexFun(input_protocol_string, CONFIG_UPDATE_SERVER_ADDR, 21), 
              21);
    //if(strpos(Sys_keydata.update_server_add,':') !== false)//替换冒号为逗号
    Check_Sever_Addr((char *)Sys_keydata.update_server_add);
    Sys_keydata.update_server_add[21] = '\0';
  }
  //获取运营商类型
  if (channel_en_bit & BIT_CONFIG_CARRIER_TYPE)
  {
    ArrayCopy(&Sys_keydata.carrier_type, 
              Str2HexFun(input_protocol_string,CONFIG_CARRIER_TYPE_ADDR, 1), 
              1);
  }
  
  //获取模块类型
  if (channel_en_bit & BIT_CONFIG_MODULE_TYPE)
  {
    ArrayCopy(&Sys_keydata.module_type,
              Str2HexFun(input_protocol_string, CONFIG_MODULE_TYPE_ADDR, 1), 
              1);
  }
  //物联网卡号
  if (channel_en_bit & BIT_CONFIG_CARD_ID)
  {
    ArrayCopy(Sys_keydata.Num_Cmd, 
              Str2HexFun(input_protocol_string, CONFIG_CARD_ID_ADDR, 20), 
              20);//物联卡号字符串	物联卡号20位
    //ArrayCopy(Sys_keydata.device_id,Str2HexFun_16(input_protocol_string,CONFIG_ID_ADDR),16);
  }
  //时间戳
  if (channel_en_bit & BIT_TIMESTAMP_ID)
  {
    ArrayCopy((u8 *)&Sys_keydata.TimesTamp, 
              Str2HexFun(input_protocol_string, CONFIG_TIMESTAMP_ADDR, 4), 
              4);//
    TickGetTime(Sys_keydata.TimesTamp); //根据时间戳配置时间。
#if SD3078_WIRTE_INIT_FLAG_EN
    RTC_SD3078_WriteInitFlag(); //写入标志
#endif
//    TimeGetTick();
  }
  
  if (channel_en_bit & BIT_PRESSURE_UPLIMIT_ID)//压力上限    UINT    0~65535
  {
    ArrayCopy((u8 *)&Sys_keydata.pressure_upLimit, 
              Str2HexFun(input_protocol_string, CONFIG_PRESSURE_UPLIMIT_ADDR, 2), 
              2);//
  }
  if (channel_en_bit & BIT_PRESSURE_DOWNLIMIT_ID)//压力上限    UINT    0~65535
  {
    ArrayCopy((u8 *)&Sys_keydata.pressure_downLimit, 
              Str2HexFun(input_protocol_string, CONFIG_PRESSURE_DOWNLIMIT_ADDR, 2), 
              2);//
  }
  if(STATUS_CLEAR_FLASH == Sys_keydata.status) //检测到系统需要清除FLASH
  {
    debug_send("开始清除FLASH！");   
    //这里有重启的现象。
    Watchdog_Feed();
    ClearFLASH_Queue();
    Sys_keydata.status = STATUS_NORMAL; //恢复到正常状态。
  }
  Write_sys_keydata();
  
  Set_Key_Para();
  
  Usart3_RcvClr();
#endif
  return 1;
}




//标志位置一

//重启设备
void sys_restart(void) {
	//WWDG(WWDG_CR_WDGA)
  
  PrintString_("=============================");
	WWDG->CR |= WWDG_CR_WDGA;
	WWDG->CR &= ~WWDG_CR_T6;
}




//判断配置包校验码
_Bool CheckCheckSumFromConfigData(char *configdata) {
	HEX2STR_2 get_checksum = {'0', '0', '0', '0'};
	//u8 get_heartdata_len = 0;
	//get_heartdata_len = strlen(configdata);

	CalcChecksum(&get_checksum, configdata, PROTOCOL_CONFIG_TOTAL_LEN-6); //计算校验码
	PrintString("CHESUM :");
	PrintString_((char *)get_checksum.str2 + 0);

	//PrintString_(get_checksum.str2+1);
	//PrintString_(get_checksum.str2+2);
	//PrintString_(get_checksum.str2+3);

	//ArrayCmp 对比校验码
	if (ArrayCmp((u8 *)(&get_checksum), (u8 *)configdata + PROTOCOL_CONFIG_TOTAL_LEN-6, 4)) {
		PrintString_("CHESUM CORRECT");
		return 1;
	}
	else {
		PrintString_("CHESUM ERROR");
		return 0;
	}
}


//打包参数配置回复包的RTN
void Pack_Ack_Config_RTN(u8 rtn_value) {
	HEX2STR_1 temp_result = Hex2StrFun_1(&rtn_value);
	ArrayCopy(ack_protocol_config_array + 23, temp_result.str1, 2);
	ack_protocol_config_array[33] = 0x0D;
}


//回复串口配置
void Ack_config_cmd_2_pc(RTN_TypeDef rtn_value) {
	//回复串口配置

	Pack_Ack_Config_RTN(rtn_value);
	CalcChecksum((HEX2STR_2 *)(&ack_protocol_config_array[29]), (char *)(&ack_protocol_config_array[17]), 12); //计算校验码
	ack_protocol_config_array[33] = 0x0D;
	PrintString_("=");
	Usart_SendString(USART3, ack_protocol_config_array + 16, 20);
	//PrintStringWithlen_(ack_protocol_config_array+16,18);
	PrintString_("=");
}

void print_config_correct_with_chinese(void) {
	PrintString_("");
	PrintString_("***********************************");
	PrintString_("{{{{{OK!OK!OK!OK!OK!OK!OK!OK!OK!}}}}}");
	PrintString_("***********************************");
	char test_string[10] = {0xC5, 0xE4, 0xD6, 0xC3, 0xB3, 0xC9, 0xB9, 0xA6, 0xA3, 0xA1};
	PrintChinese(test_string);
}

void print_config_err_with_chinese(void) {
	PrintString_("");
	PrintString_("***********************************");
	PrintString_("{{{{{ERROR!ERROR!ERROR!ERROR!ERROR!}}}}}");
	PrintString_("***********************************");
	char test_string[10] = {0xC5, 0xE4, 0xD6, 0xC3, 0xCA, 0xA7, 0xB0, 0xDC, 0xA3, 0xA1};
	PrintChinese(test_string);
}



_Bool IsConfigConnect(USART_INFO *usartInfo) 
{
  u16 i = 0;
  u16 j = 0;
  if (usartInfo->usartLenPre < PROTOCOL_CONFIG_LEN)return 0;
  //for (i = 0; i <= (usartInfo->usartLenPre - PROTOCOL_CONFIG_LEN); i++) {
  for (i = 0; i < (usartInfo->usartLen - PROTOCOL_CONFIG_LEN); i++) 
  {
    if (usartInfo->usartBuf[i] == 0x7E) { //协议头
      for (j = 0; j < PROTOCOL_CONFIG_LEN; j++) {
        if ((j == 1) || (j == 2) || (j == 3) || (j == 4)) { //地址位
          
        } else { //其他位
          if (usartInfo->usartBuf[i + j] != protocol_config_array[j]) {
            //任何一个位不相等，退出当前开始的判断
            break;
          }
        }
      }
      if (j == PROTOCOL_CONFIG_LEN) 
      { //所有的都符合协议串，那么就是升级协议
        PrintString_("config parameter starting!");
//        PrintString_(usartInfo->usartBuf); //by hx 2017-06-15 
        //if (Para_config_protocol((char *)usartInfo->usartBuf)) {
        
        //核对长度
        ///////////////////////////////////////////////////////
        for (u16 temp_i = 1; temp_i < USARTBUF_MAX; temp_i++) 
        {
          if ((i + temp_i) < USARTBUF_MAX) 
          { //没有超出缓冲区
            if (temp_i == CONFIG_LEN - 1) 
            {//最后个是否是0x0D  ，
              //如果是0x0D 则表示正确 跳出判断
              if (usartInfo->usartBuf[i + PROTOCOL_CONFIG_TOTAL_LEN-1] == 0x0D) 
              {
                break;
              } 
              else 
              {
                PrintString("configdata length err!");
                PrintString("none end string !");
                PrintU8int2String_(temp_i);
                Ack_config_cmd_2_pc(RTN_ERR_INVALID);///回复串口参数配置 RTN_ERR_INVALID
                print_config_err_with_chinese();
                return 0;//直接返回错误 return 0
              }
            } 
            else 
            { //中间的数字不得是0x0D 否则表示长度不对
              if (usartInfo->usartBuf[i + temp_i] == 0x0D) {
                //中间有数字等于0x0D 表示长度不对
                //调试输出打印信息
                PrintString("configdata length err!");
                PrintU8int2String_(temp_i);
                
                Ack_config_cmd_2_pc(RTN_ERR_INVALID);//回复串口参数配置 RTN_ERR_INVALID
                print_config_err_with_chinese();
                return 0;//直接返回错误 return 0
                
              }
            }
          }
          else { //超出缓冲区 没找到结尾0X0D 也代表长度不对
            PrintString("configdata length err!");
            PrintString("none end string in the buf!");
            PrintU8int2String_(temp_i);
            
            Ack_config_cmd_2_pc(RTN_ERR_INVALID);//回复串口参数配置 RTN_ERR_INVALID
            print_config_err_with_chinese();
            return 0;//直接返回错误 return 0
          }
          
        }
        PrintString("configdata length check pass!");
        ///////////////////////////////////////////////////////
        //核对校验码
        ///////////////////////////////////////////////////////
        if (!CheckCheckSumFromConfigData((char *)usartInfo->usartBuf + i + 1)) 
        {
          Ack_config_cmd_2_pc(RTN_ERR_CHKSUM);//回复串口参数配置 RTN_ERR_CHKSUM
          print_config_err_with_chinese();
          return 0;//直接返回错误 return 0
        }
        PrintString("configdata checksum check pass!");
        ///////////////////////////////////////////////////////
        //核对格式是否符合要求
        ///////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////
        Para_config_protocol((char *)(usartInfo->usartBuf + i));//获取参数
        
        
        
        Ack_config_cmd_2_pc(RTN_OK);//回复串口参数配置 RTN_OK
        print_config_correct_with_chinese();
        return 1;
      }
    }
  }
  return 0;
}

_Bool IsUpdataConnect(void) {
	u16 i = 0;
	u16 j = 0;
	if (usart3Info.usartLenPre < PROTOCOL_UPDATA_LEN)return 0;
	for (i = 0; i <= (usart3Info.usartLenPre - PROTOCOL_UPDATA_LEN); i++) {

		if (usart3Info.usartBuf[i] == 0xAA) { //协议头
			for (j = 0; j < PROTOCOL_UPDATA_LEN; j++) {
				if ((j == 1) && (j == 2)) { //地址位

				} else if (j == 6) { //校验位1

				} else if (j == 7) { //校验位2

				} else { //其他位
//					if (usart3Info.usartBuf[i + j] != protocol_updata_array[j]) {
//						//任何一个位不相等，退出当前开始的判断
//						break;
//					}
				}
			}
			if (j == PROTOCOL_UPDATA_LEN) { //所有的都符合协议串，那么就是升级协议
				PrintString_("updata starting!");
				return 1;
			}
		}
	}
	return 0;
}





//协议分析
_Bool Select_protocol(USART_INFO *usartInfo) 
{
  UsartReciveFlag(usartInfo);
  if (usartInfo->usartReceiveFlag == REV_OK)
  {
    usartInfo->usartReceiveFlag = REV_WAIT;
    if (IsConfigConnect(usartInfo)) //是否是参数配置协议
    { 
      Usart3_RcvClr();//清除缓存
      return 1;
    } 
    else 
    {
      Usart3_RcvClr();//20170216新增
      return 0;
    }
  }
  return 0;
}



//////////////数组拷贝给数组////////////////////////////
void ArrayCopy(u8 *dest, u8 *src, u8 len) {
	while (len--)
	{
		*dest++ = *src++;
	}
}
////////////////////////////////////////////


//////////////数组是否相同数组////////////////////////////
_Bool ArrayCmp(u8 *dest, u8 *src, u8 len) {
	while (len--)
	{
		if (*dest++ != *src++)return 0;
	}
	return 1;
}
////////////////////////////////////////////

_Bool Is_string_hex_format(char *protol_string, u8 len) {
	for (u8 i = 0; i < len; i++) {
		if (((*protol_string >= '0') && (*protol_string <= '9')) || \
		        ((*protol_string >= 'A') && (*protol_string <= 'F'))) {

		}
		else {
			return 0;
		}
	}
	return 1;
}

//检查所有码是否符合协议标准
_Bool Isformatcorrect(char *protol_string) {
	//校对长度

	//校对所有值 都是可见码 都是0-9 A-F的ascii码范围
	//if(!Is_string_hex_format(,248)){
	//	return 0;
	//}
	//校对校验码
	return 1;
}

_Bool Is_string_numformat(char *tempstr) {

	return 0;
}
///////////////////////协议内的ascii码转协议码转换////////////////
u8* Str2HexFun_16(char *tempstr, u8 addr) {

	//PrintString_("result_u16 transfor");
	//PrintString_(tempstr+addr);
	for (u8 i = 0; i < 16; i++) {
		if ((*(tempstr + addr + (i << 1)) >= '0') && (*(tempstr + addr + (i << 1)) <= '9')) {
			//如果不超9
			result_u16[i] = ((*(tempstr + addr + (i << 1))) - '0') << 4;
		} else if ((*(tempstr + addr + (i << 1)) >= 'A') && (*(tempstr + addr + (i << 1)) <= 'F')) {
			//如果超过9
			result_u16[i] = ((*(tempstr + addr + (i << 1) + 1)) - 'A' + 10) << 4;
		}
		//else{
		//	PrintString_("error data!");
		//}
		if ((*(tempstr + addr + (i << 1) + 1) >= '0') && (*(tempstr + addr + (i << 1) + 1) <= '9')) {
			//如果不超9
			result_u16[i] |= ((*(tempstr + addr + (i << 1) + 1)) - '0');
		} else if ((*(tempstr + addr + (i << 1) + 1) >= 'A') && (*(tempstr + addr + (i << 1) + 1) <= 'F')) {
			//如果超过9
			result_u16[i] |= ((*(tempstr + addr + (i << 1) + 1)) - 'A' + 10);
		}
	}
	//PrintString_("result_u16");
	//PrintString_((char *)result_u16);
	return result_u16;
}

u8* Str2HexFun(char *tempstr, u16 addr, u8 len) {

	//PrintString_("result_u16 transfor");
	//PrintString_(tempstr+addr);
	for (u8 i = 0; i < len; i++) {
		//高字节
		if (((*(tempstr + addr + (i << 1))) >= '0') && (*(tempstr + addr + (i << 1)) <= '9')) {
			//如果不超9
			result_u16[i] = ((*(tempstr + addr + (i << 1))) - '0') << 4;
		} else if ((*(tempstr + addr + (i << 1)) >= 'A') && (*(tempstr + addr + (i << 1)) <= 'F')) {
			//如果超过9
			result_u16[i] = ((*(tempstr + addr + (i << 1))) - 'A' + 10) << 4;
		}
		else {
			PrintString_("ERRPOR!!!");
			PrintU8int2String_((*(tempstr + addr + (i << 1))));
		}
		//else{
		//	PrintString_("error data!");
		//}
		//低字节
		if ((*(tempstr + addr + (i << 1) + 1) >= '0') && (*(tempstr + addr + (i << 1) + 1) <= '9')) {
			//如果不超9
			result_u16[i] |= ((*(tempstr + addr + (i << 1) + 1)) - '0');
		} else if ((*(tempstr + addr + (i << 1) + 1) >= 'A') && (*(tempstr + addr + (i << 1) + 1) <= 'F')) {
			//如果超过9
			result_u16[i] |= ((*(tempstr + addr + (i << 1) + 1)) - 'A' + 10);
		}
		else {
			PrintString_("ERRPOR!!!");
			PrintU8int2String_((*(tempstr + addr + (i << 1) + 1)));
		}
	}
	//PrintString_("result_u16");
	//PrintString_((char *)result_u16);
	return result_u16;
}
/*
u32 Str2HexFun_16(char *tempstr, u8 addr) {
	u32 result_u32 = 0;

}*/
HEX2STR_21 Hex2StrFun_21(char *tempstr) {//转换IP地址为
	HEX2STR_21 outputdata = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
	u8 inputdata = 0;
	u8 tempstr_len = strlen((const char *)tempstr);
	for (u8 j = 0; j < tempstr_len; ++j)
	{
		inputdata = *(tempstr + j);
		for (u8 i = 0; i < 2; ++i)
		{
			u8 shiftdata = (inputdata >> ((1 - i) << 2) & 0x0f);
			if (shiftdata < 10 ) {
				outputdata.str21[(j << 1) + i] = '0' + shiftdata;
			} else {
				outputdata.str21[(j << 1) + i] = 'A' + (shiftdata - 10);
			}
			//sprintf(str, "chesum %d", outputdata.str2[i]);
			//debug_send(str);
		}
	}
	return outputdata;
}

HEX2STR_16 Hex2StrFun_16(char *tempstr) {
	HEX2STR_16 outputdata = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
	u8 inputdata = 0;
	u8 tempstr_len = strlen((const char *)tempstr);
	for (u8 j = 0; j < tempstr_len; ++j)
	{
		inputdata = *(tempstr + j);
		for (u8 i = 0; i < 2; ++i)
		{
			u8 shiftdata = (inputdata >> ((1 - i) << 2) & 0x0f);
			if (shiftdata < 10 ) {
				outputdata.str16[(j << 1) + i] = '0' + shiftdata;
			} else {
				outputdata.str16[(j << 1) + i] = 'A' + (shiftdata - 10);
			}
			//sprintf(str, "chesum %d", outputdata.str2[i]);
			//debug_send(str);
		}
	}
	return outputdata;
}

HEX2STR_13 Hex2StrFun_13(char *tempstr) {
	HEX2STR_13 outputdata = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
	u8 inputdata = 0;
	u8 tempstr_len = strlen((const char *)tempstr);
	for (u8 j = 0; j < tempstr_len; ++j)
	{
		inputdata = *(tempstr + j);
		for (u8 i = 0; i < 2; ++i)
		{
			u8 shiftdata = (inputdata >> ((1 - i) << 2) & 0x0f);
			if (shiftdata < 10 ) {
				outputdata.str13[(j << 1) + i] = '0' + shiftdata;
			} else {
				outputdata.str13[(j << 1) + i] = 'A' + (shiftdata - 10);
			}
			//sprintf(str, "chesum %d", outputdata.str2[i]);
			//debug_send(str);
		}
	}
	return outputdata;
}

HEX2STR_1 Hex2StrFun_1(u8 *addr) {
	HEX2STR_1 outputdata = {'0', '0'};
	u8 inputdata = *addr;
	for (u8 i = 0; i < 2; ++i)
	{
		u8 shiftdata = (inputdata >> ((1 - i) << 2) & 0x0f);
		if (shiftdata < 10 ) {
			outputdata.str1[i] = '0' + shiftdata;
		} else {
			outputdata.str1[i] = 'A' + (shiftdata - 10);
		}
		//sprintf(str, "chesum %d", outputdata.str2[i]);
		//debug_send(str);
	}
	return outputdata;
}


HEX2STR_2 Hex2StrFun_2(u16 *addr) {
	HEX2STR_2 outputdata = {'0', '0', '0', '0'};
	u16 inputdata = *addr;
	for (u8 i = 0; i < 4; ++i)
	{
		u8 shiftdata = (inputdata >> ((3 - i) << 2) & 0x000f);
		if (shiftdata < 10 ) {
			outputdata.str2[i] = '0' + shiftdata;
		} else {
			outputdata.str2[i] = 'A' + (shiftdata - 10);
		}
//		sprintf(str, "chesum %d", outputdata.str2[i]);
//		debug_send(str);
	}
	return outputdata;
}





char *num2string(u16 tempnum) {
	char* result_str = "00000";
	result_str[0] = '0' + (tempnum / 10000) % 10;
	result_str[1] = '0' + (tempnum / 1000) % 10;
	result_str[2] = '0' + (tempnum / 100) % 10;
	result_str[3] = '0' + (tempnum / 10) % 10;
	result_str[4] = '0' + (tempnum / 1) % 10;
	return result_str;
}
char *intnum2hex(u16 tempnum1) {
	static char* result_str = "0000";
	static u16 intnum_in;
	intnum_in = tempnum1;
	u8 temp_4bit = 0;
	for (u8 i = 0; i < 4; i++) {
		temp_4bit = (intnum_in >> ((3 - i) << 2)) & 0x0f;
		if (temp_4bit <= 9) {
			result_str[i] = '0' + temp_4bit;
		} else {
			result_str[i] = 'A' + temp_4bit - 10;
		}
	}
	return result_str;
}
///////////////////////////////////////////////////////////////////