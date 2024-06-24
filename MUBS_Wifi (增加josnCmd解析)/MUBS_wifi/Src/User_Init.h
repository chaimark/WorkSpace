#ifndef USER_INIT_H
#define USER_INIT_H

#include "main.h"

#define RCHF_CLOCK  LL_RCC_RCHF_FREQUENCY_8MHZ

#define NVR_CHECK(_N_VALUE_, _T_VALUE_)                         \
                            ((((_N_VALUE_ >> 16) & 0xffff) ==   \
                            ((~_N_VALUE_) & 0xffff)) ? _N_VALUE_ : _T_VALUE_)
#define RCHF8M_DEF_TRIM     (0x30)      // RC8M 经验校准值
#define RCHF16M_DEF_TRIM    (0x2A)      // RC16M 经验校准值
#define RCHF24M_DEF_TRIM    (0x27)      // RC24M 经验校准值

#define RCHF8M_NVR_TRIM     (*(uint32_t *)0x1FFFFB40)	// RC8M 常温校准值
#define RCHF16M_NVR_TRIM 	(*(uint32_t *)0x1FFFFB3C)	// RC16M 常温校准值
#define RCHF24M_NVR_TRIM 	(*(uint32_t *)0x1FFFFB38)	// RC24M 常温校准值


extern void LPUART_Init(LPUART_Type* LPUARTx);
extern void GPIO_FM33LC02X_Init(void);
extern void GPIO_Interrupt_Init(void);
extern void RTC_Init(void);
extern void DelayUs(uint32_t count);
extern ErrorStatus Test_IWDT(void);
extern void Uartx_Init(UART_Type* UARTx);
extern void DelayUs(uint32_t count);
extern void DelayMs(uint32_t count);

#endif /* USER_INIT_H */
