#include "main.h" 

unsigned char METERID[2]={0x12,0x34};

unsigned char LPUART0_Send_In_Main_Flage=0;  
unsigned char Responed_RF0_TX_In_Main_Flage=0;  
unsigned char Responed_RF1_TX_In_Main_Flage=0; 
unsigned char EEprom_Check_In_Main_Flage=0;

unsigned int Check_EEprom_1Second_Count=0;
unsigned char Sending_RF0=0;  
unsigned char Sending_RF1=0;

unsigned int Dog_In_RTCMin_Save=0;
unsigned int Dog_In_RTCMin_Flage=0;
unsigned int Dog_In_RTCMin_Count=0;
unsigned int Dog_In_Main_Count=0;

void EEprom_SDA_SET_IN(void)  
{	
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.OutputType =LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void EEprom_SDA_SET_OUT(void)
{		
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType =LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_0);
}
