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
#include "config.h"
#include "stm8l15x_clk.h"
/**
  * @brief  delay for some time in ms unit
  * @caller auto_test
  * @param  n_ms is how many ms of time to delay
  * @retval None
  */
/******************************************************************************/  
/* Function name:       Delay_ms                                              */  
/* Descriptions:        毫秒级延时函数函数，16MHz时钟                         */  
/* input parameters:    nCount延时时间                                        */  
/* output parameters:   无                                                    */  
/* Returned value:      无                                                    */  
/******************************************************************************/  
void DelayT_ms(unsigned int nCount)  
{  
    uint16_t i=0,j=0;;  
    for(i=0;i<nCount;i++)  
    {  
        Watchdog_Feed();
        for(j=0;j<430;j++)  //2mhz--430
        {;}  
    }  
}  

void delay_ms(u16 n_ms)
{
/* Init TIMER 4 */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

/* Init TIMER 4 prescaler: / (2^3) = /8 */
  TIM4->PSCR = 3;

/* HSI div by 1 --> Auto-Reload value: 16M/8/8 = 250000, 250000/1k = 250*/
  TIM4->ARR = 250;
  
/* Counter value: 2, to compensate the initialization of TIMER*/
  TIM4->CNTR = 2;

/* clear update flag */
  TIM4->SR1 &= ~TIM4_SR1_UIF;

/* Enable Counter */
  TIM4->CR1 |= TIM4_CR1_CEN;

  while(n_ms--)
  {
    while((TIM4->SR1 & TIM4_SR1_UIF) == 0) ;//中断时容易出不来
    TIM4->SR1 &= ~TIM4_SR1_UIF;
  }

/* Disable Counter */
  TIM4->CR1 &= ~TIM4_CR1_CEN;
}

/**
  * @brief  delay for some time in 10us unit(partial accurate)
  * @caller auto_test
  * @param n_10us is how many 10us of time to delay
  * @retval None
  */
void delay_10us(u16 n_10us)
{
/* Init TIMER 4 */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

/* prescaler: / (2^0) = /1 */
  TIM4->PSCR = 0;

/* SYS_CLK_HSI_DIV1 Auto-Reload value: 16M /8/ 1 = 2M, 2M / 100k = 160 */
  TIM4->ARR = 20;

/* Counter value: 10, to compensate the initialization of TIMER */
  TIM4->CNTR = 10;

/* clear update flag */
  TIM4->SR1 &= ~TIM4_SR1_UIF;

/* Enable Counter */
  TIM4->CR1 |= TIM4_CR1_CEN;

  while(n_10us--)
  {
    while((TIM4->SR1 & TIM4_SR1_UIF) == 0) ;//中断时容易出不来
    TIM4->SR1 &= ~TIM4_SR1_UIF;
  }

/* Disable Counter */
  TIM4->CR1 &= ~TIM4_CR1_CEN;
 CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);
}



/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
