/***************************************************************************************
**�ļ���:     
**����:       Haixing.Huang
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   �汾     �޸���    ʱ��                     
**						
**��ע:       2016/3/23
****************************************************************************************/
#ifndef   __ADC_H__
#define   __ADC_H__
//#include "stm8l15x_adc.h" 
////#include "ADC.h"
//#include "stm8l15x.h"
#include "stm8l15x_adc.h" 
#include "stm8l15x.h"

#define ADC_PORT GPIOF
#define ADC_PIN GPIO_Pin_0
#define ADC_PRESSURE (ADC_Channel_24)

#define ADC_PORT GPIOF
#define ADC_PIN GPIO_Pin_0

//extern uint16_t VREF_Value(ADC_Channel_TypeDef ADC_Channel);//�����ڲ��ο���ѹVREFֵ

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef ADC_GLOBALS
#define ADC_EXT
#else
#define ADC_EXT extern
#endif
  ADC_EXT void AdcInit(void);
  ADC_EXT uint16_t VREF_Value(uint16_t ADC_Channel);//�����ڲ��ο���ѹVREFֵ
  ADC_EXT u16 filter(uint16_t x);
  ADC_EXT u32 final_value(uint16_t x);
//  ADC_EXT u16 ADC_GetValue(uint16_t x);
  ADC_EXT void AdcClear(void);
//ADC_EXT void YY_Init(void);
//ADC_EXT void YY_Service(void);
//ADC_EXT void YY_UART_Service(void);
//ADC_EXT void YY_Timer_Service(void);
#ifdef  __cplusplus
}
#endif
#endif
/************************************Code End******************************************/