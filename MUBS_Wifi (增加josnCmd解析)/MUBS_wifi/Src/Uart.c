#include "Uart.h"
#include "StrLib.h"

UARTOpStruct UART0Ddata, UART1Ddata;

void UART_DMA(unsigned char * buffer, uint32_t length, UART_Type * UARTx) {
    LL_DMA_InitTypeDef DMA_InitStruct = {0};
    switch ((uint32_t)UARTx) {
        case UART0_BASE:
            DMA_InitStruct.PeriphAddress = LL_DMA_PERIPHERAL_FUNCTION2;
            DMA_InitStruct.MemoryAddress = (uint32_t)buffer;
            DMA_InitStruct.Direction = LL_DMA_DIR_RAM_TO_PERIPHERAL;
            DMA_InitStruct.MemoryAddressIncMode = LL_DMA_INCREMENTAL_INCREASE;
            DMA_InitStruct.DataSize = LL_DMA_BAND_WIDTH_BYTE;
            DMA_InitStruct.NbData = length - 1;
            DMA_InitStruct.Priority = LL_DMA_CHANNEL_PRIORITY_HIGH;
            DMA_InitStruct.CircMode = DISABLE;
            LL_DMA_Init(DMA, &DMA_InitStruct, LL_DMA_CHANNEL_2);
            LL_DMA_Enable_DMA(DMA);
            LL_DMA_ClearFlag_Finished(DMA, LL_DMA_CHANNEL_2);
            LL_DMA_Enable_Channel(DMA, LL_DMA_CHANNEL_2);
            break;
        case UART1_BASE:
            DMA_InitStruct.PeriphAddress = LL_DMA_PERIPHERAL_FUNCTION5;
            DMA_InitStruct.MemoryAddress = (uint32_t)buffer;
            DMA_InitStruct.Direction = LL_DMA_DIR_RAM_TO_PERIPHERAL;
            DMA_InitStruct.MemoryAddressIncMode = LL_DMA_INCREMENTAL_INCREASE;
            DMA_InitStruct.DataSize = LL_DMA_BAND_WIDTH_BYTE;
            DMA_InitStruct.NbData = length - 1;
            DMA_InitStruct.Priority = LL_DMA_CHANNEL_PRIORITY_HIGH;
            DMA_InitStruct.CircMode = DISABLE;
            LL_DMA_Init(DMA, &DMA_InitStruct, LL_DMA_CHANNEL_4);
            LL_DMA_Enable_DMA(DMA);
            LL_DMA_ClearFlag_Finished(DMA, LL_DMA_CHANNEL_4);
            LL_DMA_Enable_Channel(DMA, LL_DMA_CHANNEL_4);
            break;
        case UART4_BASE:
            DMA_InitStruct.PeriphAddress = LL_DMA_PERIPHERAL_FUNCTION4;
            DMA_InitStruct.MemoryAddress = (uint32_t)buffer;
            DMA_InitStruct.Direction = LL_DMA_DIR_RAM_TO_PERIPHERAL;
            DMA_InitStruct.MemoryAddressIncMode = LL_DMA_INCREMENTAL_INCREASE;
            DMA_InitStruct.DataSize = LL_DMA_BAND_WIDTH_BYTE;
            DMA_InitStruct.NbData = length - 1;
            DMA_InitStruct.Priority = LL_DMA_CHANNEL_PRIORITY_HIGH;
            DMA_InitStruct.CircMode = DISABLE;
            LL_DMA_Init(DMA, &DMA_InitStruct, LL_DMA_CHANNEL_3);
            LL_DMA_Enable_DMA(DMA);
            LL_DMA_ClearFlag_Finished(DMA, LL_DMA_CHANNEL_3);
            LL_DMA_Enable_Channel(DMA, LL_DMA_CHANNEL_3);
            break;
        case UART5_BASE:
            DMA_InitStruct.PeriphAddress = LL_DMA_PERIPHERAL_FUNCTION4;
            DMA_InitStruct.MemoryAddress = (uint32_t)buffer;
            DMA_InitStruct.Direction = LL_DMA_DIR_RAM_TO_PERIPHERAL;
            DMA_InitStruct.MemoryAddressIncMode = LL_DMA_INCREMENTAL_INCREASE;
            DMA_InitStruct.DataSize = LL_DMA_BAND_WIDTH_BYTE;
            DMA_InitStruct.NbData = length - 1;
            DMA_InitStruct.Priority = LL_DMA_CHANNEL_PRIORITY_HIGH;
            DMA_InitStruct.CircMode = DISABLE;
            LL_DMA_Init(DMA, &DMA_InitStruct, LL_DMA_CHANNEL_6);
            LL_DMA_Enable_DMA(DMA);
            LL_DMA_ClearFlag_Finished(DMA, LL_DMA_CHANNEL_6);
            LL_DMA_Enable_Channel(DMA, LL_DMA_CHANNEL_6);
            break;
        default:
            break;
    }
}

void Uartx_Init(UART_Type * UARTx) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    LL_UART_InitTypeDef UART_InitStruct = {0};
    switch ((uint32_t)UARTx) {
        case UART0_BASE:
            //PA2:UART0-RX   PA3:UART0-TX
            GPIO_InitStruct.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
            GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;// LL_GPIO_MODE_OUTPUT;//LL_GPIO_MODE_DIGITAL;
            GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;//LL_GPIO_OUTPUT_OPENDRAIN;// LL_GPIO_OUTPUT_PUSHPULL;
            GPIO_InitStruct.Pull = ENABLE;// DISABLE;
            GPIO_InitStruct.RemapPin = DISABLE;
            LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
            LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_2 | LL_GPIO_PIN_3);
            UART_InitStruct.ClockSrc = LL_RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1;
            /*NVIC中断配置*/
            NVIC_DisableIRQ(UART0_IRQn);
            NVIC_SetPriority(UART0_IRQn, 2);//中断优先级配置
            NVIC_EnableIRQ(UART0_IRQn);
            break;
        case UART1_BASE:
            //PC2:UART1-RX   PC3:UART1-TX
            GPIO_InitStruct.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
            GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
            GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
            GPIO_InitStruct.Pull = ENABLE;// DISABLE;
            GPIO_InitStruct.RemapPin = DISABLE;
            LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
            UART_InitStruct.ClockSrc = LL_RCC_UART_OPERATION_CLOCK_SOURCE_APBCLK1;
          /*NVIC中断配置*/
            NVIC_DisableIRQ(UART1_IRQn);
            NVIC_SetPriority(UART1_IRQn, 2);//中断优先级配置
            NVIC_EnableIRQ(UART1_IRQn);
            break;
        case UART4_BASE:
            //PB2:UART4-RX   PB3:UART4-TX
            GPIO_InitStruct.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_3;
            GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
            GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
            GPIO_InitStruct.Pull = DISABLE;
            GPIO_InitStruct.RemapPin = DISABLE;
            LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            break;
        case UART5_BASE:
            //PC4:UART5-RX   PC5:UART5-TX
            GPIO_InitStruct.Pin = LL_GPIO_PIN_4 | LL_GPIO_PIN_5;
            GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;
            GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
            GPIO_InitStruct.Pull = DISABLE;//ENABLE;// DISABLE;
            GPIO_InitStruct.RemapPin = DISABLE;
            LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
            LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_4 | LL_GPIO_PIN_5);
              /*NVIC中断配置*/
            NVIC_DisableIRQ(UART5_IRQn);
            NVIC_SetPriority(UART5_IRQn, 2);//中断优先级配置
            NVIC_EnableIRQ(UART5_IRQn);
            break;
        default:
            break;
    }
    UART_InitStruct.BaudRate = 115200;     	//波特率	
    UART_InitStruct.DataWidth = LL_UART_DATAWIDTH_8B;				//数据位数
    UART_InitStruct.StopBits = LL_UART_STOPBITS_1;					//停止位
    UART_InitStruct.Parity = LL_UART_PARITY_NONE;//LL_UART_PARITY_EVEN;					//奇偶校验
    UART_InitStruct.TransferDirection = LL_UART_DIRECTION_TX_RX;//接收使能    LL_UART_DIRECTION_NONE;	//接收-发送不使能
    UART_InitStruct.InfraredModulation = DISABLE;
    LL_UART_Init(UARTx, &UART_InitStruct);
    LL_UART_EnableIT_ReceiveBuffFull(UARTx);	 //接收完成中断	
}

void Test_Uartx(UART_Type * UARTx) {
    unsigned char TestTxData[11] = "TestUart-TX";
    volatile unsigned char tmp08;
    UART_DMA(TestTxData, 11, UARTx);
    LL_UART_EnableDirectionTx(UARTx);
    while (!LL_DMA_IsActiveFlag_Finished(DMA, LL_DMA_CHANNEL_6));
    LL_DMA_ClearFlag_Finished(DMA, LL_DMA_CHANNEL_6);
    LL_UART_ClearFlag_TXSE(UARTx);
  // 等待发送完成
    while (LL_UART_IsActiveFlag_TXSE(UARTx) == RESET) {};
    LL_UART_ClearFlag_TXSE(UARTx);
}

//void DebugUartSend(unsigned char *txBuffer,uint16_t length)
//{
//	UART_DMA(txBuffer,length,UART5);
//	LL_UART_EnableDirectionTx(UART5);
//	while (!LL_DMA_IsActiveFlag_Finished(DMA, LL_DMA_CHANNEL_6));
//	LL_DMA_ClearFlag_Finished(DMA, LL_DMA_CHANNEL_6);
//	LL_UART_ClearFlag_TXSE(UART5);			
//	// 等待发送完成
//	while (LL_UART_IsActiveFlag_TXSE(UART5) == RESET){};
//	LL_UART_ClearFlag_TXSE(UART5);
//}

//void Uart0Send(unsigned char *txBuffer,uint16_t length)
//{
//	UART_DMA(txBuffer,length,UART0);
//	LL_UART_EnableDirectionTx(UART0);
//	while (!LL_DMA_IsActiveFlag_Finished(DMA, LL_DMA_CHANNEL_2));
//	LL_DMA_ClearFlag_Finished(DMA, LL_DMA_CHANNEL_2);
//	LL_UART_ClearFlag_TXSE(UART0);			
//	// 等待发送完成
//	while (LL_UART_IsActiveFlag_TXSE(UART0) == RESET){};
//	LL_UART_ClearFlag_TXSE(UART0);
//}

//void Uart5Send(unsigned char *txBuffer,uint16_t length)
//{
//	UART_DMA(txBuffer,length,UART5);
//	LL_UART_EnableDirectionTx(UART5);
//	while (!LL_DMA_IsActiveFlag_Finished(DMA, LL_DMA_CHANNEL_6));
//	LL_DMA_ClearFlag_Finished(DMA, LL_DMA_CHANNEL_6);
//	LL_UART_ClearFlag_TXSE(UART5);			
//	// 等待发送完成
//	while (LL_UART_IsActiveFlag_TXSE(UART5) == RESET){};
//	LL_UART_ClearFlag_TXSE(UART5);
//}
//		LL_UART_TransmitData(UARTx, TestTxData[i]);		//将发送数据写入发送寄存器
//		while(SET != LL_UART_IsActiveFlag_TXSE(UARTx));		//等待发送完成

void uart_send(UART_Type * UARTx, uint8_t * buf, uint16_t len) {
    uint16_t i;
    for (i = 0; i < len; i++) {
        LL_UART_TransmitData(UARTx, buf[i]);              //将发送数据写入发送寄存器            
        while (SET != LL_UART_IsActiveFlag_TXSE(UARTx));		//等待发送完成
    }
}

void Uart0Send(uint8_t * txBuffer, uint16_t length) {
    uart_send(UART0, txBuffer, length);
}
void Uart1Send(uint8_t * txBuffer, uint16_t length) {
    uart_send(UART1, txBuffer, length);
}

static void uart_receive_u8(UARTOpStruct * uart, uint8_t tmp) {
    if (uart->RxLen < UARTMAX)
        uart->RxBuf[uart->RxLen++] = tmp;
    else
        uart->RxLen = 0;
}


void UART0_IRQHandler(void) {
    unsigned char tmp08;
    //接收中断处理
    if ((ENABLE == LL_UART_IsEnbaledIT_ReceiveBuffFull(UART0)) && (SET == LL_UART_IsActiveFlag_RXBF(UART0))) {
        //中断转发接收到的数据
        tmp08 = LL_UART_ReceiveData(UART0);//接收中断标志可通过读取rxreg寄存器清除
        uart_receive_u8(&UART0Ddata, tmp08);
        wifi_timer_clean();
        //LL_UART_TransmitData(UART0, tmp08);
//		if(UART0Ddata.RxLen<UARTMAX)UART0Ddata.RxBuf[UART0Ddata.RxLen++]=tmp08;
//		else{
//			memset(&UART0Ddata.RxBuf, 0x00, UART0Ddata.RxLen);
//			UART0Ddata.RxLen=0;
//		}
    }
}

void UART1_IRQHandler(void) {
    unsigned char tmp08;
    //接收中断处理
    if ((ENABLE == LL_UART_IsEnbaledIT_ReceiveBuffFull(UART1)) && (SET == LL_UART_IsActiveFlag_RXBF(UART1))) {
        //中断转发接收到的数据
        tmp08 = LL_UART_ReceiveData(UART1);//接收中断标志可通过读取rxreg寄存器清除
        //LL_UART_TransmitData(UART1, tmp08);
        if (UART1Ddata.RxLen < UARTMAX)UART1Ddata.RxBuf[UART1Ddata.RxLen++] = tmp08;
        else {
            memset(&UART1Ddata.RxBuf, 0x00, UART1Ddata.RxLen);
            UART1Ddata.RxLen = 0;
        }
    }
}

void UART5_IRQHandler(void) {
//	unsigned char tmp08 = 0;
////	//接收中断处理
//	if((ENABLE == LL_UART_IsEnbaledIT_ReceiveBuffFull(UART5))&&(SET == LL_UART_IsActiveFlag_RXBF(UART5))){
////		//中断转发接收到的数据
//		tmp08 = LL_UART_ReceiveData(UART5);//接收中断标志可通过读取rxreg寄存器清除
//////		if(UART5Ddata.RxLen<10)UART5Ddata.RxBuf[UART5Ddata.RxLen++]=tmp08;
//////		else{
//////			if(UART5Ddata.RxLen>=9){				
//////				memset(&UART5Ddata.RxBuf, 0x00, UART5Ddata.RxLen);
//////				UART5Ddata.RxLen=0;
//////			}
//////		}
//	}
}
//搬运串口数据 
void copyDataForUART(void) {
    NVIC_DisableIRQ(UART0_IRQn);	//关闭中断使能
    if (HTTPBuff_NowLen + UART0Ddata.RxLen <= HTTPBuff_MaxLen) {
        HTTPBuff_NowLen = CatString(HTTPBuff_p, (char *)UART0Ddata.RxBuf, HTTPBuff_MaxLen, UART0Ddata.RxLen);	//追加到 HTTPBuff_p	
    } else {
        memset(HTTPBuff_p, 0, HTTPBuff_MaxLen);   								//释放 HTTPBuff_p	
        CopyString(HTTPBuff_p, (char *)UART0Ddata.RxBuf, HTTPBuff_MaxLen, UART0Ddata.RxLen);		//接收数据
        HTTPBuff_NowLen = UART0Ddata.RxLen;
    }
    UART0Ddata.RxLen = 0;
    NVIC_EnableIRQ(UART0_IRQn);  	//开启中断使能
}
