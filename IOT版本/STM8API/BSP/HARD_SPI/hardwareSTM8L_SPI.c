#include "hardwareSTM8L_SPI.h"
/***************************************************************************************
**������:         --
**��������:     
SPI_CLOCK:PG5,
SPI_MOSI: PG6,
SPI_MISO: PG7,
**�������:     --
**�������:     --
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void SPI2_Init(void)  
{    
    GPIO_Init(GPIOG, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast);  //SPI_CLOCK
    GPIO_Init(GPIOG, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast);  //SPI_MOSI
    GPIO_Init(GPIOG, GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);  //SPI_MISO ����ģʽ������Ϊ���� �����úܹؼ�  
//    GPIO_Init(GPIOG, GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);  //SPI_MISO ����ģʽ������Ϊ���� �����úܹؼ� 
    CLK_PeripheralClockConfig(CLK_Peripheral_SPI2, ENABLE); //����SPIʱ��
    
//    SPI_Init(SPI2, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_256, SPI_Mode_Master,
//         SPI_CPOL_High, SPI_CPHA_2Edge,
//        SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 0x07);  //��ʼ��SPI
    SPI_Init(SPI2, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2, SPI_Mode_Master,
         SPI_CPOL_High, SPI_CPHA_2Edge,
        SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 0x07);  //��ʼ��SPI
    SPI_Cmd(SPI2, ENABLE);  //ʹ��SPI 

    GPIO_Init(SPI2_CS_GPIO , SPI2_CS_GPIO_PIN, GPIO_Mode_Out_PP_High_Fast);     //  ����CS�ܽ� 
    GPIO_WriteBit(SPI2_CS_GPIO, SPI2_CS_GPIO_PIN, SET);     //���߲�ʹ���ⲿSPI�豸
}  
/***************************************************************************************
**������:         --
**��������:     
SPI_CLOCK:PG5,
SPI_MOSI: PG6,
SPI_MISO: PG7,
**�������:     --
**�������:     --
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void SPI2_Disable()
{
  CLK_PeripheralClockConfig(CLK_Peripheral_SPI2, DISABLE); //�ر�SPIʱ��
  SPI_Cmd(SPI2, DISABLE);  //ʧ��SPI 
}

u8 SPI2_SendByte(u8 byte)  
{  
  while (SPI_GetFlagStatus(SPI2, SPI_FLAG_TXE) == RESET);  /* Loop while DR register in not emplty */   
  SPI_SendData(SPI2, byte);  /* Send byte through the SPI1 peripheral */   
  while (SPI_GetFlagStatus(SPI2, SPI_FLAG_RXNE) == RESET);  /* Wait to receive a byte */
  return SPI_ReceiveData(SPI2);    /* Return the byte read from the SPI bus */  
}  


