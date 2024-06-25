#ifndef __Uart_H__
#define __Uart_H__

#include "main.h"

#define  UARTMAX   1024

typedef struct{
	uint8_t TxBuf[UARTMAX];	//��������ָ��
	uint16_t TxLen;	//���������ݳ���
	uint16_t TxOpc;	//�ѷ������ݳ���	
	uint8_t RxBuf[UARTMAX];	//��������ָ��
	uint16_t RxLen;	//���������ݳ���
	uint16_t RxOpc;	//�ѽ������ݳ���
}UARTOpStruct;
extern void copyDataForUART(void);

#endif
