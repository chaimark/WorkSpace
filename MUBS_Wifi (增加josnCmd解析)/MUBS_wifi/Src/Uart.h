#ifndef __Uart_H__
#define __Uart_H__

#include "main.h"
#include <stdbool.h>
#include <stdint.h>

#define  UARTMAX   1024

typedef struct{
	unsigned char TxBuf[UARTMAX];
	uint16_t TxLen;
	unsigned char TxOpc;
	unsigned char RxBuf[UARTMAX];
	uint16_t RxLen;
	unsigned char RxOpc;
}UARTOpStruct;


extern bool copyDataForUART(void);

#endif
