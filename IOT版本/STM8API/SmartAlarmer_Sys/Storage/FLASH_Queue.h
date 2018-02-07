/***************************************************************************************
**�ļ���:     
**����:       Haixing.Huang
**�汾:       V1.00
**˵��:      
  ���ļ���ΪFLASH�洢�����ò㣬ֱ�ӶԽ�оƬ�ṩ�Ľӿڡ�
���ò���д��ķ�ʽ��������FLASHд��֮�����֮ǰ�����ݡ�    
�ļ��洢�ṹ��
Block0
  setor0~1  4K*2
  xx00FFh
  xx0FFFh
  4k*2��Ϊ�ļ����ڴ洢���ݶ��еĶ���ͷ��ַ������β�͵�ַ�����г��ȡ�
  setor2~setor15 60K�������ݴ洢
Block1~Block14 64K*14�������ݴ洢���ɸ���ʵ�ʵ�оƬ�����޸ģ�
Block15 64K ���ڴ�����������ʱ�洢��
**�޸ļ�¼:   �汾     �޸���    ʱ��                     
**						
**��ע:       2016/3/23
****************************************************************************************/
#ifndef   __FLASH_QUEUE_H__
#define   __FLASH_QUEUE_H__
#include "stm8l15x.h"
//#include "FLASH_Queue.h"
#define FLASH_QUEUE_EEPROM_LEN (16)

#define FLASH_QUEUE_A_DATA_LEN (16) //һ�����ݵĳ���  
//#define FLASH_QUEUE_LEN ((((u32)0XFFFFF)-0X1FFF)/8);   //���еĳ���
//#define FLASH_QUEUE_LEN ((((u32)0X3FFF)-0X1FFF)/8);   //���еĳ���
#define FLASH_QUEUE_LEN ((((u32)0X3FFF)-FLASH_QUEUE_DATA_STARTADDR-1)/FLASH_QUEUE_A_DATA_LEN);   //���еĳ���


#define FLASH_QUEUE_STARTADDR ((uint32_t)0x011000)  //��ǰ���64k�ռ����ڴ�������
#define FLASH_QUEUE_ENDADDR ((uint32_t)0xFFFFF)
#define FLASH_QUEUE_SETOR_SIZE ((uint32_t)0x1000)
#define FLASH_QUEUE_BLOCK_SIZE ((uint32_t)0xFFFF)
//#define FLASH_QUEUE_DATA_STARTADDR (FLASH_QUEUE_STARTADDR+FLASH_QUEUE_SETOR_SIZE+FLASH_QUEUE_SETOR_SIZE) //���ݴ洢�Ŀ�ʼ��ַ��
#define FLASH_QUEUE_DATA_STARTADDR (FLASH_QUEUE_STARTADDR+0x2000) //���ݴ洢�Ŀ�ʼ��ַ��

//#define FLASH_QUEUE_CLEAR_LEN ((0X1000)/16-2) //�ڴﵽ������ݴ�С֮��������У���ͷд��
#define FLASH_QUEUE_CLEAR_LEN ((uint32_t)254) //�ڴﵽ������ݴ�С֮��������У���ͷд��  

#define FLASHBUFF_LEN ((uint32_t)32)
#define FLASH_STORAGE_HEADA ((u8)0xAA)
//#define FLASH_STORAGE_HEADA ((u8)0xBB)
#define FLASH_STORAGE_HEADB ((u8)0x55)
#define FLASH_STORAGE_TAILA ((u8)0xBB)
#define FLASH_STORAGE_TAILB ((u8)0x66)

#define FLASH_QUEUE_PRINT(num) PrintU32int2String(num)

typedef struct 
{
  u32 queuesize;   //����Ĵ�С
  u32 head, tail;  //���е�ͷ��β�±�
//  u8 queue_addr;   //������оƬ�ڲ�EEPROM��λ�á�
}_struFLASH_Queue;

typedef struct 
{
  bool DVALID_Flag;  //������Ч�Ա�־��
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