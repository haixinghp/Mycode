#ifndef 	__RADAR_AND_ULTRASONIC_H_
#define 	__RADAR_AND_ULTRASONIC_H_
#define DEVICE_RADAR			0x01
#define DEVICE_ULTRASONIC		0x02

#define POWER_ON_RADAR         GPIO_ResetBits(GPIOF,GPIO_Pin_5);//�״��Դ��
#define POWER_OFF_RADAR        GPIO_SetBits(GPIOF,GPIO_Pin_5);//�״��Դ�ض�

#define CHECKVALUE_BUFF_LEN 40  //���63

extern u8 CheakValue_Buf[80]; //��Ųɼ�������ֵ
extern u8 CheakValue_Buf_Cnt;  //����������»�ֵ
extern u8 Uart2_RevFlag;
extern u8 Uart2_Init_Flag;    //0=����2��ʼ��   1=������ʼ��
extern u8 Uart2_Int_Flag;  //�жϱ�־��1���жϣ� 0=���ж�
extern u8 Uart2_FIFO_Full_Flag;
extern u16 Tim4_Int_Cnt;

extern u8 SecondQuery_With_QueryCount(u8 query_count);
extern u8 CheakValue_Buf[80]; //��Ųɼ�������ֵ
extern u8 CheakValue_Buf_Cnt;
extern u8 Uart2_RevFlag;
extern u16 RadarOrUltrasonic_CheakValue(u8 device_type);
//extern void USART2_Rxd_Service(void);
extern u8 Cheak_Sum(u8* buf);
extern void CheakValue_Sort(u16* a,u8 n);
#endif

