#include "W25QXX.h"
#include "hardwareSTM8L_SPI.h"
#include "config.h"
/***************************************************************************************
**������:       W25QXX_Init
**��������:     ��ʼ��W25QXX
**�������:     --
**�������:     --
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
u8 W25QXX_Init()
{
//  GPIO_Init(GPIOA,GPIO_Pin_5,GPIO_Mode_Out_PP_Low_Slow);
//  GPIO_ResetBits(GPIOA, GPIO_Pin_5);  
  GPIO_Init(GPIOE, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_SetBits(GPIOE, GPIO_Pin_2);
  GPIO_Init(GPIOE, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_SetBits(GPIOE, GPIO_Pin_1);
  SPI2_Init();
  W25QXX_WAKEUP();
//  W25QXX_EXIT_LWP();
  return 0;
}
/***************************************************************************************
**������:       W25QXX_Test
**��������:     ����FLASH�Ƿ���Ч��
**�������:     --
**�������:     --
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
u8 W25QXX_Test()
{ 
  u8 i;
  for(i=0;i<3;i++)
  {
    if(W25Q80_ID == W25QXX_ReadID())
    {
      debug_send("FLASH �� W25Q80");
      return 1;
    }
  }
  debug_send("FLASH ����");
  return 0;
}
/***************************************************************************************
**������:         --
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
u8 W25QXX_GetStatus(void)
{
  u16 au16DestinationData = 0;
  W25QXX_CS_ENABLE();
  SPI2_SendByte(W25QXX_READ_STATUS_REGISTER1);
  au16DestinationData = SPI2_SendByte(W25QXX_READ);
  W25QXX_CS_DISABLE();
  return au16DestinationData;
}
/***************************************************************************************
**������:         --
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_WaitReady(void)
{
  uint32_t t = 0;
  uint32_t ReturnValue;
  do
  {
    ReturnValue = W25QXX_GetStatus();
    ReturnValue = ReturnValue & 1;
  }
  while ((ReturnValue != 0) && (++t < 80000)); // check the BUSY bit
}
/***************************************************************************************
**������:       W25QXX_ReadID
**��������:     
��ȡFLASH���ͺ�
W25Q80Ϊ0XEF13

**�������:     --
**�������:     Device_ID
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
u16 W25QXX_ReadID(void)  
{  
    u16 Device_ID = 0;  
    
    W25QXX_CS_ENABLE(); // Select the FLASH: Chip Select low  
    SPI2_SendByte(0x90); //��ȡ�豸IDָ��  Send "RDID " instruction 
    SPI2_SendByte(0X00);  
    SPI2_SendByte(0X00);  
    SPI2_SendByte(0X00);  
 
    Device_ID = (SPI2_SendByte(0x00)<<8);  
    Device_ID |= SPI2_SendByte(0x00);      //Read a byte from the FLASH 
    
    W25QXX_CS_DISABLE(); 
    return Device_ID;  
}  

/***************************************************************************************
**������:       W25QXX_HighSpeedReadData
**��������:     
��ָ���ĵ�ַ��ȡ���ݡ�
**�������:     uint8_t *DataBuffer0, uint32_t StartAddress, uint32_t ByteCount
**�������:     
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_HighSpeedReadData(uint32_t StartAddress,uint8_t *DataBuffer0 , uint32_t ByteCount)
{
  uint16_t t = 0;
  uint32_t Counter;
  
  while ((W25QXX_GetStatus() & 0x01) && (++t < 8000));
  W25QXX_CS_ENABLE();
  SPI2_SendByte(0x0B);
  SPI2_SendByte(LOBYTE(HIWORD(StartAddress)));
  SPI2_SendByte(HIBYTE(StartAddress));
  SPI2_SendByte(LOBYTE(StartAddress));
  
  SPI2_SendByte(0XFF);
  for (Counter = 0; Counter < ByteCount; Counter++)
  {
    DataBuffer0[Counter] = (uint8_t)SPI2_SendByte(W25QXX_READ);
  }
  W25QXX_CS_DISABLE(); 
  t = 0;
  while ((W25QXX_GetStatus() & 0x01) && (++t < 8000));
}
/***************************************************************************************
**������:       W25QXX_ChipSectorErase
**��������:     
�߳�һ��4KBYTE������
**�������:     uint32_t StartAddress
**�������:     
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_ChipSectorErase(uint32_t StartAddress)
{
  uint32_t t = 0;
  
  while ((W25QXX_GetStatus() & 0x01) && (++t < 8000));
  W25QXX_CS_ENABLE();
  SPI2_SendByte(0x06);
  W25QXX_CS_DISABLE(); 
  W25QXX_CS_ENABLE();
  SPI2_SendByte(0x20);
  SPI2_SendByte(LOBYTE(HIWORD(StartAddress)));
  SPI2_SendByte(HIBYTE(StartAddress));
  SPI2_SendByte(LOBYTE(StartAddress));
  W25QXX_CS_DISABLE();
  t = 0;
  while ((W25QXX_GetStatus() & 0x01) && (++t < 8000));
}
/***************************************************************************************
**������:       W25QXX_ChipBlockErase
**��������:     
�߳�һ��64KBYTE������
**�������:     uint32_t StartAddress
**�������:     
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_ChipBlockErase(uint32_t StartAddress)
{
   uint32_t t = 0;
   
  while ((W25QXX_GetStatus() & 0x01) && (++t < 8000));
  W25QXX_CS_ENABLE();
  SPI2_SendByte(0x06);
  W25QXX_CS_DISABLE(); 
  W25QXX_CS_ENABLE();;
  SPI2_SendByte(0xD8);
  SPI2_SendByte(LOBYTE(HIWORD(StartAddress)));
  SPI2_SendByte(HIBYTE(StartAddress));
  SPI2_SendByte(LOBYTE(StartAddress));
  W25QXX_CS_DISABLE();
  t = 0;
  while ((W25QXX_GetStatus() & 0x01) && (++t < 8000));
}
/***************************************************************************************
**������:       W25QXX_ChipBlockErase
**��������:     
�߳�����оƬ
**�������:     
**�������:     
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_ChipErase(void)
{
  uint32_t t = 0;
  while ((W25QXX_GetStatus() & 0x01) && (++t < 8000));
  W25QXX_CS_ENABLE();
  SPI2_SendByte(W25X_WriteEnable);
//  while ((W25QXX_GetStatus() & 0x01) && (++t < 8000));
  W25QXX_CS_DISABLE(); 
  W25QXX_CS_ENABLE();
  SPI2_SendByte(W25X_ChipErase);
  W25QXX_CS_DISABLE();
//  DelayT_ms(500);
  t = 0;
  while ((W25QXX_GetStatus() & 0x01) && (++t < 8000));
//  while ((W25QXX_GetStatus() & 0x01));
}
/***************************************************************************************
**������:       W25QXX_PageProgram
**��������:     
��ָ����λ��д�����ݡ�
**�������:     
**�������:     
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_PageProgram(uint32_t StartAddress,unsigned char *DataBuffer , uint32_t ByteCount)
{
  uint16_t t = 0;
  uint32_t Counter;
//  while ((W25QXX_GetStatus() & 0x01) && (++t < 8000));
  W25QXX_CS_ENABLE();
  SPI2_SendByte(0x06); //Write enable
  W25QXX_CS_DISABLE(); 
  W25QXX_CS_ENABLE();
  SPI2_SendByte(0x02);  //Page program
  SPI2_SendByte(LOBYTE(HIWORD(StartAddress)));
  SPI2_SendByte(HIBYTE(StartAddress));
  SPI2_SendByte(LOBYTE(StartAddress));
  for (Counter = 0; Counter < ByteCount; Counter++)
  {		
    // send data to program
    SPI2_SendByte(DataBuffer[Counter]);
  }
  W25QXX_CS_DISABLE();
  t = 0;
  while ((W25QXX_GetStatus() & 0x01) && (++t < 8000));
}
/***************************************************************************************
**������:       W25QXX_Write_nByte
**��������:     
��ָ����λ��д�����ݡ�
**�������:     
**�������:     
**��ע:         YY.c,2017-09-05,By Haixing.Huang
������ж�ջ��������⡣
****************************************************************************************/
int W25QXX_Write_nByte(unsigned int StartAddress, unsigned char *DataBuffer, unsigned int ByteCount)
{
  uint8_t RDataBuffer[256] = {0};
  uint8_t u8ByteCnt1 = 0;
  uint16_t u16Index = StartAddress % 256;
  
  u8ByteCnt1 = 256 - u16Index;
  
  if ((u16Index + ByteCount) > 256)
  {
    W25QXX_PageProgram(StartAddress, DataBuffer, u8ByteCnt1);
    W25QXX_PageProgram(StartAddress + u8ByteCnt1, DataBuffer + u8ByteCnt1, ByteCount - u8ByteCnt1);
  }
  else
  {
    W25QXX_PageProgram(StartAddress, DataBuffer, ByteCount);
  }
  
  W25QXX_HighSpeedReadData(StartAddress,RDataBuffer , ByteCount);
  
  if (memcmp(RDataBuffer, DataBuffer, ByteCount))
  {
    return 1;
  }
  return 0;  
}


/***************************************************************************************
**������:       W25QXX_HighSpeedReadData
**��������:     
��ָ���ĵ�ַ��ȡ���ݡ�
**�������:     uint8_t *DataBuffer0, uint32_t StartAddress, uint32_t ByteCount
**�������:     
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_EraseSector(uint32_t u32AddrStart, uint32_t u32AddrEnd)
{
  uint32_t u32Addr;
  for (u32Addr = u32AddrStart; u32Addr < u32AddrEnd; u32Addr += 4096)
  {
//    WDT_RESET(); ���������Ҫι��
    W25QXX_ChipSectorErase(u32Addr);
    W25QXX_WaitReady();/* �ȴ�������� */
  }
}

/***************************************************************************************
**������:       W25QXX_Write_NoCheck
**��������:     
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
**�������:     uint8_t *DataBuffer0, uint32_t StartAddress, uint32_t ByteCount
**�������:     
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_Write_NoCheck(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
{  
  u16 pageremain;	   
  pageremain=256-WriteAddr%256; 	//��ҳʣ����ֽ���		 	    
  if(NumByteToWrite<=pageremain)
    pageremain=NumByteToWrite;		//������256���ֽ� һҳ��д
  while(1)
  {	   
    W25QXX_PageProgram(WriteAddr,pBuffer,pageremain);
    if(NumByteToWrite==pageremain)
      break;//д�������
    else 			//NumByteToWrite>pageremain  һҳ�ڴ治��д
    {
      pBuffer+=pageremain;
      WriteAddr+=pageremain;	
      
      NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
      if(NumByteToWrite>256)
        pageremain=256; //һ�ο���д��256���ֽ� ���µ�һҳ�ڴ滹�ǲ���д
      else 
        pageremain=NumByteToWrite; 	  //����256���ֽ���       �µ�һҳ�ڴ湻д
    }
  };	    
} 

/***************************************************************************************
**������:       W25QXX_Write_Check
**��������:     
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
**�������:     uint8_t *DataBuffer0, uint32_t StartAddress, uint32_t ByteCount
**�������:     
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_Write_Check(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
{  
  u16 pageremain;	   
  pageremain=256-WriteAddr%256; 	//��ҳʣ����ֽ���		 	    
  if(NumByteToWrite<=pageremain)
    pageremain=NumByteToWrite;		//������256���ֽ� һҳ��д
  while(1)
  {	   
    W25QXX_PageProgram(WriteAddr,pBuffer,pageremain);
    if(NumByteToWrite==pageremain)
      break;//д�������
    else 			//NumByteToWrite>pageremain  һҳ�ڴ治��д
    {
      pBuffer+=pageremain;
      WriteAddr+=pageremain;	
      
      NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
      if(NumByteToWrite>256)
        pageremain=256; //һ�ο���д��256���ֽ� ���µ�һҳ�ڴ滹�ǲ���д
      else 
        pageremain=NumByteToWrite; 	  //����256���ֽ���       �µ�һҳ�ڴ湻д
    }
  };	    
} 

/***************************************************************************************
**������:       void W25QXX_PowerDown(void)   
**��������:     
��W25оƬ�������״̬��
��Ҫ�ر�ע�⣬ʹ�õ���ģʽʱ��CS��Ҫ���ָߵ�ƽ��
��������͵�ƽ�Ϳ����ˡ�
���ʹ��Ӳ����spi������Ҫ�ر�SPIģ�顣
����STM8L�������£�
  GPIO_Init(GPIOE, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOE, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
**�������:     
**�������:     
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_PowerDown(void)   
{
  W25QXX_CS_ENABLE();
  SPI2_SendByte(W25X_PowerDown);
  W25QXX_CS_DISABLE();
  DelayT_ms(30);
}
/***************************************************************************************
**������:       W25QXX_PowerDownIOConfig
**��������:     
��Ҫ���W25QXX_PowerDownʹ�ã����������Ҫ���ݲ�ͬ�ĵ�Ƭ�����á�

W25оƬ�������״̬��
��Ҫ�ر�ע�⣬ʹ�õ���ģʽʱ��CS��Ҫ���ָߵ�ƽ��
��������͵�ƽ�Ϳ����ˡ�
���ʹ��Ӳ����spi������Ҫ�ر�SPIģ�顣
����STM8L�������£�
  GPIO_Init(GPIOE, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOE, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);


**�������:     
**�������:     
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_PowerDownIOConfig()
{
  GPIO_Init(GPIOE, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Slow); 
  GPIO_Init(GPIOE, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
}
/***************************************************************************************
**������:       W25QXX_LWP()
**��������:     


W25оƬ�������״̬��
��Ҫ�ر�ע�⣬ʹ�õ���ģʽʱ��CS��Ҫ���ָߵ�ƽ��
��������͵�ƽ�Ϳ����ˡ�
���ʹ��Ӳ����spi������Ҫ�ر�SPIģ�顣
����STM8L�������£�
  SPI2_Disable();
  GPIO_Init(GPIOE, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Slow);      //20 HOLD_W
  GPIO_Init(GPIOE, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Slow);      //21 WP_W
  GPIO_Init(GPIOG, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);     //49 POW_FLASH
  GPIO_Init(GPIOG, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Slow);      //50 SCK_W      
  GPIO_Init(GPIOG, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);      //51 MOSI_W
  GPIO_Init(GPIOG, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);      //52 MISO_W
�����漰����io���������¼�ˡ�
**�������:     
**�������:     
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_LWP()
{
  W25QXX_Init();
  W25QXX_PowerDown();
  SPI2_Disable();
  W25QXX_PowerDownIOConfig();
}

/***************************************************************************************
**������:       void W25QXX_PowerDown(void)   
**��������:     
��W25оƬ�ӵ���״̬����
**�������:     
**�������:     
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_WAKEUP(void)   
{
  W25QXX_CS_ENABLE();
  SPI2_SendByte(W25X_ReleasePowerDown);
  W25QXX_CS_DISABLE();
}
/***************************************************************************************
**������:       W25QXX_EXIT_LWP()
**��������:     
W25оƬ�˳�����״̬��
**�������:     
**�������:     
**��ע:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_EXIT_LWP()
{
  W25QXX_Init();
//  W25QXX_WAKEUP();
}


