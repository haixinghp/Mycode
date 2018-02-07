#include "config.h"
#include "iap_updata.h"
#define STM32F10X_MD
#ifdef STM32F10X_MD
#define PAGE_SIZE                         (uint32_t)(0x400)
#define FLASH_SIZE                        (uint32_t)(0x40000) /* 128K */
#elif defined STM32F10X_HD
#define PAGE_SIZE                         (uint32_t)(0x800)
#define FLASH_SIZE                        (uint32_t)(0x80000) /* 512K */
#elif defined STM32F10X_CL
#define PAGE_SIZE                         (uint32_t)(0x800)
#define FLASH_SIZE                        (uint32_t)(0x40000) /* 256K */
#else 
#error "Please select first the STM32 device to be used (in stm32f10x.h)"    
#endif /* STM32F10X_MD */

//STM8
#define WHOLE_FLASH_SIZE        (uint32_t)0x10000         //内部FLASH总大小64K
#define EEPRROM_SIZE            (uint32_t)0x8000          //外部EEPROM大小 默认32K


#if CONF_EX_MEMORY_EN == UPLOAD_SPI_FLASH     //外部FLASH
//#define START_ADDR	((uint32_t*)(0x00008000+((IAP_ROOM_SIZE+WHOLE_FLASH_SIZE)>>1)+EEPRROM_SIZE))
#define INER_START_ADDR	(uint32_t)(0x00015000)//(0x00008000+0x0000D000)
#elif CONF_EX_MEMORY_EN == UPLOAD_EEPROM//外部32K EEPROM
//定义的是内部FLASH开始的地址 存储的是下载BIN文件的尾段
//#define START_ADDR	((uint32_t*)(0x00008000+((IAP_ROOM_SIZE+WHOLE_FLASH_SIZE)>>1)+EEPRROM_SIZE))
#define INER_START_ADDR	(uint32_t)(0x00015000)//(0x00008000+0x0000D000)
#endif 


/*程序开始地址0x8000*/
//  #ifdef STM8_BOOT
//	
//          #if CONF_SPI_FLASH_EN
//              #define START_ADDR	((uint32_t*)(0x0))
//	  #else
//	      #define START_ADDR	((uint32_t*)(0x8000))
//	  #endif 
//
//  #else	
//	#define START_ADDR	((uint32_t*)(0x10000))
//  #endif 


/*允许程序空间为218K*/	  
#define MAX_SIZE	1024 * 218	

#define PAGE_ADDR(ADDR)		(ADDR & (~(((uint32_t)PAGE_SIZE) - 1)))



/*******************************************************************************/
#define WRITE_FLASH(Addr,Data)	FLASH_ProgramWord(Addr,Data)
#define CLEAR_FLASH(Addr)		IAP_Page_Clear(Addr)
#define LOCK_FLASH()	     	FLASH_Lock()
#define UNLOCK_FLASH()	     	FLASH_Unlock()


/*******************************************************************************/
typedef struct 
{
  uint32_t	startAddr; 		//起始地址
  uint32_t	pageSize;		//页大小
  uint32_t	endAddr;		//最大的地址
  
}tagUpdateArg;

/*******************************************************************************/
const static tagUpdateArg	   _gUpdateArg = {
  EEPROM_24C256_START_ADDR,
  PAGE_SIZE,					//块尺寸
  EEPROM_24C256_START_ADDR + (MAX_SIZE>>2)	//最大结束地址
};

/*******************************************************************************/
typedef struct
{
  uint32_t writePtr;						 //写指针
  char remainBuf[sizeof(uint32_t)];	 //剩余未写入flash的字符数组
  uint16_t	remainSize;				 //剩余未写入flash的字符数	
}tagUpdateVal;

/*******************************************************************************/
typedef union _WRData_
{
  uint32_t Data32;	    
  char char8[sizeof(uint32_t)];  
}WRData;

union data  
{  
  unsigned long a;  
  unsigned char tab[4];  
}LONG_TO_CHAR4; 




/*******************************************************************************/
static tagUpdateVal _gUpdateVal = {
  EEPROM_24C256_START_ADDR,
  "",
  0
}; 


/*******************************************************************************/
static int IAP_Page_Clear(uint32_t startAddr)
{
#if CONF_SPI_FLASH_EN
#ifndef STM32_BOOT
  Flash_Config(Flash_FLASH_Unlock);//	UNLOCK_FLASH();
  Watchdog_Feed();
  //	for(;FLASH_ErasePage(startAddr)!=FLASH_COMPLETE;)
  //		Watchdog_Feed();
  Flash_Config(Flash_FLASH_Lock);//	LOCK_FLASH();
#endif
#else
  Flash_Config(Flash_FLASH_Unlock);//	UNLOCK_FLASH();
  Watchdog_Feed();
  //	for(;FLASH_ErasePage(startAddr)!=FLASH_COMPLETE;)
  //		Watchdog_Feed();
  Flash_Config(Flash_FLASH_Lock);//	LOCK_FLASH();
#endif	
  return 0;
  
}

/*******************************************************************************/
DWORD IAP_Update_Start(BOOL force,char* buf,size_t size)
{
  //uint32_t *kkk = START_ADDR;
  BOOL reStart = true;
  if(!force)
  {
    if(size >= 8 && buf != NULL)
    {
      int i = 0;
      DEBUG("\n");
      for(i =0; i < 8; i++)
      {
        if(buf[i] != iap_data.WCDMAUpdateArg.md5[i])
          break;
      }
      if(i ==8)
      {
        
        reStart = false;//匹配 ,断点续传
      }
    }
  }
  _gUpdateVal.remainSize = 0;
  if(reStart)	
  {
    PrintString_("从头下载");
    iap_data.program_checksum = 0;//清零固件校验
    _gUpdateVal.writePtr = _gUpdateArg.startAddr;
    iap_data.WCDMAUpdateArg.length = 0;
    CLEAR_FLASH((uint32_t)_gUpdateVal.writePtr);
#if CONF_EX_MEMORY_EN == UPLOAD_SPI_FLASH  //外存储器为flash
//    W25QXX_ChipErase();//整片擦除
    W25QXX_ChipBlockErase(0);
#endif
    memcpy(iap_data.WCDMAUpdateArg.md5,buf,8);
    Save_Iap_Config();	
  }
  else
  {
    _gUpdateVal.writePtr = 	(_gUpdateArg.startAddr) +  iap_data.WCDMAUpdateArg.length;
    //    DEBUG("\nstart to continue last update:%d\n",iap_data.WCDMAUpdateArg.length);
    return 	iap_data.WCDMAUpdateArg.length;	//返回已经上传的长度
  }	
  //要先进行擦除块操作
  return 0;
}

/*******************************************************************************/
int IAP_Update_Write(char* buf,size_t size)
{
  uint32_t startAddr =  (uint32_t)_gUpdateVal.writePtr;
  uint32_t endAddr = startAddr + size;
  uint32_t startPage = PAGE_ADDR(startAddr);
  uint32_t endPage = PAGE_ADDR(endAddr);
  uint32_t chAlign = 0;
  DWORD writenLength	= 0;		//已经写入到flash上的字节数
  
  u8 error_count = 0;//写入错误的次数
  WRData wrData;
  int count = 0;
  
  if(_gUpdateVal.remainSize >=  sizeof(uint32_t))
    return -1;
  
  Watchdog_Feed();
  
  if(startPage < endPage)
  {
    CLEAR_FLASH(endPage);	
  }
  else if(startPage > endPage)
  {
    return -1;	//应该不可能出现吧
  }
  chAlign = startAddr % sizeof(uint32_t);
  
  if(chAlign > 0)
  {
    return -1;	//字节未对齐，有bug 
  }
  
  writenLength = 	(_gUpdateVal.writePtr - _gUpdateArg.startAddr) + _gUpdateVal.remainSize ;
  

  //先写上面留下来的多余的字节
  if(_gUpdateVal.remainSize > 0)
  {
    memcpy(wrData.char8,_gUpdateVal.remainBuf,_gUpdateVal.remainSize);
    count += _gUpdateVal.remainSize;
  }
  
  
#if CONF_EX_MEMORY_EN == UPLOAD_SPI_FLASH  //外存储器为flash
  {
    USART3_sendchar(0x0d);
    USART3_sendchar(0x0a);
    Watchdog_Feed();
    PrintString("长度:");
    
    PrintU16int2HEX_(size);
    
    PrintString("总长度:");
    
    PrintU32int2HEX_(_gUpdateVal.writePtr);
    Watchdog_Feed();
    
    for(int j=0;j<size;j++)
      USART3_sendchar(buf[j]);
    
    count = 0;//排除count>4情况，去掉中途续传功能，升级包要保证4的倍数
    while(size > 0)
    {
      u8 TempTran[4];
      Watchdog_Feed();
      wrData.char8[count] = *buf;
      size--;
      count++;
      buf++;
      if(count == sizeof(uint32_t))
      {
        do{
          Watchdog_Feed();
          /*写外部FLASH*/
          W25QXX_Write_NoCheck((uint32_t)_gUpdateVal.writePtr,wrData.char8,sizeof(uint32_t));
          Watchdog_Feed();
          W25QXX_HighSpeedReadData((uint32_t)_gUpdateVal.writePtr,TempTran,sizeof(uint32_t));
          for(u8 i=0;i<sizeof(uint32_t);i++){ //读出校验
            if(wrData.char8[i] != TempTran[i]){
              error_count++;
              PrintString_("下载错误");
            }
          }
        }while(error_count>100);
        
        iap_data.program_checksum += wrData.Data32;//程序内容校验累加
        _gUpdateVal.writePtr += sizeof(uint32_t);
        count = 0;
      }
    }
  }
#elif CONF_EX_MEMORY_EN ==  UPLOAD_EEPROM//外部eeprom存储
  {
    USART3_sendchar(0x0d);
    USART3_sendchar(0x0a);
    Watchdog_Feed();
    PrintString("长度:");
    
    PrintU16int2HEX_(size);
    
    PrintString("总长度:");
    
    PrintU32int2HEX_(_gUpdateVal.writePtr);
    Watchdog_Feed();
    
    for(int j=0;j<size;j++)
      USART3_sendchar(buf[j]);
    
    count = 0;//排除count>4情况，去掉中途续传功能，升级包要保证4的倍数
    while(size > 0)
    {
      _Bool error_store = 0;
      Watchdog_Feed();
      wrData.char8[count] = *buf;
      size--;
      count++;
      buf++;
      if(count == sizeof(uint32_t))
      {
        do{
          Watchdog_Feed();
          if(_gUpdateVal.writePtr<EEPROM_24C256_END_ADDR)
          {/*写外部EEPROM*/
            WriteEepromNoChek((uint32_t)_gUpdateVal.writePtr,sizeof(uint32_t),wrData.char8);
            Watchdog_Feed();
            for(u8 i=0;i<sizeof(uint32_t);i++){ //读出校验
              if(wrData.char8[i] != read_eep_byte((uint32_t)_gUpdateVal.writePtr+i)){
                error_count ++;
                PrintString_("下载错误");
              }
            }
          }
          else
          {
            Flash_Config(Flash_FLASH_Unlock);
            FLASH_ProgramWord((uint32_t)(_gUpdateVal.writePtr-EEPROM_24C256_END_ADDR+INER_START_ADDR),wrData.Data32); 
            Flash_Config(Flash_FLASH_Lock);
            Watchdog_Feed();
            for(u8 i=0;i<sizeof(uint32_t);i++){//读出校验
              if(wrData.char8[i] != Flash_FLASH_ReadByte((uint32_t)_gUpdateVal.writePtr-EEPROM_24C256_END_ADDR+INER_START_ADDR+i)){
                error_count ++;
                PrintString_("下载错误");
              }
            }
          }
        }while(error_count>100);
        
        iap_data.program_checksum += wrData.Data32;//程序内容校验累加
        _gUpdateVal.writePtr += sizeof(uint32_t);
        count = 0;
      }
    }
  }
#endif	

  if(error_count>100)//如果写入出错,重启重连；一直出错，则使采集升级跳转次数用完
  {
    PrintString_("写入错误重启!");
    sys_restart();
  }
  
  _gUpdateVal.remainSize = count;
  if(_gUpdateVal.remainSize > 0)
  {
    memcpy(_gUpdateVal.remainBuf,wrData.char8,_gUpdateVal.remainSize);
  }
  else
  {
    writenLength = 	(_gUpdateVal.writePtr - _gUpdateArg.startAddr);
  }
  
  if(writenLength != iap_data.WCDMAUpdateArg.length)
  {
    iap_data.WCDMAUpdateArg.length = writenLength;
    Save_Iap_Config();	//储存长度数据
    
  }
  Watchdog_Feed();
  return 0;
}

/*******************************************************************************/
void IAP_ReStart(void)
{
  
  disableInterrupts();
  PrintString_("IAP重启");
  sys_restart();      // 复位
}
extern void AT24C256_TO_Print(void);
/*******************************************************************************/
int IAP_Update_End(void)
{
  
  DWORD fileLen = _gUpdateVal.writePtr- _gUpdateArg.startAddr + _gUpdateVal.remainSize; 
  if(	_gUpdateVal.remainSize > 0 && _gUpdateVal.remainSize < sizeof(uint32_t))
  {
    WRData wrData;
    memcpy(wrData.char8,_gUpdateVal.remainBuf,_gUpdateVal.remainSize);
    
    iap_data.program_checksum += wrData.Data32 & (u32)(0xFFFFFFFF<<((sizeof(uint32_t)-_gUpdateVal.remainSize)*8));//程序内容校验累加 
    
#if CONF_EX_MEMORY_EN == UPLOAD_SPI_FLASH 
    {
      /*写外部FLASH*/
      W25QXX_Write_NoCheck((uint32_t)_gUpdateVal.writePtr,wrData.char8,sizeof(uint32_t));
    }
#elif CONF_EX_MEMORY_EN ==  UPLOAD_EEPROM//外部eeprom存储
    {	
      if(_gUpdateVal.writePtr<EEPROM_24C256_END_ADDR)
      {/*写外部EEPROM*/
        WriteEepromNoChek((uint32_t)_gUpdateVal.writePtr,_gUpdateVal.remainSize,wrData.char8);
      }
      else
      {
        Flash_Config(Flash_FLASH_Unlock);  
        FLASH_ProgramWord((uint32_t)(_gUpdateVal.writePtr-EEPROM_24C256_END_ADDR+INER_START_ADDR),wrData.Data32); 
        Flash_Config(Flash_FLASH_Lock);
      }
    }
#endif
  }
  
  
  memset(&iap_data.WCDMAUpdateArg,NULL,8);//清空断点续传参数
  
 #if CONF_EX_MEMORY_EN == UPLOAD_SPI_FLASH //外部flash
  iap_data.writeflag = 2; //请求更新应用程序
  iap_data.copyflag = 2;//外部flash
  iap_data.src_add = (u32)EEPROM_24C256_START_ADDR;
  iap_data.dest_add = 0x00A000;
#elif CONF_EX_MEMORY_EN ==  UPLOAD_EEPROM//外部eeprom存储
  iap_data.writeflag = 2; //请求更新应用程序
  iap_data.copyflag = 3;//外部eeprom和内部flash搬运
  iap_data.src_add = (u32)EEPROM_24C256_START_ADDR;
  iap_data.dest_add = 0x00A000;
#endif 
  
  iap_data.copy_long = fileLen;//总长度
  Save_Iap_Config();
  Is_Flag_IAP_ReStart = 1;//接收完升级包，重启标志 等发送完重启
//  SetTimer(3000,1,TIMER_TYPE_IAP_UPDATE_END,NULL,IAP_ReStart);//等3秒，将结束回复发出后，重启拷贝程序
  /*跳转到用户程序*/	
  
  return 0;
}

/*******************************************************************************/
typedef  void (*pFunction)(void);

/*程序开始地址*/
#ifdef STM32_BOOT
uint32_t ApplicationAddress = 0x8008000;
uint32_t FlashDestination = 0x8020000;
#else
uint32_t ApplicationAddress = 0x10000;
uint32_t FlashDestination = 0x10000;
#endif 


/*******************************************************************************/
uint32_t FLASH_PagesMask(__IO uint32_t Size)
{
  uint32_t pagenumber = 0x0;
  uint32_t size = Size;
  
  if ((size % PAGE_SIZE) != 0)
  {
    pagenumber = (size / PAGE_SIZE) + 1;
  }
  else
  {
    pagenumber = size / PAGE_SIZE;
  }
  return pagenumber;
}

/*******************************************************************************/
int IAP_copyflash(u32 src_add,u32 dest_add,u32 copy_long)
{	
  int32_t  j;
  //  uint32_t EraseCounter = 0x0;
  uint32_t RamSource;
  //  u8 i;
  //  u8 flash_data[10];
  //  //	FLASH_Status FLASHStatus = FLASH_COMPLETE;
  uint32_t NbrOfPage = 0;
  /*if(iap_data.copyflag !=2)
  {
  if(dest_add<0x08008000||dest_add+copy_long>src_add) 
  {
  return -1;
}
}*/
  Flash_Config(Flash_FLASH_Unlock);//	FLASH_Unlock();
  /* Erase the FLASH pages */
  NbrOfPage = FLASH_PagesMask(copy_long);
  ApplicationAddress = dest_add;
  FlashDestination = ApplicationAddress;
  //	for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
  //   	{
  //    	FLASHStatus = FLASH_ErasePage(FlashDestination + (PAGE_SIZE * EraseCounter));
  //		Watchdog_Feed();
  //   	}
  /*copy*/
  RamSource = src_add;
  for (j = 0;j < copy_long;j ++)
  {
    /* Program the data received into STM32F10x Flash */
    Flash_FLASH_WriteByte(FlashDestination,Flash_FLASH_ReadByte(RamSource));
    
    if (Flash_FLASH_ReadByte(FlashDestination) != Flash_FLASH_ReadByte(RamSource))
    {
      Flash_Config(Flash_FLASH_Lock);//FLASH_Lock();
      return 3;
    }
    FlashDestination += 1;
    RamSource += 1;
    Watchdog_Feed();
  }
  Flash_Config(Flash_FLASH_Lock);//	FLASH_Lock();
  IAP_ReStart();
  
  return 0;
}
/*******************************************************************************/
#if CONF_SPI_FLASH_EN
int IAP_copyflash1(u32 src_add,u32 dest_add,u32 copy_long)
{	
  
  return 0;
}
#endif
/*******************************************************************************/
typedef void (*pFunction)(void);
pFunction Jump_To_Application;
#define APPLICATION_ADDRESS     (u32)APP_StartAddress

//跳转应用程序
int Jump_To_UserProgram(void)
{
  Jump_To_Application = (pFunction)APP_StartAddress;
  Jump_To_Application();
  
  return 0;
}
//int Jump_To_UserProgram(void)
//{
////	uint32_t JumpAddress;
////	pFunction Jump_To_Application;
////
////	/* Test if user code is programmed starting from address "ApplicationAddress" */
////    if (((*(u32*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
////    { 
////		/* Jump to user application */
////		JumpAddress = *(u32*) (ApplicationAddress + 4);
////		Jump_To_Application = (pFunction) JumpAddress;
////		__set_MSP(*(u32*) ApplicationAddress);
////		Jump_To_Application();
////    }
//
//	return 0;
//}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : Select_Program_run
// 功能描述  : 选择进入用户或引导程序
// 输入参数  : None
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void Select_Program_run(void)
{
  
  /*获取程序升级参数*/
  if(API_Get_Iap_Config() > -1)
  {
    ModAddress = iap_data.Address;
#ifndef STM32_BOOT
    /*有程序更新标志,进入升级*/
    if(iap_data.writeflag == 0)
    {
      if(iap_data.copyflag == 0)
      {
        /*复制程序，来源内部flash*/
        API_IAP_copyflash(iap_data.src_add,
                          iap_data.dest_add,
                          iap_data.copy_long );
      }
      else if(iap_data.copyflag == 1)
      {
        
      }
      else if(iap_data.copyflag == 2)
      {
        /*复制程序,来源外部flash*/
        API_IAP_copyflash1(iap_data.src_add,
                           iap_data.dest_add,
                           iap_data.copy_long );
      }
      else
      {
        /*进入boot进行升级*/
      }
    }
    
    
    
    /*直接进入用户程序*/															 
    else if(iap_data.writeflag ==1)
    {
      /*跳转到用户程序前必须将打开的所有外设关闭*/
      TIM4_Cmd(DISABLE);
      
      USART_Cmd(USART1, DISABLE);
      USART_Cmd(USART2, DISABLE);
      USART_Cmd(USART3, DISABLE);
      
      //			USART_DMACmd(USART1, USART_DMAReq_Rx, DISABLE);
      //			USART_DMACmd(USART2, USART_DMAReq_Rx, DISABLE);
      //			USART_DMACmd(USART3, USART_DMAReq_Rx, DISABLE);
      
      DMA_Cmd(DMA1_Channel1, DISABLE);
      DMA_Cmd(DMA1_Channel2, DISABLE);
      DMA_Cmd(DMA1_Channel3, DISABLE);
      
      API_Jump_To_UserProgram();
    }
    
    else
    {
      /*进入boot进行升级*/
    }
#endif
    
  }		
  /*看门狗初始化*/
  API_WatchDog_Init();
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : Check_Run_User_Program_Falg
// 功能描述  : 检查进入用户或引导程序标志
// 输入参数  : None
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void Check_Run_User_Program_Falg(void)
{
  if(Run_User_Program_Falg == true)
  {
    Run_User_Program_Falg = false;
    API_KillTimer(TIMER_TYPE_IAP_SELECT_PROGRAM);	
  }
}
/*******************************************************************************/

