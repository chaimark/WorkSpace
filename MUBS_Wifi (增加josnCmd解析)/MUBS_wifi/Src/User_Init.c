#include "User_Init.h"

uint32_t systemClock;
void DelayUs(uint32_t count);
void DelayMs(uint32_t count);

static void ClockInit(void)
{
	uint32_t trim;
	switch (RCHF_CLOCK){
		case LL_RCC_RCHF_FREQUENCY_8MHZ:
				systemClock = 8000000;
				LL_RCC_SetRCHFFrequency(RCHF_CLOCK);
				trim = NVR_CHECK(RCHF8M_NVR_TRIM, RCHF8M_DEF_TRIM) & 0x7f;
				LL_RCC_SetRCHFTrimValue(trim);
				break;
		case LL_RCC_RCHF_FREQUENCY_16MHZ:
				systemClock = 16000000;
				LL_RCC_SetRCHFFrequency(RCHF_CLOCK);
				trim = NVR_CHECK(RCHF16M_NVR_TRIM, RCHF16M_DEF_TRIM) & 0x7f;
				LL_RCC_SetRCHFTrimValue(trim);
				break;
		case LL_RCC_RCHF_FREQUENCY_24MHZ:
				systemClock = 24000000;
				LL_RCC_SetRCHFFrequency(RCHF_CLOCK);
				trim = NVR_CHECK(RCHF24M_NVR_TRIM, RCHF24M_DEF_TRIM) & 0x7f;
				LL_RCC_SetRCHFTrimValue(trim);
				break;
		default:
				systemClock = 8000000;
				LL_RCC_SetRCHFFrequency(RCHF_CLOCK);
				trim = NVR_CHECK(RCHF8M_NVR_TRIM, RCHF8M_DEF_TRIM) & 0x7f;
				LL_RCC_SetRCHFTrimValue(trim);
				break;
	}
	LL_RCC_SetSystemClockSource(LL_RCC_SYSTEM_CLKSOURCE_RCHF);
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
}

static void SystickInit(void)
{
	LL_RCC_SetSystickClockSource(LL_RCC_SYSTICK_CLKSOURCE_SYSCLK);
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void LptimInit(void)
{
	LL_LPTIM_TimeInitTypeDef  LPTIM_InitStruct={0};
	LPTIM_InitStruct.PrescalerClockSource = LL_LPTIM_PRESCALER_INPUT_LPTIM_CLK;   
	LPTIM_InitStruct.ClockSource = LL_RCC_LPTIM_OPERATION_CLOCK_SOURCE_LSCLK;     
	LPTIM_InitStruct.Prescaler = LL_LPTIM_CLOCKDIVISION_DIV1;                     
	LPTIM_InitStruct.Mode = LL_LPTIM_OPERATION_MODE_NORMAL;                       
	LPTIM_InitStruct.OneState = LL_LPTIM_ONE_STATE_TIMER_CONTINUE;                
	LL_LPTIM_TimeModeInit(LPTIM,&LPTIM_InitStruct);
	LL_LPTIM_SetAutoReload(LPTIM,8192);    
	LL_LPTIM_ClearFlag_CounterOver(LPTIM);   
	LL_LPTIM_EnableIT_CounterOver(LPTIM);   
	NVIC_DisableIRQ(LPTIM_IRQn);
	NVIC_SetPriority(LPTIM_IRQn,2);
	NVIC_EnableIRQ(LPTIM_IRQn);    
	LL_LPTIM_Enable(LPTIM);   
}

void FoutOut(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};    
	GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);    
	LL_GPIO_SetFout1(GPIO_COMMON, LL_GPIO_FOUT1_SELECT_LSCLK); 
}

void FoutClose(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};    
	GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = DISABLE;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);    
}

void UserInit(void)
{
	ClockInit();
	SystickInit();
	RTC_Init();	
	Test_IWDT();	
	Uartx_Init(UART0);    //与WIFI 通信
	Uartx_Init(UART1);    //对上接口	
	GPIO_FM33LC02X_Init();	
	BSTIM_Init(10);	
	LptimInit();
	DelayMs(300);
}
void DelayUs(uint32_t count)
{
	count = (uint64_t)systemClock * count / 1000000;
	count = count > 16777216 ? 16777216 : count;
	SysTick->LOAD = count - 1;
	SysTick->VAL = 0;
	while (!((SysTick->CTRL >> 16) & 0x1));
}
void DelayMs(uint32_t count)
{
	while (count--){
		DelayUs(1000);
	}
}
