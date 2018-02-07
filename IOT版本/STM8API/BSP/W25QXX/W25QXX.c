#include "W25QXX.h"
#include "hardwareSTM8L_SPI.h"
#include "config.h"
/***************************************************************************************
**函数名:       W25QXX_Init
**功能描述:     初始化W25QXX
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-09-05,By Haixing.Huang
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
**函数名:       W25QXX_Test
**功能描述:     测试FLASH是否有效。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
u8 W25QXX_Test()
{ 
  u8 i;
  for(i=0;i<3;i++)
  {
    if(W25Q80_ID == W25QXX_ReadID())
    {
      debug_send("FLASH 是 W25Q80");
      return 1;
    }
  }
  debug_send("FLASH 错误！");
  return 0;
}
/***************************************************************************************
**函数名:         --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-09-05,By Haixing.Huang
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
**函数名:         --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-09-05,By Haixing.Huang
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
**函数名:       W25QXX_ReadID
**功能描述:     
读取FLASH的型号
W25Q80为0XEF13

**输入参数:     --
**输出参数:     Device_ID
**备注:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
u16 W25QXX_ReadID(void)  
{  
    u16 Device_ID = 0;  
    
    W25QXX_CS_ENABLE(); // Select the FLASH: Chip Select low  
    SPI2_SendByte(0x90); //读取设备ID指令  Send "RDID " instruction 
    SPI2_SendByte(0X00);  
    SPI2_SendByte(0X00);  
    SPI2_SendByte(0X00);  
 
    Device_ID = (SPI2_SendByte(0x00)<<8);  
    Device_ID |= SPI2_SendByte(0x00);      //Read a byte from the FLASH 
    
    W25QXX_CS_DISABLE(); 
    return Device_ID;  
}  

/***************************************************************************************
**函数名:       W25QXX_HighSpeedReadData
**功能描述:     
从指定的地址读取数据。
**输入参数:     uint8_t *DataBuffer0, uint32_t StartAddress, uint32_t ByteCount
**输出参数:     
**备注:         YY.c,2017-09-05,By Haixing.Huang
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
**函数名:       W25QXX_ChipSectorErase
**功能描述:     
檫除一个4KBYTE的扇区
**输入参数:     uint32_t StartAddress
**输出参数:     
**备注:         YY.c,2017-09-05,By Haixing.Huang
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
**函数名:       W25QXX_ChipBlockErase
**功能描述:     
檫除一个64KBYTE的扇区
**输入参数:     uint32_t StartAddress
**输出参数:     
**备注:         YY.c,2017-09-05,By Haixing.Huang
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
**函数名:       W25QXX_ChipBlockErase
**功能描述:     
檫除整个芯片
**输入参数:     
**输出参数:     
**备注:         YY.c,2017-09-05,By Haixing.Huang
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
**函数名:       W25QXX_PageProgram
**功能描述:     
在指定的位置写入数据。
**输入参数:     
**输出参数:     
**备注:         YY.c,2017-09-05,By Haixing.Huang
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
**函数名:       W25QXX_Write_nByte
**功能描述:     
在指定的位置写入数据。
**输入参数:     
**输出参数:     
**备注:         YY.c,2017-09-05,By Haixing.Huang
这里会有堆栈溢出的问题。
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
**函数名:       W25QXX_HighSpeedReadData
**功能描述:     
从指定的地址读取数据。
**输入参数:     uint8_t *DataBuffer0, uint32_t StartAddress, uint32_t ByteCount
**输出参数:     
**备注:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_EraseSector(uint32_t u32AddrStart, uint32_t u32AddrEnd)
{
  uint32_t u32Addr;
  for (u32Addr = u32AddrStart; u32Addr < u32AddrEnd; u32Addr += 4096)
  {
//    WDT_RESET(); 这里可能需要喂狗
    W25QXX_ChipSectorErase(u32Addr);
    W25QXX_WaitReady();/* 等待擦除完成 */
  }
}

/***************************************************************************************
**函数名:       W25QXX_Write_NoCheck
**功能描述:     
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
**输入参数:     uint8_t *DataBuffer0, uint32_t StartAddress, uint32_t ByteCount
**输出参数:     
**备注:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_Write_NoCheck(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
{  
  u16 pageremain;	   
  pageremain=256-WriteAddr%256; 	//单页剩余的字节数		 	    
  if(NumByteToWrite<=pageremain)
    pageremain=NumByteToWrite;		//不大于256个字节 一页够写
  while(1)
  {	   
    W25QXX_PageProgram(WriteAddr,pBuffer,pageremain);
    if(NumByteToWrite==pageremain)
      break;//写入结束了
    else 			//NumByteToWrite>pageremain  一页内存不够写
    {
      pBuffer+=pageremain;
      WriteAddr+=pageremain;	
      
      NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
      if(NumByteToWrite>256)
        pageremain=256; //一次可以写入256个字节 若新的一页内存还是不够写
      else 
        pageremain=NumByteToWrite; 	  //不够256个字节了       新的一页内存够写
    }
  };	    
} 

/***************************************************************************************
**函数名:       W25QXX_Write_Check
**功能描述:     
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
**输入参数:     uint8_t *DataBuffer0, uint32_t StartAddress, uint32_t ByteCount
**输出参数:     
**备注:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_Write_Check(u32 WriteAddr,u8* pBuffer,u16 NumByteToWrite)   
{  
  u16 pageremain;	   
  pageremain=256-WriteAddr%256; 	//单页剩余的字节数		 	    
  if(NumByteToWrite<=pageremain)
    pageremain=NumByteToWrite;		//不大于256个字节 一页够写
  while(1)
  {	   
    W25QXX_PageProgram(WriteAddr,pBuffer,pageremain);
    if(NumByteToWrite==pageremain)
      break;//写入结束了
    else 			//NumByteToWrite>pageremain  一页内存不够写
    {
      pBuffer+=pageremain;
      WriteAddr+=pageremain;	
      
      NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
      if(NumByteToWrite>256)
        pageremain=256; //一次可以写入256个字节 若新的一页内存还是不够写
      else 
        pageremain=NumByteToWrite; 	  //不够256个字节了       新的一页内存够写
    }
  };	    
} 

/***************************************************************************************
**函数名:       void W25QXX_PowerDown(void)   
**功能描述:     
是W25芯片进入掉电状态。
需要特别注意，使用掉电模式时，CS需要保持高电平。
其他输出低电平就可以了。
如果使用硬件的spi，还需要关闭SPI模块。
对于STM8L配置如下：
  GPIO_Init(GPIOE, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOE, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);
**输入参数:     
**输出参数:     
**备注:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_PowerDown(void)   
{
  W25QXX_CS_ENABLE();
  SPI2_SendByte(W25X_PowerDown);
  W25QXX_CS_DISABLE();
  DelayT_ms(30);
}
/***************************************************************************************
**函数名:       W25QXX_PowerDownIOConfig
**功能描述:     
需要配合W25QXX_PowerDown使用，这个函数需要根据不同的单片机配置。

W25芯片进入掉电状态。
需要特别注意，使用掉电模式时，CS需要保持高电平。
其他输出低电平就可以了。
如果使用硬件的spi，还需要关闭SPI模块。
对于STM8L配置如下：
  GPIO_Init(GPIOE, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOE, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);


**输入参数:     
**输出参数:     
**备注:         YY.c,2017-09-05,By Haixing.Huang
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
**函数名:       W25QXX_LWP()
**功能描述:     


W25芯片进入掉电状态。
需要特别注意，使用掉电模式时，CS需要保持高电平。
其他输出低电平就可以了。
如果使用硬件的spi，还需要关闭SPI模块。
对于STM8L配置如下：
  SPI2_Disable();
  GPIO_Init(GPIOE, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Slow);      //20 HOLD_W
  GPIO_Init(GPIOE, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Slow);      //21 WP_W
  GPIO_Init(GPIOG, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);     //49 POW_FLASH
  GPIO_Init(GPIOG, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Slow);      //50 SCK_W      
  GPIO_Init(GPIOG, GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);      //51 MOSI_W
  GPIO_Init(GPIOG, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Slow);      //52 MISO_W
所有涉及到的io都在上面记录了。
**输入参数:     
**输出参数:     
**备注:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_LWP()
{
  W25QXX_Init();
  W25QXX_PowerDown();
  SPI2_Disable();
  W25QXX_PowerDownIOConfig();
}

/***************************************************************************************
**函数名:       void W25QXX_PowerDown(void)   
**功能描述:     
是W25芯片从掉电状态唤醒
**输入参数:     
**输出参数:     
**备注:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_WAKEUP(void)   
{
  W25QXX_CS_ENABLE();
  SPI2_SendByte(W25X_ReleasePowerDown);
  W25QXX_CS_DISABLE();
}
/***************************************************************************************
**函数名:       W25QXX_EXIT_LWP()
**功能描述:     
W25芯片退出掉电状态。
**输入参数:     
**输出参数:     
**备注:         YY.c,2017-09-05,By Haixing.Huang
****************************************************************************************/
void W25QXX_EXIT_LWP()
{
  W25QXX_Init();
//  W25QXX_WAKEUP();
}


