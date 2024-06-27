#ifndef __Uart_H__
#define __Uart_H__

#include "main.h"

#define  UARTMAX   1024

typedef struct{
	uint8_t TxBuf[UARTMAX];	//发送数据指针
	uint16_t TxLen;	//待发送数据长度
	uint16_t TxOpc;	//已发送数据长度	
	uint8_t RxBuf[UARTMAX];	//接收数据指针
	uint16_t RxLen;	//待接收数据长度
	uint16_t RxOpc;	//已接收数据长度
}UARTOpStruct;
extern void copyDataForUART(void);

#endif
