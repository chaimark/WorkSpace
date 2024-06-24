#include "main.h" 

// wifi
//#define    wifi_rst_on()    LL_GPIO_SetOutputPin (GPIOB, LL_GPIO_PIN_2) 
//#define    wifi_rst_off()   LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2) 

#define    wifi_power_on()    LL_GPIO_SetOutputPin (GPIOB, LL_GPIO_PIN_2) 
#define    wifi_power_off()   LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2) 
