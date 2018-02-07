#ifndef _DOWNLOAD_H
#define _DOWNLOAD_H

void DownLoad_SendByte(USART_FUNCTION uf,u8 ChDa);
void DownLoad_SendHead(USART_FUNCTION uf,u8 Addr,u8 cmd);
void DownLoad_SendChksum(USART_FUNCTION uf);
void DownLoad_SendTail(USART_FUNCTION uf);
void DownLoad_DealProtocol(USART_FUNCTION uf,u8 *DaPtr,u16 len);

#endif


