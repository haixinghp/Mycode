
//#include "stm8l15x.h"
#include "config.h"
void adxl362_int2_gpio_init(){
  adxl362_int2_enable();
}

void adxl362_int2_enable(){
  GPIO_Init(GPIOB,GPIO_Pin_2,GPIO_Mode_In_FL_IT);
  GPIO_Init(GPIOB,GPIO_Pin_3,GPIO_Mode_In_FL_IT);
  EXTI_SetPinSensitivity(EXTI_Pin_2,EXTI_Trigger_Falling); 
  EXTI_SetPinSensitivity(EXTI_Pin_3,EXTI_Trigger_Falling); 
}

void adxl362_int2_disable(){
  GPIO_Init(GPIOB,GPIO_Pin_3,GPIO_Mode_In_FL_No_IT);
  EXTI_SetPinSensitivity(EXTI_Pin_6,EXTI_Trigger_Falling); 
}
