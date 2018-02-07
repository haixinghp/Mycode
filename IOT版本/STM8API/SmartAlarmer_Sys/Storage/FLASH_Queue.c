#include "FLASH_Queue.h"
#include "W25QXX.h"
#include "hardwareSTM8L_SPI.h"
#include <stdio.h>
//#include "queue.h"
//#include "FM24CLXX.h"
_struFLASH_Queue struFLASH_Queue;
_struFLASH_Queue struFLASH_QueueBuckup;

_struFLASH_QueueControl struFLASH_QueueControl;

u8 gaFlashBuff[FLASHBUFF_LEN]; 
u8 gaFlashBuff_A[FLASHBUFF_LEN]; 
void ArrayToArray(u8 *pa,u8 *pb,u16 len)
{
  int i;
  for(i=0;i<len;i++)
  {
    *pa = *pb;
    pa++;
    pb++;
  }
}


/***************************************************************************************
**������:       FLASH_Queue_FindHead
**��������:     
���ڲ��Ҷ�������ͷ��
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-28,By Haixing.Huang
****************************************************************************************/
s8 FLASH_Queue_HeadTailRefresh()
{
  union _uni_u32_tmp
  {
    u32 tmp;
    u8  tmp_tab[4];
  };
  union _uni_u32_tmp uni_u32_tmp;
//  union _uni_u32_tmp uni_u32_tmp_head;
//  union _uni_u32_tmp uni_u32_tmp_tail;
//  union _uni_u32_tmp uni_u32_tmp_queuesize;
  gaFlashBuff[0] = FLASH_STORAGE_HEADA;
  gaFlashBuff[1] = FLASH_STORAGE_HEADB;
  uni_u32_tmp.tmp = struFLASH_Queue.head;
  ArrayToArray(&gaFlashBuff[2],uni_u32_tmp.tmp_tab,4);
  uni_u32_tmp.tmp = struFLASH_Queue.tail;
  ArrayToArray(&gaFlashBuff[6],uni_u32_tmp.tmp_tab,4);
  uni_u32_tmp.tmp = struFLASH_Queue.queuesize;
  ArrayToArray(&gaFlashBuff[10],uni_u32_tmp.tmp_tab,4);
  gaFlashBuff[14] = FLASH_STORAGE_TAILA;
  gaFlashBuff[15] = FLASH_STORAGE_TAILB;    
  EEPROM_Write(SYS_KEYDATA_ADDR+sizeof(Sys_keydata)+EEPROM_HISTORY_ADDR,gaFlashBuff, FLASH_QUEUE_EEPROM_LEN);  //д�����ݵ�EEPROM
  EEPROM_Read(SYS_KEYDATA_ADDR+sizeof(Sys_keydata)+EEPROM_HISTORY_ADDR,gaFlashBuff, FLASH_QUEUE_EEPROM_LEN);
  return 0;
}
/***************************************************************************************
**������:       FLASH_Queue_FindHead
**��������:     
���ڲ��Ҷ�������ͷ��
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-28,By Haixing.Huang
****************************************************************************************/
s8 FLASH_Queue_FindHead()
{
//  u32 i,j;
  
  
//  u32 i_back,j_back;
  
//  typedef struct 
//  {
//    u8 a;
//    u32 b;
//    u8 c;
//  }stru_tmp;
//  union _uni_tmp
//  {
//    u8 tmp[18];
//    stru_tmp head;
//    stru_tmp tail;
//    stru_tmp queuesize;
//  };
//  union _uni_tmp uni_tmp;
  union _uni_u32_tmp
  {
    u32 tmp;
    u8  tmp_tab[4];
  };
//  union _uni_u32_tmp uni_u32_tmp;
  union _uni_u32_tmp uni_u32_tmp_head;
  union _uni_u32_tmp uni_u32_tmp_tail;
//  union _uni_u32_tmp uni_u32_tmp_queuesize;
  
  EEPROM_Read(SYS_KEYDATA_ADDR+sizeof(Sys_keydata)+EEPROM_HISTORY_ADDR,gaFlashBuff, FLASH_QUEUE_EEPROM_LEN);
  if((FLASH_STORAGE_HEADA == gaFlashBuff[0]) && (FLASH_STORAGE_HEADB == gaFlashBuff[1])&&
     ((FLASH_STORAGE_TAILA == gaFlashBuff[14]) && (FLASH_STORAGE_TAILB == gaFlashBuff[15]))
     ) //У������ͷAA 55 У������β��BB 66
  {  
    ArrayToArray(uni_u32_tmp_head.tmp_tab,&gaFlashBuff[2],4);
    ArrayToArray(uni_u32_tmp_tail.tmp_tab,&gaFlashBuff[6],4);
    struFLASH_Queue.head = uni_u32_tmp_head.tmp;
    struFLASH_Queue.tail = uni_u32_tmp_tail.tmp;
//    FLASH_QUEUE_PRINT(struFLASH_Queue.head);
//    FLASH_QUEUE_PRINT(struFLASH_Queue.tail);
//    FLASH_QUEUE_PRINT(struFLASH_Queue.queuesize);
    struFLASH_Queue.queuesize = FLASH_QUEUE_LEN; //���ȹ̶�
    return 0; //���ҳɹ���
  }
  
//  struFLASH_QueueControl.DVALID_Flag = FALSE;
//  for(i=0;i<0xF00;i++)
//  {
//    W25QXX_HighSpeedReadData(FLASH_QUEUE_STARTADDR+i*FLASHBUFF_LEN,gaFlashBuff,FLASHBUFF_LEN);  //
//    for(j=0;j<16;j++)
//    {
//      
//      if((FLASH_STORAGE_HEADA == gaFlashBuff[j*16]) && (FLASH_STORAGE_HEADB == gaFlashBuff[j*16+1])&&
//         ((FLASH_STORAGE_TAILA == gaFlashBuff[j*16+14]) && (FLASH_STORAGE_TAILB == gaFlashBuff[j*16+15]))
//         ) //У������ͷAA 55 У������β��BB 66
//      {
//          struFLASH_QueueControl.DVALID_Flag = TRUE;  //��ʾ��ȡ��һ����Ч���ݡ�
//          ArrayToArray(uni_u32_tmp_head.tmp_tab,&gaFlashBuff[j*16+2],4);
//          ArrayToArray(uni_u32_tmp_tail.tmp_tab,&gaFlashBuff[j*16+6],4);
//          ArrayToArray(uni_u32_tmp_queuesize.tmp_tab,&gaFlashBuff[j*16+6],4);
//      }
//      else
//      {
//        if(TRUE == struFLASH_QueueControl.DVALID_Flag)  //֮ǰ���ֹ�����Ч������,ֱ����дǰ�����ݵ����б����С� 
//        {
//          struFLASH_Queue.head = uni_u32_tmp_head.tmp;
//          struFLASH_Queue.tail = uni_u32_tmp_tail.tmp;
//          struFLASH_Queue.queuesize = FLASH_QUEUE_LEN; //���ȹ̶�
//          W25QXX_ChipSectorErase(FLASH_QUEUE_STARTADDR);
//          gaFlashBuff[0] = FLASH_STORAGE_HEADA;
//          gaFlashBuff[1] = FLASH_STORAGE_HEADB;
//          uni_u32_tmp.tmp = struFLASH_Queue.head;
//          ArrayToArray(&gaFlashBuff[2],uni_u32_tmp.tmp_tab,4);
//          uni_u32_tmp.tmp = struFLASH_Queue.tail;
//          ArrayToArray(&gaFlashBuff[6],uni_u32_tmp.tmp_tab,4);
//          uni_u32_tmp.tmp = struFLASH_Queue.queuesize;
//          ArrayToArray(&gaFlashBuff[10],uni_u32_tmp.tmp_tab,4);
//          gaFlashBuff[14] = FLASH_STORAGE_TAILA;
//          gaFlashBuff[15] = FLASH_STORAGE_TAILB;//���еĸ�ʽΪAA 55 XX XX XX XX YY YY YY YY ZZ ZZ ZZ ZZ BB 66
//          W25QXX_Write_NoCheck(FLASH_QUEUE_STARTADDR+struFLASH_QueueControl.queue_addr*16,gaFlashBuff,16); //��FLASHд������
//          return 0; //���ҳɹ���
//        }
//        else
//        {
//          return EOF;  //û�в�ѯ����Ч�����ݡ�
//        }
//      }
//    }
//  }
  return EOF;  //û�в�ѯ����Ч�����ݡ�
}
/***************************************************************************************
**������:       ClearFLASH_Queue
**��������:     ���FLASH���С�
**�������:     --
**�������:     --
**��ע:         YY.c,2017-11-15,By Haixing.Huang
****************************************************************************************/
void ClearFLASH_Queue(void)
{
  union _uni_u32_tmp
  {
    u32 tmp;
    u8  tmp_tab[4];
  };
  union _uni_u32_tmp uni_u32_tmp;
  struFLASH_Queue.head = 0;
  struFLASH_Queue.tail=0;
  struFLASH_Queue.queuesize=FLASH_QUEUE_LEN;
  W25QXX_ChipErase(); //������е�����,�����Ƕ���ƬFLASH����������������������Ҫ��ֻ���������Ҫ�޸���� 
  gaFlashBuff[0] = FLASH_STORAGE_HEADA;
  gaFlashBuff[1] = FLASH_STORAGE_HEADB;
  uni_u32_tmp.tmp = struFLASH_Queue.head;
  ArrayToArray(&gaFlashBuff[2],uni_u32_tmp.tmp_tab,4);
  uni_u32_tmp.tmp = struFLASH_Queue.tail;
  ArrayToArray(&gaFlashBuff[6],uni_u32_tmp.tmp_tab,4);
  uni_u32_tmp.tmp = struFLASH_Queue.queuesize;
  ArrayToArray(&gaFlashBuff[10],uni_u32_tmp.tmp_tab,4);
  gaFlashBuff[14] = FLASH_STORAGE_TAILA;
  gaFlashBuff[15] = FLASH_STORAGE_TAILB;    
  EEPROM_Write(SYS_KEYDATA_ADDR+sizeof(Sys_keydata)+EEPROM_HISTORY_ADDR,gaFlashBuff, FLASH_QUEUE_EEPROM_LEN);  //д�����ݵ�EEPROM
  EEPROM_Read(SYS_KEYDATA_ADDR+sizeof(Sys_keydata)+EEPROM_HISTORY_ADDR,gaFlashBuff, FLASH_QUEUE_EEPROM_LEN);
}

/***************************************************************************************
**������:       Queue_GetDataStorage
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-28,By Haixing.Huang
****************************************************************************************/
void InitFLASH_Queue(void)
{
//  u8 i;
//  union _uni_u32_tmp
//  {
//    u32 tmp;
//    u8  tmp_tab[4];
//  };
//  union _uni_u32_tmp uni_u32_tmp;

  
  if( EOF == FLASH_Queue_FindHead())    //��ʼ��ϵͳ��
  {
    ClearFLASH_Queue(); 
//    struFLASH_Queue.head = 0;
//    struFLASH_Queue.tail=0;
//    struFLASH_Queue.queuesize=FLASH_QUEUE_LEN;
//    W25QXX_ChipErase(); //������е�����
//    gaFlashBuff[0] = FLASH_STORAGE_HEADA;
//    gaFlashBuff[1] = FLASH_STORAGE_HEADB;
//    uni_u32_tmp.tmp = struFLASH_Queue.head;
//    ArrayToArray(&gaFlashBuff[2],uni_u32_tmp.tmp_tab,4);
//    uni_u32_tmp.tmp = struFLASH_Queue.tail;
//    ArrayToArray(&gaFlashBuff[6],uni_u32_tmp.tmp_tab,4);
//    uni_u32_tmp.tmp = struFLASH_Queue.queuesize;
//    ArrayToArray(&gaFlashBuff[10],uni_u32_tmp.tmp_tab,4);
//    gaFlashBuff[14] = FLASH_STORAGE_TAILA;
//    gaFlashBuff[15] = FLASH_STORAGE_TAILB;    
//    EEPROM_Write(SYS_KEYDATA_ADDR+sizeof(Sys_keydata),gaFlashBuff, FLASH_QUEUE_EEPROM_LEN);  //д�����ݵ�EEPROM
//    EEPROM_Read(SYS_KEYDATA_ADDR+sizeof(Sys_keydata),gaFlashBuff, FLASH_QUEUE_EEPROM_LEN);
    return ;
  }
//  if( EOF == FLASH_Queue_FindHead())    //��ʼ��ϵͳ��
//  {
//    struFLASH_Queue.head = 0;
//    struFLASH_Queue.tail=0;
//    struFLASH_Queue.queuesize=FLASH_QUEUE_LEN;
//    struFLASH_QueueControl.queue_addr = 0;
//    W25QXX_ChipErase(); //������е�����
//    gaFlashBuff[0] = FLASH_STORAGE_HEADA;
//    gaFlashBuff[1] = FLASH_STORAGE_HEADB;
//    uni_u32_tmp.tmp = struFLASH_Queue.head;
//    ArrayToArray(&gaFlashBuff[2],uni_u32_tmp.tmp_tab,4);
//    uni_u32_tmp.tmp = struFLASH_Queue.tail;
//    ArrayToArray(&gaFlashBuff[6],uni_u32_tmp.tmp_tab,4);
//    uni_u32_tmp.tmp = struFLASH_Queue.queuesize;
//    ArrayToArray(&gaFlashBuff[10],uni_u32_tmp.tmp_tab,4);
//    gaFlashBuff[14] = FLASH_STORAGE_TAILA;
//    gaFlashBuff[15] = FLASH_STORAGE_TAILB;
//    W25QXX_Write_NoCheck(FLASH_QUEUE_STARTADDR,gaFlashBuff,16); //��FLASHд������
//      W25QXX_HighSpeedReadData(FLASH_QUEUE_STARTADDR,gaFlashBuff_A,16);
//
//    
//    return ;
//  }
}
void EnFLASH_Queue(u32 TimesTamp,u16 PressureData,u16 Alarmlist)
{
  u16 i;
  u32 u32_tmp;
  union _uni_u32_tmp
  {
    u32 tmp;
    u8  tmp_tab[4];
  };
  union _uni_u32_tmp uni_u32_tmp;
  union _uni_u16_tmp
  {
    u16 tmp;
    u8 tmp_tab[2];
  };
  union _uni_u16_tmp uni_u16_tmp;
  u32 tail = (struFLASH_Queue.tail+1) % struFLASH_Queue.queuesize; //ȡ�ౣ֤����quil=queuesize-1ʱ����ת��0
  if (tail == struFLASH_Queue.head)                   //��ʱ����û�пռ�
  {
    debug_send("�������ˣ�");
  }
  else
  {
    struFLASH_Queue.tail = tail;
    //FLASH�е����⶯����
    if(0 == ((tail*FLASH_QUEUE_A_DATA_LEN) % FLASH_QUEUE_SETOR_SIZE)) //�����µ�һ�Ρ���Ҫǿ����������һ��,����head�����ݻᷢ����ʧ��headҲ�ᱻǿ�ƺ��ơ�
    {
      debug_send("�����µ�һ��");
      if(((struFLASH_Queue.head*FLASH_QUEUE_A_DATA_LEN) >= (struFLASH_Queue.tail*FLASH_QUEUE_A_DATA_LEN))&&
         ((struFLASH_Queue.head*FLASH_QUEUE_A_DATA_LEN) <= (struFLASH_Queue.tail*FLASH_QUEUE_A_DATA_LEN+FLASH_QUEUE_SETOR_SIZE-1)) 
           ) //
      {
        debug_send("����������������������������������ͷβ���һ��");
        
        u32_tmp=FLASH_QUEUE_SETOR_SIZE - (struFLASH_Queue.tail*FLASH_QUEUE_A_DATA_LEN)%FLASH_QUEUE_SETOR_SIZE; //��������һ�εĲ�ֵ��
        struFLASH_Queue.head = (struFLASH_Queue.tail+(u32_tmp/FLASH_QUEUE_A_DATA_LEN))%struFLASH_Queue.queuesize; 
//        struFLASH_Queue.head = (struFLASH_Queue.head+1) %struFLASH_Queue.queuesize;  //����head����
      }
      W25QXX_ChipSectorErase(FLASH_QUEUE_DATA_STARTADDR+struFLASH_Queue.tail*FLASH_QUEUE_A_DATA_LEN); //ǿ�������һ�Ρ�
    }
    /***********************************д�����ݵ�FLASH��*****************************************/  
    gaFlashBuff[0] = FLASH_STORAGE_HEADA;
    uni_u32_tmp.tmp = TimesTamp;
    ArrayToArray(&gaFlashBuff[1],uni_u32_tmp.tmp_tab,4);
    uni_u16_tmp.tmp = PressureData;
    ArrayToArray(&gaFlashBuff[5],uni_u16_tmp.tmp_tab,2);
    uni_u16_tmp.tmp = Alarmlist;
    ArrayToArray(&gaFlashBuff[7],uni_u16_tmp.tmp_tab,2);
    gaFlashBuff[FLASH_QUEUE_A_DATA_LEN-1] = FLASH_STORAGE_HEADB;  //���ݵĸ�ʽΪAA XX XX XX XX YY YY ZZ ZZ ** ** ** ** ** ** 55

    for(i=0;i<3;i++)
    {
      W25QXX_Write_NoCheck(FLASH_QUEUE_DATA_STARTADDR+struFLASH_Queue.tail*FLASH_QUEUE_A_DATA_LEN,gaFlashBuff,FLASH_QUEUE_A_DATA_LEN); //��FLASHд������
      W25QXX_HighSpeedReadData(FLASH_QUEUE_DATA_STARTADDR+struFLASH_Queue.tail*FLASH_QUEUE_A_DATA_LEN,gaFlashBuff_A,FLASH_QUEUE_A_DATA_LEN);
      if(0 == memcmp(&gaFlashBuff,&gaFlashBuff_A,FLASH_QUEUE_A_DATA_LEN)) //�Ա����ݡ�
      {
        break;
      }
    }
    if(3==i)
    {
      debug_send("�������ݣ�FLASHд�����!");
    }
    /***********************************д�����ݵ�FLASH�н���*****************************************/  
    /***********************************ˢ��Flash�еĶ�������*****************************************/  
    debug_send_no_end("�����-->");
//    debug_send_no_end("head:");
//    FLASH_QUEUE_PRINT(struFLASH_Queue.head);
//    debug_send_no_end(" tail:");
//    FLASH_QUEUE_PRINT(struFLASH_Queue.tail);
//    debug_send_no_end(" queuesize:");
//    FLASH_QUEUE_PRINT(struFLASH_Queue.queuesize);
//    FLASH_Queue_HeadTailRefresh();
    debug_send("");
    FLASH_Queue_HeadTailRefresh();
    /***********************************ˢ��Flash�еĶ������ݽ���*****************************************/ 
  }
}

int DeFLASH_Queue(u32 *TimesTamp,u16 *PressureData,u16 *Alarmlist)  //
{
//  u16 i;
  u8 buf_tmp[32];
  union _uni_u32_tmp
  {
    u32 tmp;
    u8  tmp_tab[4];
  };
  union _uni_u32_tmp uni_u32_tmp;
  union _uni_u16_tmp
  {
    u16 tmp;
    u8 tmp_tab[2];
  };
  union _uni_u16_tmp uni_u16_tmp;
  
  //  union _uni_tmp
  //  {
  //    u8 tmp[6];
  //    struct 
  //    {
  //      u32 a;
  //      u16 b;
  //    }stru_tmp;
  //  };
  //  union _uni_tmp uni_tmp;
  
  if(struFLASH_Queue.tail == struFLASH_Queue.head)     //�ж϶��в�Ϊ��
  {
    //            printf("the queue is NULL\n");
    debug_send("the queue is NULL!");
  }
  else
  {  
    FLASH_Queue_Buckup(); //���ݵ�ǰ����
    struFLASH_Queue.head = (struFLASH_Queue.head+1) %struFLASH_Queue.queuesize;  //����head����
    W25QXX_HighSpeedReadData(FLASH_QUEUE_DATA_STARTADDR+struFLASH_Queue.head*FLASH_QUEUE_A_DATA_LEN,buf_tmp,FLASH_QUEUE_A_DATA_LEN);  
    if((FLASH_STORAGE_HEADA ==buf_tmp[0])&&(FLASH_STORAGE_HEADB ==buf_tmp[FLASH_QUEUE_A_DATA_LEN -1]))  //У��ͷβ
    {
      ArrayToArray(uni_u32_tmp.tmp_tab,&buf_tmp[1],4); // ��ȡʱ��
      *TimesTamp = uni_u32_tmp.tmp;
      ArrayToArray(uni_u16_tmp.tmp_tab,&buf_tmp[5],2); // ��ȡѹ��ֵ��
      *PressureData = uni_u16_tmp.tmp;
      ArrayToArray(uni_u16_tmp.tmp_tab,&buf_tmp[7],2); // ��ȡ�澯�б�
      *Alarmlist = uni_u16_tmp.tmp;
    }
    else        //ͷβУ���������Ĭ��ֵ��
    {
      *TimesTamp = 0;
      *PressureData =0;
    }
//    struFLASH_Queue.head = (struFLASH_Queue.head+1) %struFLASH_Queue.queuesize;  //����head����
    /***********************************ˢ��Flash�еĶ�������*****************************************/  
    debug_send_no_end("������-->");
//    debug_send_no_end("head:");
//    FLASH_QUEUE_PRINT(struFLASH_Queue.head);
//    debug_send_no_end(" tail:");
//    FLASH_QUEUE_PRINT(struFLASH_Queue.tail);
//    debug_send_no_end(" queuesize:");
//    FLASH_QUEUE_PRINT(struFLASH_Queue.queuesize);
    FLASH_Queue_HeadTailRefresh();
    debug_send("");
    /***********************************ˢ��Flash�еĶ������ݽ���*****************************************/ 
  }
  return 0;
}

int IsFLASH_QueueEmpty(void)
{
  if(struFLASH_Queue.head == struFLASH_Queue.tail)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/***************************************************************************************
**������:       FLASH_Queue_Buckup
**��������:     
���ڲ��Ҷ�������ͷ��
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-28,By Haixing.Huang
****************************************************************************************/
s8 FLASH_Queue_Buckup()
{
  struFLASH_QueueBuckup.head = struFLASH_Queue.head;
  struFLASH_QueueBuckup.tail = struFLASH_Queue.tail;
  return 0;
}
/***************************************************************************************
**������:       FLASH_Queue_Buckup
**��������:     
���ڲ��Ҷ�������ͷ��
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-28,By Haixing.Huang
****************************************************************************************/
s8 FLASH_Queue_Recover()
{
  struFLASH_Queue.head = struFLASH_QueueBuckup.head;
  struFLASH_Queue.tail = struFLASH_QueueBuckup.tail;
  FLASH_Queue_HeadTailRefresh();
  return 0;
}
//int IsQueueFull(Queue *Q)
//{
//  if((Q->tail+1)% Q->queuesize == Q->head)
//  {
//    return 1;
//  }
//  else
//  {
//    return 0;
//  }
//}
////int main()
////{
////	int i;
////	InitQueue(&myqueue);
////	EnQueue(&myqueue,20);
////	EnQueue(&myqueue,8);
////	EnQueue(&myqueue,12);
////	EnQueue(&myqueue,6);
////	printf("result is:%d\n",DeQueue(&myqueue));
////	printf("result is:%d\n",DeQueue(&myqueue));
////	printf("result is:%d\n",DeQueue(&myqueue));
////	printf("result is:%d\n",DeQueue(&myqueue));
////	printf("result is:%d\n",DeQueue(&myqueue));
////}