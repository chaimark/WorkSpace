#include "main.h"
#include "Define.h"
#include "LPUart.h"


extern unsigned char LPUART0_Send_In_Main_Flage;  
extern unsigned char Responed_RF0_TX_In_Main_Flage;  
extern unsigned char Responed_RF1_TX_In_Main_Flage;
extern unsigned char EEprom_Check_In_Main_Flage;

extern unsigned char Sending_RF0;  
extern unsigned char Sending_RF1;

extern unsigned int Dog_In_RTCMin_Flage;
extern unsigned int Dog_In_RTCMin_Count;
extern unsigned int Dog_In_Main_Count;
extern unsigned char LoRa_work_channel; 

extern LPUARTOpTypeDef LPUART0Op;
extern unsigned char EEprom_Byte1Read(unsigned char addr);
extern unsigned char EEprom_Byte1Write(unsigned char addr,unsigned char data);
extern void EEprom_Byte2Write(unsigned char addr,unsigned int data);
extern unsigned int EEprom_Byte2Read(unsigned char addr);
extern void UserInit(void);
extern void Responed_RF0_DATA(void);
extern void Responed_RF1_DATA(void);
extern void LPUART0_Send(unsigned char *txBuffer,unsigned short int length);
extern void DeepSleep(void);

extern void wifi_init(void);


int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	/* SHOULD BE KEPT!!! */
	MF_Clock_Init();    
	/* Configure the system clock */
	/* SHOULD BE KEPT!!! */
	MF_SystemClock_Config();    
	/* Initialize all configured peripherals */
	/* SHOULD BE KEPT!!! */
	MF_Config_Init();    
	UserInit();

	wifi_init();	
	while(1){		
		IWDT_Clr();
		wifi_task();	
	}
}


