#include "ADC.h"

//-------------------------------




void AdcInit(void)
{
  
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE); //使能ADC1的时钟
  
  ADC_DeInit(ADC1);
  /*/==Configures the sampling time for the selected ADC channel group.
  配置选中的ADC通道组的采样时间*/
  ADC_Init(ADC1,ADC_ConversionMode_Single,ADC_Resolution_12Bit,ADC_Prescaler_1);        
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels,ADC_SamplingTime_96Cycles);
  
  ADC_DMACmd(ADC1, DISABLE);
  /*Enables or disables the selected ADC peripheral.*/
  /*使能或失能所选的ADC外设*/
  ADC_Cmd(ADC1, ENABLE);
  
  /*将adc输入脚配置为浮空输入模式*/
  GPIO_Init( ADC_PORT,ADC_PIN,GPIO_Mode_In_FL_No_IT  );
}
/***************************************************************************************
**函数名:       AdcClear
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-07-02,By Haixing.Huang
****************************************************************************************/
void AdcClear(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE); //失能ADC1的时钟
    ADC_DeInit(ADC1);
    ADC_DMACmd(ADC1, DISABLE);
    ADC_Cmd(ADC1, DISABLE);
//    GPIO_Init( ADC_PORT,ADC_PIN,GPIO_Mode_Out_PP_Low_Slow);
    
    
}

uint16_t VREF_Value(uint16_t ADC_Channel)//测量内部参考电压VREF值
{
  uint16_t ADC_value=0;
  /*使能或失能所选的ADC通道*/
  ADC_ChannelCmd(ADC1, ADC_Channel, ENABLE);
  /*Starts ADC conversion, by software trigger.*/
  ADC_SoftwareStartConv(ADC1);
  while( ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0 );
  ADC_value=ADC_GetConversionValue(ADC1);
  ADC_ChannelCmd(ADC1, ADC_Channel, DISABLE);
  ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
  return ADC_value;
}

#define Filter_Num  8                   //AD滤波次数
//AD采样平均值滤波
u16 filter(uint16_t x) 
{ 
  u32  sum = 0; 
  u8 count;
  for ( count=0;count<(Filter_Num);count++)			//??2¨?ù±??μêy3′?
  { 
    sum += VREF_Value(x);		 						//12??AD￡?×?′óêy?Yê?4096
//    	  delay_ms(5);
  } 
  return (u16)(sum/(Filter_Num));
} 


/**********************
AD	       Vx (V)
-------  =   ---------
4096	      3.3(V)	

**********************/
//·μ??×???μ??1?μ￡?D?êyμ???êy
//2?êyx￡oμú??í¨μàAD	  
u32 final_value(uint16_t x)
{
  
  u32 Vin,AD;
  
  AD=filter(x);
  
  Vin=AD*3300/4096;	//3300表示小数点3位
  return Vin;
}

//u16 ADC_GetValue(uint16_t x)
//{
//  u16 AD;
//  u16 Vin;
//  double a;
//  AD=filter(x);
//  if(0!= AD)
//  {
////    a=((AD*3.3/4096-0.6)/(2.4))*1600;	//1.6mp版本的压力传感器。
//    a=(((AD*3.3)/4096-0.6)/(2.4))*PRESSURE_MODE;	//1.0mp版本的压力传感器。
//    if(a < 0) //不出现负数。
//    {
//      a =0;
//    }
//  }
//  else
//  {
//    a =0;  //传感器没有连接。
//  }
//  Vin=(u16)(a);
//  return Vin;
//}
