//#include "stm8l15x.h"
#include "config.h"

uint16 tim4_ticks;

void TIM4_Init(void)
{      
   TIM4_ARRPreloadConfig(ENABLE);         //使能自动重载影子寄存器
   //TIM4_TimeBaseInit(TIM4_Prescaler_64,CLK_GetClockFreq()/128/1000-1);  //TIM4 64分频，中断频率1000Hz  
   
    TIM4_TimeBaseInit(TIM4_Prescaler_64,125);
    //TIM4_TimeBaseInit(TIM4_Prescaler_128,249);//1/（16M时钟/128）*（249+1）=2ms
    
    TIM4_Cmd(ENABLE);                      //启动TIM4定时器                  
   TIM4_ITConfig(TIM4_IT_Update,ENABLE);  //打开TIM4溢出中断       
}

void timer4Init(void){

    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);  //使能TIM4时钟
    TIM4_Init();  //TIM1初始化，1ms中断一次，生成节拍
}


void timer4Deinit(void){
  TIM4_DeInit();
}

/*void TIM1_Init(void)
{      
   TIM1_ARRPreloadConfig(ENABLE);         //使能自动重载影子寄存器
   //TIM1_TimeBaseInit(TIM1_Prescaler_64,CLK_GetClockFreq()/64/1000-1);  //TIM1 64分频，中断频率1000Hz 
   TIM1_Cmd(ENABLE);                      //启动TIM1定时器                  
   TIM1_ITConfig(TIM1_IT_Update,ENABLE);  //打开TIM1溢出中断       
}

void timer1Init(void){

    CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);  //使能TIM1时钟
    TIM1_Init();  //TIM1初始化，1ms中断一次，生成节拍
}*/