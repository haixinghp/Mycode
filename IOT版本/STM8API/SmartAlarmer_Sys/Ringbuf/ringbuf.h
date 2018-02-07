#ifndef RING_BUF_H
#define RING_BUF_H


#define NMAX 64					//缓冲区大小，根据实际需要设置，但不能大于65535


//以下是本函数中用到的几个数据类型的宏定义
/*#define u8  unsigned char
#define u16 unsigned short
#define u32 unsigned int*/


extern u8  g_ucFull;			//FIFO满标志
extern u8  g_ucEmpty;			//FIFO空标志


u8 FIFO_DataIn(u8 data);		//数据写入FIFO
u8 FIFO_DataOut(void);			//从FIFO中读出数据
u16 FIFO_GetNum(void);			//获得FIFO中数据个数



#endif 
