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
#define IAP_ROOM_SIZE   0x2000   //引导区域大小
  
#define APP_StartAddress  (u32)(FLASH_PROGRAM_START_PHYSICAL_ADDRESS+IAP_ROOM_SIZE)    //应用起始位置 引导空间 16K
#define iap_data_add 	    (u32)(APP_StartAddress-0xFF)                         //引导信息存储空间      1K
#define OutStorage_text_add     (EEPROM_24C256_START_ADDR+IAP_ROOM_SIZE-0xFF-0x0F)
#define APP_StorageSize   48
//#define iap_data_add  0x08007800    /*256个字节,128个字*/

typedef  struct tag_WCDMA_UpdateArg
{
  	char md5[8];
	DWORD length;	//长度
}tagWCDMAUpdateArg;

/*iap用到的数据*/
typedef  struct
{
	/*串口通信参数*/
	uint16_t USART_num;   					/*串口号			   */
	uint16_t remap;						    /*是否重映射		   */
	uint32_t bps;         					/*波特率			   */
	uint16_t writecompleteflag;             /*用户程序完整性	   */
	uint16_t writeflag;					    /*用户请求编程标志	   */
	uint16_t InitUserParFlag;				/*初始化用户参数标志   */
	/*网络通信参数*/ 
	uint8_t rip[4];
	uint16_t rport; 
	uint8_t lip[12];
	uint8_t lown_hw_adr[6];
	/*程序复制*/
	u32 copyflag;							/*复制标志位 0有效 	   */
	u32 src_add;							/*源地址			   */
	u32 dest_add;							/*目标地址			   */
	u32 copy_long; 							/*长度，19+8=27个半字  */
	u32 program_checksum;                                           /*程序内容校验和*/
	uint16_t Address;						/*设备地址*/
	tagWCDMAUpdateArg WCDMAUpdateArg;		/*应用程序升级,断点续传参数*/
	uint16 CheckSum;						/*校验和,此参数须在结构体尾部*/
}IAP;


//#define iap_constlong  sizeof(IAP)/4        /*iap数据, 半字（16位）个数*/
#define iap_constlong  sizeof(IAP)       /*iap数据, 字节个数*/

extern IAP iap_data ;
void Init_Iap_Config(void);
void goto_iap(void) ;
int Save_Iap_Config(void);
int Get_Iap_Config(void);

#ifdef __cplusplus
}
#endif

#endif

