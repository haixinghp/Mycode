/***************************************************************************************
**文件名:     
**作者:       Haixing.Huang
**版本:       V1.00
**说明:      
  此文件作为FLASH存储的引用层，直接对接芯片提供的接口。
采用不断写入的方式，当整个FLASH写满之后擦除之前的数据。    
文件存储结构：
Block0
  setor0~1  4K*2
  xx00FFh
  xx0FFFh
  4k*2作为文件用于存储数据队列的队列头地址，队列尾巴地址，队列长度。
  setor2~setor15 60K用于数据存储
Block1~Block14 64K*14用于数据存储（可根据实际的芯片容量修改）
Block15 64K 用于代码升级的临时存储。
**修改记录:   版本     修改者    时间                     
**						
**备注:       2016/3/23
****************************************************************************************/
#ifndef   __FLASH_QUEUE_H__
#define   __FLASH_QUEUE_H__
#include "stm8l15x.h"
//#include "FLASH_Queue.h"
#define FLASH_QUEUE_EEPROM_LEN (16)

#define FLASH_QUEUE_A_DATA_LEN (16) //一条数据的长度  
//#define FLASH_QUEUE_LEN ((((u32)0XFFFFF)-0X1FFF)/8);   //队列的长度
//#define FLASH_QUEUE_LEN ((((u32)0X3FFF)-0X1FFF)/8);   //队列的长度
#define FLASH_QUEUE_LEN ((((u32)0X3FFF)-FLASH_QUEUE_DATA_STARTADDR-1)/FLASH_QUEUE_A_DATA_LEN);   //队列的长度


#define FLASH_QUEUE_STARTADDR ((uint32_t)0x011000)  //最前面的64k空间用于代码升级
#define FLASH_QUEUE_ENDADDR ((uint32_t)0xFFFFF)
#define FLASH_QUEUE_SETOR_SIZE ((uint32_t)0x1000)
#define FLASH_QUEUE_BLOCK_SIZE ((uint32_t)0xFFFF)
//#define FLASH_QUEUE_DATA_STARTADDR (FLASH_QUEUE_STARTADDR+FLASH_QUEUE_SETOR_SIZE+FLASH_QUEUE_SETOR_SIZE) //数据存储的开始地址。
#define FLASH_QUEUE_DATA_STARTADDR (FLASH_QUEUE_STARTADDR+0x2000) //数据存储的开始地址。

//#define FLASH_QUEUE_CLEAR_LEN ((0X1000)/16-2) //在达到这个数据大小之后，清除队列，从头写起
#define FLASH_QUEUE_CLEAR_LEN ((uint32_t)254) //在达到这个数据大小之后，清除队列，从头写起  

#define FLASHBUFF_LEN ((uint32_t)32)
#define FLASH_STORAGE_HEADA ((u8)0xAA)
//#define FLASH_STORAGE_HEADA ((u8)0xBB)
#define FLASH_STORAGE_HEADB ((u8)0x55)
#define FLASH_STORAGE_TAILA ((u8)0xBB)
#define FLASH_STORAGE_TAILB ((u8)0x66)

#define FLASH_QUEUE_PRINT(num) PrintU32int2String(num)

typedef struct 
{
  u32 queuesize;   //数组的大小
  u32 head, tail;  //队列的头和尾下标
//  u8 queue_addr;   //保存在芯片内部EEPROM的位置。
}_struFLASH_Queue;

typedef struct 
{
  bool DVALID_Flag;  //数据有效性标志。
  u32 queue_addr;
}_struFLASH_QueueControl;

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef FLASH_QUEUE_GLOBALS
#define FLASH_QUEUE_EXT
#else
#define FLASH_QUEUE_EXT extern
#endif
  FLASH_QUEUE_EXT _struFLASH_Queue struFLASH_Queue;
  FLASH_QUEUE_EXT _struFLASH_Queue struFLASH_QueueBuckup;;
  FLASH_QUEUE_EXT _struFLASH_QueueControl struFLASH_QueueControl;
  
  FLASH_QUEUE_EXT void ClearFLASH_Queue(void);
  FLASH_QUEUE_EXT void InitFLASH_Queue(void);
  FLASH_QUEUE_EXT void EnFLASH_Queue(u32 TimesTamp,u16 PressureData,u16 Alarmlist);
  FLASH_QUEUE_EXT int DeFLASH_Queue(u32 *TimesTamp,u16 *PressureData,u16 *Alarmlist);
  FLASH_QUEUE_EXT int IsFLASH_QueueEmpty(void);
  FLASH_QUEUE_EXT s8 FLASH_Queue_Buckup();
  FLASH_QUEUE_EXT s8 FLASH_Queue_Recover();
  FLASH_QUEUE_EXT void ArrayToArray(u8 *pa,u8 *pb,u16 len);
//FLASH_QUEUE_EXT void YY_Init(void);
//FLASH_QUEUE_EXT void YY_Service(void);
//FLASH_QUEUE_EXT void YY_UART_Service(void);
//FLASH_QUEUE_EXT void YY_Timer_Service(void);
#ifdef  __cplusplus
}
#endif
#endif
/************************************Code End******************************************/