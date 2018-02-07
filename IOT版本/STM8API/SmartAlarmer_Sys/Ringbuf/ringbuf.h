#ifndef RING_BUF_H
#define RING_BUF_H


#define NMAX 64					//��������С������ʵ����Ҫ���ã������ܴ���65535


//�����Ǳ��������õ��ļ����������͵ĺ궨��
/*#define u8  unsigned char
#define u16 unsigned short
#define u32 unsigned int*/


extern u8  g_ucFull;			//FIFO����־
extern u8  g_ucEmpty;			//FIFO�ձ�־


u8 FIFO_DataIn(u8 data);		//����д��FIFO
u8 FIFO_DataOut(void);			//��FIFO�ж�������
u16 FIFO_GetNum(void);			//���FIFO�����ݸ���



#endif 
