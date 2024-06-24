#ifndef __LPUART_H__
#define __LPUART_H__

#define  LPUARTMAX  255

typedef struct 
{	
	unsigned char TxBuf[LPUARTMAX];	//发送数据指针
	unsigned char TxLen;	//待发送数据长度
	unsigned char TxOpc;	//已发送数据长度	
	unsigned char RxBuf[LPUARTMAX];	//接收数据指针
	unsigned char RxLen;	//待接收数据长度
	unsigned char RxOpc;	//已接收数据长度
	unsigned char RxData;
}LPUARTOpTypeDef;

#endif

