/**
  * @brief  configure GPIOs before entering low power
	* @caller Halt_Init and ADC_Icc_Test
  * @param None
  * @retval None
  */  
//#include "stm8l15x.h"
#include "config.h"
// **************************************************************
// 函数:GPIO_LowPower_Config
// 参数:无
// 返回:无
// 描述:配置低功耗模式下的端口属性
// **************************************************************
#if CONF_HARDWARE_VERSION == HARDWARE_VERSION_V105
  void GPIO_LowPower_Config(void) 
  {
    GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
    
    GPIO_Init(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOB,GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);//注意不要开中断
    adxl362_int2_enable();//PIN2 PIN3配置为下降沿触发
      
   /*外部eepromIO初始化*/
    GPIO_EEPROM_INIT();//2017/10/27
    //adxl362_int2_gpio_init();
    
   GPIO_Init(GPIOC, GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOC, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Slow);


    GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Slow);//串口1发送
    //GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_FL_No_IT);//串口1接收
    GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);//串口1接收
    

    //GPIO_ResetBits(GPIOC, GPIO_Pin_2);//串口1接收 置低
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);//串口1发送 置低
    #ifdef SYS_LDO_20V
    uart1_disable();
     #endif

    GPIO_Init(GPIOD, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Mode_Out_OD_HiZ_Slow);//电源脚 设置为高输出 关断MC8618电源
    GPIO_ResetBits(GPIOB,GPIO_Pin_1);//MC8618电源升压失能
    GPIO_SetBits(GPIOD,GPIO_Pin_4);//MC8618电源关断

    GPIO_Init(GPIOE, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Slow);
    
    GPIO_Init(GPIOE, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Slow);

    GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Slow);//电源脚 设置为高输出 打开ADXL电源
    GPIO_SetBits(GPIOE,GPIO_Pin_7);//ADXL电源打开
    
    GPIO_Init(GPIOF, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
//    GPIO_Init(GPIOF,GPIO_Pin_1,GPIO_Mode_In_FL_No_IT);//by hx 2017-06-07


    GPIO_Init(GPIOG, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOG, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Slow);
    GPIO_Init(GPIOG,GPIO_Pin_0,GPIO_Mode_In_PU_No_IT);//串口3
    
    SetUart3RxAsExInt();//设置串口为终端模式 只有外部中断打开  才能从RTC模式下唤醒 并接受数据

  }
#endif


#if CONF_HARDWARE_VERSION == HARDWARE_VERSION_V205//硬件版本为V2.05版本
void GPIO_LowPower_Config(void) 
{
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码  
  //  GPIO_Init(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  //  GPIO_SetBits(GPIOA,GPIO_Pin_4);
  //  GPIO_Init(GPIOA,GPIO_Pin_5,GPIO_Mode_In_PU_IT);//浮空输入//by yw 2017-8-31
  //  EXTI_SetPinSensitivity(EXTI_Pin_5,EXTI_Trigger_Rising_Falling);  //开关检测脚，中断模式
  
  GPIO_Init(GPIOA,GPIO_Pin_0,GPIO_Mode_Out_PP_Low_Slow);        //1 SWIM
  GPIO_Init(GPIOA,GPIO_Pin_1,GPIO_Mode_Out_PP_Low_Slow);        //2 RESET_MCU
  GPIO_Init(GPIOA,GPIO_Pin_2,GPIO_Mode_Out_PP_Low_Slow);        //3 NO
  GPIO_Init(GPIOA,GPIO_Pin_3,GPIO_Mode_Out_PP_Low_Slow);        //4 NO
  //    GPIO_Init(GPIOA,GPIO_Pin_4,GPIO_Mode_Out_OD_HiZ_Slow);        //5 POW_MES
  GPIO_Init(GPIOA,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Slow);        //5 POW_MES
  GPIO_SetBits(GPIOA,GPIO_Pin_4);
  //  GPIO_ResetBits(GPIOA,GPIO_Pin_4);
  //  GPIO_Init(GPIOA,GPIO_Pin_5,GPIO_Mode_In_PU_No_IT);            //6 SW2_IN
  //  GPIO_Init(GPIOA,GPIO_Pin_5,GPIO_Mode_Out_OD_HiZ_Slow);            //6 SW2_IN
  GPIO_Init(GPIOA,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Slow);            //6 SW2_IN
  GPIO_Init(GPIOA,GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Slow);        //7 NO
  GPIO_Init(GPIOA,GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);        //8 NO
  
  
  //  GPIO_Init(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
  //  //GPIO_Init(GPIOB,GPIO_Pin_3, GPIO_Mode_In_FL_IT);//接ADXL362 INT2脚
  //  GPIO_Init(GPIOB,GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);//注意不要开中断
  //  adxl362_int2_enable();//PIN2 PIN3配置为下降沿触发
  
  GPIO_Init(GPIOB,GPIO_Pin_0,GPIO_Mode_Out_PP_Low_Slow);        //31 NO
  GPIO_Init(GPIOB,GPIO_Pin_1,GPIO_Mode_Out_PP_Low_Slow);        //32 NO
  //  adxl362_int2_enable();//33 INT1_AD 34 INT2_AD 配置为下降沿触发
  GPIO_Init(GPIOB,GPIO_Pin_2,GPIO_Mode_Out_PP_Low_Slow);        //33 INT1_AD
  GPIO_Init(GPIOB,GPIO_Pin_3,GPIO_Mode_Out_PP_Low_Slow);        //34 INT2_AD
  GPIO_Init(GPIOB,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Slow);        //35 C\S\_AD
  GPIO_Init(GPIOB,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Slow);        //36 SCLK_AD
  GPIO_Init(GPIOB,GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Slow);        //37 MOSI_AD
  GPIO_Init(GPIOB,GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);            //38 MISO_AD
  //  GPIO_Init(GPIOB,GPIO_Pin_7,GPIO_Mode_In_FL_No_IT);            //38 MISO_AD
  
  //  GPIO_Init(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  //
  //  //#ifdef SYS_LDO_20V
  //  uart1_disable();
  
  GPIO_Init(GPIOC,GPIO_Pin_0,GPIO_Mode_Out_OD_HiZ_Slow);        //53 SD_SDA
  GPIO_Init(GPIOC,GPIO_Pin_1,GPIO_Mode_Out_OD_HiZ_Slow);        //54 SD_SCL
  //  GPIO_Init(GPIOC,GPIO_Pin_0,GPIO_Mode_Out_PP_Low_Slow);        //53 SD_SDA
  //  GPIO_Init(GPIOC,GPIO_Pin_1,GPIO_Mode_Out_PP_Low_Slow);        //54 SD_SCL  
  uart1_disable(); //57 RX1_MCU  58 TX1_MCU
  //  GPIO_Init(GPIOC,GPIO_Pin_2,GPIO_Mode_Out_PP_Low_Slow);          //57 RX1_MCU
  //  GPIO_Init(GPIOC,GPIO_Pin_3,GPIO_Mode_Out_PP_Low_Slow);          //58 TX1_MCU
  GPIO_Init(GPIOC,GPIO_Pin_4,GPIO_Mode_In_PU_No_IT);            //59 SD_INT
  //  GPIO_Init(GPIOC,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Slow);            //59 SD_INT
  GPIO_Init(GPIOC,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Slow);      //60 OSC32_OUT
  GPIO_Init(GPIOC,GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Slow);      //61 OSC32_IN
  GPIO_Init(GPIOC,GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);        //62 BAT_UP  
  
  //#endif
  
  //  GPIO_Init(GPIOD, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOD,GPIO_Pin_0,GPIO_Mode_Out_PP_Low_Slow);        //25 NO
  GPIO_Init(GPIOD,GPIO_Pin_1,GPIO_Mode_Out_PP_Low_Slow);        //26 NO
  GPIO_Init(GPIOD,GPIO_Pin_2,GPIO_Mode_Out_PP_Low_Slow);        //27 NO
  GPIO_Init(GPIOD,GPIO_Pin_3,GPIO_Mode_Out_PP_Low_Slow);        //28 NO
  GPIO_Init(GPIOD,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Slow);        //45 NO
  GPIO_Init(GPIOD,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Slow);        //46 NO
  GPIO_Init(GPIOD,GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Slow);        //47 ON/OFF_MCU
  GPIO_Init(GPIOD,GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);        //48 RES_MCU 
  
  
  //  //GPIO_Init(GPIOE, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  //  GPIO_Init(GPIOE, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  //  //GPIO_Init(GPIOE, GPIO_Pin_0, GPIO_Mode_Out_OD_HiZ_Slow);//雷达电源脚 设置为高输出 关断雷达电源
  //  GPIO_SetBits(GPIOE,GPIO_Pin_0);//雷达电源关断
  //  //GPIO_ResetBits(GPIOE,GPIO_Pin_0);//雷达电源打开
  
  //  GPIO_Init(GPIOE,GPIO_Pin_0,GPIO_Mode_Out_PP_High_Slow);       //19 LED
  GPIO_Init(GPIOE,GPIO_Pin_0,GPIO_Mode_Out_PP_Low_Slow);       //19 LED
  GPIO_SetBits(GPIOE,GPIO_Pin_0);
  GPIO_Init(GPIOE,GPIO_Pin_1,GPIO_Mode_Out_PP_Low_Slow);        //20 HOLD_W
  //  GPIO_Init(GPIOE,GPIO_Pin_1,GPIO_Mode_Out_OD_HiZ_Slow);        //20 HOLD_W
  GPIO_Init(GPIOE,GPIO_Pin_2,GPIO_Mode_Out_PP_Low_Slow);        //21 WP_W
  //  USART_ClearFlag(USART2, USART_FLAG_RXNE);//清楚接收中断标志
  //  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//关闭接收中断
  //  USART_DeInit(USART2);
  //  USART_Cmd(USART2, DISABLE);
  GPIO_Init(GPIOE,GPIO_Pin_3,GPIO_Mode_Out_PP_Low_Slow);        //22 RX2_MCU
  GPIO_Init(GPIOE,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Slow);        //23 TX2_MCU
  //  GPIO_Init(GPIOE,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Slow);        //24 NO
  GPIO_Init(GPIOE,GPIO_Pin_5,GPIO_Mode_In_PU_IT); 
  EXTI_SetPinSensitivity(EXTI_Pin_5,EXTI_Trigger_Falling); 
  
  GPIO_Init(GPIOE,GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Slow);        //63 BAT_DO
  GPIO_Init(GPIOE,GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);        //64 NO
  
  //  GPIO_Init(GPIOF, GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  //  GPIO_Init(GPIOF, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);//电源脚 设置为高输出 打开ADXL电源
  //  GPIO_SetBits(GPIOF,GPIO_Pin_4);//ADXL电源打开
  //
  //  GPIO_Init(GPIOF,GPIO_Pin_1,GPIO_Mode_In_PU_IT);//浮空输入
  //  EXTI_SetPinSensitivity(EXTI_Pin_1,EXTI_Trigger_Rising_Falling);  //开关检测脚，中断模式
  
  GPIO_Init(GPIOF,GPIO_Pin_0,GPIO_Mode_Out_PP_Low_Slow);        //39 ADC_IN
  GPIO_Init(GPIOF,GPIO_Pin_1,GPIO_Mode_In_PU_No_IT);            //40 SW1_IN
  GPIO_Init(GPIOF,GPIO_Pin_2,GPIO_Mode_Out_PP_Low_Slow);        // NO
  GPIO_Init(GPIOF,GPIO_Pin_3,GPIO_Mode_Out_PP_Low_Slow);        // NO
  GPIO_Init(GPIOF,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Slow);        //41 VDD_ADXL
  //  GPIO_Init(GPIOF,GPIO_Pin_4,GPIO_Mode_Out_PP_High_Slow);        //41 VDD_ADXL
  GPIO_SetBits(GPIOF,GPIO_Pin_4);
  
  GPIO_Init(GPIOF,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Slow);       //42 NO
  GPIO_Init(GPIOF,GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Slow);        //43 NO
  GPIO_Init(GPIOF,GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);        //44 NO
  //  GPIO_Init(GPIOG, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  
  SetUart3RxAsExInt();//设置串口为终端模式 只有外部中断打开  才能从RTC模式下唤醒 并接受数据 14 RX3_MCU 15 TX3_MCU
  //  GPIO_Init(GPIOG,GPIO_Pin_0,GPIO_Mode_Out_PP_Low_Slow);        //14 RX3_MCU
  //  GPIO_Init(GPIOG,GPIO_Pin_1,GPIO_Mode_Out_PP_Low_Slow);        //15 TX3_MCU
  GPIO_Init(GPIOG,GPIO_Pin_2,GPIO_Mode_Out_OD_HiZ_Slow);        //16 POW_SDB
  GPIO_Init(GPIOG,GPIO_Pin_3,GPIO_Mode_Out_OD_HiZ_Slow);        //17 POW_TPS
  GPIO_Init(GPIOG,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Slow);        //49 CS_W
  GPIO_Init(GPIOG,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Slow);        //50 SCK_W
  GPIO_Init(GPIOG,GPIO_Pin_6,GPIO_Mode_Out_PP_Low_Slow);        //51 MOSI_W
  GPIO_Init(GPIOG,GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);        //52 MISO_W
#endif
  
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
  GPIO_Init(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_7,GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOA,GPIO_Pin_4,GPIO_Mode_Out_OD_HiZ_Slow);
  GPIO_SetBits(GPIOA,GPIO_Pin_4);  
  GPIO_Init(GPIOA,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Slow);
  XiaoFang_Interrupt_Init();
  
  
  GPIO_Init(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOB,GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);//注意不要开中断
  adxl362_int2_enable();//PIN2 PIN3配置为下降沿触发
  
  GPIO_Init(GPIOC,GPIO_Pin_0|GPIO_Pin_1,GPIO_Mode_Out_OD_HiZ_Slow);
  GPIO_Init(GPIOC,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow); 
//  uart1_disable();
  USART_ClearFlag(USART1, USART_FLAG_RXNE);//清楚接收中断标志
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//关闭接收中断
  USART_DeInit(USART1);
  USART_Cmd(USART1, DISABLE);
  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast); //接收口
  GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast); //发送口
  
  GPIO_ResetBits(GPIOC, GPIO_Pin_2);
  GPIO_ResetBits(GPIOC, GPIO_Pin_3);
  //  GPIO_Init(GPIOC,GPIO_Pin_4,GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOC,GPIO_Pin_4,GPIO_Mode_Out_PP_High_Slow);
  
  GPIO_Init(GPIOD, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  
  GPIO_Init(GPIOE,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOE,GPIO_Pin_0, GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(GPIOE, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Slow);
  
  //GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Slow);//电源脚 设置为高输出 打开ADXL电源
  //GPIO_SetBits(GPIOE,GPIO_Pin_7);//ADXL电源打开
  
  //GPIO_Init(GPIOF, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOF, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOF, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);//电源脚 设置为高输出 打开ADXL电源
  GPIO_SetBits(GPIOF,GPIO_Pin_4);//ADXL电源打开
  
  
  GPIO_Init(GPIOG, GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
  
  GPIO_Init(GPIOG, GPIO_Pin_3, GPIO_Mode_Out_OD_HiZ_Slow);//电源脚 设置为高输出 关断MC8618电源
  GPIO_ResetBits(GPIOG,GPIO_Pin_2);//MC8618电源升压失能
  GPIO_SetBits(GPIOG,GPIO_Pin_3);//MC8618电源关断
  
  GPIO_Init(GPIOG, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(GPIOG,GPIO_Pin_0,GPIO_Mode_In_PU_No_IT);//串口3
  
  SetUart3RxAsExInt();//设置串口为终端模式 只有外部中断打开  才能从RTC模式下唤醒 并接受数据
  //GPIO_Init(GPIOG, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Slow);//串口3
#endif 
}
#endif


// **************************************************************
// 函数:IntoLowpowerMode
// 参数:无
// 返回:无
// 描述:进入低功耗模式的相关配置修改
// **************************************************************
void IntoLowpowerMode(void)
{
  
//  Watchdog_Feed();
//  //GPIO_LowPower_Config(); 
//  /*Watchdog_Feed();
//  CLK_HaltConfig(CLK_Halt_FastWakeup,ENABLE);
//  GPIO_LowPower_Config(); 
//  PWR_FastWakeUpCmd(ENABLE);
//  PWR_UltraLowPowerCmd(ENABLE);  
//  RTC_ITConfig(RTC_IT_WUT, ENABLE);
//  RTC_WakeUpCmd(ENABLE);
//  Watchdog_Feed();
//  halt(); */
//  if((!TashSchedule_State)&&(!sysFunState)&&(!GetNewInactAngle_Fun_state)){
//      PrintString("<<<<<<<<<<<<<<<<<<<<<<<<<<<<< into lowpower_mode[IntoLowpowerMode]!!!");
//      PrintString_(">>>>>>>>>>>>>>>>>>>>>>>>>>>>");
//      Watchdog_Feed();
//      GPIO_LowPower_Config(); 
//      W25QXX_PowerDown();
//      
//      
//      CLK_HaltConfig(CLK_Halt_FastWakeup,ENABLE);
//      PWR_FastWakeUpCmd(ENABLE);
//      PWR_UltraLowPowerCmd(ENABLE);  
//      RTC_ITConfig(RTC_IT_WUT, ENABLE);
//      RTC_WakeUpCmd(ENABLE);
//      Watchdog_Feed();
//      halt(); 
//      GPIO_LowPower_Config(); 
//  }
  
    Watchdog_Feed();
  
//  GPIO_Init(GPIOE, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Slow);
//  GPIO_SetBits(GPIOE, GPIO_Pin_2);
//  GPIO_Init(GPIOE, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Slow);
//  GPIO_SetBits(GPIOE, GPIO_Pin_1);   
//  SPI2_Init();
//  W25QXX_PowerDown();
  
//  GPIO_LowPower_Config(); 
  
  
  Watchdog_Feed();
  CLK_HaltConfig(CLK_Halt_FastWakeup,ENABLE);
  GPIO_LowPower_Config(); 
  W25QXX_LWP();
  PWR_FastWakeUpCmd(ENABLE);
  PWR_UltraLowPowerCmd(ENABLE);  
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  RTC_WakeUpCmd(ENABLE);
  Watchdog_Feed();
  halt(); 
  W25QXX_EXIT_LWP();
}