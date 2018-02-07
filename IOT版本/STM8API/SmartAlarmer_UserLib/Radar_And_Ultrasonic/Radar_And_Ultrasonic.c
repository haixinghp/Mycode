//#include "stm8l15x.h"
#include "config.h"
#include <string.h>


u16 Tim4_Int_Cnt=0;
u8 CheakValue_Buf[80]; 	     //��Ųɼ�������ֵ
//u8 CheakValue_Buf[CHECKVALUE_BUFF_LEN][4]; //��Ųɼ�������ֵ
u8 CheakValue_Buf_Cnt=0;  //����������»�ֵ
u8 Uart2_RevFlag=0;
u8 Uart2_Init_Flag= 0;    //0=����2��ʼ��   1=������ʼ��
u8 Uart2_Int_Flag=0;  //�жϱ�־��1���жϣ� 0=���ж�
u8 Uart2_FIFO_Full_Flag=0;  //0=DMA -->  ���� û��    1 = ����
u16 Value[CHECKVALUE_BUFF_LEN];
u8  Value_Cnt=0;
#define SECONDQUERY_VALUE 40   //�״���Ч����
/********************************************************
�������ã��״ﴫ�����ж϶���ȷ�Ϸ���
����һ��1ȷ�ϴ���
����ֵ��
        0 ��Ч
        1 ����ȷ����Ч
        2 ����ȷ����Ч
        3 ��ʱ�˳�--Ҳ����Ϊ�Ƕ���ȷ����Ч
*********************************************************/
u8 SecondQuery_With_QueryCount(u8 query_count)
{
  static u8 sencond_query_fun_state = 0;
  static u16 check_result = 0;
  switch(sencond_query_fun_state)
  {
  case 0:
          PrintString_("�����״�!");
          POWER_ON_RADAR;
          DelayT_ms(3000);
          sencond_query_fun_state = 1; 
    break;
  case 1:
    check_result= RadarOrUltrasonic_CheakValue(DEVICE_RADAR);
    if(check_result){
      //_printf("check_result=%u\r\n", check_result);
      if(check_result<=SECONDQUERY_VALUE){//����ȷ����Ч
         
        sencond_query_fun_state = 0;
        return 1;
      }else{
        sencond_query_fun_state = 0;
        return 2;
      }
    }else{
      return 0;
    }
    break;
  default:
            sencond_query_fun_state = 0;
    break;
  }
  return 0;
}

/*********************************************************
�������ã���ȡ��ǰ��������⵽�ľ���
����һ��1=ʹ���״��豸���(��λ��cm)  2=ʹ�ó������豸���(��λ��mm)
����ֵ����ǰ��������⵽�������ľ���

*********************************************************/
u16 RadarOrUltrasonic_CheakValue(u8 device_type)
{
 /*   u8 checkvalue_filter_num = CHECKVALUE_BUFF_LEN>>1;//9
    u8 checkvalue_filter_head = ((CHECKVALUE_BUFF_LEN-checkvalue_filter_num)>>1);//5
    u8 checkvalue_filter_end = checkvalue_filter_head + checkvalue_filter_num;

    u8 flag;
    u32 total = 0;
    if(Uart2_Init_Flag == 0)
    {
        if(device_type == DEVICE_RADAR)
          uart2_init(115200);
        else
          uart2_init(9600);
        Uart2_Init_Flag = 1;
    }
    //���߼���ʱ
    if(Uart2_FIFO_Full_Flag == 1)
    {
        Uart2_FIFO_Full_Flag = 0;
        Uart2_Init_Flag = 0;
        CheakValue_Buf_Cnt = 0;
        for(u8 i=0;i<CHECKVALUE_BUFF_LEN;i++)
        {
          flag = Cheak_Sum(&CheakValue_Buf[i][0]);
          if(flag == 1)
          {
              Value[Value_Cnt++] = (u16)(((u16)CheakValue_Buf[i][1]<<8) | CheakValue_Buf[i][2]);
              _printf("value=%u\r\n", Value[Value_Cnt-1]);
              if(Value_Cnt == CHECKVALUE_BUFF_LEN)  break;
          }
        }
        if(Value_Cnt<CHECKVALUE_BUFF_LEN)
        {
            _printf("Continue Sampling!\r\n");
            
            memset(CheakValue_Buf,0,sizeof(CheakValue_Buf));
        }
        else
        {
            _printf("Sampling Full\r\n");
            memset(CheakValue_Buf,0,sizeof(CheakValue_Buf));
            Value_Cnt = 0;
            CheakValue_Sort(Value,CHECKVALUE_BUFF_LEN);
            
            for(u8 i=checkvalue_filter_head;i<checkvalue_filter_end;i++)        //ȥ��5����Сֵ ȥ��5�����ֵ 
                total += Value[i];
            _printf("sum =%u\r\n",(u16)(total / checkvalue_filter_num));
            memset(Value,0,sizeof(Value));
 
            return (u16)(total / checkvalue_filter_num);
        }  
    }
        return 0;*/
      u8 flag;
    u32 total = 0;
    if(Uart2_Init_Flag == 0)
    {
        if(device_type == DEVICE_RADAR)
          uart2_init(115200);
        else
          uart2_init(9600);
        Uart2_Init_Flag = 1;
    }
    
    DMA_Cmd(DMA1_Channel3, ENABLE);  
    /* Enable the USART Rx DMA channel */  
//    DMA_Cmd(DMA1_Channel0, ENABLE);           
  USART_Cmd(USART2, ENABLE);
	Tim4_Int_Cnt= 0;
	/*20s*/
    Watchdog_Feed();
	while(Tim4_Int_Cnt<6000)
	{
    Watchdog_Feed();
		if(Uart2_FIFO_Full_Flag == 1)
		{
			Uart2_FIFO_Full_Flag = 0;
			Uart2_Init_Flag = 0;
			CheakValue_Buf_Cnt = 0;
			while(CheakValue_Buf_Cnt<77) 
			{
				if(CheakValue_Buf[CheakValue_Buf_Cnt]!=0xFF) 
				{
					CheakValue_Buf_Cnt++;
					continue;
				}
				flag = Cheak_Sum(&CheakValue_Buf[CheakValue_Buf_Cnt]);
				if(flag == 1)
				{
					Value[Value_Cnt++] = (u16)(((u16)CheakValue_Buf[CheakValue_Buf_Cnt+1]<<8) | CheakValue_Buf[CheakValue_Buf_Cnt+2]);
					CheakValue_Buf_Cnt += 4;
                                        PrintString("value=");
                                        printUNum2Str(Value[Value_Cnt-1]);
					//_printf("value=%u\r\n", Value[Value_Cnt-1]);
					if(Value_Cnt == 20)  break;
				}
				else
					CheakValue_Buf_Cnt++;
			}
			
			if(Value_Cnt<20)
			{
				PrintString("Continue Sampling!\r\n");
			//	uart2_disable();
				memset(CheakValue_Buf,0,sizeof(CheakValue_Buf));
			//	Uart2_FIFO_Full_Flag = 0;
			//	CheakValue_Buf_Cnt = 0;
				uart2_init(115200);// ������Ҫ�ٴ���DMA �ɼ� ����ҪDMA���³�ʼ����
                            DMA_Cmd(DMA1_Channel3, ENABLE);  
                            /* Enable the USART Rx DMA channel */  
                        //    DMA_Cmd(DMA1_Channel0, ENABLE);           
                          USART_Cmd(USART2, ENABLE);
				continue;
			}
			else
			{
				PrintString("Sampling Full\r\n");
			//	uart2_disable();
				memset(CheakValue_Buf,0,sizeof(CheakValue_Buf));
				Value_Cnt = 0;
				CheakValue_Sort(Value,20);
				for(u8 i=5;i<15;i++)        //ȥ��5����Сֵ ȥ��5�����ֵ 
					total += Value[i];
				PrintString("sum =");
                                printUNum2Str((u16)(total / 10));
				memset(Value,0,sizeof(Value));
				Uart2_FIFO_Full_Flag =0;
				return (u16)(total / 10);
			}  
		}
	}
    Watchdog_Feed();
	Tim4_Int_Cnt = 0;
	Value_Cnt = 0;
	memset(Value,0,sizeof(Value));
	Uart2_FIFO_Full_Flag = 0;
	Uart2_Init_Flag = 0;
	uart2_disable();
	memset(CheakValue_Buf,0,sizeof(CheakValue_Buf));
    return 0;
}


/***************************************
�������ã��ɼ�ֵУ��
����һ��ҪУ���ֵ
����ֵ��1=У��ɹ�  0=У��ʧ��
***************************************/
u8 Cheak_Sum(u8* buf)
{
	u8 sum;
	sum = (buf[0]+buf[1] +buf[2]) & 0x00ff;
	if(sum == buf[3]) //У��ɹ�
		return 1;
	return 0;
}


/*���������� ��С����*/
void CheakValue_Sort(u16* a,u8 n)
{
	int i,j,temp; 
	for(i=1;i<n;i++) { 
		temp=a[i]; /*tempΪҪ�����Ԫ��*/ 
		j=i-1; 
		while(j>=0&&temp<a[j]) { /*��a[i-1]��ʼ�ұ�a[i]С������ͬʱ������Ԫ�������*/ 
			a[j+1]=a[j]; 
			j--; 
		} 
		a[j+1]=temp; /*����*/ 
	} 
}
