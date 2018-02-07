#ifndef __EEPROM_H
#define __EEPROM_H

#define EEPROM_STARTADDR 0x1000 
#define EEPROM_WRITE_FLAG_A 0X55
#define EEPROM_WRITE_FLAG_B 0XBB
extern u8 EEPROM_Read(u16 Addr,u8 *RxBuffer,u8 Length);
extern u8 EEPROM_Write(u16 Addr,u8 *RxBuffer,u8 Lenth);


//读取系统关键参数
void Read_sys_keydata(void);
//存储系统关键参数
void Write_sys_keydata(void);
//恢复默认参数
void Init_sys_keydata(void);
//存储发送总次数
void Write_sys_keydata_totalcount(void);

#define SYS_KEYDATA_ADDR 2  //从第二个开始存放
//#define SYS_KEYDATA_SEND_TOTALCOUNT_ADDR_OFFSET	25
//#define SYS_KEYDATA_SEND_TOTALCOUNT_ADDR_OFFSET	27	
#define SYS_KEYDATA_SEND_TOTALCOUNT_ADDR_OFFSET	21	
#endif