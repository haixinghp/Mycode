#include"IAP.h"

IAP iap_data = {0};

/************************烧写程序**********************************************
* 函数名   	     : goto_iap
* 说明   	     : 将权限交给iap模块，更新用户程序
*******************************************************************************/
void goto_iap(void)
{
	sim();//__disable_fault_irq();

	Save_Iap_Config();
        PrintString_("将权限交给iap模块，更新用户程序");
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
	  sys_restart();//NVIC_SystemReset();
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : Save_Iap_Config
// 功能描述  : 保存当前升级用户程序参数
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : 成功 0  失败 -1
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
 int Save_Iap_Config(void)
{
	int i;
	char *ps = (char*)&iap_data;

	iap_data.CheckSum = 0;

	for(i = 0; i<sizeof(IAP)-2; i++)
	{
		iap_data.CheckSum += *ps;
		ps++;	
	}

        Flash_Config(Flash_FLASH_Unlock);//	FLASH_Unlock();

//	FLASH_ErasePage(iap_data_add);

	for(i=0;i<iap_constlong;i++)
	{   
        
          //FLASH_ProgramWord((iap_data_add+(i<<1)),*(u32*)((u32)&iap_data+(i*4)));
            FLASH_ProgramByte((iap_data_add+(i)),*(u8*)((u32)&iap_data+(i)));
//		FLASH_ProgramHalfWord( (iap_data_add+(i<<1)), *(u16*)((u32)&iap_data+(i<<1)));
	}

        Flash_Config(Flash_FLASH_Lock);//	FLASH_Lock();

	return 0;
}
void Init_Iap_Config(void)
{
  
    memset(&iap_data.WCDMAUpdateArg,NULL,8);//清空断点续传参数
    iap_data.writeflag = 0; //请求更新应用程序
    iap_data.copyflag = 0;//外部eeprom和内部flash搬运
    iap_data.src_add = 0;
    iap_data.dest_add = 0;
    iap_data.copy_long = 0;//总长度
    Save_Iap_Config();
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : Get_Iap_Config
// 功能描述  : 获取当前升级用户程序参数
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : 成功 0  失败 -1
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
int Get_Iap_Config(void)
{
	uint i;
	uint16 CheckSum = 0;
	char *ps = (char*)&iap_data;
//        char *temp = (char*)&iap_data;
//        u32 tempadd = iap_data_add;
//         for(i = 0;i<iap_constlong;i++)
//	{   
//          
//          *(temp++) = *(char *)(tempadd++);
//	}
	memcpy((char *)&iap_data,(char *)iap_data_add,sizeof(IAP));

	for(i = 0; i<sizeof(IAP)-2; i++)
	{
		CheckSum += *ps;
		ps++;	
	}
	if(CheckSum != iap_data.CheckSum)
	{
		return -1;
	}
	return 0;
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
//                          程序结束(END)
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※


