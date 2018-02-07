#include "hardwareSTM8L_SPI.h"
/***************************************************************************************
**函数名:         --
**功能描述:     
SPI_CLOCK:PG5,
SPI_MOSI: PG6,
SPI_MISO: PG7,
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void SPI2_Init(void)  
{    
    GPIO_Init(GPIOG, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast);  //SPI_CLOCK
    GPIO_Init(GPIOG, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast);  //SPI_MOSI
    GPIO_Init(GPIOG, GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);  //SPI_MISO 主机模式，配置为输入 该设置很关键  
//    GPIO_Init(GPIOG, GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);  //SPI_MISO 主机模式，配置为输入 该设置很关键 
    CLK_PeripheralClockConfig(CLK_Peripheral_SPI2, ENABLE); //开启SPI时钟
    
//    SPI_Init(SPI2, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_256, SPI_Mode_Master,
//         SPI_CPOL_High, SPI_CPHA_2Edge,
//        SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 0x07);  //初始化SPI
    SPI_Init(SPI2, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2, SPI_Mode_Master,
         SPI_CPOL_High, SPI_CPHA_2Edge,
        SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 0x07);  //初始化SPI
    SPI_Cmd(SPI2, ENABLE);  //使能SPI 

    GPIO_Init(SPI2_CS_GPIO , SPI2_CS_GPIO_PIN, GPIO_Mode_Out_PP_High_Fast);     //  配置CS管脚 
    GPIO_WriteBit(SPI2_CS_GPIO, SPI2_CS_GPIO_PIN, SET);     //拉高不使能外部SPI设备
}  
/***************************************************************************************
**函数名:         --
**功能描述:     
SPI_CLOCK:PG5,
SPI_MOSI: PG6,
SPI_MISO: PG7,
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void SPI2_Disable()
{
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI2, DISABLE); //关闭SPI时钟
  SPI_Cmd(SPI2, DISABLE);  //失能SPI 
}

u8 SPI2_SendByte(u8 byte)  
{  
  while (SPI_GetFlagStatus(SPI2, SPI_FLAG_TXE) == RESET);  /* Loop while DR register in not emplty */   
  SPI_SendData(SPI2, byte);  /* Send byte through the SPI1 peripheral */   
  while (SPI_GetFlagStatus(SPI2, SPI_FLAG_RXNE) == RESET);  /* Wait to receive a byte */
  return SPI_ReceiveData(SPI2);    /* Return the byte read from the SPI bus */  
}  


