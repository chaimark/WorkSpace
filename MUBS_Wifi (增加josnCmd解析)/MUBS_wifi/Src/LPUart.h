#ifndef __LPUART_H__
#define __LPUART_H__

#define  LPUARTMAX  255

typedef struct 
{	
	unsigned char TxBuf[LPUARTMAX];	//��������ָ��
	unsigned char TxLen;	//���������ݳ���
	unsigned char TxOpc;	//�ѷ������ݳ���	
	unsigned char RxBuf[LPUARTMAX];	//��������ָ��
	unsigned char RxLen;	//���������ݳ���
	unsigned char RxOpc;	//�ѽ������ݳ���
	unsigned char RxData;
}LPUARTOpTypeDef;

#endif

