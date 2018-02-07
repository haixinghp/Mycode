#include "config.h"

#include "AtCmdAnalysis.h"
#if CONF_WCDMA_EN

typedef int (*NetdataProtocolAnlysisFun)(USART_FUNCTION uf,uchar ChrData,BOOL isStart);  // ����һ��ȫ����������
typedef int (*NetdataProtocolEndFun)(USART_FUNCTION uf);  // ����һ��ȫ����������

typedef struct
{
	PROTOCOL_ANALYSIS_TYPE type;		 //Э�������
	uchar cmdNum;						//��Ӧ��ָ����
	NetdataProtocolAnlysisFun analysisfun;		//�ڽ������������еĺ���
	NetdataProtocolEndFun endFun;		//���������еĺ���
}tagNetdataProtocolTypeToFun;

#define NETDATA_ANLYSIS_NAME(fun) 	  Netdata_NetdataProtocolAnlysisFun_##fun
#define NETDATA_ANLYSIS_FUN(fun)	static int NETDATA_ANLYSIS_NAME(fun)##(USART_FUNCTION uf,uchar ChrData,BOOL isStart)

static PROTOCOL_ANALYSIS_TYPE 	 Netdata_PAType_Analysis(uchar c);

static int Netdata_NetdataProtocolAnlysisFun_SendToDevice(USART_FUNCTION uf,uchar ChrData,BOOL isStart);

static int Netdata_NetdataProtocolAnlysisFun_WcdmaUpdate(USART_FUNCTION uf,uchar ChrData,BOOL isStart);	//�������ݽ���

static int Netdata_NetdataProtocolAnlysisFun_WcdmaCtrl(USART_FUNCTION uf,uchar ChrData,BOOL isStart);	//�������ݽ���

static int Netdata_NetdataProtocolEndFun_SendToDevice(USART_FUNCTION uf);

static int Netdata_NetdataProtocolEndFun_WcdmaUpdate(USART_FUNCTION uf); //���ͽ�����ʼ���³���

static int Netdata_NetdataProtocolEndFun_WcdmaCtrl(USART_FUNCTION uf); //���ͽ�����ʼ���³���

const static tagNetdataProtocolTypeToFun _gNetdataProtocolTypeToFunArray[PROTOCOL_ANALYSIS_TYPE_MAX] =
{
	{PROTOCOL_ANALYSIS_TYPE_DEVICE_BEAT,0x10,NULL,NULL},
	{PROTOCOL_ANALYSIS_TYPE_CENTER_REPLY,0x20,NULL,NULL},
	{PROTOCOL_ANALYSIS_TYPE_BEATE_TEST,0x30,NULL,NULL},
	{PROTOCOL_ANALYSIS_TYPE_SEND_TO_DEVICE,0x50,Netdata_NetdataProtocolAnlysisFun_SendToDevice,Netdata_NetdataProtocolEndFun_SendToDevice},
	{PROTOCOL_ANALYSIS_TYPE_WCDMA_PROGRAM,0x52,Netdata_NetdataProtocolAnlysisFun_WcdmaUpdate,Netdata_NetdataProtocolEndFun_WcdmaUpdate},	//WCDMA�����������
	{PROTOCOL_ANALYSIS_TYPE_WCDMA_CTRL,0x54,Netdata_NetdataProtocolAnlysisFun_WcdmaCtrl,Netdata_NetdataProtocolEndFun_WcdmaCtrl}, //WCDMA����
	{PROTOCOL_ANALYSIS_TYPE_SEND_TO_CENTER,0x60,NULL,NULL},
	{PROTOCOL_ANALYSIS_TYPE_WCDMA_PROGRAM_REPLY,0x62,NULL,NULL},
	{PROTOCOL_ANALYSIS_TYPE_WCDMA_CTRL_REPLY,0x64,NULL,NULL},
	{PROTOCOL_ANALYSIS_TYPE_UNKNOWN,0xFE,NULL,NULL}
};

static char _gRecvBuffer[MOUNT_DEVICE_COUNT][MAX_RECEIVE_BUFFER] = {0};	//���ջ���,����WCDMA�������
static tagProtocolAnalysis _gProtocolAnalysis[MOUNT_DEVICE_COUNT] = {0};

static tagSystemSetValStruct* _gSystemSetValPtr;
#define _gSystemSetVal	(*_gSystemSetValPtr)

static int Netdata_NetdataProtocolAnlysisFun_SendToDevice(USART_FUNCTION uf,uchar ChrData,BOOL isStart)
{
	ProtocolResource * Protocol =  GetProtocolResource(uf);

	if(Protocol == NULL) {
		return -1;
	}
	if(isStart)
	{
		Reset_Protocol_Tx(Protocol);
	}

	/*Э��δ����*/
	if((*Protocol).TxPro_Lenth<Protocol->size)
	{
		(*Protocol).TxMessageBuffer[(*Protocol).TxPro_Lenth]=ChrData;
		(*Protocol).TxPro_Lenth++;

		return 0;
	}

	//DEBUG("full memrey:size:%d\n",Protocol->size);
	return -1;

}



static int Netdata_NetdataProtocolAnlysisFun_WcdmaUpdate(USART_FUNCTION uf,uchar ChrData,BOOL isStart)
{
	tagProtocolAnalysis* pProAns = NULL;
	if(uf < USART_FUNCTION_MOUNT && uf >= USART_FUNCTION_MOUNT + MOUNT_DEVICE_COUNT)
		return -1;

	pProAns =  &_gProtocolAnalysis[uf- USART_FUNCTION_MOUNT];

	if(isStart)
	{
		pProAns->recvSize = 0;	//��ʼ�׶Σ����ݳ�������		
	}

	pProAns->recvBuffer[pProAns->recvSize] = ChrData;
	pProAns->recvSize++;

	return 0;
}

static int Netdata_NetdataProtocolAnlysisFun_WcdmaCtrl(USART_FUNCTION uf,uchar ChrData,BOOL isStart)
{
	tagProtocolAnalysis* pProAns = NULL;
	if(uf < USART_FUNCTION_MOUNT && uf >= USART_FUNCTION_MOUNT + MOUNT_DEVICE_COUNT)
		return -1;

	pProAns =  &_gProtocolAnalysis[uf- USART_FUNCTION_MOUNT];

	if(isStart)
	{
		pProAns->recvSize = 0;	//��ʼ�׶Σ����ݳ�������		
	}

	pProAns->recvBuffer[pProAns->recvSize] = ChrData;
	pProAns->recvSize++;

	return 0;
}

static int Netdata_NetdataProtocolEndFun_WcdmaCtrl(USART_FUNCTION uf)
{

	return AtCmdAnalysis_WCDMA_StartWcdmaCtrl(uf);
}


static int Netdata_NetdataProtocolEndFun_SendToDevice(USART_FUNCTION uf)
{
	return AtCmdAnalysis_UartFuntion_StartSend(uf);
}

static int Netdata_NetdataProtocolEndFun_WcdmaUpdate(USART_FUNCTION uf) //���ͽ�����ʼ���³���
{

	return AtCmdAnalysis_WCDMA_StartUpadate(uf);
}

static PROTOCOL_ANALYSIS_TYPE  Netdata_PAType_Analysis(uchar c)
{
	int i = 0;
	for(i = 0;i < PROTOCOL_ANALYSIS_TYPE_MAX; i++)
	{
		if(_gNetdataProtocolTypeToFunArray[i].cmdNum == c)
		{
			return (PROTOCOL_ANALYSIS_TYPE)i;
		}
	}
	
	return PROTOCOL_ANALYSIS_TYPE_UNKNOWN;

	
}

static BOOL Netdata_CheckSum_Check(uint16_t sum, char* tail)
{	
	uint16_t CS=0,S = 0;

	CS = tail[0];
	CS <<= 8;
	CS += tail[1];

	S = CS + sum;

	if(S==0)
		return true;

	return false;

}

PROTOCOL_READ_POS  Netdata_Protocol_GetPostion(WORD deviceID)
{
	PROTOCOL_READ_POS posType = PROTOCOL_READ_POS_UNKNOWN;

	if(deviceID < MOUNT_DEVICE_COUNT)
	{
		tagProtocolAnalysis* pa = NULL;
		pa = &_gProtocolAnalysis[deviceID];
		if(pa->dataSizeSend == pa->dataSizeSendConst)
		{
			posType = PROTOCOL_READ_POS_START;

		}
		else if(pa->dataSizeSend > 0 && pa->dataSizeSend < pa->dataSizeSendConst)
		{
			posType = PROTOCOL_READ_POS_CENTER;			
		} else if(pa->dataSizeSend == 0 )
		{
			posType = PROTOCOL_READ_POS_END;
		}
	}


	return 	posType;
}

uint16_t Netdata_Protocol_ReadAddressAndSize(WORD deviceID,PROTOCOL_READ_POS pos, char** buf,uint16_t* size,uint16_t sizeMax)
{
	if(deviceID < MOUNT_DEVICE_COUNT)
	{
		tagProtocolAnalysis* pa = NULL;

		pa = &_gProtocolAnalysis[deviceID];
		switch(pos)
		{
			case PROTOCOL_READ_POS_START:
				*buf = pa->headerSender;
				*size = MAX_PROTOCOL_HEADER_SIZE;
				break;
			case PROTOCOL_READ_POS_CENTER:
			{
				ProtocolResource * Protocol =  GetProtocolResource((USART_FUNCTION)(USART_FUNCTION_MOUNT + deviceID));
				if(Protocol != NULL)
				{
					*size = pa->dataSizeSend >sizeMax ? sizeMax:pa->dataSizeSend; 
					*buf = (char*)&Protocol->PRxDaBuf[Protocol->RxPro_Lenth- pa->dataSizeSend];
					pa->dataSizeSend -= (*size);
				}
			}
				break;
			case PROTOCOL_READ_POS_END:
				*buf = pa->tailSender;
				*size = MAX_PROTOCOL_TAIL_SIZE;
				break;
		
		}

		return 	*size;
	}
	return 0;
}

uint16_t Netdata_Protocol_ReadSize(WORD deviceID,uint16_t size)
{
	if(deviceID < MOUNT_DEVICE_COUNT)
	{
		uint16_t retSize = 0;
		tagProtocolAnalysis* pa = NULL;

		pa = &_gProtocolAnalysis[deviceID];
		retSize = pa->dataSizeSend >size ? size:pa->dataSizeSend;
		return 	retSize;
	}
	return 0;
}	

/* �豸��Ϣ�Ķ�ȡ */
uint16_t Netdata_Protocol_Read(WORD deviceID,char* data, uint16_t size)
{
	if(data	!= NULL && deviceID < MOUNT_DEVICE_COUNT)
	{
		uint16_t retSize = 0;
		tagProtocolAnalysis* pa = NULL;
		ProtocolResource * Protocol =  GetProtocolResource((USART_FUNCTION)(USART_FUNCTION_MOUNT + deviceID));

		if(Protocol == NULL) {
			return 0;
		}

		pa = &_gProtocolAnalysis[deviceID];
		retSize = pa->dataSizeSend >size ? size:pa->dataSizeSend;
		memcpy(data,&Protocol->PRxDaBuf[Protocol->RxPro_Lenth- pa->dataSizeSend],retSize);
		pa->dataSizeSend -= retSize;
		return 	retSize;
	}
	return 0;
}

int Netdata_ProtocolAnalysis_Packet(tagProtocolAnalysis* pa,PROTOCOL_ANALYSIS_TYPE paType ,char* data,uint16_t size)
{
	uint16_t checkSum = 0;
	int i = 1;
	if(pa == NULL)
		return -1;

	memcpy(pa->headerSender,pa->header,MAX_PROTOCOL_HEADER_SIZE);
	memset(pa->tailSender,NULL,MAX_PROTOCOL_TAIL_SIZE);
	pa->headerSender[0] = 0xF0;
	pa->headerSender[1] = 0x02;
	pa->headerSender[2] = _gNetdataProtocolTypeToFunArray[paType].cmdNum;
	pa->headerSender[MAX_PROTOCOL_HEADER_SIZE-2] = size>>8;
	pa->headerSender[MAX_PROTOCOL_HEADER_SIZE-1] = (size <<8) >>8;	//���ȼ���

	while(i< MAX_PROTOCOL_HEADER_SIZE)
	{
		checkSum += pa->headerSender[i];
		i++;
	}

	i = 0;

	while(i< size)
	{
		checkSum += data[i];
		i++;
	}

	pa->dataSizeSend = size;
	pa->dataSizeSendConst = size;

	checkSum =(~checkSum+1);
	 
	pa->tailSender[0] = (checkSum >> 8);
	pa->tailSender[1]	= ((checkSum<<8)>>8);
	pa->tailSender[2] = 0x0d;	//ĩβ
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : Netdata_Protocol_Packet
// ��������  : �������ݴ������
// �������  : deviceID �豸ID,paType Э���������, data ��Ҫ���������, size ��Ҫ��������ݵĴ�С
// �������  : None
// ���ز���  : �ɹ� 0;ʧ��-1
//������������������������������������������������������������������������������
int Netdata_Protocol_Packet(WORD deviceID,PROTOCOL_ANALYSIS_TYPE paType ,char* data,uint16_t size)
{
	tagProtocolAnalysis* pa = NULL;

	if(deviceID >=  MOUNT_DEVICE_COUNT && paType >= PROTOCOL_ANALYSIS_TYPE_MAX)
		return -1;
	pa = &_gProtocolAnalysis[deviceID];

	return 	Netdata_ProtocolAnalysis_Packet(pa,paType,data,size);
}
void Netdata_ReStart(void)
{
	_gProtocolAnalysis[1].headerCount = 0;
}

//������������������������������������������������������������������������������
// ��������  : Netdata_Protocol_Anlysis
// ��������  : ����Э�����ݽ��ս�������
// �������  : uf ����ģ��,ChrData ���յ�һ���ֽ�.
// �������  : None
// ���ز���  : �ɹ� 0;ʧ��-1
//������������������������������������������������������������������������������
int Netdata_Protocol_Anlysis(USART_FUNCTION uf,uchar ChrData)  // ����һ��ȫ����������
{	

	tagProtocolAnalysis* pa = &_gProtocolAnalysis[(uf - USART_FUNCTION_MOUNT)];

   	if(ChrData==0xf0 && pa->headerCount == 0)  	//��ȡ��ͷ����ʽ��ʼ
	{
		pa->headerCount = 0;
		pa->tailCount = 0;
		pa->checkSum = 0;
		pa->header[pa->headerCount] = ChrData;
		pa->headerCount++;
	//	API_SetTimer(5000L,1,TIMER_TYPE_WCDMA_PROS,NULL,Netdata_ReStart);
		API_SetTimerFast(3000L,1,TIMER_TYPE_WCDMA_PROS,NULL,Netdata_ReStart);
		return 0;
	}
	if(pa->headerCount > 0)	  //ͷȡ����
	{
		if(pa->headerCount < MAX_PROTOCOL_HEADER_SIZE)
		{
			pa->header[pa->headerCount] = ChrData;
			//��ȡЭ��ͷ�׶�
			pa->headerCount++;
			pa->checkSum += ChrData;
			if(pa->headerCount == MAX_PROTOCOL_HEADER_SIZE)
			{
				ProtocolResource * pRes = NULL;
				DWORD maxSize = 0;
				//��ȡЭ������
				pa->paType = Netdata_PAType_Analysis(pa->header[2]);
				//��ȡ���ݳ���
				pa->dataSizeConst = (int16)((pa->header[13])<<8) + pa->header[14];
				pa->dataSize = 	pa->dataSizeConst;
				pRes = GetProtocolResource(uf);
				if(pRes != NULL)
				{
					maxSize = pRes->size;
				}
				//Э��ͷ��ȡ��ϣ���ʼ���н���
				if(pa->dataSize > maxSize)
					goto End_Receive_Data;
			}
		}
		else
		{

			if(pa->dataSize > 0)
			{
				
				const tagNetdataProtocolTypeToFun* ptr =  &_gNetdataProtocolTypeToFunArray[pa->paType];
				pa->checkSum += ChrData;
				if(ptr->analysisfun != NULL)
				{
					ptr->analysisfun(uf,ChrData,pa->dataSize ==	pa->dataSizeConst);							
				}

				pa->dataSize--;
				//�������û�н������
				//��������
			}
			else
			{
				if(pa->tailCount < MAX_PROTOCOL_TAIL_SIZE)
				{
					pa->tail[pa->tailCount] = ChrData;
					pa->tailCount++;
				}
				if(pa->tailCount == MAX_PROTOCOL_TAIL_SIZE)
				{
					//���ݷ��������
					if(pa->tail[MAX_PROTOCOL_TAIL_SIZE-1] == 0x0d)
					{
						const tagNetdataProtocolTypeToFun* ptr = & _gNetdataProtocolTypeToFunArray[pa->paType];

						//У���
						if(ptr->endFun != NULL)
						{
							if(Netdata_CheckSum_Check(pa->checkSum,pa->tail))
							{
								ptr->endFun(uf);
							}
						}


						//�����Ƿ���ȷ����Ҫ���ͷſռ�������ݽ���						
					}
					goto End_Receive_Data;
				}
			}		
		}	
	}

	return 0;
End_Receive_Data:
	if(_gSystemSetVal.sysMountMapStatus == SYSTEM_MOUNT_MAP_STATUS_ON
		&& _gSystemSetVal.sysSetValCtrl.mapDeviceID == (uf - USART_FUNCTION_MOUNT))
	{
		API_SetTimerFast(_gSystemSetVal.sysSetValCtrl.timeOut*1000,1,TIMER_TYPE_WCDMA_SYSTEM_MAP,TIMER_NAME(SystemMap),NULL);
	}
//	if(API_SetTimer(sys->sysSetValCtrl.timeOut * 1000,1,TIMER_TYPE_WCDMA_SYSTEM_MAP,TIMER_NAME(SystemMap),NULL);
//		)
	pa->headerCount = 0;
	//API_KillTimer(TIMER_TYPE_WCDMA_PROS);
//	DEBUG("End==>********");
	API_KillTimerFast(TIMER_TYPE_WCDMA_PROS);
	return -1;	//�����Ƿ���ȷ����Ҫ���ͷſռ�������ݽ���
	
}

typedef __packed struct
{
	WORD type;
	WORD length;
}tagSystemCtrlHead;

typedef __packed struct
{
	WORD type;
	WORD length;
	char result;
}tagSystemCtrlHeadReply;  //��Ӧ��ֵ
int Netdata_System_Ctrl(WORD deviceID)
{
	tagProtocolAnalysis* pa = NULL;
//	tagProtocolAnalysis* packet = NULL;


	if(deviceID < MOUNT_DEVICE_COUNT)
	{
		pa = &_gProtocolAnalysis[deviceID];
			
	   	WirelessDeal(deviceID,pa->recvBuffer,pa->recvSize);

//		tagSystemCtrlHead* pHead = NULL; 
//		pa = &_gProtocolAnalysis[deviceID];
//
//		if(pa->recvSize >= sizeof(tagSystemCtrlHead))
//		{
//			char reply[230] = {0}; 	//���ص����ֵ���ܳ���255��
//			tagSystemCtrlHeadReply systemCtrlHeadSend = {0};
//			int replyLength = 0;
//			systemCtrlHeadSend.result = 1;
//			pHead = (tagSystemCtrlHead*)pa->recvBuffer;
//			systemCtrlHeadSend.type = pHead->type;
//			InvertedOrder((uchar*)&(pHead->type),2);
//			InvertedOrder((uchar*)&(pHead->length),2);
//			replyLength= SystemCtrl_Process(deviceID,pHead->type,pa->recvBuffer + sizeof(tagSystemCtrlHead),pHead->length,reply + sizeof(tagSystemCtrlHeadReply));
//			if(replyLength > 230 || replyLength < 0)
//			{
//				replyLength = 1;
//				systemCtrlHeadSend.result = 0;
//			}
//			systemCtrlHeadSend.length = replyLength +1;
//
//			//���Ⱥ���������ȫ���������
//			replyLength = systemCtrlHeadSend.length + sizeof(tagSystemCtrlHeadReply) -1;
//
//			InvertedOrder((uchar*)&(systemCtrlHeadSend.length),2);
//
//			memcpy(reply,&systemCtrlHeadSend ,sizeof(tagSystemCtrlHeadReply));
//
//			if(Netdata_Protocol_Packet(deviceID,PROTOCOL_ANALYSIS_TYPE_WCDMA_CTRL_REPLY ,reply,replyLength) > -1)
//			{
//				char buf[255] = {0};
//				char* pBuf = buf;
//				int len = 0;
//				int i = 0;
//				packet = &_gProtocolAnalysis[deviceID];
//				memcpy(pBuf,packet->headerSender,MAX_PROTOCOL_HEADER_SIZE);
//				pBuf += MAX_PROTOCOL_HEADER_SIZE;
//				len += 	MAX_PROTOCOL_HEADER_SIZE;
//				memcpy(pBuf,reply,replyLength);
//				pBuf +=  replyLength;
//				len += 	replyLength;
//				memcpy(pBuf,packet->tailSender,MAX_PROTOCOL_TAIL_SIZE);
//				len += MAX_PROTOCOL_TAIL_SIZE;
//				AtCmdAnalysis_Net_InfoSend(deviceID,buf,len);
//				DEBUG("Send Buffer:\n");
//				for(i = 0; i < len; i++)
//				{
//					DEBUG("%02x ",buf[i]);
//				}
//				return 0;
//			} 
//		}
		
	}

	return -1;		
}

typedef __packed struct
{
	char type;
	WORD length;
}tagWCDMAUpdataHead;



//������
int Netdata_Program_Update(WORD deviceID)
{
	tagProtocolAnalysis* pa = NULL;
	tagProtocolAnalysis* packet = NULL;


	if(deviceID < MOUNT_DEVICE_COUNT)
	{

		tagWCDMAUpdataHead* pHead = NULL; 
		pa = &_gProtocolAnalysis[deviceID];
	//	DEBUG("%s,%d\n",__FUNCTION__,__LINE__);
		if(pa->recvSize >= sizeof(tagWCDMAUpdataHead))
		{
			char reply[50] = {0};
			DWORD dataLength = 0;
			int ret = -1;
			int replyLength = 0;

			pHead = (tagWCDMAUpdataHead*)pa->recvBuffer;
			DEBUG("Recevie type:%d\n",pHead->type);
			switch(pHead->type)
			{
				case 1:
					dataLength = API_WCDMA_Update_Start(pHead->length < 8,pa->recvBuffer + sizeof(tagWCDMAUpdataHead),8);						
					ret = 0;
					break;
				case 2:
					InvertedOrder((uchar*)&(pHead->length),2);
					if(pa->header[4] != pa->headerSender[4] || pa->header[4] == 0) 
					{
						
					//	DEBUG("\n==>length:%d\n",pHead->length);
						DEBUG("#%d\n",pa->header[4]);
						ret = API_WCDMA_Update_Write(pa->recvBuffer + sizeof(tagWCDMAUpdataHead),pHead->length);
						
					//	DEBUG("\n==>write buffer End!\n");
					}
					else
					{
						ret = 0;
					}
					break;
				case 3:
				//	DEBUG("%s,%d\n",__FUNCTION__,__LINE__);
					ret = API_WCDMA_Update_End();	

					break;
			}
			if(pHead->type == 1)
			{
				sprintf(reply,"%c%c%c%c%c%c",
						ret > -1?1:0,
						pHead->type,
						((char*)&dataLength)[3],
						((char*)&dataLength)[2],
						((char*)&dataLength)[1],
						((char*)&dataLength)[0]);
				replyLength = 6;
			}
			else
			{
				sprintf(reply,"%c%c",ret > -1?1:0,pHead->type);
				replyLength = 2;
			}
			if(Netdata_Protocol_Packet(deviceID,PROTOCOL_ANALYSIS_TYPE_WCDMA_PROGRAM_REPLY ,reply,replyLength) > -1)
			{
				char buf[100] = {0};
				char* pBuf = buf;
				int len = 0;
				int i = 0;
				packet = &_gProtocolAnalysis[deviceID];
				memcpy(pBuf,packet->headerSender,MAX_PROTOCOL_HEADER_SIZE);
				pBuf += MAX_PROTOCOL_HEADER_SIZE;
				len += 	MAX_PROTOCOL_HEADER_SIZE;
				memcpy(pBuf,reply,replyLength);
				pBuf +=  replyLength;
				len += 	replyLength;
				memcpy(pBuf,packet->tailSender,MAX_PROTOCOL_TAIL_SIZE);
				len += MAX_PROTOCOL_TAIL_SIZE;
				AtCmdAnalysis_Net_InfoSend(deviceID,buf,len);
				DEBUG("Send Buffer:\n");
				for(i = 0; i < len; i++)
				{
					DEBUG("%02x ",buf[i]);
				}
			//	DEBUG("\n");
			}
		//	DEBUG("Recevie type:%d == >end\n",pHead->type);


		}
		
	}

	return -1;			
}
							   
int Netdata_Init(void)
{
	int i = 0;
	_gSystemSetValPtr = AtCmdAnalysis_SystemSetValCtrl_Get();
	for(i = 0; i < MOUNT_DEVICE_COUNT ; i++)
	{
		_gProtocolAnalysis[i].recvBuffer = 	_gRecvBuffer[i];
	}

	return 0; 	
}
#endif
//������������������������������������������������������������������������������
//                          �������(END)
//������������������������������������������������������������������������������


