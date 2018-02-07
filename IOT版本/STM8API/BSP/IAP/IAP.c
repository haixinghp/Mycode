#include"IAP.h"

IAP iap_data = {0};

/************************��д����**********************************************
* ������   	     : goto_iap
* ˵��   	     : ��Ȩ�޽���iapģ�飬�����û�����
*******************************************************************************/
void goto_iap(void)
{
	sim();//__disable_fault_irq();

	Save_Iap_Config();
        PrintString_("��Ȩ�޽���iapģ�飬�����û�����");
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
	  sys_restart();//NVIC_SystemReset();
}

//������������������������������������������������������������������������������
// ��������  : Save_Iap_Config
// ��������  : ���浱ǰ�����û��������
// �������  : None.
// �������  : None
// ���ز���  : �ɹ� 0  ʧ�� -1
//������������������������������������������������������������������������������
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
  
    memset(&iap_data.WCDMAUpdateArg,NULL,8);//��նϵ���������
    iap_data.writeflag = 0; //�������Ӧ�ó���
    iap_data.copyflag = 0;//�ⲿeeprom���ڲ�flash����
    iap_data.src_add = 0;
    iap_data.dest_add = 0;
    iap_data.copy_long = 0;//�ܳ���
    Save_Iap_Config();
}
//������������������������������������������������������������������������������
// ��������  : Get_Iap_Config
// ��������  : ��ȡ��ǰ�����û��������
// �������  : None.
// �������  : None
// ���ز���  : �ɹ� 0  ʧ�� -1
//������������������������������������������������������������������������������
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

//������������������������������������������������������������������������������
//                          �������(END)
//������������������������������������������������������������������������������


