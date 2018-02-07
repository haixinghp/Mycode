#include "config.h"

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : TimingDelay_Decrement
// 功能描述  : 使用SysTick来精确延时,TimingDelay：周期为1ms的相减基准变量
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
static __IO uint8_t KeySacnTimeNum = 0;

void TimingDelay_Decrement(void)
{
  if(KeySacnTimeNum++ >= 5)		//5ms进行扫描
  {
    KeySacnTimeNum = 0;
  }
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : InitChip
// 功能描述  : STM32芯片的初始化
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void InitChip(void)			
{
  
  /*初始化芯片*/
  //    SystemInit();
  
  
#ifdef STM32_BOOT
  /* Set the Vector Table base location at 0x08000000*/  
  //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000);   
  
#endif 
  
  
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : InitHardWare
// 功能描述  : 包括串口、板载资源的硬件初始化
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void InitHardWare(void)			
{   
  /*STM32芯片的初始化*/
  //    InitChip();
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//时钟初始化HSI——16Mhz
  /*led初始化*/  
  API_Led_Init();
  
  /*模块电源开启*/
  GPIO_Init(GPIOG, GPIO_Pin_2|GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOD, GPIO_Pin_7|GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_ResetBits(GPIOG,GPIO_Pin_3);GPIO_SetBits(GPIOG,GPIO_Pin_2);
  
#if CONF_EX_MEMORY_EN == UPLOAD_SPI_FLASH  //外存储器为flash
  W25QXX_Init();
#elif CONF_EX_MEMORY_EN == UPLOAD_EEPROM   //外存储器为eeprom
  GPIO_EEPROM_INIT();/*外部eepromIO初始化*/
#endif    
  
  /*协议初始化*/
  API_Protocols_Init();
  
  
  /*定时器模块初始化*/
  API_Timers_Init();
  
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : InitSoftSystem
// 功能描述  : 使用的功能模块进行配置
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void InitSoftSystem(void)		
{
  
  /*选择进入用户或引导程序*/
  //	API_SetTimer(2000,1,TIMER_TYPE_IAP_SELECT_PROGRAM,API_Select_Program_run,NULL);
  
  /*网络初始化*/
  API_qwl_net_init();
  
  /*网络数据发送初始化*/
  API_net_data_send_init();
  
  /*启动定时器开始运行*/
  API_StartRunTimer();
}
void upgrade_init(void)
{
  
  //          /******eeprom读写测试小程序ok放在while前************/
  //          if(EEPROM_CheckOk()==1)
  //            qwl_sendstring("eeprom测试OK\r\n");
  //          else
  //            qwl_sendstring("eeprom测试ERR\r\n");
  //          /***************************************************/ 
  /*包括串口、板载资源的硬件初始化*/	
  disableInterrupts();
  InitHardWare();	
  
  PrintString_("开始通讯");
  /*使用的功能模块进行配置*/
  InitSoftSystem();  
  //         DEBUG("time:%s\n",__TIME__); 
  enableInterrupts();//打开全局中断
  //        /*********串口3打印eeprom里数据************/
  //        AT24C256_ALL_CLEAR();
  //        AT24C256_TO_Print();      
  //  
  //        /*****************************************/ 
}


void SA_init(void)
{ 
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);//时钟初始化HSI——2Mhz
  
  disableInterrupts();//关闭全局中断
//  SysKeydataInit();
  DelayT_ms(200);
  uart1_init(9600);//串口1初始化
  //uart1_init(115200);//串口1初始化
  uart3_init(9600);//串口3初始化
  DelayT_ms(200);
  /*********串口3打印eeprom里数据************/
  //              AT24C256_TO_Print();  
  //              while(1);
  //  
  //    for(u16 Temp_ADDR=0; Temp_ADDR<0x7FFF; Temp_ADDR++){
  //      USART_SendData8(USART3, read_eep_byte(Temp_ADDR));
  //      while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
  //    }
  /*****************************************/ 
  Read_sys_keydata();//从EEPROM读取关键参数
  /*****************低功耗设置********************/
  RTC_Config();     //RTC唤醒时电流730UA
  GPIO_LowPower_Config();//IO设置
  /***********************************************/
  GPIO_Init(GPIOG, GPIO_Pin_3, GPIO_Mode_Out_OD_HiZ_Slow);//电源脚 设置为高输出 关断MC8618电源
  GPIO_ResetBits(GPIOG,GPIO_Pin_2);//MC8618电源升压失能
  GPIO_SetBits(GPIOG,GPIO_Pin_3);//MC8618电源关断
  /*****************外设初始化********************/
  
  /*****************打印系统信息*******************************************************/
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
  PrintString_("智能消防栓水压");
#if SPEED_UP
  PrintString_("注意！这个是RTC加速版本，不能用于实际生产！");
#else 
  PrintString_("正式版本");
#endif 
#if PRESSURE_MODE_SELECT
  PrintString_("传感器：1MPA版本"); //1mPa版本
#else
  PrintString_("传感器：1.6MPA版本"); //1.6mPa版本
#endif
#endif   
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
  PrintString_("智能消防栓闷盖");
#if SPEED_UP
  PrintString_(">>>>!注意!!注意!!注意!<<<<");
  PrintString_(">>>>!!!这是RTC加速版本！不能用于实际产品!!!<<<<");
#endif
#if RTC_CAN_BE_SET 
  PrintString_("RTC闹钟根据检测间隔可设置模式");
#else
  PrintString_("RTC闹钟定时固定半小时模式");
#endif  
  
#endif    
  /******************外部储存芯片测试***************/
#if CONF_EX_MEMORY_EN == UPLOAD_SPI_FLASH  //外存储器为flash
  W25QXX_Init();
  W25QXX_Test();
#elif CONF_EX_MEMORY_EN == UPLOAD_EEPROM   //外存储器为eeprom
  if(EEPROM_CheckOk()==1)
    PrintString_("eeprom测试OK\r\n");
  else
    PrintString_("eeprom测试ERR\r\n");
#endif
  /***************************************************/   
  
//  ShowModule();
  PrintString_("#####");
  PrintString_("设备ID:");
  PrintString_((char *)(Sys_keydata.device_id));
  PrintString_("采集IP地址：");
  PrintString_((char *)(Sys_keydata.default_cloud_server_add));
  PrintSysInfo();//打印固件版本号
  
  /***********************************************/
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
  adxl362_spi_config();//SPI1初始化
  Parameter_Init(); //清除一些动态参数
#endif   
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
  
#endif  
  timer4Init();//计数器4初始化
  /***********************************************/
  
  /*****************参数初始化********************/
  //  SysParaInit();//系统参数初始化
  PrintSysParaList();
  /***********************************************/
#if WDG_EN   
  Watchdog_INIT();
#else
  PrintString_("!!!看门狗被关闭!!!");
#endif
  Set_Key_Para();
  enableInterrupts();//打开全局中断
  //DelayT_ms(500);
  /***********************************************/
  //  ClsAnalyseState();//清除状态
  //uart2_init(115200);
  waitforconfig_timecount = 0;
  
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
  GetTime(0);  //产生系统内部RTC，只是用来做告警间隔。
  GetNewInactAngle();//设置零点角度
  //  ClsAnalyseState();//清除状态
  XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//清除分析模式的所有状态
  XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//清除分析模式的所有状态 
  IntoLowpowerMode(); //进入低功耗模式
  GPIO_LowPower_Config(); 
  NewInactAngle_InitPara();
#endif   
  
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
  InitFLASH_Queue();    //初始化队列
  RTC_SD3078_Init();  //初始化时钟。
  if(RTC_SD3078_CheckInitFlag())
  {
    TickGetTime(946684800); //初始化为2000/1/1 8:0:0
  }
  
  PressureSysFunStateInit(); //初始化设备状态机
  ResetUpdataInit();
  init_all_alarmtime();
//  PressureSysFunState = PressureSysFunState_Initialize;
#endif  
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※ ※※※※※※※※
// 函数名称  : main
// 功能描述  : 系统主函数
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
int main(void)  
{   
  SA_init();
  if(API_Get_Iap_Config() > -1)
  {
    PrintString_("API参数读取成功");//如果有更新，会先进发心跳并读版本
  }
  else
  {
    Init_Iap_Config();
    PrintString_("API参数读取失败");
    iap_data.writeflag = MODE_ING_DOWNLOAD;//失败后进低功耗先发一次心跳
  }
  Net_Data_Send.Link_Type = HEARTBEAT;//默认先进心跳采集
//  Net_Data_Send.Link_Type = UPDATA;
  while (1)
  {
    switch(iap_data.writeflag)
    {
    case MODE_ING_SA_SYS://井盖程序
      
      switch(SA_Pro)
      {  
      case PRO_INIT:
        CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);
        timer4Init(); //定时器初始化
        uart3_init(9600);//串口3初始化
        uart1_init(9600);//串口1初始化
        SA_Pro = PRO_RUN;
        break;
      case PRO_RUN:
        Watchdog_Feed();
        TaskSchedule();
        break;
      }
      break;
    case MODE_ING_DOWNLOAD:
      switch(Down_Pro)
      {
      case PRO_INIT:
        //状态初始化
        upgrade_init();
        Increase_send_totalcount();//累加电池发送次数
        Down_Pro = PRO_RUN;
        break;
      case PRO_RUN:
        /*检查进入用户或引导程序标志*/
        API_Check_Run_User_Program_Falg();
        Watchdog_Feed();
        
        /*定时器模块处理*/
        DoTimer();
        Watchdog_Feed();
        
        /*用于分析接收到的数据并应答*/  
        API_DoProtocols();//回调该函数ReceivePro_Debug_Dispose
        Watchdog_Feed();
        
        /*自动建立网络*/
        //                API_net_state();
        if(Run_User_Program_Falg ==false)
          net_state();
        Watchdog_Feed();
        
        /*用于主动发送数据*/
        API_net_data_send();
        Watchdog_Feed();
        break;
      case PRO_END:
        //upgrade_init();.
        //进程初始化
        Down_Pro=PRO_INIT;
        //跳转井盖检测
        Net_Data_Send.Link_Type = HEARTBEAT;
        iap_data.writeflag=MODE_ING_SA_SYS;
        Save_Iap_Config();
        //其他状态
        SA_Pro=PRO_INIT;
        //        GetNewInactAngle_Fun_state = 3;//重新设置零点角度
        //报警清零
        //        Sys_keydata.Angle = 0;//倾斜角度
        //        Sys_keydata.X_Value = 0;//X轴加速度
        //        Sys_keydata.Y_Value = 0;//Y轴加速度
        //        Sys_keydata.Z_Value = 0;//Z轴加速度
        //        Sys_keydata.Low_Temp_Alarm = 0;//低温告警
        //        Sys_keydata.Trigger_Alarm = 0;//触发告警
        break;
      default:
        break;
      }
      break;
    default:
      break;
    }
  }
  
}

/******************************FLASH测试********************************/
// Flash_Config(Flash_FLASH_Unlock); Flash_Config(Flash_FLASH_Unlock); Flash_Config(Flash_FLASH_Unlock); Flash_Config(Flash_FLASH_Unlock);
// Flash_Config(Flash_FLASH_Unlock); Flash_Config(Flash_FLASH_Unlock); Flash_Config(Flash_FLASH_Unlock);
//  Flash_Config(Flash_FLASH_Unlock);
//  Flash_FLASH_WriteByte((uint32_t)0xFFFF,0XFF);
//  Flash_FLASH_WriteByte((uint32_t)0x10000,0XFF);
//  Flash_FLASH_WriteByte((uint32_t)0x10001,0X00);
//  Flash_FLASH_WriteByte((uint32_t)0x10010,0X49);
//  if(Flash_FLASH_Test())
//  {  if(Flash_FLASH_Test())
//  {        
////    Flash_Config(Flash_FLASH_Unlock);
////	FLASH_ProgramWord((uint32_t)0x10000,0XFF);
////        Flash_Config(Flash_FLASH_Lock);
//    qwl_sendstring("1231");
//  }
//  }
/***********************************end of line ********************************/

