#include "config.h"

//������������������������������������������������������������������������������
// ��������  : TimingDelay_Decrement
// ��������  : ʹ��SysTick����ȷ��ʱ,TimingDelay������Ϊ1ms�������׼����
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static __IO uint8_t KeySacnTimeNum = 0;

void TimingDelay_Decrement(void)
{
  if(KeySacnTimeNum++ >= 5)		//5ms����ɨ��
  {
    KeySacnTimeNum = 0;
  }
}

//������������������������������������������������������������������������������
// ��������  : InitChip
// ��������  : STM32оƬ�ĳ�ʼ��
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void InitChip(void)			
{
  
  /*��ʼ��оƬ*/
  //    SystemInit();
  
  
#ifdef STM32_BOOT
  /* Set the Vector Table base location at 0x08000000*/  
  //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000);   
  
#endif 
  
  
}

//������������������������������������������������������������������������������
// ��������  : InitHardWare
// ��������  : �������ڡ�������Դ��Ӳ����ʼ��
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void InitHardWare(void)			
{   
  /*STM32оƬ�ĳ�ʼ��*/
  //    InitChip();
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);//ʱ�ӳ�ʼ��HSI����16Mhz
  /*led��ʼ��*/  
  API_Led_Init();
  
  /*ģ���Դ����*/
  GPIO_Init(GPIOG, GPIO_Pin_2|GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOD, GPIO_Pin_7|GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_ResetBits(GPIOG,GPIO_Pin_3);GPIO_SetBits(GPIOG,GPIO_Pin_2);
  
#if CONF_EX_MEMORY_EN == UPLOAD_SPI_FLASH  //��洢��Ϊflash
  W25QXX_Init();
#elif CONF_EX_MEMORY_EN == UPLOAD_EEPROM   //��洢��Ϊeeprom
  GPIO_EEPROM_INIT();/*�ⲿeepromIO��ʼ��*/
#endif    
  
  /*Э���ʼ��*/
  API_Protocols_Init();
  
  
  /*��ʱ��ģ���ʼ��*/
  API_Timers_Init();
  
}

//������������������������������������������������������������������������������
// ��������  : InitSoftSystem
// ��������  : ʹ�õĹ���ģ���������
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void InitSoftSystem(void)		
{
  
  /*ѡ������û�����������*/
  //	API_SetTimer(2000,1,TIMER_TYPE_IAP_SELECT_PROGRAM,API_Select_Program_run,NULL);
  
  /*�����ʼ��*/
  API_qwl_net_init();
  
  /*�������ݷ��ͳ�ʼ��*/
  API_net_data_send_init();
  
  /*������ʱ����ʼ����*/
  API_StartRunTimer();
}
void upgrade_init(void)
{
  
  //          /******eeprom��д����С����ok����whileǰ************/
  //          if(EEPROM_CheckOk()==1)
  //            qwl_sendstring("eeprom����OK\r\n");
  //          else
  //            qwl_sendstring("eeprom����ERR\r\n");
  //          /***************************************************/ 
  /*�������ڡ�������Դ��Ӳ����ʼ��*/	
  disableInterrupts();
  InitHardWare();	
  
  PrintString_("��ʼͨѶ");
  /*ʹ�õĹ���ģ���������*/
  InitSoftSystem();  
  //         DEBUG("time:%s\n",__TIME__); 
  enableInterrupts();//��ȫ���ж�
  //        /*********����3��ӡeeprom������************/
  //        AT24C256_ALL_CLEAR();
  //        AT24C256_TO_Print();      
  //  
  //        /*****************************************/ 
}


void SA_init(void)
{ 
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);//ʱ�ӳ�ʼ��HSI����2Mhz
  
  disableInterrupts();//�ر�ȫ���ж�
//  SysKeydataInit();
  DelayT_ms(200);
  uart1_init(9600);//����1��ʼ��
  //uart1_init(115200);//����1��ʼ��
  uart3_init(9600);//����3��ʼ��
  DelayT_ms(200);
  /*********����3��ӡeeprom������************/
  //              AT24C256_TO_Print();  
  //              while(1);
  //  
  //    for(u16 Temp_ADDR=0; Temp_ADDR<0x7FFF; Temp_ADDR++){
  //      USART_SendData8(USART3, read_eep_byte(Temp_ADDR));
  //      while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
  //    }
  /*****************************************/ 
  Read_sys_keydata();//��EEPROM��ȡ�ؼ�����
  /*****************�͹�������********************/
  RTC_Config();     //RTC����ʱ����730UA
  GPIO_LowPower_Config();//IO����
  /***********************************************/
  GPIO_Init(GPIOG, GPIO_Pin_3, GPIO_Mode_Out_OD_HiZ_Slow);//��Դ�� ����Ϊ����� �ض�MC8618��Դ
  GPIO_ResetBits(GPIOG,GPIO_Pin_2);//MC8618��Դ��ѹʧ��
  GPIO_SetBits(GPIOG,GPIO_Pin_3);//MC8618��Դ�ض�
  /*****************�����ʼ��********************/
  
  /*****************��ӡϵͳ��Ϣ*******************************************************/
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
  PrintString_("��������˨ˮѹ");
#if SPEED_UP
  PrintString_("ע�⣡�����RTC���ٰ汾����������ʵ��������");
#else 
  PrintString_("��ʽ�汾");
#endif 
#if PRESSURE_MODE_SELECT
  PrintString_("��������1MPA�汾"); //1mPa�汾
#else
  PrintString_("��������1.6MPA�汾"); //1.6mPa�汾
#endif
#endif   
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
  PrintString_("��������˨�Ƹ�");
#if SPEED_UP
  PrintString_(">>>>!ע��!!ע��!!ע��!<<<<");
  PrintString_(">>>>!!!����RTC���ٰ汾����������ʵ�ʲ�Ʒ!!!<<<<");
#endif
#if RTC_CAN_BE_SET 
  PrintString_("RTC���Ӹ��ݼ����������ģʽ");
#else
  PrintString_("RTC���Ӷ�ʱ�̶���Сʱģʽ");
#endif  
  
#endif    
  /******************�ⲿ����оƬ����***************/
#if CONF_EX_MEMORY_EN == UPLOAD_SPI_FLASH  //��洢��Ϊflash
  W25QXX_Init();
  W25QXX_Test();
#elif CONF_EX_MEMORY_EN == UPLOAD_EEPROM   //��洢��Ϊeeprom
  if(EEPROM_CheckOk()==1)
    PrintString_("eeprom����OK\r\n");
  else
    PrintString_("eeprom����ERR\r\n");
#endif
  /***************************************************/   
  
//  ShowModule();
  PrintString_("#####");
  PrintString_("�豸ID:");
  PrintString_((char *)(Sys_keydata.device_id));
  PrintString_("�ɼ�IP��ַ��");
  PrintString_((char *)(Sys_keydata.default_cloud_server_add));
  PrintSysInfo();//��ӡ�̼��汾��
  
  /***********************************************/
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
  adxl362_spi_config();//SPI1��ʼ��
  Parameter_Init(); //���һЩ��̬����
#endif   
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
  
#endif  
  timer4Init();//������4��ʼ��
  /***********************************************/
  
  /*****************������ʼ��********************/
  //  SysParaInit();//ϵͳ������ʼ��
  PrintSysParaList();
  /***********************************************/
#if WDG_EN   
  Watchdog_INIT();
#else
  PrintString_("!!!���Ź����ر�!!!");
#endif
  Set_Key_Para();
  enableInterrupts();//��ȫ���ж�
  //DelayT_ms(500);
  /***********************************************/
  //  ClsAnalyseState();//���״̬
  //uart2_init(115200);
  waitforconfig_timecount = 0;
  
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
  GetTime(0);  //����ϵͳ�ڲ�RTC��ֻ���������澯�����
  GetNewInactAngle();//�������Ƕ�
  //  ClsAnalyseState();//���״̬
  XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangZ);//�������ģʽ������״̬
  XiaoFang_ClsAnalyseState(&Analyse_data_para_XiaoFangX);//�������ģʽ������״̬ 
  IntoLowpowerMode(); //����͹���ģʽ
  GPIO_LowPower_Config(); 
  NewInactAngle_InitPara();
#endif   
  
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
  InitFLASH_Queue();    //��ʼ������
  RTC_SD3078_Init();  //��ʼ��ʱ�ӡ�
  if(RTC_SD3078_CheckInitFlag())
  {
    TickGetTime(946684800); //��ʼ��Ϊ2000/1/1 8:0:0
  }
  
  PressureSysFunStateInit(); //��ʼ���豸״̬��
  ResetUpdataInit();
  init_all_alarmtime();
//  PressureSysFunState = PressureSysFunState_Initialize;
#endif  
}
//�������������������������������������������������������������� ����������������
// ��������  : main
// ��������  : ϵͳ������
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int main(void)  
{   
  SA_init();
  if(API_Get_Iap_Config() > -1)
  {
    PrintString_("API������ȡ�ɹ�");//����и��£����Ƚ������������汾
  }
  else
  {
    Init_Iap_Config();
    PrintString_("API������ȡʧ��");
    iap_data.writeflag = MODE_ING_DOWNLOAD;//ʧ�ܺ���͹����ȷ�һ������
  }
  Net_Data_Send.Link_Type = HEARTBEAT;//Ĭ���Ƚ������ɼ�
//  Net_Data_Send.Link_Type = UPDATA;
  while (1)
  {
    switch(iap_data.writeflag)
    {
    case MODE_ING_SA_SYS://���ǳ���
      
      switch(SA_Pro)
      {  
      case PRO_INIT:
        CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_8);
        timer4Init(); //��ʱ����ʼ��
        uart3_init(9600);//����3��ʼ��
        uart1_init(9600);//����1��ʼ��
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
        //״̬��ʼ��
        upgrade_init();
        Increase_send_totalcount();//�ۼӵ�ط��ʹ���
        Down_Pro = PRO_RUN;
        break;
      case PRO_RUN:
        /*�������û������������־*/
        API_Check_Run_User_Program_Falg();
        Watchdog_Feed();
        
        /*��ʱ��ģ�鴦��*/
        DoTimer();
        Watchdog_Feed();
        
        /*���ڷ������յ������ݲ�Ӧ��*/  
        API_DoProtocols();//�ص��ú���ReceivePro_Debug_Dispose
        Watchdog_Feed();
        
        /*�Զ���������*/
        //                API_net_state();
        if(Run_User_Program_Falg ==false)
          net_state();
        Watchdog_Feed();
        
        /*����������������*/
        API_net_data_send();
        Watchdog_Feed();
        break;
      case PRO_END:
        //upgrade_init();.
        //���̳�ʼ��
        Down_Pro=PRO_INIT;
        //��ת���Ǽ��
        Net_Data_Send.Link_Type = HEARTBEAT;
        iap_data.writeflag=MODE_ING_SA_SYS;
        Save_Iap_Config();
        //����״̬
        SA_Pro=PRO_INIT;
        //        GetNewInactAngle_Fun_state = 3;//�����������Ƕ�
        //��������
        //        Sys_keydata.Angle = 0;//��б�Ƕ�
        //        Sys_keydata.X_Value = 0;//X����ٶ�
        //        Sys_keydata.Y_Value = 0;//Y����ٶ�
        //        Sys_keydata.Z_Value = 0;//Z����ٶ�
        //        Sys_keydata.Low_Temp_Alarm = 0;//���¸澯
        //        Sys_keydata.Trigger_Alarm = 0;//�����澯
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

/******************************FLASH����********************************/
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

