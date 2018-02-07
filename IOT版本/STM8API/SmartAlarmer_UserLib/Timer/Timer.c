//#include "stm8l15x.h"
#include "config.h"

uint16 tim4_ticks;

void TIM4_Init(void)
{      
   TIM4_ARRPreloadConfig(ENABLE);         //ʹ���Զ�����Ӱ�ӼĴ���
   //TIM4_TimeBaseInit(TIM4_Prescaler_64,CLK_GetClockFreq()/128/1000-1);  //TIM4 64��Ƶ���ж�Ƶ��1000Hz  
   
    TIM4_TimeBaseInit(TIM4_Prescaler_64,125);
    //TIM4_TimeBaseInit(TIM4_Prescaler_128,249);//1/��16Mʱ��/128��*��249+1��=2ms
    
    TIM4_Cmd(ENABLE);                      //����TIM4��ʱ��                  
   TIM4_ITConfig(TIM4_IT_Update,ENABLE);  //��TIM4����ж�       
}

void timer4Init(void){

    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);  //ʹ��TIM4ʱ��
    TIM4_Init();  //TIM1��ʼ����1ms�ж�һ�Σ����ɽ���
}


void timer4Deinit(void){
  TIM4_DeInit();
}

/*void TIM1_Init(void)
{      
   TIM1_ARRPreloadConfig(ENABLE);         //ʹ���Զ�����Ӱ�ӼĴ���
   //TIM1_TimeBaseInit(TIM1_Prescaler_64,CLK_GetClockFreq()/64/1000-1);  //TIM1 64��Ƶ���ж�Ƶ��1000Hz 
   TIM1_Cmd(ENABLE);                      //����TIM1��ʱ��                  
   TIM1_ITConfig(TIM1_IT_Update,ENABLE);  //��TIM1����ж�       
}

void timer1Init(void){

    CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);  //ʹ��TIM1ʱ��
    TIM1_Init();  //TIM1��ʼ����1ms�ж�һ�Σ����ɽ���
}*/