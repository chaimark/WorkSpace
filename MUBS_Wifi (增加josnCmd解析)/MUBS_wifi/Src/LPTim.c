#include "main.h"
#include <stdlib.h>

extern unsigned int Dog_In_Main_Count;

extern void IWDT_Clr(void);

void LPTIM_IRQHandler(void)
{
	if((LL_LPTIM_IsEnabled_CounterOver(LPTIM) == ENABLE) &&(LL_LPTIM_IsActiveFlag_CounterOver(LPTIM) == SET)){
		LL_LPTIM_ClearFlag_CounterOver(LPTIM);       //清除标志 
    
    /*****************************************************/
		Dog_In_Main_Count++;
		if(Dog_In_Main_Count<1920){
			/* 喂狗 */    
			IWDT_Clr();	
		}else{
      NVIC_SystemReset();	
		}
		/*****************************************************/
	}	
}
