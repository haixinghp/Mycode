/*----------------------------------------------------------------
  Copyright (C), 1995-2014, Shandong Yahua Electronics Co., Ltd.
  FileName: 	ringbuf.c
  Description:  ���λ��庯����ʵ�����ݵĻ��崦�������ڴ����շ��Ļ���
                ͨ�� FIFO_DataOut �����ӻ������ж�������
				ͨ�� FIFO_DataIn  �����򻺳�����д������
				ͨ�� FIFO_GetNum  ������û����������ݸ���
  Version: 		V1.0
  Author:   	SongKexin
  Date:  		2014-11-12
-----------------------------------------------------------------*/
#include "stm8l15x.h"
#include "ringbuf.h"



u8  g_ucBuf[NMAX];      //������

u16 g_ucWrIndex = 0;    //д������������ǰ����д���λ��
u16 g_ucRdIndex = 0;	//��������������ǰ�������ݵ�λ��
u8  g_ucOverstep = 0;   //Խ���־

u8  g_ucFull = 0;		//����������־
u8  g_ucEmpty = 1;		//�������ձ�־



//��ս��ջ�����
void FIFO_Cls(void) {
	disableInterrupts();

	g_ucWrIndex = 0;    //д������λ
	g_ucRdIndex = 0;	//��������λ
	g_ucOverstep = 0;   //Խ���־��λ

	g_ucFull = 0;		//����������־��λ
	g_ucEmpty = 1;		//�������ձ�־��λ
	enableInterrupts();
}

//������д�뻺����
//����������ʱ����λ g_ucFull ��־����ʾ��������
//����������������������д�����ݣ�������д������ݱ����
//data: д�뻺����������
//����ֵ�̶�Ϊ0
u8 FIFO_DataIn(u8 data)
{
	//g_ucWrIndex &= (NMAX - 1);
	g_ucBuf[g_ucWrIndex] = data;
	g_ucWrIndex++;

	//ȷ��ָ��һֱ����Ч��Χ�ڣ�дָ��Խ��ʱ�����´�0��ʼд����������Խ���־
	if (g_ucWrIndex == NMAX)
	{
		g_ucWrIndex = 0;
		g_ucOverstep = 1;
	}

	//FIFO��������д��һ�����ݣ�������д�������Ҫ�����
	//���Զ�ָ��Ҫ������дָ�����
	if (g_ucFull)
	{
		g_ucRdIndex = g_ucWrIndex;
	}

	//���дָ��Խ�磬�����ָ����ȣ���˵��д��
	if (g_ucOverstep & (g_ucWrIndex == g_ucRdIndex))
	{
		g_ucFull = 1;
	}
	else
	{
		g_ucFull = 0;
	}

	//д��һ�����ݣ��򻺳�����һ���ǲ��յ�
	g_ucEmpty = 0;

	return 0;
}

//�ӻ�������ȡ��1�ֽ�����
//�������������һ�����ݱ�ȡ����ʱ����λ g_ucEmpty ��־����ʾ��������
//�������ѿգ��ֽ��ж����ݲ��������������������Ч��
//����ֵΪ����������
u8 FIFO_DataOut(void)
{
	u8 ucData;

	//g_ucRdIndex &= (NMAX - 1);
	ucData = g_ucBuf[g_ucRdIndex];
	g_ucRdIndex++;

	//ȷ��ָ��һֱ����Ч��Χ�ڣ���ָ��Խ��ʱ�����´�0��ʼ�����������Խ���־
	if (g_ucRdIndex == NMAX)
	{
		g_ucRdIndex = 0;
		g_ucOverstep = 0;
	}

	//FIFO�ѿգ��ֶ�һ�����ݣ����������������Ч��
	//����дָ��Ҫ���Ŷ�ָ��仯�������´�д�������λ���ǲ���ȷ��
	if (g_ucEmpty)
	{
		g_ucWrIndex = g_ucRdIndex;
	}

	//��Խ���־Ϊ0���Ҷ�дָ����ȣ���˵���Ѿ�������
	if ((!g_ucOverstep) & (g_ucWrIndex == g_ucRdIndex))
	{
		g_ucEmpty = 1;
	}
	else
	{
		g_ucEmpty = 0;
	}

	//����һ�����ݣ��򻺳�����һ���ǲ�����
	g_ucFull = 0;

	return ucData;
}




//��ȡ�����������ݸ���
u16 FIFO_GetNum(void)
{
	u16 ucDataNum;

	if (g_ucOverstep)
	{
		ucDataNum = NMAX + g_ucWrIndex - g_ucRdIndex;
	}
	else
	{
		ucDataNum = g_ucWrIndex - g_ucRdIndex;
	}

	return 	ucDataNum;
}


