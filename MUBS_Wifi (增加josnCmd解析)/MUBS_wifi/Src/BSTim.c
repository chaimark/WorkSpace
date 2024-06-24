#include "main.h"

void BSTIM_Init(uint16_t ms)
{
	LL_BSTIM_InitTypeDef   InitStructer;
	InitStructer.Prescaler         = 799;
	InitStructer.Autoreload        = ms*10-1;    
	InitStructer.AutoreloadState   = ENABLE;
	InitStructer.ClockSource       = LL_RCC_BSTIM_OPERATION_CLK_SOURCE_APBCLK2;
	LL_BSTIM_Init(BSTIM,&InitStructer);
	NVIC_DisableIRQ(BSTIM_IRQn);
	NVIC_SetPriority(BSTIM_IRQn, 2);
	NVIC_EnableIRQ(BSTIM_IRQn);
	LL_BSTIM_ClearFlag_UpdataEvent(BSTIM);
	LL_BSTIM_EnabledIT_UpdataEvent(BSTIM);
	LL_BSTIM_EnableCounter(BSTIM);
}

void Disable_BSTIM(void)
{
	LL_BSTIM_DisableIT_UpdataEvent(BSTIM);
	LL_BSTIM_DisableCounter(BSTIM);
}

void BSTIM_IRQHandler(void)
{
	if(( LL_BSTIM_IsEnabledIT_UpdataEvent(BSTIM) == SET ) &&(LL_BSTIM_IsActiveFlag_UpdataEvent(BSTIM)==SET)){	 
		LL_BSTIM_ClearFlag_UpdataEvent(BSTIM);   
//		Disable_BSTIM();			      //停止定时器   
    wifi_timer_add();		
	}			
}
