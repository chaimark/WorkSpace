#include "main.h"
#include "Define.h"
#include "LPUart.h"

LPUARTOpTypeDef LPUART0Op;
LPUARTOpTypeDef LPUART1Op;

extern unsigned char LPUART0_Send_In_Main_Flage; 
extern unsigned char EEprom_Byte1Write(unsigned char addr,unsigned char data);

void LPUART_Init(LPUART_Type* LPUARTx)
{
	LL_LPUART_InitTypeDef  InitStructer = {0};
	LL_GPIO_InitTypeDef  GPIO_InitStruct = {0};
	volatile unsigned char tmp08;

	if(LPUARTx==LPUART0){
		GPIO_InitStruct.Pin = LL_GPIO_PIN_13;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;              //数字功能
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;     //推挽输出
		GPIO_InitStruct.Pull = ENABLE;                            //接收使能上拉
		GPIO_InitStruct.RemapPin = ENABLE;                        //使能额外数字功能
		LL_GPIO_Init(GPIOA,&GPIO_InitStruct);                     //PA13
		
		GPIO_InitStruct.Pin = LL_GPIO_PIN_14;                     
		GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;               //数字功能
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;      //推挽输出
		GPIO_InitStruct.Pull = DISABLE;                            //发送不上拉
		GPIO_InitStruct.RemapPin = ENABLE;                         //使能额外数字功能    
		LL_GPIO_Init(GPIOA,&GPIO_InitStruct);                      //PA14

		NVIC_DisableIRQ(LPUART0_IRQn);
		NVIC_SetPriority(LPUART0_IRQn,2);//中断优先级配置
		NVIC_EnableIRQ(LPUART0_IRQn);   
	}
	if(LPUARTx==LPUART1){
		GPIO_InitStruct.Pin = LL_GPIO_PIN_2;                           
		GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;                //数字功能
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;       //推挽输出
		GPIO_InitStruct.Pull = ENABLE;                              //接收使能上拉
		GPIO_InitStruct.RemapPin = ENABLE;                          //使能额外数字功能  
		LL_GPIO_Init(GPIOC,&GPIO_InitStruct);                       //PC2

		GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_DIGITAL;                //数字功能
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;       //推挽输出
		GPIO_InitStruct.Pull = DISABLE;                             //发送不上拉
		GPIO_InitStruct.RemapPin = ENABLE;                          //使能额外数字功能  
		LL_GPIO_Init(GPIOC,&GPIO_InitStruct);                       //PC3

		NVIC_DisableIRQ(LPUART1_IRQn);
		NVIC_SetPriority(LPUART1_IRQn,2);//中断优先级配置
		NVIC_EnableIRQ(LPUART1_IRQn);     //使能NVIC
	}
	InitStructer.ClockSrc 			= LL_RCC_LPUART_OPERTION_CLOCK_SOURCE_LSCLK;  //时钟源LSCLK
	InitStructer.BaudRate            = LL_LPUART_BAUDRATE_9600;  //波特率9600
	InitStructer.DataWidth           = LL_LPUART_DATAWIDTH_8B;   //8位数据位
	InitStructer.StopBits            = LL_LPUART_STOP_1BITS;     //1位停止位
	InitStructer.Parity              = LL_LPUART_PARITY_NONE ;   //无校验
	InitStructer.TransferDirection   = LL_LPUART_DIRECTION_TX_RX;   //使能发送和接收
	LL_LPUART_Init(LPUARTx,&InitStructer);     
	LL_LPUART_SetDataRecvedWakeupMode(LPUARTx,LL_LPUART_WAKE_UP_AFTER_CHECK);//接收中断加校验
	LL_LPUART_DisableIT_TXSE(LPUARTx);   //禁止发送中断
	LL_LPUART_EnabledIT_RXBF(LPUARTx);   //使能接收中断
}

void LPUART0_Send(unsigned char *txBuffer,unsigned short int length)
{             
	LL_LPUART_ClearFlag_TXSE(LPUART0);   //清除发送中断标志  
	// LL_LPUART_EnabledIT_TXSE(LPUART0);   //发送寄存器空且移位寄存器空中断使能
	while(length--){
		LL_LPUART_TransmitData(LPUART0,*txBuffer); //启动第一个数据的发送
		while(LL_LPUART_IsActiveFlag_TXSE(LPUART0)==0);//等待发送完成
		LL_LPUART_ClearFlag_TXSE(LPUART0);txBuffer++;
	}
}


void LPUART1_Send(unsigned char *txBuffer,unsigned short int length)
{             
	LL_LPUART_ClearFlag_TXSE(LPUART1);   //清除发送中断标志  
	//LL_LPUART_EnabledIT_TXSE(LPUART1);   //发送寄存器空且移位寄存器空中断使能
	while(length--){
		LL_LPUART_TransmitData(LPUART1,*txBuffer); //启动第一个数据的发送
		while(LL_LPUART_IsActiveFlag_TXSE(LPUART1)==0);//等待发送完成
		LL_LPUART_ClearFlag_TXSE(LPUART1);txBuffer++;
	}
}

void Clear_LEUART0_RXDATA(void)
{  
	memset(&LPUART0Op.RxBuf, 0x00, LPUART0Op.RxLen);
	LPUART0Op.RxLen=0;
}

void SAVED_LPUART0_DATA()
{
	LPUART0Op.RxBuf[LPUART0Op.RxLen++]=LPUART0Op.RxData;
}

void Adjust_LPUART0_Head(void)
{	
	if(LPUART0Op.RxData == 0x68){
	  LPUART0Op.RxBuf[0] = LPUART0Op.RxData; 
    LPUART0Op.RxLen=1;
	}
	else{
	  if((LPUART0Op.RxBuf[0] == 0x68)&&((LPUART0Op.RxData==0x20)||(LPUART0Op.RxData==0x56))){
		  LPUART0Op.RxBuf[1] = LPUART0Op.RxData;
		  LPUART0Op.RxLen = 2;
	  }else{
		  LPUART0Op.RxLen = 0;           
	  }
	}      
}

unsigned char Cault_CS(unsigned char *Temp_Bufferf,unsigned char start_num,unsigned char stop_num)//?????
{
  unsigned char temp,cs;
  cs=0;
  for(temp=start_num;temp<stop_num;temp++){
    cs=cs+*(Temp_Bufferf+temp);
  }
  return cs;
}

void Send_Data_To_LPUART0_LCD(void)  //发送数据
{
//	LPUART0Op.TxBuf[0]=0xFE;
//	LPUART0Op.TxBuf[1]=0xFE;
//	LPUART0Op.TxBuf[2]=0xFE;
//	LPUART0Op.TxBuf[3]=0xFE;
//	LPUART0Op.TxBuf[4]=0x68;
//	LPUART0Op.TxBuf[5]=0x20;
//	LPUART0Op.TxBuf[6]=0x11;
//	LPUART0Op.TxBuf[7]=0x11;
//	LPUART0Op.TxBuf[8]=METERID[0];
//	LPUART0Op.TxBuf[9]=METERID[1];
//	LPUART0Op.TxBuf[10]=0x00;
//	LPUART0Op.TxBuf[11]=0x11;
//	LPUART0Op.TxBuf[12]=0x11;
//	LPUART0Op.TxBuf[13]=0x81;
//	LPUART0Op.TxBuf[14]=0x19; //长度
//	LPUART0Op.TxBuf[15]=0x90; 
//	LPUART0Op.TxBuf[16]=0x1F; 
//	LPUART0Op.TxBuf[17] = FM33LE02X_VREFP_Voltage>>8;
//	LPUART0Op.TxBuf[18] = FM33LE02X_VREFP_Voltage&0XFF;
//	LPUART0Op.TxBuf[19] = BatVotage>>8;
//	LPUART0Op.TxBuf[20] = BatVotage&0XFF;
//	LPUART0Op.TxBuf[21] = Meter_Type;    //仪表类型  1：PT1000,2:压力	
//	LPUART0Op.TxBuf[22] = RF_PWR_Status;
//	LPUART0Op.TxBuf[23] = Test_1Second_Count>>8;
//	LPUART0Op.TxBuf[24] = Test_1Second_Count&0XFF;
//	LPUART0Op.TxBuf[25] = PT1000_T2>>8;
//	LPUART0Op.TxBuf[26] = PT1000_T2&0XFF;
//	LPUART0Op.TxBuf[27] = PressValue>>8;
//	LPUART0Op.TxBuf[28] = PressValue&0XFF;
//	LPUART0Op.TxBuf[29] = TEMP_VALUE_PS2>>8;    //实际值
//	LPUART0Op.TxBuf[30] = TEMP_VALUE_PS2&0XFF;	
//	LPUART0Op.TxBuf[31] = PressVotage>>8;    //实际值
//	LPUART0Op.TxBuf[32] = PressVotage&0XFF;	
//	LPUART0Op.TxBuf[33] = SingalStatus;      //lora信号
//	LPUART0Op.TxBuf[34] = Test_PT1000_Press_Interval>>8;   //测量周期  分钟
//	LPUART0Op.TxBuf[35] = Test_PT1000_Press_Interval&0XFF;	
//	LPUART0Op.TxBuf[36] = Responed_RF0_DATA_Interval>>8;  //无线发送周期  分钟
//	LPUART0Op.TxBuf[37] = Responed_RF0_DATA_Interval&0XFF;	
//	LPUART0Op.TxBuf[38] = Send_total_count;	
//	LPUART0Op.TxBuf[39] = Receive_total_count;	
//	LPUART0Op.TxBuf[40] = Cault_CS(LPUART0Op.TxBuf,4,40);
//  LPUART0Op.TxBuf[41] = 0x16;
	
	LPUART0Op.TxLen=42;
	LPUART0_Send_In_Main_Flage=1;
}

void LPUART0_ReadData_Deal(void)
{
	if(Cault_CS(LPUART0Op.RxBuf,0,(LPUART0Op.RxLen-2)) == LPUART0Op.RxBuf[LPUART0Op.RxLen-2]){		
		if((LPUART0Op.RxBuf[2]==0xFF)&&	(LPUART0Op.RxBuf[3]==0xFF)&&(LPUART0Op.RxBuf[4]==0xFF)&&(LPUART0Op.RxBuf[5]==0xFF)) goto LPUARTNEXT0;
//		if((LPUART0Op.RxBuf[4]==METERID[0])&&(LPUART0Op.RxBuf[5]==METERID[1])){
			LPUARTNEXT0:				
			switch (LPUART0Op.RxBuf[9]){     					 
        case 0x03: 
            if((LPUART0Op.RxBuf[10]==0x04)&&(LPUART0Op.RxBuf[11]==0x90)&&(LPUART0Op.RxBuf[12]==0x4F)){			
							Send_Data_To_LPUART0_LCD();
						}
						break;							
				 default: break;
			}
//		}
	}
}

void LPUART0_IRQHandler(void)
{	
	//接收中断处理
	if((ENABLE == LL_LPUART_IsEnabledIT_RXBF(LPUART0))&&(SET == LL_LPUART_IsActiveFlag_RXBF(LPUART0))){//接收数据匹配中断
		LPUART0Op.RxData = LL_LPUART_ReceiveData(LPUART0);  	
		if(LPUART0Op.RxLen==0)Adjust_LPUART0_Head(); 
		else{
			if(LPUART0Op.RxLen>23){
				Clear_LEUART0_RXDATA();
			}else{
				if(LPUART0Op.RxLen<12){
					SAVED_LPUART0_DATA();
				}else{ 
					if(LPUART0Op.RxLen <= LPUART0Op.RxBuf[10]+11){
						SAVED_LPUART0_DATA();
					}else{
						if(LPUART0Op.RxData == 0x16){
							SAVED_LPUART0_DATA();
							LPUART0_ReadData_Deal();
							Clear_LEUART0_RXDATA();
						}else{
							SAVED_LPUART0_DATA();  																	 
						}
					}
				}
			}
		}	
	}		
}

void LPUART1_IRQHandler(void)
{
	//接收中断处理
	if((ENABLE == LL_LPUART_IsEnabledIT_RXBF(LPUART1))&&(SET == LL_LPUART_IsActiveFlag_RXBF(LPUART1))){//接收数据匹配中断
		LPUART1Op.RxData = LL_LPUART_ReceiveData(LPUART1);  			
  }	
	//发送中断处理
	if((ENABLE == LL_LPUART_IsEnabledIT_TXSE(LPUART1))&&(SET == LL_LPUART_IsActiveFlag_TXSE(LPUART1))){
		//清除发送中断标志
    LL_LPUART_ClearFlag_TXSE(LPUART1);
	}
}
