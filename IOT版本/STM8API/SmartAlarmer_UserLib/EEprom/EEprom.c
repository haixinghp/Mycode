//#include "stm8l15x.h"
#include "config.h"

u8 EEPROM_Read(u16 Addr,u8 *RxBuffer,u8 Length)  
{  
    u16 T=0;  
    u8 *EEP;  
    EEP=(u8 *)(EEPROM_STARTADDR+Addr);  
//    FLASH->DUKR=0xAE;  
//    FLASH->DUKR=0x56;  //bug：读取时解锁，需注释 20171013
//    FLASH->CR2=0x00;  
//    while((!(FLASH->IAPSR&(1<<3)))&(T<0xFFFF))
//      T++;  
//    if(T==0xFFFF)return 0;  
    while(Length--){  
        *RxBuffer++=*EEP++;  
    }  
    return 1;  
}  
u8 EEPROM_Write(u16 Addr,u8 *RxBuffer,u8 Lenth)  
{  
    u16 T=0;  
    u8 *EEP;  
    EEP=(u8 *)(EEPROM_STARTADDR+Addr);  
    FLASH->DUKR=0xAE;  
    FLASH->DUKR=0x56;  
    FLASH->CR2=0x00;  
    while((!(FLASH->IAPSR&(1<<3)))&(T<0xFFFF))T++;  
    if(T==0xFFFF)return 0;  
    while(Lenth--){  
        *EEP++=*RxBuffer++;  
        T=0;  
        while((!(FLASH->IAPSR&(1<<2)))&(T<0xFFFF))T++;  
        if(T==0xFFFF)return 0;  
    }  
    FLASH->IAPSR&=~(1<<3); 
    return 1;  
}  



void InitEEprom(void) {

}

//从EEPRPOM获取系统参数 并做范围的矫正
//第0位 和第1位 做为系统的参数标志位 分别为0x55和0xaa时 表示系统的参数是后期存储的
//不然就需要初始化EE
//读取系统关键参数
void Read_sys_keydata(void) 
{
  u8 temp_eeprom_read_array[2] = {0, 0};
  
  for (u8 i = 0; i < 2; i++) 
  {
    EEPROM_Read(i, temp_eeprom_read_array + i, 1);
  }
  
  if ((temp_eeprom_read_array[0] == EEPROM_WRITE_FLAG_A) && (temp_eeprom_read_array[1] == EEPROM_WRITE_FLAG_B)) 
  { 
    EEPROM_Read(SYS_KEYDATA_ADDR, Sys_keydata.device_id, SYS_KEYDATA_LEN);//如果存有参数，则读出参数 总长度SYS_KEYDATA_LEN
  } 
  else 
  {
    Init_sys_keydata(); //恢复默认参数
  }
}

//存储系统关键参数
void Write_sys_keydata(void) {
    u8 temp_eeprom_read_array[2] = {EEPROM_WRITE_FLAG_A, EEPROM_WRITE_FLAG_B};
    EEPROM_Write(SYS_KEYDATA_ADDR,Sys_keydata.device_id,SYS_KEYDATA_LEN);
    EEPROM_Write(0,temp_eeprom_read_array,2);
}


//恢复默认参数
void Init_sys_keydata(void){
    
    for(u8 i = 0;i<SYS_KEYDATA_LEN;i++){
        *(Sys_keydata.device_id+i) = *(Default_Sys_keydata.device_id+i);
    }

    //存储到EEPROM
    Write_sys_keydata();
}


//存储发送总次数
void Write_sys_keydata_totalcount(void) {
    //Sys_keydata.send_totalcount=40000;
    EEPROM_Write(SYS_KEYDATA_ADDR+SYS_KEYDATA_SEND_TOTALCOUNT_ADDR_OFFSET,(u8 *)(&Sys_keydata.send_totalcount),2);
}


