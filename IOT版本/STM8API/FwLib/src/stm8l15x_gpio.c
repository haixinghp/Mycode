/**
  ******************************************************************************
  * @file    stm8l15x_gpio.c
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    13-May-2011
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the GPIO peripheral:
  *           - Initialization and Configuration
  *           - GPIO Read and Write
  *
  *          ===================================================================
  *                                 How to use this driver
  *          ===================================================================       
  *           1. Configure the GPIO pin(s) using GPIO_Init()
  *              Two main configuration are available for each pin:
  *                - Input: Floating 
  *                         Pull-up.
  *                  In Input mode, external interrupt can be enabled or disabled
  *                - Output: Push-Pull
  *                          Open Drain.
  *                  In output mode, the GPIO pin speed is configurable: 
  *                  Slow (2 MHz) or Fast (10MHz).
  *  
  *           2. To get the level of a pin configured in input mode use GPIO_ReadInputDataBit()
  * 
  *           3. To set/reset GPIO pins sharing the same GPIO port use
  *              GPIO_SetBits() / GPIO_ResetBits()
  *
  *           4. To enable external interrupt, the GPIO pin must be configured
  *              in input mode with interrupt enabled. Interrupt sensitivity
  *              (rising, falling...) is configurable using 
  *              EXTI_SetPinSensitivity() in the EXTI peripheral driver "stm8l15x_exti.c"
  *  
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x_gpio.h"

/** @addtogroup STM8L15x_StdPeriph_Driver
  * @{
  */

/** @defgroup CLK 
  * @brief CLK driver modules
  * @{
  */ 
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup GPIO_Private_Functions
  * @{
  */ 


/** @defgroup GPIO_Group1 Initialization and Configuration
 *  @brief   Initialization and Configuration
 *
@verbatim   
 ===============================================================================
                        Initialization and Configuration
 ===============================================================================  

@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the GPIOx peripheral registers to their default reset values.
  * @param  GPIOx: Select the GPIO peripheral number (x = A to I).
  * @retval None
  */
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
  GPIOx->CR2 = GPIO_CR2_RESET_VALUE; /* Reset Control Register 2 */
  GPIOx->ODR = GPIO_ODR_RESET_VALUE; /* Reset Output Data Register */
  GPIOx->DDR = GPIO_DDR_RESET_VALUE; /* Reset Data Direction Register */
  GPIOx->CR1 = GPIO_CR1_RESET_VALUE; /* Reset Control Register 1 */
}

/**
  * @brief  Initializes the GPIOx according to the specified parameters.
  * @param  GPIOx : Select the GPIO peripheral number (x = A to I).
  * @param  GPIO_Pin : This parameter contains the pin number
  *           This parameter can be one of the following values:
  *            @arg GPIO_Pin_0: Pin 0
  *            @arg GPIO_Pin_1: Pin 1
  *            @arg GPIO_Pin_2: Pin 2
  *            @arg GPIO_Pin_3: Pin 3
  *            @arg GPIO_Pin_4: Pin 4
  *            @arg GPIO_Pin_5: Pin 5
  *            @arg GPIO_Pin_6: Pin 6
  *            @arg GPIO_Pin_7: Pin 7              
  * @param  GPIO_Mode : This parameter can be a value of the
  *           This parameter can be one of the following values:
  *            @arg GPIO_Mode_In_FL_No_IT: Input floating, no external interrupt
  *            @arg GPIO_Mode_In_PU_No_IT: Input pull-up, no external interrupt
  *            @arg GPIO_Mode_In_FL_IT: Input pull-up, external interrupt
  *            @arg GPIO_Mode_Out_OD_Low_Fast: Output open-drain, low level, 10MHz
  *            @arg GPIO_Mode_Out_PP_Low_Fast: Output push-pull, low level, 10MHz
  *            @arg GPIO_Mode_Out_OD_Low_Slow: Output open-drain, low level, 2MHz
  *            @arg GPIO_Mode_Out_PP_Low_Slow: Output push-pull, low level, 2MHz
  *            @arg GPIO_Mode_Out_OD_HiZ_Fast: Output open-drain, high-impedance level, 10MHz
  *            @arg GPIO_Mode_Out_PP_High_Fast: Output push-pull, high level, 10MHz
  *            @arg GPIO_Mode_Out_OD_HiZ_Slow: Output open-drain, high-impedance level, 2MHz
  *            @arg GPIO_Mode_Out_PP_High_Slow: Output push-pull, high level, 2MHz
  * @retval None
  */

void GPIO_Init(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin, GPIO_Mode_TypeDef GPIO_Mode)
{
  /*----------------------*/
  /* Check the parameters */
  /*----------------------*/

  assert_param(IS_GPIO_MODE(GPIO_Mode));
  assert_param(IS_GPIO_PIN(GPIO_Pin));

  /* Reset corresponding bit to GPIO_Pin in CR2 register */
  GPIOx->CR2 &= (uint8_t)(~(GPIO_Pin));

  /*-----------------------------*/
  /* Input/Output mode selection */
  /*-----------------------------*/

  if ((((uint8_t)(GPIO_Mode)) & (uint8_t)0x80) != (uint8_t)0x00) /* Output mode */
  {
    if ((((uint8_t)(GPIO_Mode)) & (uint8_t)0x10) != (uint8_t)0x00) /* High level */
    {
      GPIOx->ODR |= GPIO_Pin;
    } else /* Low level */
    {
      GPIOx->ODR &= (uint8_t)(~(GPIO_Pin));
    }
    /* Set Output mode */
    GPIOx->DDR |= GPIO_Pin;
  } else /* Input mode */
  {
    /* Set Input mode */
    GPIOx->DDR &= (uint8_t)(~(GPIO_Pin));
  }

  /*------------------------------------------------------------------------*/
  /* Pull-Up/Float (Input) or Push-Pull/Open-Drain (Output) modes selection */
  /*------------------------------------------------------------------------*/

  if ((((uint8_t)(GPIO_Mode)) & (uint8_t)0x40) != (uint8_t)0x00) /* Pull-Up or Push-Pull */
  {
    GPIOx->CR1 |= GPIO_Pin;
  } else /* Float or Open-Drain */
  {
    GPIOx->CR1 &= (uint8_t)(~(GPIO_Pin));
  }

  /*-----------------------------------------------------*/
  /* Interrupt (Input) or Slope (Output) modes selection */
  /*-----------------------------------------------------*/

  if ((((uint8_t)(GPIO_Mode)) & (uint8_t)0x20) != (uint8_t)0x00) /* Interrupt or Slow slope */
  {
    GPIOx->CR2 |= GPIO_Pin;
  } else /* No external interrupt or No slope control */
  {
    GPIOx->CR2 &= (uint8_t)(~(GPIO_Pin));
  }

}

/**
  * @brief  Configures the external pull-up on GPIOx pins.
  * @param  GPIOx : Select the GPIO peripheral number (x = A to I).
  * @param  GPIO_Pin : Specifies the pin number
  *           This parameter can be one of the following values:
  *            @arg GPIO_Pin_0: Pin 0
  *            @arg GPIO_Pin_1: Pin 1
  *            @arg GPIO_Pin_2: Pin 2
  *            @arg GPIO_Pin_3: Pin 3
  *            @arg GPIO_Pin_4: Pin 4
  *            @arg GPIO_Pin_5: Pin 5
  *            @arg GPIO_Pin_6: Pin 6
  *            @arg GPIO_Pin_7: Pin 7     
  * @param  NewState : The new state of the pull up pin.
  *           Can be ENABLE or DISABLE  
  * @retval None
  */
void GPIO_ExternalPullUpConfig(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE) /* External Pull-Up Set*/
  {
    GPIOx->CR1 |= GPIO_Pin;
  } else /* External Pull-Up Reset*/
  {
    GPIOx->CR1 &= (uint8_t)(~(GPIO_Pin));
  }
}

/**
  * @}
  */

/** @defgroup GPIO_Group2 GPIO Read and Write
 *  @brief   GPIO Read and Write
 *
@verbatim   
 ===============================================================================
                              GPIO Read and Write
 ===============================================================================  

@endverbatim
  * @{
  */

/**
  * @brief  Writes data to the specified GPIO data port.
  * @note   The port must be configured in output mode.
  * @param  GPIOx : Select the GPIO peripheral number (x = A to I).
  * @param  GPIO_PortVal : Specifies the value to be written to the port output
  *         data register.
  * @retval None
  */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint8_t GPIO_PortVal)
{
  GPIOx->ODR = GPIO_PortVal;
}

/**
  * @brief  Sets or clears the selected data port bit.
  * @param  GPIOx : Select the GPIO peripheral number (x = A to I).
  * @param  GPIO_Pin: Specifies the port bit to be written.
  *           This parameter can be one of the following values:
  *            @arg GPIO_Pin_0: Pin 0
  *            @arg GPIO_Pin_1: Pin 1
  *            @arg GPIO_Pin_2: Pin 2
  *            @arg GPIO_Pin_3: Pin 3
  *            @arg GPIO_Pin_4: Pin 4
  *            @arg GPIO_Pin_5: Pin 5
  *            @arg GPIO_Pin_6: Pin 6
  *            @arg GPIO_Pin_7: Pin 7   
  * @param  GPIO_BitVal: specifies the desired status to be written.
  *         This parameter can be SET or RESET
  * @retval None
  */
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin, BitAction GPIO_BitVal)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  assert_param(IS_STATE_VALUE(GPIO_BitVal));

  if (GPIO_BitVal != RESET)
  {
    GPIOx->ODR |= GPIO_Pin;

  }
  else
  {
    GPIOx->ODR &= (uint8_t)(~GPIO_Pin);
  }
}

/**
  * @brief  Writes high level to the specified GPIO pins.
  * @note   The port must be configured in output mode.
  * @param  GPIOx : Select the GPIO peripheral number (x = A to I).
  * @param  GPIO_Pin : Specifies the pins to be turned high.
  *           This parameter can be one of the following values:
  *            @arg GPIO_Pin_0: Pin 0
  *            @arg GPIO_Pin_1: Pin 1
  *            @arg GPIO_Pin_2: Pin 2
  *            @arg GPIO_Pin_3: Pin 3
  *            @arg GPIO_Pin_4: Pin 4
  *            @arg GPIO_Pin_5: Pin 5
  *            @arg GPIO_Pin_6: Pin 6
  *            @arg GPIO_Pin_7: Pin 7   
  * @retval None
  */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin)
{
  GPIOx->ODR |= GPIO_Pin;
}

/**
  * @brief  Writes low level to the specified GPIO pins.
  * @note   The port must be configured in output mode.
  * @param  GPIOx : Select the GPIO peripheral number (x = A to I).
  * @param  GPIO_Pin : Specifies the pins to be turned low
  *           This parameter can be one of the following values:
  *            @arg GPIO_Pin_0: Pin 0
  *            @arg GPIO_Pin_1: Pin 1
  *            @arg GPIO_Pin_2: Pin 2
  *            @arg GPIO_Pin_3: Pin 3
  *            @arg GPIO_Pin_4: Pin 4
  *            @arg GPIO_Pin_5: Pin 5
  *            @arg GPIO_Pin_6: Pin 6
  *            @arg GPIO_Pin_7: Pin 7 
  * @retval None
  */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin)
{
  GPIOx->ODR &= (uint8_t)(~GPIO_Pin);
}

/**
  * @brief  Toggles the specified GPIO pins.
  * @note   The port must be configured in output mode.
  * @param  GPIOx : Select the GPIO peripheral number (x = A to I).
  * @param  GPIO_Pin : Specifies the pins to be toggled.
  * @retval None
  */
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin)
{
  GPIOx->ODR ^= GPIO_Pin;
}

/**
  * @brief  Reads the specified GPIO input data port.
  * @note   The port must be configured in input mode.
  * @param  GPIOx : Select the GPIO peripheral number (x = A to I).
  * @retval The GPIOx input data port value.
  */
uint8_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
  return ((uint8_t)GPIOx->IDR);
}

/**
  * @brief  Reads the specified GPIO output data port.
  * @note   The port must be configured in input mode.
  * @param  GPIOx : Select the GPIO peripheral number (x = A to I).
  * @retval The GPIOx  output data port value.
  */
uint8_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
  return ((uint8_t)GPIOx->ODR);
}

/**
  * @brief  Reads the specified GPIO input data pin.
  * @param  GPIOx : Select the GPIO peripheral number (x = A to I).
  * @param  GPIO_Pin : Specifies the pin number.
  *           This parameter can be one of the following values:
  *            @arg GPIO_Pin_0: Pin 0
  *            @arg GPIO_Pin_1: Pin 1
  *            @arg GPIO_Pin_2: Pin 2
  *            @arg GPIO_Pin_3: Pin 3
  *            @arg GPIO_Pin_4: Pin 4
  *            @arg GPIO_Pin_5: Pin 5
  *            @arg GPIO_Pin_6: Pin 6
  *            @arg GPIO_Pin_7: Pin 7 
  * @retval BitStatus : GPIO input pin status.
  */
BitStatus GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin)
{
  return ((BitStatus)(GPIOx->IDR & (uint8_t)GPIO_Pin));
}

/**
  * @brief  Reads the specified GPIO Output data pin.
  * @param  GPIOx : Select the GPIO peripheral number (x = A to I).
  * @param  GPIO_Pin : Specifies the pin number
  * @retval BitStatus : GPIO output pin status.
  */
BitStatus GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin)
{
  return ((BitStatus)(GPIOx->ODR & (uint8_t)GPIO_Pin));
}


/**
  * @brief  Deinitializes the PWR peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void PWR_DeInit(void)
{
  PWR->CSR1 = PWR_CSR1_PVDIF;
  PWR->CSR2 = PWR_CSR2_RESET_VALUE;
}

/**
  * @brief  Configures the voltage threshold detected by the Power Voltage Detector(PVD).
  * @param  PWR_PVDLevel: specifies the PVD detection level
  *          This parameter can be one of the following values:
  *            @arg PWR_PVDLevel_1V85: PVD detection level set to 1.85V
  *            @arg PWR_PVDLevel_2V05: PVD detection level set to 2.05V
  *            @arg PWR_PVDLevel_2V26: PVD detection level set to 2.26V
  *            @arg PWR_PVDLevel_2V45: PVD detection level set to 2.45V
  *            @arg PWR_PVDLevel_2V65: PVD detection level set to 2.65V
  *            @arg PWR_PVDLevel_2V85: PVD detection level set to 2.85V
  *            @arg PWR_PVDLevel_3V05: PVD detection level set to 3.05V
  *            @arg PWR_PVDLevel_PVDIn: External input analog voltage (Compare internally to VREFINT)
  * @retval None
  */
void PWR_PVDLevelConfig(PWR_PVDLevel_TypeDef PWR_PVDLevel)
{
  /* Check the parameters */
  assert_param(IS_PWR_PVD_LEVEL(PWR_PVDLevel));

  /* Clear the PVD level */
  PWR->CSR1 &= (uint8_t)(~PWR_CSR1_PLS);

  /* Configure the PVD level */
  PWR->CSR1 |= PWR_PVDLevel;

}

/**
  * @brief  Enables or disables the Power Voltage Detector(PVD).
  * @param  NewState: new state of the PVD.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void PWR_PVDCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the PWR PVD */
    PWR->CSR1 |= PWR_CSR1_PVDE;
  }
  else
  {
    /* Disable the PWR PVD */
    PWR->CSR1 &= (uint8_t)(~PWR_CSR1_PVDE);
  }
}

/**
  * @}
  */

/** @defgroup PWR_Group2 Ultra Low Power mode configuration functions
 *  @brief   Ultra Low Power mode configuration functions 
 *
@verbatim   
 ===============================================================================
                    Ultra Low Power mode configuration functions
 ===============================================================================  

 - The internal voltage reference consumption is not negligible, in particular 
   in Halt and Active Halt modes. To reduce power consumption, use the PWR_UltraLowPowerCmd()
   function (ULP bit (Ultra low power) in the PWR_CSR2 register) to disable the 
   internal voltage reference. However, in this case, when exiting from the 
   Halt/Active Halt modes, the functions managed through the internal voltage reference 
   are not reliable during the internal voltage reference startup time (up to 3 ms).
   To reduce the wakeup time, the device can exit from Halt/Active Halt modes without 
   waiting for the internal voltage reference startup time. This is performed 
   by using the PWR_FastWakeUpCmd() function (setting the FWU bit (Fast
   wakeup) in the PWR_CSR2 register) before entering Halt/Active Halt mode.

@endverbatim
  * @{
  */
  
/**
  * @brief  Enables or disables the Fast WakeUp from Ultra Low Power mode.
  * @param  NewState: new state of the Fast WakeUp  functionality.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void PWR_FastWakeUpCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the PWR FWU */
    PWR->CSR2 |= PWR_CSR2_FWU;
  }
  else
  {
    /* Disable the PWR FWU */
    PWR->CSR2 &= (uint8_t)(~PWR_CSR2_FWU);
  }
}

/**
  * @brief  Enables or disables the Ultra Low Power mode.
  * @param  NewState: new state of the Ultra Low Power mode.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void PWR_UltraLowPowerCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the PWR ULP */
    PWR->CSR2 |= PWR_CSR2_ULP;
  }
  else
  {
    /* Disable the PWR ULP */
    PWR->CSR2 &= (uint8_t)(~PWR_CSR2_ULP);
  }
}

/**
  * @}
  */

/** @defgroup PWR_Group3 Interrupts and Flags management functions
 *  @brief   Interrupts and Flags management functions 
 *
@verbatim   
 ===============================================================================
                 Interrupts and Flags management functions
 ===============================================================================  

@endverbatim
  * @{
  */

/**
  * @brief  Configures the Programmable Voltage Detector (PVD) Interrupt .
  * @param  NewState : Indicates the new state of the PVD interrupt.
  * @retval None
  */
void PWR_PVDITConfig(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the PVD interrupt */
    PWR->CSR1 |= PWR_CSR1_PVDIEN;
  }
  else
  {
    /* Disable the PVD interrupt */
    PWR->CSR1 &= (uint8_t)(~PWR_CSR1_PVDIEN);
  }
}

/**
  * @brief  Checks whether the specified PWR flag is set or not.
  * @param  PWR_FLAG: specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg PWR_FLAG_PVDOF: PVD Output. This flag is valid only if PVD is enabled 
  *                                 by the PWR_PVDCmd() function.
  *            @arg PWR_FLAG_VREFINTF: Internal Voltage Reference Ready flag. This 
  *                                    flag indicates the state of the internal voltage reference, VREFINT.
  *            @arg PWR_FLAG_PVDIF: PVD Interrupt Flag, it's set by hardware when a PVD
  *       event occurs  
  * @retval The new state of PWR_FLAG (SET or RESET).
  */
FlagStatus PWR_GetFlagStatus(PWR_FLAG_TypeDef PWR_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_PWR_FLAG(PWR_FLAG));

  if ((PWR_FLAG & PWR_FLAG_VREFINTF) != 0)
  {
    if ((PWR->CSR2 & PWR_CR2_VREFINTF) != (uint8_t)RESET )
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  else
  {
    if ((PWR->CSR1 & PWR_FLAG) != (uint8_t)RESET )
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }

  /* Return the flag status */
  return((FlagStatus)bitstatus);
}

/**
  * @brief  Clears the PWR PVDIF Flag.
  * @param  None
  * @retval None
  */
void PWR_PVDClearFlag(void)
{
  /* Set the PVDIF to clear it */
  PWR->CSR1 |= PWR_CSR1_PVDIF;
}

/**
  * @brief  Checks whether the PVD interrupt has occurred or not.
  * @param  None
  * @retval The new state of the PVD Interrupt (SET or RESET).
  */
ITStatus PWR_PVDGetITStatus(void)
{
  ITStatus bitstatus = RESET;

  uint8_t PVD_itStatus = 0x0, PVD_itEnable = 0x0;

  PVD_itStatus = (uint8_t)(PWR->CSR1 & (uint8_t)PWR_CSR1_PVDIF);
  PVD_itEnable = (uint8_t)(PWR->CSR1 & (uint8_t)PWR_CSR1_PVDIEN);

  if ((PVD_itStatus != (uint8_t)RESET ) && (PVD_itEnable != (uint8_t)RESET))
  {
    bitstatus = (ITStatus)SET;
  }
  else
  {
    bitstatus = (ITStatus)RESET;
  }
  return ((ITStatus)bitstatus);
}

/**
  * @brief  Clears the PWR interrupt pending bit.
  * @param  None
  * @retval None
  */
void PWR_PVDClearITPendingBit(void)
{
  /* Set the PVDIF to clear it */
  PWR->CSR1 |= PWR_CSR1_PVDIF;
}

/**
  * @}
  */

/**
  * @}
  */
  
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
