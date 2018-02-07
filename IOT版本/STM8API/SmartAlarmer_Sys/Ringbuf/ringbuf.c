/*----------------------------------------------------------------
  Copyright (C), 1995-2014, Shandong Yahua Electronics Co., Ltd.
  FileName: 	ringbuf.c
  Description:  环形缓冲函数，实现数据的缓冲处理，适用于串口收发的缓冲
                通过 FIFO_DataOut 函数从缓冲区中读出数据
				通过 FIFO_DataIn  函数向缓冲区中写入数据
				通过 FIFO_GetNum  函数获得缓冲区中数据个数
  Version: 		V1.0
  Author:   	SongKexin
  Date:  		2014-11-12
-----------------------------------------------------------------*/
#include "stm8l15x.h"
#include "ringbuf.h"



u8  g_ucBuf[NMAX];      //缓冲区

u16 g_ucWrIndex = 0;    //写索引，描述当前数据写入的位置
u16 g_ucRdIndex = 0;	//读索引，描述当前读出数据的位置
u8  g_ucOverstep = 0;   //越界标志

u8  g_ucFull = 0;		//缓冲区满标志
u8  g_ucEmpty = 1;		//缓冲区空标志



//清空接收缓冲区
void FIFO_Cls(void) {
	disableInterrupts();

	g_ucWrIndex = 0;    //写索引复位
	g_ucRdIndex = 0;	//读索引复位
	g_ucOverstep = 0;   //越界标志复位

	g_ucFull = 0;		//缓冲区满标志复位
	g_ucEmpty = 1;		//缓冲区空标志复位
	enableInterrupts();
}

//将数据写入缓冲区
//当缓冲区满时，置位 g_ucFull 标志，表示缓冲区满
//缓冲区已满，但又向其中写入数据，则最先写入的数据被冲掉
//data: 写入缓冲区的数据
//返回值固定为0
u8 FIFO_DataIn(u8 data)
{
	//g_ucWrIndex &= (NMAX - 1);
	g_ucBuf[g_ucWrIndex] = data;
	g_ucWrIndex++;

	//确保指针一直在有效范围内，写指针越界时，重新从0开始写，并且设置越界标志
	if (g_ucWrIndex == NMAX)
	{
		g_ucWrIndex = 0;
		g_ucOverstep = 1;
	}

	//FIFO已满，又写入一个数据，则最早写入的数据要被冲掉
	//所以读指针要保持与写指针相等
	if (g_ucFull)
	{
		g_ucRdIndex = g_ucWrIndex;
	}

	//如果写指针越界，且与读指针相等，则说明写满
	if (g_ucOverstep & (g_ucWrIndex == g_ucRdIndex))
	{
		g_ucFull = 1;
	}
	else
	{
		g_ucFull = 0;
	}

	//写入一个数据，则缓冲区就一定是不空的
	g_ucEmpty = 0;

	return 0;
}

//从缓冲区中取出1字节数据
//当缓冲区中最后一个数据被取出的时候，置位 g_ucEmpty 标志，表示缓冲区空
//缓冲区已空，又进行读数据操作，则读出的数据是无效的
//返回值为读出的数据
u8 FIFO_DataOut(void)
{
	u8 ucData;

	//g_ucRdIndex &= (NMAX - 1);
	ucData = g_ucBuf[g_ucRdIndex];
	g_ucRdIndex++;

	//确保指针一直在有效范围内，读指针越界时，重新从0开始读，并且清除越界标志
	if (g_ucRdIndex == NMAX)
	{
		g_ucRdIndex = 0;
		g_ucOverstep = 0;
	}

	//FIFO已空，又读一个数据，则读出的数据是无效的
	//并且写指针要跟着读指针变化，否则下次写入的数据位置是不正确的
	if (g_ucEmpty)
	{
		g_ucWrIndex = g_ucRdIndex;
	}

	//若越界标志为0，且读写指针相等，则说明已经读空了
	if ((!g_ucOverstep) & (g_ucWrIndex == g_ucRdIndex))
	{
		g_ucEmpty = 1;
	}
	else
	{
		g_ucEmpty = 0;
	}

	//读出一个数据，则缓冲区就一定是不满的
	g_ucFull = 0;

	return ucData;
}




//获取缓冲区中数据个数
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


