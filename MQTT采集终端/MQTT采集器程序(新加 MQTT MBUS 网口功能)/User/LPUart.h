#ifndef __LPUarT_H
#define __LPUarT_H

#define  LPUARTMAX  250
typedef struct {
	unsigned char TxBuf[LPUARTMAX];
	unsigned char TxLen;
	unsigned char TxOpc;
	unsigned char RxBuf[LPUARTMAX];
	unsigned char RxLen;
	unsigned char RxOpc;
	unsigned char Rxdata;
}LPUARTOpTypeDef;

extern unsigned char CH340_LPUART1_RX_Count_Above30;

extern LPUARTOpTypeDef LPUART0Ddata, LPUART1Ddata;
extern unsigned char Cault_CS(unsigned char* Buffer, unsigned char start_num, unsigned char stop_num);
extern void LPUart0Send(unsigned char* txBuffer, unsigned short int length);
extern void LPUart1Send(unsigned char* txBuffer, unsigned short int length);
#endif 

