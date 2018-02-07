#include "ADC.h"

//-------------------------------




void AdcInit(void)
{
  
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE); //ʹ��ADC1��ʱ��
  
  ADC_DeInit(ADC1);
  /*/==Configures the sampling time for the selected ADC channel group.
  ����ѡ�е�ADCͨ����Ĳ���ʱ��*/
  ADC_Init(ADC1,ADC_ConversionMode_Single,ADC_Resolution_12Bit,ADC_Prescaler_1);        
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels,ADC_SamplingTime_96Cycles);
  
  ADC_DMACmd(ADC1, DISABLE);
  /*Enables or disables the selected ADC peripheral.*/
  /*ʹ�ܻ�ʧ����ѡ��ADC����*/
  ADC_Cmd(ADC1, ENABLE);
  
  /*��adc���������Ϊ��������ģʽ*/
  GPIO_Init( ADC_PORT,ADC_PIN,GPIO_Mode_In_FL_No_IT  );
}
/***************************************************************************************
**������:       AdcClear
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2017-07-02,By Haixing.Huang
****************************************************************************************/
void AdcClear(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE); //ʧ��ADC1��ʱ��
    ADC_DeInit(ADC1);
    ADC_DMACmd(ADC1, DISABLE);
    ADC_Cmd(ADC1, DISABLE);
//    GPIO_Init( ADC_PORT,ADC_PIN,GPIO_Mode_Out_PP_Low_Slow);
    
    
}

uint16_t VREF_Value(uint16_t ADC_Channel)//�����ڲ��ο���ѹVREFֵ
{
  uint16_t ADC_value=0;
  /*ʹ�ܻ�ʧ����ѡ��ADCͨ��*/
  ADC_ChannelCmd(ADC1, ADC_Channel, ENABLE);
  /*Starts ADC conversion, by software trigger.*/
  ADC_SoftwareStartConv(ADC1);
  while( ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0 );
  ADC_value=ADC_GetConversionValue(ADC1);
  ADC_ChannelCmd(ADC1, ADC_Channel, DISABLE);
  ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
  return ADC_value;
}

#define Filter_Num  8                   //AD�˲�����
//AD����ƽ��ֵ�˲�
u16 filter(uint16_t x) 
{ 
  u32  sum = 0; 
  u8 count;
  for ( count=0;count<(Filter_Num);count++)			//??2��?����??�̨�y3��?
  { 
    sum += VREF_Value(x);		 						//12??AD��?��?�䨮��y?Y��?4096
//    	  delay_ms(5);
  } 
  return (u16)(sum/(Filter_Num));
} 


/**********************
AD	       Vx (V)
-------  =   ---------
4096	      3.3(V)	

**********************/
//����??��???��??1?�̡�?D?��y��???��y
//2?��yx��o�̨�??�����̨�AD	  
u32 final_value(uint16_t x)
{
  
  u32 Vin,AD;
  
  AD=filter(x);
  
  Vin=AD*3300/4096;	//3300��ʾС����3λ
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
////    a=((AD*3.3/4096-0.6)/(2.4))*1600;	//1.6mp�汾��ѹ����������
//    a=(((AD*3.3)/4096-0.6)/(2.4))*PRESSURE_MODE;	//1.0mp�汾��ѹ����������
//    if(a < 0) //�����ָ�����
//    {
//      a =0;
//    }
//  }
//  else
//  {
//    a =0;  //������û�����ӡ�
//  }
//  Vin=(u16)(a);
//  return Vin;
//}
