#ifndef _IAP_H
#define _IAP_H
#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
  


#define EEPROM_24C256_START_ADDR   0x0000
#define EEPROM_24C256_END_ADDR     0x8000
#define FLASH_START_ADDR	0x16000
#define FLASH_END_ADDR          0x17FFF
#define IAP_ROOM_SIZE   0x2000   //���������С
  
#define APP_StartAddress  (u32)(FLASH_PROGRAM_START_PHYSICAL_ADDRESS+IAP_ROOM_SIZE)    //Ӧ����ʼλ�� �����ռ� 16K
#define iap_data_add 	    (u32)(APP_StartAddress-0xFF)                         //������Ϣ�洢�ռ�      1K
#define OutStorage_text_add     (EEPROM_24C256_START_ADDR+IAP_ROOM_SIZE-0xFF-0x0F)
#define APP_StorageSize   48
//#define iap_data_add  0x08007800    /*256���ֽ�,128����*/

typedef  struct tag_WCDMA_UpdateArg
{
  	char md5[8];
	DWORD length;	//����
}tagWCDMAUpdateArg;

/*iap�õ�������*/
typedef  struct
{
	/*����ͨ�Ų���*/
	uint16_t USART_num;   					/*���ں�			   */
	uint16_t remap;						    /*�Ƿ���ӳ��		   */
	uint32_t bps;         					/*������			   */
	uint16_t writecompleteflag;             /*�û�����������	   */
	uint16_t writeflag;					    /*�û������̱�־	   */
	uint16_t InitUserParFlag;				/*��ʼ���û�������־   */
	/*����ͨ�Ų���*/ 
	uint8_t rip[4];
	uint16_t rport; 
	uint8_t lip[12];
	uint8_t lown_hw_adr[6];
	/*������*/
	u32 copyflag;							/*���Ʊ�־λ 0��Ч 	   */
	u32 src_add;							/*Դ��ַ			   */
	u32 dest_add;							/*Ŀ���ַ			   */
	u32 copy_long; 							/*���ȣ�19+8=27������  */
	u32 program_checksum;                                           /*��������У���*/
	uint16_t Address;						/*�豸��ַ*/
	tagWCDMAUpdateArg WCDMAUpdateArg;		/*Ӧ�ó�������,�ϵ���������*/
	uint16 CheckSum;						/*У���,�˲������ڽṹ��β��*/
}IAP;


//#define iap_constlong  sizeof(IAP)/4        /*iap����, ���֣�16λ������*/
#define iap_constlong  sizeof(IAP)       /*iap����, �ֽڸ���*/

extern IAP iap_data ;
void Init_Iap_Config(void);
void goto_iap(void) ;
int Save_Iap_Config(void);
int Get_Iap_Config(void);

#ifdef __cplusplus
}
#endif

#endif

