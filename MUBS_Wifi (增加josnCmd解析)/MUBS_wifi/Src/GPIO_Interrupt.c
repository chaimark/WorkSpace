#include "main.h"

extern unsigned char LoRa_work_channel; 

extern void SX1276_Interupt(void);

extern void GPIO_Interrupt_Init(void);// 外部引脚中断初始化
extern void GPIO_FM33LC02X_Init(void);
extern void SX1276_2_Interupt(void);
    
void GPIO_FM33LC02X_Init(void)
{
     LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
   			
		//PA0  PA1  PA4  PA5 PA6 PA7 PA8 PA9  PA10 PA13 PA14 PA15  开漏输出高，防止漏电  没用的IO口
		GPIO_InitStruct.Pin =LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7|LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10|LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15;                    	
		GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = DISABLE;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7|LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10|LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15);
				
		//PB2_POWE_CTR  
		GPIO_InitStruct.Pin =LL_GPIO_PIN_2;		
		GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = DISABLE;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//		LL_GPIO_ResetOutputPin(GPIOB,LL_GPIO_PIN_2);
		LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_2);
					
		//PB3  PB4  PB5  PB8  PB9   PB10  PB11 PB12 PB13 PB14 开漏输出高，防止漏电  没用的IO口
    GPIO_InitStruct.Pin = LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12|LL_GPIO_PIN_13|LL_GPIO_PIN_14;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = DISABLE;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12|LL_GPIO_PIN_13|LL_GPIO_PIN_14);	
														 
		// PC0  PC1 PC4 PC5 PC6 PC7 PC8 PC9  PC10开漏输出高，防止漏电  没用的IO口
    GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7|LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = DISABLE;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7|LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10);
	
		//PD0 PD1 PD6 PD11  开漏输出高，防止漏电  没用的IO口 
		GPIO_InitStruct.Pin =LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_6|LL_GPIO_PIN_11;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = DISABLE;
    LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		LL_GPIO_SetOutputPin(GPIOD,LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_6|LL_GPIO_PIN_11);	
}

// 外部引脚中断初始化
void GPIO_Interrupt_Init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};    
    
    //引脚边沿触发选择
    LL_RCC_Enable_SleepmodeExternalInterrupt();//休眠使能外部中断采样
    LL_RCC_Group1_EnableOperationClock(LL_RCC_OPERATION1_CLOCK_EXTI);//EXTI工作时钟使能    
    	
    //用到的GPIO引脚  PB2  设为输入 
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = DISABLE;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);


    LL_EXTI_SetTrigEdge(GPIO_COMMON, LL_GPIO_EXTI_LINE_4, LL_GPIO_EXTI_TRIGGER_NONE);//关闭边沿触发
    //每条LINE 只能选一个引脚 
    LL_GPIO_SetExitLine4(GPIO_COMMON, LL_GPIO_EXTI_LINE_4_PB2);//中断引脚选择
    LL_EXTI_EnableDigitalFilter(GPIO_COMMON, LL_GPIO_EXTI_LINE_4);//EXTI数字滤波功能
    LL_EXTI_SetTrigEdge(GPIO_COMMON, LL_GPIO_EXTI_LINE_4, LL_GPIO_EXTI_TRIGGER_RISING);//边沿选择    
    LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_4);//清除中断标志
		
		 //用到的GPIO引脚  PD11  设为输入 
    GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = DISABLE;
    LL_GPIO_Init(GPIOD, &GPIO_InitStruct);


    LL_EXTI_SetTrigEdge(GPIO_COMMON, LL_GPIO_EXTI_LINE_14, LL_GPIO_EXTI_TRIGGER_NONE);//关闭边沿触发
    //每条LINE 只能选一个引脚 
    LL_GPIO_SetExitLine14(GPIO_COMMON, LL_GPIO_EXTI_LINE_14_PD11);//中断引脚选择
    LL_EXTI_EnableDigitalFilter(GPIO_COMMON, LL_GPIO_EXTI_LINE_14);//EXTI数字滤波功能
    LL_EXTI_SetTrigEdge(GPIO_COMMON, LL_GPIO_EXTI_LINE_14, LL_GPIO_EXTI_TRIGGER_RISING);//边沿选择    
    LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_14);//清除中断标志
    		   
    /*NVIC中断配置*/
    NVIC_DisableIRQ(GPIO_IRQn);
    NVIC_SetPriority(GPIO_IRQn,0);//中断优先级配置
    NVIC_EnableIRQ(GPIO_IRQn);
}

//引脚中断
void GPIO_IRQHandler(void)
{  
	  if(SET == LL_EXTI_IsActiveFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_4))
    {
        LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_4);
    } 
		if(SET == LL_EXTI_IsActiveFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_14))
		{
			  LL_EXTI_ClearFlag(GPIO_COMMON, LL_GPIO_EXTI_LINE_14);
		}
}
