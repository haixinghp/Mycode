/**
  ******************************************************************************
  * @file
  * @author  ll
  * @version V0.0.1
  * @date    10/2016
  * @brief
             QQ:308621800
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "stm8l15x_it.h"
//#include "discover_board.h"
//#include "lowpower_spi.h"

u8 DATA_Buf;
extern bool KeyPressed;
extern uint8_t state_machine;
extern bool Auto_test;
extern uint8_t t_bar[2];

//extern unsigned int sysFunTimecount;
//extern ACCEL_DATA adxl362TestData;


u16 tim4BaseCount = 0;

/** @addtogroup IT_Functions
  * @{
  */
INTERRUPT_HANDLER(NonHandledInterrupt, 0)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}
/**
  * @brief TRAP interrupt routine
  * @par Parameters:
  * None
  * @retval
  * None
*/
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}
/**
  * @brief FLASH Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}
/**
  * @brief DMA1 channel0 and channel1 Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}
/**
  * @brief DMA1 channel2 and channel3 Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
    sim();
    if(DMA_GetFlagStatus(DMA1_FLAG_TC3)!= RESET)
    {
        uart2_disable();
         DMA_Cmd(DMA1_Channel3, DISABLE);
        Uart2_FIFO_Full_Flag = 1;
        DMA_ClearITPendingBit(DMA1_IT_TC3);
    }
    rim();

}
/**
  * @brief RTC Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */

INTERRUPT_HANDLER(RTC_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */

  //RTC_WakeUpCmd(DISABLE);



  Watchdog_Feed();
    if(Analyse_data_para.sencond_query_flag && (!Analyse_data_para.sencond_query_start_flag)){
      //if(!Analyse_data_para.send_busy_flag){
        Analyse_data_para.sencond_query_start_flag = 1;//����ȷ�Ͽ�ʼ
      //}
    }else{
      PrintString_("$"); //�������
      SendHeartPacket(Sys_keydata.heartdata_sent_interval_minute_count);
    }
    RTC_ClearITPendingBit(RTC_IT_WUT);
    Watchdog_Feed();

     //PrintString("$"); //�������



  //RTC_ITConfig(RTC_IT_WUT, DISABLE);

  //RTC_WakeUpCmd(ENABLE);
}
/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}

/**
  * @brief External IT PORTB Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(EXTIB_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}

/**
  * @brief External IT PORTD Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(EXTID_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */

INTERRUPT_HANDLER(EXTI0_IRQHandler, 8)//����3�Ľ��ս�  ���ó����ⲿ�жϿ�
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  //while (1);
     //PrintString_("get rx exitint");
     /*if(TashSchedule_State==0){//����������ģʽ�£�����д���3�Ľ������ݣ�\
                                ��ô��Ҫ���뵽��������ȥ���д������ݵ�Э�����
      TashSchedule_State = 1;
     }*/
    //PrintString_("rx ex");
    /*if(TashSchedule_State==0){//����������ģʽ�£�����д���3�Ľ������ݣ�\
                                ��ô��Ҫ���뵽��������ȥ���д������ݵ�Э�����
      TashSchedule_State = 1;
     }*/
     EXTI_ClearITPendingBit(EXTI_IT_Pin0);
}
/**
  * @brief External IT PIN1 Interrupt routine.
  *   On User button pressed:
  *   Check if button presse a long time (4-5 sec.) if yes --> Set Autotest
  *   Else update status_machine to pass to next measuremetn.
  *   Update the LCD bar graph
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler, 9)
{
  debug_send(">��ʼˮλ���!");   //by yw 2017-6-30
  LiquidAlarm.Read_flag = 1;//��Ϊˮλ����־
  LiquidAlarm.Complete_flag = 0;//����ˮλ�����ɱ�־
  EXTI_ClearITPendingBit(EXTI_IT_Pin1);
}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */

INTERRUPT_HANDLER(EXTI2_IRQHandler, 10) //��̬ӳ��
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  //while (1);
  if (EXTI_GetITStatus(EXTI_IT_Pin2)) {
    //
    debug_send("INACT!");
    Analyse_data_para.analyse_state = 0;//����״̬��λ
    Analyse_data_para.inact_flag = 1;//��̬��־��һ
  }
  EXTI_ClearITPendingBit(EXTI_IT_Pin2);
}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(EXTI3_IRQHandler, 11) //����ӳ��
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  //disableInterrupts();
  //DelayT_ms(3000);
  debug_send("ACT!!");
  if (Adxl362Para.Read_flag == 0) {
    Adxl362Para.Read_flag = 1;
    Analyse_data_para.inact_flag = 0;//��̬��־����
    GetNewInactAngle_Fun_state = 1;//ֻҪ����  �ؽ���1
    debug_send("waked");
  }
  //if(EXTI_GetITStatus(EXTI_IT_PortB)){
  if (EXTI_GetITStatus(EXTI_IT_Pin3)) {
    /*if(Adxl362Para.Read_flag==0){
      Adxl362Para.Read_flag = 1;
      debug_send("step1:");
    }*/
    //  debug_send("adxl362 into ACT!!!!!!");
  }
  //}


  /* To clear External IT Pin3 */

  /*
  RTC_WakeUpCmd(DISABLE);
  //IWDG_ReloadCounter();
  RTC_ClearITPendingBit(RTC_IT_WUT);
  RTC_ITConfig(RTC_IT_WUT, DISABLE);
  */

  EXTI_ClearITPendingBit(EXTI_IT_Pin3);
  

  /*
  RTC_WakeUpCmd(DISABLE);
  //IWDG_ReloadCounter();
  RTC_ClearITPendingBit(RTC_IT_WUT);
  RTC_WakeUpCmd(ENABLE);
  */

  //enableInterrupts();
  //while (1);

}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}

/**
  * @brief External IT PIN5 Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler, 13)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler, 14)
{

  while (1);
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler, 15)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */

  while (1);

  
}
/**
  * @brief LCD start of new frame Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(LCD_IRQHandler, 16)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}
/**
  * @brief CLK switch/CSS/TIM1 break Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler, 17)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}

/**
  * @brief ADC1/Comparator Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler, 18)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_IRQHandler, 19)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}

/**
  * @brief Timer2 Capture/Compare Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(TIM2_CAP_IRQHandler, 20)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
      sim(); 
   
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		 USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
    rim();
}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(USART3_TX_IRQHandler, 21)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}
/**
  * @brief Timer3 Capture/Compare Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(USART3_RX_IRQHandler, 22) //����3�����ж�
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  //while (1);
  if(TashSchedule_State==0){//����������ģʽ�£�����д���3�Ľ������ݣ�\
                                ��ô��Ҫ���뵽��������ȥ���д������ݵ�Э�����
      TashSchedule_State = 1;
     }
  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //�����ж�
  {
    DATA_Buf = USART3->DR;

    if (usart3Info.usartLen >= sizeof(usart3Info.usartBuf))  usart3Info.usartLen = 0; //��ֹ���ڱ�ˢ��
    usart3Info.usartBuf[usart3Info.usartLen++] = DATA_Buf;

    if (usart3Info.stopRcv == 1)
    {
      if (usart3Info.usartExtLen >= sizeof(usart3Info.usartExtBuf))  usart3Info.usartExtLen = 0; //��ֹ���ڱ�ˢ��
      usart3Info.usartExtBuf[usart3Info.usartExtLen++] = DATA_Buf;
    }

    if (DATA_Buf == '{')
    {
      usart3Info.startRcv = 1;
      usart3Info.stopRcv = 0;
    }
    if (usart3Info.startRcv == 1)
    {
      if (DATA_Buf == '}')
      {
        usart3Info.startRcv = 0;
        usart3Info.stopRcv = 1;
      }
      else
      {
        if (usart3Info.usartCmdLen >= sizeof(usart3Info.usartCmdBuf))  usart3Info.usartCmdLen = 0; //��ֹ���ڱ�ˢ��
        usart3Info.usartCmdBuf[usart3Info.usartCmdLen++] = DATA_Buf;
      }
    }
    USART_ClearFlag(USART3, USART_FLAG_RXNE);
    


    //USART_ClearFlag(USART3, USART_FLAG_RXNE);
  }
}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler, 23)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}
/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(TIM1_CAP_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}

/**
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  //tim4BaseCount ++;
  disableInterrupts();
  
  GetNewInactAngle_timecount ++;
  Tim4_Int_Cnt++;
  sysFunTimecount++;
  getXyzOffsetAngleTimecount++;
  SetZeroPosTimecount++;
  Adxl362Para.Read_timecount++;
  task_schedule_timecount++;
  if(TashSchedule_State||sysFunState){
    if(sys_overtime_count++>=SYS_OVERTIME_COUNT){//120�� ��û�з��ص�����״̬  ��ô��Ҫ�����豸
    sys_overtime_count=0;
    PrintString_("tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt");
    //��ʱ�˳�
    sys_restart();
    }
  }else{
    sys_overtime_count = 0;
  }
  
  /*
  if(++tim4BaseCount>=3000){
    ToggleLed;
    tim4BaseCount = 0;
  }*/
  waitforconfig_timecount ++;
  TIM4_ClearFlag(TIM4_FLAG_Trigger);
  TIM4->SR1 = 0;//
  //while (1);
  enableInterrupts();
}
/**
  * @brief SPI1 Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler, 26)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}

/**
  * @brief USART1 TX Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(USART1_TX_IRQHandler, 27)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);

}

/**
  * @brief USART1 RX Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(USART1_RX_IRQHandler, 28)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  //while (1);
  //USART_IT_RXNE
  //if(USART_GetITStatus(USART1,  USART_IT_RXNE))
  /*if(USART_GetITStatus(USART1, USART_IT_RXNE))
  {
  //DATA_Buf=USART_ReceiveData8(USART1);

  //alarmer_uart3_send_byte(DATA_Buf);
  FIFO_DataIn(USART_ReceiveData8(USART1));//���յ����ݷ��뻺����

  //USART1->DR = DATA_Buf; //
  //while((USART1->SR & 0x80) == 0x00);//
  //USART_ClearITPendingBit(USART1,USART_IT_RXNE);
  }*/
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�����ж�
  {
    DATA_Buf = USART1->DR;

    if (usart1Info.usartLen >= sizeof(usart1Info.usartBuf))  usart1Info.usartLen = 0; //��ֹ���ڱ�ˢ��
    usart1Info.usartBuf[usart1Info.usartLen++] = DATA_Buf;

    if (usart1Info.stopRcv == 1)
    {
      if (usart1Info.usartExtLen >= sizeof(usart1Info.usartExtBuf))  usart1Info.usartExtLen = 0; //��ֹ���ڱ�ˢ��
      usart1Info.usartExtBuf[usart1Info.usartExtLen++] = DATA_Buf;
    }

    if (DATA_Buf == '{')
    {
      usart1Info.startRcv = 1;
      usart1Info.stopRcv = 0;
    }
    if (usart1Info.startRcv == 1)
    {
      if (DATA_Buf == '}')
      {
        usart1Info.startRcv = 0;
        usart1Info.stopRcv = 1;
      }
      else
      {
        if (usart1Info.usartCmdLen >= sizeof(usart1Info.usartCmdBuf))  usart1Info.usartCmdLen = 0; //��ֹ���ڱ�ˢ��
        usart1Info.usartCmdBuf[usart1Info.usartCmdLen++] = DATA_Buf;
      }
    }

    USART_ClearFlag(USART1, USART_FLAG_RXNE);
  }
}

/**
  * @brief I2C1 Interrupt routine.
  * @par Parameters:
  * None
  * @retval
  * None
  */
INTERRUPT_HANDLER(I2C1_IRQHandler, 29)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  while (1);
}

/**
  * @}
  */
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

