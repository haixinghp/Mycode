#include "stm8l15x.h"
#include <string.h>
#include "config.h"


u8 sysFunState = 0;
u16 sysFunTimecount = 0;
//ADXL_XYZ_ANGLE temp_xyz_angle[10];

const char NO[16]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46};

//u8 getXyzZeroposFunState = 0;
//u8 getXyzOffsetAngleFunState = 0;
//u16 getXyzOffsetAngleTimecount = 0;
//u8 SetZeroPosFunState = 0;
//u16 SetZeroPosTimecount = 0;

//Liquid_Alarm LiquidAlarm;
ADXL362_PARA Adxl362Para;


u16 heart_timecount = 0;
_Bool heart_send_flag = 0;
char *tempstring = "";

u16 waitforconfig_timecount = 0;
_Bool para_config_flag = 0;

u16 sys_overtime_count = 0;//超时函数


//static u8 AnalyseData_State = 0;
ANALYSE_DATA_PARA Analyse_data_para = {
	20000,//超时时间 默认20S //over_time;//
	0,//零点角度 默认0度 //zero_angle;//
	6,//设定角度 默认6度 //set_angle;//
	0,//当前角度 默认0度 // u8 present_angle;//
	3,//有效角度差值 默认2度 //valid_angle_d_value;//
	6,//设定的最长有效角度连续数目 默认8个 set_valid_angle_unbroken_num;//
	0,//有效角度连续数目 默认0个 //valid_angle_unbroken_num;//
	0,//状态机状态值 默认0 analyse_state;//
	0,//静态标志 默认0 u8 inact_flag :1;//上电默认为动态
	0,//报警标志 默认0 alarm_flag :1;//
        0,//二次确认标志 默认0 sencond_query_flag
        0,//二次确认开始发送标志，默认0 sencond_query_start_flag
        0,//二次确认结束发送标志，默认0 sencond_query_finish_flag
        0,//发送忙标志，默认0 send_busy_flag
};

#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
const SYS_KEYDATA Default_Sys_keydata =
{  /***************************这里的参数需要本地协议配置得到******************************/

  DEVICE_ID,//u8 device_id[17];//设备ID
  3,//u8 soft_version[2];//软件版本//修改协议，另定软件版本固定：SOFTWARE_VER，心跳、12
  2,//u8 harware_version[2];//硬件版本 //修改协议，硬件版本增加至两个字节，如V12.34==0x0c,0x22
  0,//u8 status; //设备状态    Uchar    库存状态 运输状态 待装状态 正常状态
  0,//u8 SigStren;//信号强度
  // 电池电压
  0,//u16 send_totalcount;//电池发送总次数
//  //环境温度
  10,//u8 rotation_trigger_angle;//旋转触发角度
  10,//u8 rotation_trigger_sens;//旋转触发灵敏度
  10,//u8 lean_trigger_angle;//倾斜触发角度
  10,//u8 lean_trigger_sens;//倾斜触发灵敏度
  100,//u8 pressure_trigger_value;//压力触发阈值
  10,//u8 pressure_interval;//压力检测间隔
  0,//u16 Alarm;//告警
  4,//u16 heartdata_sent_interval_hour_count;//心跳间隔时间
  2,//u16 alarm_sent_interval_minute_count;//报警间隔时间
  30,//u16 trigger_interval_minute_count;//触发间隔时间
  DEFAULT_CLOUD_SERVER_ADD,//u8 default_cloud_server_add[22];//云端服务器地址255.255.255.255,65535
  DEFAULT_CLOUD_SERVER_ADD,//u8 update_server_add[22];//升级服务器地址255.255.255.255,65535
  1,//u8 carrier_type;//运营商   不可修改 绑定硬件版本
  1,//u8 module_type;//模块制式 不可修改 绑定硬件版本
  IOT_NUM,//u8 Num_Cmd[21];  //物联网卡号。
//#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
//  0,//s32 TimesTamp;  //时间戳 
//  800,//pressure_upLimit; //压力上限
//  100,//u16 pressure_downLimit; //压力下限
//#endif  
//  /*************************************************************************/ 
  0,//u16 pressure_value;//压力
  0,//u16 rotation_angle;//旋转角度
  0,//u8 lean_angle;//倾斜角度
//#endif
//#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
//  DEVICE_ID,//u8 device_id[17];//设备ID
//#endif  
};
#endif 
#if APP_SELECT_COVER
SYS_KEYDATA Sys_keydata = 
{  /***************************这里的参数需要本地协议配置得到******************************/
 //一下代码是闷盖程序特有的代码
  DEVICE_ID,//u8 device_id[17];//设备ID
  3,//u8 soft_version[2];//软件版本//修改协议，另定软件版本固定：SOFTWARE_VER，心跳、12
  2,//u8 harware_version[2];//硬件版本 //修改协议，硬件版本增加至两个字节，如V12.34==0x0c,0x22
  0,//u8 status; //设备状态    Uchar    库存状态 运输状态 待装状态 正常状态
  0,//u8 SigStren;//信号强度
  // 电池电压
  0,//u16 send_totalcount;//电池发送总次数
//  //环境温度
  10,//u8 rotation_trigger_angle;//旋转触发角度
  10,//u8 rotation_trigger_sens;//旋转触发灵敏度
  10,//u8 lean_trigger_angle;//倾斜触发角度
  10,//u8 lean_trigger_sens;//倾斜触发灵敏度
  100,//u8 pressure_trigger_value;//压力触发阈值
  10,//u8 pressure_interval;//压力检测间隔
  0,//u16 Alarm;//告警
  4,//u16 heartdata_sent_interval_hour_count;//心跳间隔时间
  2,//u16 alarm_sent_interval_minute_count;//报警间隔时间
  30,//u16 trigger_interval_minute_count;//触发间隔时间
  DEFAULT_CLOUD_SERVER_ADD,//u8 default_cloud_server_add[22];//云端服务器地址255.255.255.255,65535
  DEFAULT_CLOUD_SERVER_ADD,//u8 update_server_add[22];//升级服务器地址255.255.255.255,65535
  1,//u8 carrier_type;//运营商   不可修改 绑定硬件版本
  1,//u8 module_type;//模块制式 不可修改 绑定硬件版本
  IOT_NUM,//u8 Num_Cmd[21];  //物联网卡号。
//  0,//s32 TimesTamp;  //时间戳 
//  800,//pressure_upLimit; //压力上限
//  100,//u16 pressure_downLimit; //压力下限
//  /*************************************************************************/ 
  0,//u16 pressure_value;//压力
  0,//u16 rotation_angle;//旋转角度
  0,//u8 lean_angle;//倾斜角度

};
#endif
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
SYS_KEYDATA Sys_keydata=
{
  DEVICE_ID_PRESSURE, // u8 device_id[17];//设备ID
  0x0C,
  0X22,//u8 harware_version[2];//硬件版本 //修改协议，硬件版本增加至两个字节，如V12.34==0x0c,0x22
  0,//u8 status; //设备状态    Uchar    库存状态 运输状态 待装状态 正常状态
  0,//u8 SigStren;//信号强度
  0,//u16 send_totalcount;//电池发送总次数
  0,//u16 pressure_value;//压力
  0,//u16 SamplingValue; //采样原始值
  0,//u16 pressure_trigger_value;//压力变化分辨率
  0, //u16 Alarm;//告警
  24, //u16 heartdata_sent_interval_hour_count;//心跳间隔时间
  0x00, //u16 AlarmEnableBit; //告警使能0失能，1使能
  120,//上限告警间隔时间
  120,//下限告警间隔时间
  120,//未检测到传感器告警间隔
  120,//分辨率上报告警间隔
  65535,//人工触发上报间隔
  30,//触发间隔时间
  DEFAULT_CLOUD_SERVER_ADD,//云端服务器地址255.255.255.255,65535
  DEFAULT_CLOUD_SERVER_ADD,//升级服务器地址255.255.255.255,65535
  0,
  0,
  IOT_NUM,//物联网卡号。
  0,//时间戳
  600,//压力上限
  300,//压力下限
};

const SYS_KEYDATA Default_Sys_keydata=
{
  DEVICE_ID_PRESSURE, // u8 device_id[17];//设备ID
  0x0C,
  0X22,//u8 harware_version[2];//硬件版本 //修改协议，硬件版本增加至两个字节，如V12.34==0x0c,0x22
  0,//u8 status; //设备状态    Uchar    库存状态 运输状态 待装状态 正常状态
  0,//u8 SigStren;//信号强度
  0,//u16 send_totalcount;//电池发送总次数
  0,//u16 pressure_value;//压力
  0,//u16 SamplingValue; //采样原始值
  100,//u16 pressure_trigger_value;//压力变化分辨率
  0, //u16 Alarm;//告警
  24, //u16 heartdata_sent_interval_hour_count;//心跳间隔时间
  0x00, //u16 AlarmEnableBit; //告警使能0失能，1使能
  120,//上限告警间隔时间
  120,//下限告警间隔时间
  120,//未检测到传感器告警间隔
  120,//分辨率上报告警间隔
  65535,//人工触发上报间隔
  10,//触发间隔时间
  DEFAULT_CLOUD_SERVER_ADD,//云端服务器地址255.255.255.255,65535
  DEFAULT_CLOUD_SERVER_ADD,//升级服务器地址255.255.255.255,65535
  0,
  0,
  IOT_NUM,//物联网卡号。
  0,//时间戳
  600,//压力上限
  300,//压力下限
};
/***************************************************************************************
**函数名:       SamplingValue
**功能描述:     SamplingValue接口函数。
**输入参数:     --
**输出参数:     u16 *
**备注:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
void SysKeydataInit()
{
//  ArrayToArray(Sys_keydata.device_id,DEVICE_ID,17); // u8 device_id[17];//设备ID
  Sys_keydata.harware_version[0] = 0x0C;
  Sys_keydata.harware_version[1] = 0X22;//u8 harware_version[2];//硬件版本 //修改协议，硬件版本增加至两个字节，如V12.34==0x0c,0x22
  Sys_keydata.status = 0;//u8 status; //设备状态    Uchar    库存状态 运输状态 待装状态 正常状态
  Sys_keydata.SigStren = 0;//u8 SigStren;//信号强度
  Sys_keydata.send_totalcount = 0;//u16 send_totalcount;//电池发送总次数
  Sys_keydata.pressure_value = 0;//u16 pressure_value;//压力
  Sys_keydata.SamplingValue = 0;//u16 SamplingValue; //采样原始值
  Sys_keydata.pressure_trigger_value = 0;//u16 pressure_trigger_value;//压力变化分辨率
  Sys_keydata.Alarm = 0; //u16 Alarm;//告警
  Sys_keydata.heartdata_sent_interval_hour_count = 24; //u16 heartdata_sent_interval_hour_count;//心跳间隔时间
  Sys_keydata.AlarmEnableBit = 0xFF; //u16 AlarmEnableBit; //告警使能0失能，1使能
  Sys_keydata.PressureUpLimitAlarmInterval = 4;//上限告警间隔时间
  Sys_keydata.PressureDownLimitAlarmInterval = 4;//下限告警间隔时间
  Sys_keydata.NoSensorAlarmInterval =24;//未检测到传感器告警间隔
  Sys_keydata.ResolutionAlarmInterval = 4;//分辨率上报告警间隔
  Sys_keydata.TouchOffAlarmInterval = 65535;//人工触发上报间隔
  Sys_keydata.trigger_interval_minute_count = 30;//触发间隔时间
  ArrayToArray(Sys_keydata.default_cloud_server_add,DEFAULT_CLOUD_SERVER_ADD,22);//云端服务器地址255.255.255.255,65535
  ArrayToArray(Sys_keydata.update_server_add,DEFAULT_CLOUD_SERVER_ADD,22);//升级服务器地址255.255.255.255,65535
  Sys_keydata.carrier_type = 0;
  Sys_keydata.module_type = 0;
  ArrayToArray(Sys_keydata.Num_Cmd,IOT_NUM,21);//物联网卡号。
  Sys_keydata.TimesTamp = 0;//时间戳
  Sys_keydata.pressure_upLimit = 600;//压力上限
  Sys_keydata.pressure_downLimit = 300;//压力下限
}
/***************************************************************************************
**函数名:       SamplingValue
**功能描述:     SamplingValue接口函数。
**输入参数:     --
**输出参数:     u16 *
**备注:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getSamplingValue()
{
  return &Sys_keydata.SamplingValue;
}
/***************************************************************************************
**函数名:       AlarmEnableBit
**功能描述:     AlarmEnableBit接口函数。
**输入参数:     --
**输出参数:     u16 *
**备注:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getAlarmEnableBit()
{
  return &Sys_keydata.AlarmEnableBit;
}

/***************************************************************************************
**函数名:       getPressureUpLimitAlarmInterval
**功能描述:     PressureUpLimitAlarmInterval接口函数。
**输入参数:     --
**输出参数:     u16 *
**备注:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getPressureUpLimitAlarmInterval()
{
  return &Sys_keydata.PressureUpLimitAlarmInterval;
}
/***************************************************************************************
**函数名:       PressureDownLimitAlarmInterval
**功能描述:     PressureDownLimitAlarmInterval接口函数。
**输入参数:     --
**输出参数:     u16 *
**备注:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getPressureDownLimitAlarmInterval()
{
  return &Sys_keydata.PressureDownLimitAlarmInterval;
}
/***************************************************************************************
**函数名:       NoSensorAlarmInterval
**功能描述:     NoSensorAlarmInterval接口函数。
**输入参数:     --
**输出参数:     u16 *
**备注:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getNoSensorAlarmInterval()
{
  return &Sys_keydata.NoSensorAlarmInterval;
}

/***************************************************************************************
**函数名:       ResolutionAlarmInterval
**功能描述:     ResolutionAlarmInterval接口函数。
**输入参数:     --
**输出参数:     u16 *
**备注:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getResolutionAlarmInterval()
{
  return &Sys_keydata.ResolutionAlarmInterval;
}

/***************************************************************************************
**函数名:       ResolutionAlarmInterval
**功能描述:     ResolutionAlarmInterval接口函数。
**输入参数:     --
**输出参数:     u16 *
**备注:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getTouchOffAlarmInterval()
{
  return &Sys_keydata.TouchOffAlarmInterval;
}
/***************************************************************************************
**函数名:       getPressureUpLimit
**功能描述:     接口函数。
**输入参数:     --
**输出参数:     u16 *
**备注:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getPressureUpLimit()
{
  return &Sys_keydata.pressure_upLimit;
}

/***************************************************************************************
**函数名:       getPressureDownLimit
**功能描述:     接口函数。
**输入参数:     --
**输出参数:     u16 *
**备注:         YY.c,2017.12.08,By Haixing.Huang
****************************************************************************************/
u16 *getPressureDownLimit()
{
  return &Sys_keydata.pressure_downLimit;
}
#endif

//#define PRESENT_TEMP_ID '3','2','3','0','3','0','3','0','3','2','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'
//#define PRESENT_TEMP_ID '3','2','3','0','3','0','3','0','3','2','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'

#define FILTER_AT_OF_UART3 if(USARTx!=USART3)
//HEART_DATA Heart_data ={
//  0
//};
//union UNION_PACKET_DATA PacketData = {
//	'a', 't', '+', 'z', 'i', 'p', 's', 'e', 'n', 'd', '=', '0', ',', '2', '5', '0', 0x0D,
//	0x7E,//SOI
//	'1', '1', //VER
//	'0', '1', //ADR
//	'D', '0', //CID1
//	'1', 'A', //CID2
//	//'B', '0', 'B', 'A', //LEN
//        'A', '0', 'E', '8', //LEN
//	'0', '1', //产品类型 PdType_Cmd
//	'0', '0', //设备类型 DvType_Cmd
//	'3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', '3', '0', //设备序号 Id_Cmd
//	'4', '3', '4', 'C', '4', '4', '5', 'A', //客户码 Cstm_Cmd
//	'3', '1', '3', '3', '3', '7', '3', '3', '3', '6', '3', '7', '3',
//	'2', '3', '6', '3', '6', '3', '6', '3', '4', '3', '8', '3', '8', //物联网卡号 Num_Cmd
//	'3', '2', '3', '6', //固件版本 Firm_Cmd
//	'0', '0', //设备状态 State_Cmd
//	'1', '7', //信号强度 SigStren_Cmd
//	'0', '0', //剩余电压 ResV_Cmd
//	'1', '9', //环境温度 Temp_Cmd
//	'0', '9', //倾斜角度 Angle_Cmd
//	'0', '0', '0', '0', //X轴加速度 X_Value_Cmd
//	'0', '0', '0', '0', //Y轴加速度 Y_Value_Cmd
//	'0', '0', '0', '0', //Z轴加速度 Z_Value_Cmd
//	'0', '0', //低压报警 Low_Power_Alarm_Cmd
//	'0', '0', //低温报警 Low_Temp_Alarm_Cmd
//	'0', '0', //触发报警 Trigger_Alarm_Cmd
//	'0', '0', //触发角度 Trigger_Angle_Cmd
//	'0', '0', '0', '2', //触发间隔 Trigger_Interval_Cmd
//	'0', '0', '1', '8', //报警间隔 Alarm_Interval_Cmd(心跳间隔)
//	'0', '1', '2', 'C', //二次确认间隔 Sencond_Query_Interval_Cmd 默认5分钟（300秒）
//	'0', '0', //灵敏度 Sens_Cmd
//	'0', '0', '0', '3', //累计发送次数 Send_Totalcount_cmd //重连间隔Rec_Interval_Cmd
//	'0', '3', //最大重连次数 Max_Count_Rec_Cmd
//	'0', '0', //当前重连次数 Pre_Count_Rec_Cmd
//	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 
//	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', //当前采集地址
//	'0', '0', //运营商 Carrier_Cmd
//	'0', '0', //模块制式 Mod_Type_Cmd
//	'0', '0', //MNC Mnc_Cmd
//	'0', '0', '0', '0', //LAC/TAC Lac_Tac_Cmd
//	'0', '0', '0', '0', '0', '0', '0', '0', //CI Ci_Cmd
//	'0', '0', '0', '0', //SID Sid_Cmd
//	'0', '0', '0', '0', //NID Nid_Cmd
//	'0', '0', '0', '0', //BID Bid_Cmd
//	'0', '0', //GPS 模块类型 Gps_Type_Cmd
//	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', //GPS经度 Gps_Long_Cmd
//	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', //GPS纬度 Gps_Lat_Cmd
//	'0', '0', '0', '0', //校验码 Chksum_Cmd
//	0x0D,//EOI
//	'\r', '\n'
//};

//void TaskSchedule(void);//任务切换
//void Test_protocol(void);//协议测试
u16 task_schedule_timecount = 0;

u8 TashSchedule_State = 0;


//task scheduling
void TaskSchedule(void) 
{
  switch (TashSchedule_State) 
  {
  case 0:
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
        PressureSysFun();  //水压业务流程程序
#endif
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
        CoverSysFun(); //闷盖业务流程程序
        NewInactAngle_Service();
#endif      
    break;
  case 1://串口有数据
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
//       NewInactAngle_SetSamplingLockFlag();
#endif      
    PrintString_("Start to anlayse protol");
    task_schedule_timecount = 0 ;//清零协议分析计时时间
    TashSchedule_State = 2;
    DelayT_ms(20);
    break;
  case 2:
    if (task_schedule_timecount >= 3000) {
      PrintString_("Test_protocol overtime!");
      task_schedule_timecount = 0;
      TashSchedule_State = 3;
    }//超时退出机制
    else 
    {
      if (Select_protocol(&usart3Info)) //选择协议分析
      { 
        TashSchedule_State = 3;
        PrintSysParaList();
      }
    }
    break;
  case 3:
    
    //GetNewInactAngle();//重新获取静态角度
    Usart_RcvClr(&usart3Info);
    task_schedule_timecount = 0;
    TashSchedule_State = 0;//退出分析协议，重新进入正常程序运行
    PrintString_("back to lowpwer mode!");
//    IntoLowpowerMode(); //进入低功耗模式//重新进入低功耗
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
   ENTER_LPM_STATUS;
#endif 
    break;
  case 4:
    break;
  case 5:
    break;
  default:
    break;
  }
}


u8 GetNewInactAngle_Fun_state = 3;
u16 GetNewInactAngle_timecount = 0;




//增加一次 电池发送总次数
void Increase_send_totalcount(void) {
	Sys_keydata.send_totalcount ++;
	Write_sys_keydata_totalcount();
}

//打印固件版本号
void PrintSysInfo(void) {
  debug_send(SOFT_VERSION);
  debug_send(HARDWARE_VERSION);
}
//打印编译日期
void PrintSysDebugData(void){
//  sprintf( str, "MCU Software Update %s %s\r\n", __DATE__, __TIME__ );
//  debug_send(str);
  PrintString("MCU Software Update ");
  PrintString(__DATE__);
  PrintString("   ");
  PrintString_(__TIME__);
}

//打印系统参数清单
//打印出所有的Sys_keydata参数列表  以方便查看
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码  
void PrintSysParaList(void) 
{
  PrintSysDebugData();
  PrintString_("================PrintSysParaList===================");
  PrintString_("Sys_keydata.device_id");
  PrintString_((char *)(&Sys_keydata.device_id));
  PrintString_("Sys_keydata.soft_version");
  PrintString_(SOFTWARE_VER);
  PrintString_("Sys_keydata.harware_version");
  //	PrintU8int2String_(Sys_keydata.harware_version);
  PrintString("V");
  printNumStr(Sys_keydata.harware_version[0]);
  PrintString(".");
  PrintU8int2String_(Sys_keydata.harware_version[1]);
  Watchdog_Feed();
  PrintString_("Sys_keydata.status");    //设备状态
  PrintU8int2String_(Sys_keydata.status);
  
  PrintString_("Sys_keydata.send_totalcount");    //发送次数
  PrintU16int2String_(Sys_keydata.send_totalcount);
  
  PrintString_("Sys_keydata.rotation_trigger_angle");    //旋转变化角度
  PrintU16int2String_(Sys_keydata.rotation_trigger_angle);
  
  PrintString_("Sys_keydata.rotation_trigger_sens");    //旋转灵敏度
  PrintU8int2String_(Sys_keydata.rotation_trigger_sens);
  
  PrintString_("Sys_keydata.lean_trigger_angle");    //倾斜变化角度
  PrintU8int2String_(Sys_keydata.lean_trigger_angle);
  
  PrintString_("Sys_keydata.lean_trigger_sens");    //倾斜灵敏度
  PrintU8int2String_(Sys_keydata.lean_trigger_sens);
  
  PrintString_("Sys_keydata.pressure_trigger_value");    //压力变化阈值
  PrintU16int2String_(Sys_keydata.pressure_trigger_value);
  
  PrintString_("Sys_keydata.pressure_interval");    //压力检测间隔
  PrintU16int2String_(Sys_keydata.pressure_interval); 
  
  PrintString_("Sys_keydata.heartdata_sent_interval_hour_count");    //心跳间隔时间
  PrintU16int2String_(Sys_keydata.heartdata_sent_interval_hour_count);
  
  PrintString_("Sys_keydata.alarm_sent_interval_minute_count");    //告警间隔时间
  PrintU16int2String_(Sys_keydata.alarm_sent_interval_minute_count);
  
  PrintString_("Sys_keydata.trigger_interval_minute_count");    //检测间隔时间
  PrintU16int2String_(Sys_keydata.trigger_interval_minute_count);
  
  PrintString_("Sys_keydata.default_cloud_server_add");    //可配采集地址
  PrintString_((char *)&Sys_keydata.default_cloud_server_add);
  
  PrintString_("Sys_keydata.update_server_add");    //升级服务器地址
  PrintString_((char *)&Sys_keydata.update_server_add);
  
  PrintString_("Sys_keydata.carrier_type");    //运营商
  PrintU8int2String_(Sys_keydata.carrier_type);
  
  PrintString_("Sys_keydata.module_type");    //网络制式
  PrintU8int2String_(Sys_keydata.module_type);
  
  PrintString_("Sys_keydata.Num_Cmd");    //物联网卡号
  PrintString_((char *)Sys_keydata.Num_Cmd);
  
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码  
  PrintString_("Sys_keydata.TimesTamp");    //时间
//  PrintU32int2String_(Sys_keydata.TimesTamp);
  
  PrintString_("Sys_keydata.pressure_upLimit");    //压力上限    UINT    0~65535
//  PrintU16int2String_(Sys_keydata.pressure_upLimit);
  
  PrintString_("Sys_keydata.pressure_downLimit");    //压力下限    UINT    0~65535
//  PrintU16int2String_(Sys_keydata.pressure_downLimit);
#endif
  PrintString_("******");
//  PrintString_("Sys_keydata.temp_offset");
  PrintString_("==============================================="); 
}

/***************************************************************************************
**函数名:       Parameter_Init
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
void Parameter_Init(void)
{
  Sys_keydata.Alarm =0;
  Sys_keydata.lean_angle = 0;
  Sys_keydata.rotation_angle=0;
}
#endif

#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
void PrintSysParaList(void) 
{
  PrintSysDebugData();
  PrintString_("================PrintSysParaList===================");
  PrintString_("Sys_keydata.device_id");
  PrintString_((char *)(&Sys_keydata.device_id));
  PrintString_("Sys_keydata.soft_version");
  PrintString_(SOFTWARE_VER);
  PrintString_("Sys_keydata.harware_version");
  //	PrintU8int2String_(Sys_keydata.harware_version);
  PrintString("V");
  printNumStr(Sys_keydata.harware_version[0]);
  PrintString(".");
  PrintU8int2String_(Sys_keydata.harware_version[1]);
  Watchdog_Feed();
  PrintString_("Sys_keydata.status");    //设备状态
  PrintU8int2String_(Sys_keydata.status);
  
  PrintString_("Sys_keydata.send_totalcount");    //发送次数
  PrintU16int2String_(Sys_keydata.send_totalcount);
  
//  PrintString_("Sys_keydata.pressure_value");    //压力
//  PrintU16int2String_(Sys_keydata.pressure_value); 
//  
//  PrintString_("Sys_keydata.SamplingValue");    //采样原始值
//  PrintU16int2String_(Sys_keydata.SamplingValue);
//  
//  PrintString_("Sys_keydata.lean_trigger_angle");    //倾斜变化角度
////  PrintU16int2String_(Sys_keydata.lean_trigger_angle);
//  
//  PrintString_("Sys_keydata.lean_trigger_sens");    //倾斜灵敏度
////  PrintU16int2String_(Sys_keydata.lean_trigger_sens);
  
  PrintString_("Sys_keydata.pressure_trigger_value");    //压力变化阈值
  PrintU16int2String_(Sys_keydata.pressure_trigger_value);
  
  
  PrintString_("Sys_keydata.heartdata_sent_interval_hour_count");    //心跳间隔时间
  PrintU16int2String_(Sys_keydata.heartdata_sent_interval_hour_count);
  
  PrintString_("Sys_keydata.AlarmEnableBit");    //告警使能0失能，1使能
  PrintU16int2String_(Sys_keydata.AlarmEnableBit);
  
  PrintString_("Sys_keydata.PressureUpLimitAlarmInterval");    //上限告警间隔时间
  PrintU16int2String_(Sys_keydata.PressureUpLimitAlarmInterval);
  
  PrintString_("Sys_keydata.PressureDownLimitAlarmInterval");    //下限告警间隔时间
  PrintU16int2String_(Sys_keydata.PressureDownLimitAlarmInterval);
  
  PrintString_("Sys_keydata.NoSensorAlarmInterval");    //未检测到传感器告警间隔
  PrintU16int2String_(Sys_keydata.NoSensorAlarmInterval);
  
  PrintString_("Sys_keydata.ResolutionAlarmInterval");     //分辨率上报告警间隔
  PrintU16int2String_(Sys_keydata.ResolutionAlarmInterval);
  
  PrintString_("Sys_keydata.TouchOffAlarmInterval");     //人工触发上报间隔
  PrintU16int2String_(Sys_keydata.TouchOffAlarmInterval);
  
  PrintString_("Sys_keydata.trigger_interval_minute_count");    //检测间隔时间
  PrintU16int2String_(Sys_keydata.trigger_interval_minute_count);
  
  PrintString_("Sys_keydata.default_cloud_server_add");    //可配采集地址
  PrintString_((char *)&Sys_keydata.default_cloud_server_add);
  
  PrintString_("Sys_keydata.update_server_add");    //升级服务器地址
  PrintString_((char *)&Sys_keydata.update_server_add);
  
  PrintString_("Sys_keydata.carrier_type");    //运营商
  PrintU8int2String_(Sys_keydata.carrier_type);
  
  PrintString_("Sys_keydata.module_type");    //网络制式
  PrintU8int2String_(Sys_keydata.module_type);
  
  PrintString_("Sys_keydata.Num_Cmd");    //物联网卡号
  PrintString_((char *)Sys_keydata.Num_Cmd);
  
  PrintString_("Sys_keydata.TimesTamp");    //时间
//  PrintU32int2String_(Sys_keydata.TimesTamp);
  
  PrintString_("Sys_keydata.pressure_upLimit");    //压力上限    UINT    0~65535
  PrintU16int2String_(Sys_keydata.pressure_upLimit);
  
  PrintString_("Sys_keydata.pressure_downLimit");    //压力下限    UINT    0~65535
  PrintU16int2String_(Sys_keydata.pressure_downLimit);
  PrintString_("******");
  PrintString_("===============================================");  
 
}

/***************************************************************************************
**函数名:       Parameter_Init
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
void Parameter_Init(void)
{
  Sys_keydata.Alarm =0;
  Sys_keydata.pressure_value = 0;
  Sys_keydata.SamplingValue = 0;
}
#endif 

#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
// **************************************************************
// 函数:InitAdxl362Para
// 参数1:无
// 返回:无
// 描述:初始化adxl362扫描函数的参数
// **************************************************************
void InitAdxl362Para(void) 
{
	//Adxl362Para.Inteval_timecount = 0;
	Adxl362Para.Read_timecount = 0;
	//Adxl362Para.Max_readcount = 0;
	Adxl362Para.Readcount = 0;
	//Adxl362Para.Read_flag = 0;
	Adxl362Para.Busy_flag = 0;
	Adxl362Para.Scan_state = 0;
	SetAdxl362Interval_timeAndMax_count(20, 10);
}
#endif 
// **************************************************************
// 函数:SetThresholdAngle
// 参数1:x_angle  X轴阈值角度
// 参数2:y_angle  Y轴阈值角度
// 参数3:z_angle  Z轴阈值角度
// 返回:无
// 描述:设置触发阈值角度
// **************************************************************
//设置触发阈值角度  threshold_angle
/*void SetThresholdAngle(s8 x_angle, s8 y_angle, s8 z_angle) {
	SysXyzData.xyz_angle.xyz_threshold_angle.x_angle = x_angle;
	SysXyzData.xyz_angle.xyz_threshold_angle.y_angle = y_angle;
	SysXyzData.xyz_angle.xyz_threshold_angle.z_angle = z_angle;
}*/
// **************************************************************
// 函数:SetAdxl362Interval_timeAndMax_count
// 参数1:interval_timecount 读取的间隔时间
// 参数2:max_readcount      连续读取的次数
// 返回:无
// 描述:设置ADXL362的读取间隔时间和连续读取次数
// **************************************************************
//设置ADXL362的读取间隔时间和连续读取次数
void SetAdxl362Interval_timeAndMax_count(u8 interval_timecount, u8 max_readcount) {
	Adxl362Para.Inteval_timecount = interval_timecount;
	Adxl362Para.Max_readcount = max_readcount;
}



//加载关键数据(开机重启时、)
// Sys_keydata 载入到 Analyse_data_para (角度 灵敏度)
// Sys_keydata 载入到 PacketData (ID 软件版本  硬件版本  心跳 报警 电池发送总次数 运营商 模块类型)
//(3个服务器地址)
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
void Set_Key_Para(void){
	//载入ID
//	SetTiggerAngleValue(Sys_keydata.trigger_angle);
//	SetSensitiveValue(Sys_keydata.trigger_sensitivity);
	/////////////////////////!!!!!//////////////////////
	//Analyse_data_para.valid_angle_d_value = Sys_keydata.trigger_angle;//设置触发角度 20170217 BUG
	/////////////////////////!!!!!//////////////////////
	//Analyse_data_para.set_valid_angle_unbroken_num = Sys_keydata.trigger_sensitivity;//设置灵敏度 灵敏度值越小越灵敏

  Analyse_data_para_XiaoFangZ.set_angle = Sys_keydata.lean_trigger_angle;//配置倾斜触发角度 //by hx 2017-06-15
  Analyse_data_para_XiaoFangZ.set_valid_angle_unbroken_num = Sys_keydata.lean_trigger_sens; // 配置倾斜触发灵敏度 //by hx 2017-06-15
  
  Analyse_data_para_XiaoFangX.set_angle = Sys_keydata.rotation_trigger_angle;//配置旋转触发角度 //by hx 2017-06-15
  Analyse_data_para_XiaoFangX.set_valid_angle_unbroken_num = Sys_keydata.rotation_trigger_sens;// 配置旋转触发灵敏度 //by hx 2017-06-15
}
#endif 
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
void Set_Key_Para(void){
	//载入ID
//	SetTiggerAngleValue(Sys_keydata.trigger_angle);
//	SetSensitiveValue(Sys_keydata.trigger_sensitivity);
	/////////////////////////!!!!!//////////////////////
	//Analyse_data_para.valid_angle_d_value = Sys_keydata.trigger_angle;//设置触发角度 20170217 BUG
	/////////////////////////!!!!!//////////////////////
	//Analyse_data_para.set_valid_angle_unbroken_num = Sys_keydata.trigger_sensitivity;//设置灵敏度 灵敏度值越小越灵敏

//  Analyse_data_para_XiaoFangZ.set_angle = Sys_keydata.lean_trigger_angle;//配置倾斜触发角度 //by hx 2017-06-15
//  Analyse_data_para_XiaoFangZ.set_valid_angle_unbroken_num = Sys_keydata.lean_trigger_sens; // 配置倾斜触发灵敏度 //by hx 2017-06-15
//  
//  Analyse_data_para_XiaoFangX.set_angle = Sys_keydata.rotation_trigger_angle;//配置旋转触发角度 //by hx 2017-06-15
//  Analyse_data_para_XiaoFangX.set_valid_angle_unbroken_num = Sys_keydata.rotation_trigger_sens;// 配置旋转触发灵敏度 //by hx 2017-06-15
}
#endif



//打包物联网卡卡号
void Pack_IotNum(char *num_string) {
//	PacketData.ZipsendData.TcpData_S.Data_Info.Num_Cmd = Hex2StrFun_13(num_string);
	//PacketData.ZipsendData.TcpData_S.Data_Info.SigStren_Cmd = Hex2StrFun_1(&sig_stren);
}


//设置信号强度 打包信号强度
void Pack_SigStren(u8 sig_stren) {
//	PacketData.ZipsendData.TcpData_S.Data_Info.SigStren_Cmd = Hex2StrFun_1(&sig_stren);
}

//void PacketSigStrenMsg(){
//	PacketData.ZipsendData.TcpData_S.Data_Info.SigStren_Cmd = Hex2StrFun_1(&tempdata);//封装温度信息
//}
/****************************************************************************************************
**封装液面报警，暂时占用低温报警
//20170630 更新为 0 心跳 1水位报警 2降水推送
******************************************************************************************************/
void Pack_Low_Temp_Alarm(u8 tempflag) //by hx 2017-06-07
{
//	if (!tempflag) {
//		//封装当前静止状态的零点角度
//		Pack_LeanAngle(Analyse_data_para.zero_angle);
//	}
//	PacketData.ZipsendData.TcpData_S.Data_Info.Low_Temp_Alarm_Cmd = Hex2StrFun_1(&tempflag);
//  Sys_keydata.Low_Temp_Alarm = tempflag;
}
//设置触发标志 打包报警标志
//20170425 修改标志位设定 原标志位定义 0 心跳 1报警
//更改为 0心跳 1报警 2二次上报
void Pack_AlarmFlag(u8 tempflag) {
//        if (tempflag!=1) {//如果是心跳包  上传静止角度
//		//封装当前静止状态的零点角度
//		//Pack_LeanAngle(Analyse_data_para.zero_angle);
//	}
//        Sys_keydata.Trigger_Alarm = tempflag;
//	PacketData.ZipsendData.TcpData_S.Data_Info.Trigger_Alarm_Cmd = Hex2StrFun_1(&tempflag);//打包触发标志
}

//打包温度信息
//void Pack_TempValue(void) {
//
//	u8 tempdata = GetTemp();
//	//u8 tempdata = 88;
//	PacketData.ZipsendData.TcpData_S.Data_Info.Temp_Cmd = Hex2StrFun_1(&tempdata);//封装温度信息
//	////////取温度信息/////////
//
//	//debug_send("tempdata:");
//	//printNum2Str(tempdata);
//	///////////////////////////
//}

//void Pack_ADXL362_XYZ{
//  ACCEL_AD0 TempTran;
//  TempTran = ADXL362_Get_Value();
//  Sys_keydata.X_Value = TempTran.xadc;
//  Sys_keydata.Y_Value = TempTran.yadc;
//  Sys_keydata.Z_Value = TempTran.zadc;
//}
//打包倾斜角度
void Pack_LeanAngle(s8 lean_angle) {
	/*u8 temp_angle_u8;
        
	if (lean_angle <= 0) {
		temp_angle_u8 = (u8)(0 - lean_angle);
	} else {
		temp_angle_u8 = (u8)(lean_angle);
	}
	PacketData.ZipsendData.TcpData_S.Data_Info.Angle_Cmd = Hex2StrFun_1(&temp_angle_u8);//封装倾斜角度
        */
//        s8 temp_angle_s8;
//        temp_angle_s8 = 0 - lean_angle;
//        Sys_keydata.Angle=temp_angle_s8;
//        PacketData.ZipsendData.TcpData_S.Data_Info.Angle_Cmd = Hex2StrFun_1(&temp_angle_s8);//封装倾斜角度
}

////打包触发角度
//void Pack_TriggerAngle(void) {
////	PacketData.ZipsendData.TcpData_S.Data_Info.Trigger_Angle_Cmd = Hex2StrFun_1(&Sys_keydata.trigger_angle);//封装触发角度
//}
////打包灵敏度
//void Pack_TriggerSens(void) {
////	PacketData.ZipsendData.TcpData_S.Data_Info.Sens_Cmd = Hex2StrFun_1(&Sys_keydata.trigger_sensitivity);//封装灵敏度
//}


////打包ID信息
//void Pack_IdInfo(void) {
//	//PacketData.ZipsendData.TcpData_S.Data_Info.Id_Cmd = Hex2StrFun_16(DEVICE_ID);//封装设备ID
////	PacketData.ZipsendData.TcpData_S.Data_Info.Id_Cmd = Hex2StrFun_16(Sys_keydata.device_id);//封装设备ID
//}

////打包固件软件版本  包含软件版本和硬件版本
//void Pack_Firm_Version(void) {
//	HEX2STR_1 firm_buf = {0, 0};
//	firm_buf.str1[0] = Sys_keydata.harware_version;
//	firm_buf.str1[1] = Sys_keydata.soft_version;
////	PacketData.ZipsendData.TcpData_S.Data_Info.Firm_Cmd = Hex2StrFun_2((u16 *)(&firm_buf));//封装固件版本
//}

//打包运营商
void Pack_Carrier_Type(void) {
//	PacketData.ZipsendData.TcpData_S.Data_Info.Carrier_Cmd = Hex2StrFun_1(&Sys_keydata.carrier_type);//封装运营商
}
//打包模块类型
void Pack_module_Type(void) {
//	PacketData.ZipsendData.TcpData_S.Data_Info.Mod_Type_Cmd = Hex2StrFun_1(&Sys_keydata.module_type);//封装模块类型
}


//u16 heart_timecount = 0;
//_Bool heart_send_flag = 0;

// **************************************************************
// 函数:SysParaInit
// 参数1:无
// 返回:无
// 描述:系统状态切换参数初始化
// **************************************************************
//void SysParaInit(void) {
//	Adxl362Para.Read_flag = 0;
//	EXTI_ClearITPendingBit(EXTI_IT_PortB);
//	EXTI_ClearITPendingBit(EXTI_IT_Pin3);
//	InitAdxl362Para();
//	Set_Key_Para();//设置角度和灵敏度 20170217修复  之前没有设置(BUG)
//}

/*
void printf_PackageMsg(void) {
	char *array_data = (char *)(&PacketData.ZipsendData.TcpData_S.Soi_Cmd);
	for (u8 i = 0; i < 204; ++i)
	{
		alarmer_uart3_send_byte(array_data[i]);
	}
}*/




//计算校验码
void CalcChecksum(HEX2STR_2 *checksum_out, char *array_data, u16 len) {
	u16 sumnum = 0;
	HEX2STR_2 checksum_out1;
	for (u16 i = 0; i < len; ++i)
	{
		sumnum += array_data[i];//去掉SOI EOI CHESUM
	}
	sumnum = (~(sumnum % 65535)) + 1;

	//sprintf(str, " sumnum:%d", sumnum);
	//debug_send(str);

	checksum_out1 = Hex2StrFun_2(&sumnum);

	//PrintString("CHESUM :");
	//PrintString_(checksum_out1.str2 + 0);

	ArrayCopy(checksum_out->str2, checksum_out1.str2, 4);

	ack_protocol_config_array[33] = 0x0D;
}





////打包封装
//void PackageMsg(void) {
//
//	//Pack_TempValue();//封装温度信息
//	Pack_IdInfo();//封装ID信息
//	Pack_SendTotalcount();//封装电池总的发送次数
//	Pack_TriggerAngle();//封装触发角度
//	Pack_TriggerSens();//封装触发灵敏度
//	//封装触发角度 
//	//封装号码 这个放到发送那边
//
//	Pack_Second_Query_Intervalcount();//封装二次确认间隔时间
//	Pack_Cloud_Server_Add();//封装当前采集地址
//	
//	Pack_Firm_Version();//封装固件版本
//	Pack_Heartdata_Send_Intervalcount();//封装心跳时间
////	Pack_Alarm_Intervalcount();//打包报警时间（触发间隔）
//	Pack_Carrier_Type();//打包运营商类型
//	Pack_module_Type();//打包模块类型
//
////        CalcChecksum((HEX2STR_2 *)(&PacketData.ZipsendData.TcpData_S.Chksum_Cmd), (char *)(&PacketData.ZipsendData.TcpData_S.Soi_Cmd + 1), 244);
//}


//目前需要统计的数据包
//FifoData.BufArray 里面有X Y Z 必须先转成Z角度
//触发前角度判断 FifoData.z_angle[i-1]
//触发后角度判断 SysBuf.z_angle[SysBuf.len]


//GetNewInactAngle 获取静态下的新角度


//重新进入静态状态前 持续采集角度
//超时时间 默认10s
_Bool GetAngleRealtimeBeforeInact(void) {

	return 1;
}

//触发前的数据 分析
//角度变化趋势 变化角度=前段角度-后段角度
//中间变量过程
void AnalyseDataBeforeTriggerFun(void) {

}

//触发后的数据分析
//角度与静态角度对比 相差
//最新角度-静态角度≥触发角度
void AnalyseDataAfterTriggerFun(void) {

}


//打印输出字符串 用于调试使用
void PrintString(char *temp_in_string) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
        Watchdog_Feed();
#endif
	debug_send_no_end(temp_in_string);
        Watchdog_Feed();
}

void PrintString_(char *temp_in_string) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
        Watchdog_Feed();
#endif
	debug_send(temp_in_string);
        Watchdog_Feed();
}

void PrintStringWithlen_(char *temp_in_string, u8 len) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
	while (len--) {
		debug_send(temp_in_string);
	}
}


/***************************************************************************************
**函数名:       printU32to2Str
**功能描述:     
打印小于等于4字节的十进制输出。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-11-09,By Haixing.Huang
****************************************************************************************/
void printU32to2Str(u32 num) 
{
  u8 str_temp[11];
  u32 num_tmp;
  char c_tmp;
  num_tmp = num;
  u8 i=0;
  u8 cnt=0;
  while(num_tmp)
  {
    str_temp[i]=num_tmp%10+'0';
    i++;
    num_tmp = num_tmp/10;
    if(i>11)  //强制退出。
    {
      break;
    }
    if(0 == num_tmp)
    {
      break;
    }
  }
  cnt = i-1;
  for(i=0;i<(cnt/2+1);i++) //交换位置
  {
    c_tmp=str_temp[i];
    str_temp[i]=str_temp[cnt-i];
    str_temp[cnt-i]= c_tmp;
  }
  str_temp[cnt+1] = '\0';  //末尾添加结束符
  PrintString((char*)str_temp);
}

//打印普通的数字 格式为十进制
void PrintS8int2String(s8 print_num) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
        printNumStr(print_num);
//	sprintf(str, "%d", print_num);
//	debug_send_no_end(str);
}

void PrintS8int2String_(s8 print_num) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
        printNumStr(print_num);
//	sprintf(str, "%d", print_num);
//	debug_send(str);
}

void PrintU8int2String(u8 print_num) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
        printNum2Str(print_num);
//	sprintf(str, "%d", print_num);
//	debug_send_no_end(str);
}

void PrintU8int2String_(u8 print_num) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
        printNum2Str(print_num);
//	sprintf(str, "%d", print_num);
//	debug_send(str);
}

void PrintU16int2String(u16 print_num) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
        printUNum2Str(print_num);
//	sprintf(str, "%u", print_num);
//	debug_send_no_end(str);
}

void PrintU16int2String_(u16 print_num) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
        printUNum2Str(print_num);
//	sprintf(str, "%u", print_num);
//	debug_send(str);
}

void PrintU32int2String(u32 print_num) {
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
//        debug_send("函数被屏蔽");
        printU32to2Str(print_num);
//	sprintf(str, "%u", print_num);
//	debug_send_no_end(str);
}

void PrintU32int2String_(u32 print_num) 
{
#if defined(DISPLAY_DEBUG_EN)
      debug_send_no_end("DEBUG:");
#endif
//        debug_send("函数被屏蔽");
      printU32to2Str(print_num);
      debug_send("");
//	sprintf(str, "%u", print_num);
//	debug_send(str);
}

//void PrintU16int2HEX_(u16 print_num) {
//        //debug_send("函数被屏蔽");
//        for(u8 i = 0;i<4;i++){
//          Watchdog_Feed();
//          alarmer_uart3_send_byte(NO[(u8)(print_num>>((3-i)<<2))&0x0f]);
//        }
//}
//void PrintU32int2HEX_(u32 print_num) {
//#if defined(DISPLAY_DEBUG_EN)
//	debug_send_no_end("DEBUG:");
//#endif
//        //debug_send("函数被屏蔽");
//        for(u8 i = 0;i<8;i++){
//          Watchdog_Feed();
//          alarmer_uart3_send_byte(NO[(u8)(print_num>>((7-i)<<2))&0x0f]);
//        }
////	sprintf(str, "%u", print_num);
////	debug_send_no_end(str);
//}

//打印中文字符
void PrintChinese(char *temp_in_string){
#if defined(DISPLAY_DEBUG_EN)
	debug_send_no_end("DEBUG:");
#endif
//	sprintf(str, "%s\n", temp_in_string);
//	debug_send(str);
        PrintString_(temp_in_string);
}


//判断角度是否在范围内
_Bool IsZangleWithinRange(s8 input_Zangle) {
	if ((input_Zangle <= 0) && (input_Zangle >= -90)) {
		return 1;
	}
	return 0;
}

//判断角度是否符合触发角度
_Bool IsZangleValid(s8 input_Zangle) {

	if (Analyse_data_para.zero_angle > (0 - Analyse_data_para.set_angle)) { //零点角度小于设定的角度
		//小角度摆放的情况
		if (input_Zangle >= Analyse_data_para.zero_angle) {
			//角度没有超过零点角度的范围
			//视为不触发
			return 0;
		} else {
			//角度超过零点角度的范围  需要再进行判断
			if ((Analyse_data_para.zero_angle - input_Zangle) >= Analyse_data_para.set_angle) {
				//当前角度-零点角度≥设定角度
				//判断为有效角度
				return 1;
			} else {
				//当前角度-零点角度<设定角度
				//判断为无效角度
				return 0;
			}
		}
	}
	else { //零点角度大于设定的角度
		if (input_Zangle >= Analyse_data_para.zero_angle) {
			//角度没有超过零点角度的范围
			//需要判断是否回位 反方向的角度差值
			if ((input_Zangle - Analyse_data_para.zero_angle) >= Analyse_data_para.set_angle) {
				//当前角度-零点角度≥设定角度
				//判断为有效角度
				return 1;
			} else {
				//当前角度-零点角度<设定角度
				//判断为无效角度
				return 0;
			}
		} else {
			//角度超过零点角度的范围  需要再进行判断
			if ((Analyse_data_para.zero_angle - input_Zangle) >= Analyse_data_para.set_angle) {
				//当前角度-零点角度≥设定角度
				//判断为有效角度
				return 1;
			} else {
				//当前角度-零点角度<设定角度
				//判断为无效角度
				return 0;
			}
		}
	}
	//return 0;
}


_Bool IsDvalueWithinRange(s8 pre_angle, s8 new_angle, s8 d_value) {
	if (pre_angle >= new_angle) { //角度变大
		if ((pre_angle - new_angle) <= d_value) {
			return 1;
		}
		else {
			return 0;
		}
	} else { //角度变小
		if ((new_angle - pre_angle) <= d_value) {
			return 1;
		}
		else {
			return 0;
		}
	}
	//return 0;
}

//设置触发角度
//触发角度 范围为3-40度 正偏移0
void SetTiggerAngleValue(u8 tiggerangle_value) {

	Analyse_data_para.set_angle = tiggerangle_value;
}

//设置分析的灵敏度
//灵敏度默认为4 正偏移4转换后的连续数目为8
//范围为1-7 正偏移后的范围为5-11
void SetSensitiveValue(u8 sen_value) {

	Analyse_data_para.set_valid_angle_unbroken_num = sen_value; //设定灵敏度为
}

void ClsAnalyseState(void) {
	Analyse_data_para.analyse_state = 0;
	Analyse_data_para.valid_angle_unbroken_num = 0;
	Analyse_data_para.alarm_flag = 0;
}


//分析数据 Analyse_data_para
_Bool AnalyseData(void) {
	//static s8 temp_z_angle = 0;//临时存放Z轴角度
	//static s8 temp_d_value = 0;
	static s8 pre_temp_z_angle = 0;

	Analyse_data_para.present_angle = get_angle_Z(ADXL362_Get_Value());
	//if (IsZangleWithinRange(temp_z_angle))PrintS8int2String(temp_z_angle);//范围内的值打印出来
	//temp_d_value = temp_z_angle - Analyse_data_para.zero_angle;

	//PrintS8int2String(Analyse_data_para.present_angle);//打印数据信息
	PrintS8int2String(-55);//打印伪数据信息
       /* if(Analyse_data_para.present_angle>0){
          PrintString("+");
        }else if(Analyse_data_para.present_angle<0){
          PrintString("-");
        }else{
          PrintString("0");
        }*/
        
	if (IsZangleWithinRange(Analyse_data_para.present_angle)) {
		//角度符合范围
		if (IsZangleValid(Analyse_data_para.present_angle)) {
			//角度是否合法
			PrintString("?");
			if (IsDvalueWithinRange(pre_temp_z_angle, Analyse_data_para.present_angle, Analyse_data_para.valid_angle_d_value)) {
				//前一个值减去最新的值
				//在差值范围内
				PrintString("#");
			}
			//////////////20170117增加 可能影响监测//////////////////////////////////////
			else {
				//前后差值不合理  清零不间断数目
				Analyse_data_para.valid_angle_unbroken_num = 0;
			}
			////////////////////////////////////////////////////
			if (++Analyse_data_para.valid_angle_unbroken_num >= Analyse_data_para.set_valid_angle_unbroken_num) {

				Analyse_data_para.valid_angle_unbroken_num = 0;
				debug_send("$$$");
				if ((Analyse_data_para.zero_angle <= 0)) {
					Analyse_data_para.alarm_flag = 1;//设置告警标志
					//封装当前倾斜角度
//                                        Pack_ADXL362_XYZ();//打包XYZ轴参数 添增时间2017/10/26
                                        Analyse_data_para.present_angle = get_angle_Z(ADXL362_Get_Value());//
					Pack_LeanAngle(Analyse_data_para.present_angle);
					return 1;
				}
				else {
					PrintString_("zero_angle is >0,No new alarm!");
					Analyse_data_para.alarm_flag = 0;//清除告警标志
				}
			}
			//if(pre_temp_z_angle)
		} else {
			//角度不合法  清零不间断数目
			Analyse_data_para.valid_angle_unbroken_num = 0;
		}
		pre_temp_z_angle = Analyse_data_para.present_angle;
	}
	else {
		//角度不符合范围  清零不间断数目
		Analyse_data_para.valid_angle_unbroken_num = 0;
	}
	return 0;
}

_Bool SendHeartPacket(u16 heartcount) 
{
#ifdef HEART_REPORT_EN
//	if (!heart_send_flag) { //如果没有心跳包标志
//		if (++heart_timecount >= (heartcount<<1)) { //心跳时间累加 8个小时  1小时=125计数
//			heart_send_flag = 1;//心跳包置位
//			heart_timecount = 0;//时间清零
//			return 1;
//		}
//	}
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
  if (!heart_send_flag) //如果没有心跳包标志
  { 
    if (++heart_timecount >= heartcount) 
    { //心跳时间累加 8个小时  1小时=125计数
      heart_send_flag = 1;//心跳包置位
      heart_timecount = 0;//时间清零
      return 1;
    }
  }
#endif  
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
  if (!heart_send_flag) //如果没有心跳包标志
  { 
    if (++heart_timecount >= heartcount) 
    { //心跳时间累加 8个小时  1小时=125计数
      heart_send_flag = 1;//心跳包置位
      heart_timecount = 0;//时间清零
      return 1;
    }
  }
#endif
#endif
	return 0;
}
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
/***************************************************************************************
**函数名:       ClearHeartTimeCount
**功能描述:     获取数据
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-12-10,By Haixing.Huang
****************************************************************************************/
void ClearHeartTimeCount()
{
  heart_timecount = 0;//时间清零
  heart_send_flag = 0; //清除标志位
  debug_send("心跳计数器清零");
}
/***************************************************************************************
**函数名:       getheart_send_flag
**功能描述:     获取数据
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-12-10,By Haixing.Huang
****************************************************************************************/
_Bool *getheart_send_flag()
{
  return &heart_send_flag;
}

u16 *getheart_timecount()
{
  return &heart_timecount;
}
#endif 
///***************************************************************************************
//**函数名:       PressureSysFun
//**功能描述:     水压业务流程程序
//**输入参数:     --
//**输出参数:     --
//**备注:         YY.c,2017-11-23,By Haixing.Huang
//****************************************************************************************/
//void PressureSysFun(void) 
//{
//  switch (sysFunState) 
//  {
//  case 0:
//    uniAlarm.AlarmAllData = 0X00; //清除所有的告警
//    Sys_keydata.Alarm=uniAlarm.AlarmAllData; 
//    POW24_ON();  //开启水压检测
//    //    POWSW1_ON();
//    DelayT_ms(500); //给500ms的外设上电时间
//    AdcInit(); //初始化ADC
//    Pressure_GatValue(&strPressureControl);  //获取参数
//    AdcClear();
//    POW24_OFF();  //关闭水压检测
//    //    POWSW1_OFF();
//    if(PressureKeyUpdataConfirm())
//    {
//      debug_send("按键触发上报数据!");
//      uniAlarm.struAlarmBit.KeyUpdata = 1; 
//      //      ENTER_SENDALARM_STATUS; //进入发射流程; 自动打包。
//      //      break;
//    }
//    if(1 == uniAlarm.struAlarmBit.WithoutPressureSensors  ) //添加到 未检测到传感器告警列表)
//    {
//      debug_send("未检测到传感器 ");
//    }
//    strPressureControl.CurrentValue = Sys_keydata.pressure_value;   //获取最新的压力值。
//    if(strPressureControl.CurrentValuePre >= strPressureControl.CurrentValue)   //检查压力差值。
//    {
//      strPressureControl.CurrentValueDifference = strPressureControl.CurrentValuePre- strPressureControl.CurrentValue;
//    }
//    else
//    {
//      strPressureControl.CurrentValueDifference = strPressureControl.CurrentValue- strPressureControl.CurrentValuePre;
//    }  
//    
//    if(AlarmSentIntervalCheck()) //没有告警间隔限制
//    {
//      if(uniAlarm.struAlarmBit.pressureAlarmUpDownLimit  == 1)
//      {
////        AlarmSentIntervalSet(Sys_keydata.alarm_sent_interval_minute_count);  //添加告警间隔
//        debug_send("开始压力上限报警！");
//        RESET_UPDATA_DISABLE; 
//        ENTER_SENDALARM_STATUS; //进入发射流程; 自动打包。
//        break;
//      }
//      else if(uniAlarm.struAlarmBit.pressureAlarmUpDownLimit  == 2)
//      {
////        AlarmSentIntervalSet(Sys_keydata.alarm_sent_interval_minute_count);  //添加告警间隔
//        debug_send("开始压力下限报警！");
//        RESET_UPDATA_DISABLE;
//        ENTER_SENDALARM_STATUS; //进入发射流程; 自动打包。
//        break;
//      }
//    }
//    else
//    {
//      debug_send("在告警间隔内！");
//    }
//    
//    if(strPressureControl.CurrentValueDifference > Sys_keydata.pressure_trigger_value) //变化值超过一定范围。
//    {
//      debug_send("超过变化范围报警！");
//      uniAlarm.struAlarmBit.pressureAlarm = 1;
//      RESET_UPDATA_DISABLE;
//      ENTER_SENDALARM_STATUS; //进入发射流程; 自动打包。
//      break;
//    }
//    
//    if(RESET_UPDATA_GET()) //复位发送流程
//    {
//      debug_send("复位发送数据");
//      RESET_UPDATA_DISABLE;
//      ENTER_SENDALARM_STATUS; //进入发射流程; 自动打包。
//      break;
//    }
//    if (heart_send_flag) 
//    {
//      heart_send_flag = 0;      //清除心跳标志。
//      sysFunState = 6;//发送心跳包
//      //      strPressureControl.startupdata = 1; //上传数据的时间到了，开始发送数据。 
//      debug_send("开始发送心跳");
//      break;
//    } 
//    else 
//    {
//      
//    }
//    if(1 == uniAlarm.struAlarmBit.KeyUpdata)
//    {
//      //      debug_send("按键触发上报数据!");
//      ENTER_SENDALARM_STATUS; //进入发射流程; 自动打包。
//      break;
//    }
//    sysFunState = 4;  //进入休眠，by hx 2017-06-17
//    break;
//  case 1:
//    sysFunState = 4;
//    break;
//  case 2:       //发送报警/心跳
//    Sys_keydata.Alarm=uniAlarm.AlarmAllData; 
//#if APP_CAN_UPDATA
//    iap_data.writeflag =MODE_ING_DOWNLOAD;
//#else
//    debug_send("未使能APP程序上网");
//#endif
//    {
//      sysFunState++;
//    }
//    break;
//  case 3://关闭外设
//    sysFunState++;
//    break;
//  case 4://清除标志，重新进入低功耗休眠
//    Pressure_Start(&strPressureControl);  //清除压力标志位。
//    strPressureControl.CurrentValuePre = strPressureControl.CurrentValue; //被上次的值 
//    sysFunState = 0;//休眠唤醒后，从头开始执行
//    debug_send("进入低功耗");
//    IntoLowpowerMode();
//    break;
//  case 5:// 
//    break;
//  case 6://
//    sysFunState = 2;
//    break;
//  case 10://等待配置参数过程  超时时间为5S
//    break;
//  case 11:
//    break;
//  case 12://获取配置参数  ID  校对温度
//    //设置ID号
//    break;
//  case 13://水位检测  又返回  再切换到
//    break;
//  case 14:
//    break;
//  case 15:
//    break;
//  case 16:
//    break;
//  default:
//    sysFunState = 0;
//    break;
//  }
//}

///***************************************************************************************
//**函数名:       CoverSysFun
//**功能描述:     闷盖业务流程程序
//**输入参数:     --
//**输出参数:     --
//**备注:         YY.c,2017-11-23,By Haixing.Huang
//****************************************************************************************/
//void CoverSysFun(void) 
//{
//#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
// 
//   
//  switch (CoverSysFunState)
//  {
//  case CoverSysFunState_Detection://进入检测ADXL模块的数据读取
//    
//    WaterDetection_Service();
//    if (Adxl362Para.Read_flag == 1) //如果有阈值触发标志置位
//    {
//      sysFunTimecount = 0;
//      CoverSysFunState = CoverSysFunState_AnalyzeACC; //进入加速度分析。
//      PrintString_("开始获取三轴数据!");
//      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//清除分析模式的所有状态
//      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//清除分析模式的所有状态
//      PrintString("Z轴零点角度:");
//      PrintS8int2String_(Analyse_data_para_XiaoFangZ.zero_angle);
//      PrintString("X轴零点角度:");
//      PrintS8int2String_(Analyse_data_para_XiaoFangX.zero_angle);
//      NewInactAngle_SetSamplingLockFlag();  //进入动作判断的时候，不进行静止判断。
//    }
//    else        //如果没有阈值触发
//    {
//      if (heart_send_flag) 
//      {
//        heart_send_flag = 0;   //清除心跳包
//        CoverSysFunState = CoverSysFunState_SendData;
//        debug_send("开始发送心跳包");
//        break;
//      }
//      CoverSysFunState = CoverSysFunState_WaitInterLPM;
//    }
//    break;
//  case CoverSysFunState_SendData:       //发送报警/心跳
//    if(1 == struXiaoFangAlarmTimes.StratAlarm) //这里要模拟清除一次出水主动告警，防止出现多条告警的情况。
//    {
//      struXiaoFangAlarmTimes.StratAlarm = 0;
//      struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
//      XiaoFang_ClearRtcWakeFlag();
//      debug_send("被动出水报警！");
//    }
//#if APP_CAN_UPDATA
//    iap_data.writeflag =MODE_ING_DOWNLOAD;
//#else
//    debug_send("未使能APP程序上网");
//#endif   
//    CoverSysFunState = CoverSysFunState_WaitInterLPM;
//    break;
//  case CoverSysFunState_WaitInterLPM:
//    /**********************************************************************************************************************
//    无水到有水实时触发。其他告警：开盖、倾斜、会带上出水告警。
//    **********************************************************************************************************************/
//    if(1 == struXiaoFangAlarmTimes.StratAlarm)    
//    {
//      struXiaoFangAlarmTimes.StratAlarm = 0;
//      if((TRIGGER_FALLING == struXiaoFangAlarmTimes.TriggerStatus)||(XiaoFang_GetRtcWakeFlag())) 
//      {
//        struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
//        XiaoFang_ClearRtcWakeFlag();
//        debug_send("主动出水报警！");
//        ENTER_SENDALARM_STATUS; //进入发射流程
//      }
//    }
//    else
//    {
//      NewInactAngle_ClearSamplingLockFlag(); //清除禁止检查锁定。
//      if(NewInactAngle_GetNewAngleEndFlag()) //等待角度静止完成。
//      {
//        Analyse_data_para.alarm_flag = 0;
//        Adxl362Para.Read_flag = 0;
//        uniAlarm.AlarmAllData = 0X00;     //清除所有告警
//        struXiaoFangAlarmTimes.TriggerStatus = TRIGGER_NOT;
////        XiaoFang_ClearRtcWakeFlag();
//        debug_send(">>>系统进入休眠");
//        CoverSysFunState = CoverSysFunState_InterLPM;
//      }
//    }
//    break;
//  case CoverSysFunState_AnalyzeACC://
//    if (sysFunTimecount < 5000) //超时退出
//    {
//      Analyse_data_para_XiaoFangZ.present_angle= get_angle_Z(ADXL362_Get_Value());//获取Z轴的数据。
//      XiaoFang_Analyse(&Analyse_data_para_XiaoFangZ);  //分析Z轴数据
//      Analyse_data_para_XiaoFangX.present_angle =get_angle_X(ADXL362_Get_Value());//获取X轴的数据。
//      XiaoFang_Analyse(&Analyse_data_para_XiaoFangX);  //分析X轴数据
//      //
//      if((1 == Analyse_data_para_XiaoFangZ.alarm_flag) || (1 == Analyse_data_para_XiaoFangX.alarm_flag))
//      {
////        Pack_Rotation_Angle(Analyse_data_para_XiaoFangX.present_angle); //打包旋转角度
////        Pack_LeanAngle(Analyse_data_para_XiaoFangZ.present_angle);//打包倾斜角度
//        if((1 == Analyse_data_para_XiaoFangZ.alarm_flag)) // 发生倾斜报警
//        {
//          PrintString_("开始倾斜告警!");
//          uniAlarm.struAlarmBit.laenAlarm = 1;
//          //               Pack_Lean_Alarm(1);//打包倾斜告警。
//          ENTER_SENDALARM_STATUS;
//          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//清除分析模式的所有状态
//        }
//        
//        if((1 == Analyse_data_para_XiaoFangX.alarm_flag)) // 发生旋转报警
//        {
//          PrintString_("开始开盖告警!");
//          //                Pack_Rotation_Alarm(1); //打包旋转报警
//          uniAlarm.struAlarmBit.openAlarm = 1;
//          ENTER_SENDALARM_STATUS;
//          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//清除分析模式的所有状态
//        }
//      }
//      else //如果发生未告警
//      {
//        if(NewInactAngle_GetStaticFlag()) //
//        {
////          sysFunState = 4;
//          CoverSysFunState = CoverSysFunState_WaitInterLPM;
//          //                ClsAnalyseState();//清除分析模式的所有状态
//          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//清除分析模式的所有状态
//          XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//清除分析模式的所有状态
//          //          PrintString_("none valid angle!back to lowpwer mode!");
//          //          GetNewInactAngle();
//        }
//      }
//    }
//    else
//    {
//      //PrintString_("@");
//      //            ClsAnalyseState();//清除分析模式的所有状态
//      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//清除分析模式的所有状态
//      XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//清除分析模式的所有状态
//      PrintString_("Exit!");
////      sysFunState = 4;//重新进入低功耗休眠
//      CoverSysFunState = CoverSysFunState_WaitInterLPM;
//      //Pack_AlarmFlag(1);//设置报警标志
//      //PackageMsg();//先封装，再发送
//      //sysFunState = 2;//发送消息
//    }
//    break;
//  case CoverSysFunState_InterLPM:
//    CoverSysFunState = CoverSysFunState_Detection;
//    IntoLowpowerMode();
//    break;
//  default:
//    CoverSysFunState = CoverSysFunState_Detection;
//    break;
//  }
//#endif
//}