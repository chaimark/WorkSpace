#include "Define.h"

extern unsigned char Sending_RF0;  
extern unsigned char Sending_RF1;
extern unsigned char LoRa_work_channel; 

extern unsigned char RF_RXBuffer[50];  
extern unsigned char RF_TXBuffer[30]; 
extern unsigned char METERID[2];
extern void SX1276_RF_SENDPACKET(unsigned char *RF_TRAN_P,unsigned char LEN);
extern void DelayMs(uint32_t count);

void Responed_RF0_DATA(void)
{
  unsigned char i,Sum = 0;
	Sending_RF0=1;
  RF_TXBuffer[0] = METERID[0];
  RF_TXBuffer[1] = METERID[1];
	RF_TXBuffer[2] = 0xA1;
	RF_TXBuffer[3] = 0x01;
	RF_TXBuffer[4] = 0x02;
	RF_TXBuffer[5] = 0x03;
	RF_TXBuffer[6] = 0x04;
	RF_TXBuffer[7] = 0x05;
  for(i=0;i<8;i++){
    Sum = Sum + RF_TXBuffer[i];
  }
  RF_TXBuffer[8] = Sum;
  SX1276_RF_SENDPACKET(RF_TXBuffer,9);
	Sending_RF0=0;
}

void Responed_RF1_DATA(void)
{
  unsigned char i,Sum = 0;
	Sending_RF1=1;
  RF_TXBuffer[0] = METERID[0];
  RF_TXBuffer[1] = METERID[1];
	RF_TXBuffer[2] = 0xA2;
	RF_TXBuffer[3] = 0x01;
	RF_TXBuffer[4] = 0x02;
	RF_TXBuffer[5] = 0x03;
	RF_TXBuffer[6] = 0x04;
	RF_TXBuffer[7] = 0x05;
  for(i=0;i<8;i++){
    Sum = Sum + RF_TXBuffer[i];
  }
  RF_TXBuffer[8] = Sum;
  SX1276_RF_SENDPACKET_2(RF_TXBuffer,9);
	Sending_RF1=0;
}
