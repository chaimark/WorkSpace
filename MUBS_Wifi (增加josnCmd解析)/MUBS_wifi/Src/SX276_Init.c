#include "Define.h"
#include "SX1276.h"
extern void SX1276Init(void);  //from SX1276.c
extern void SX1276_RF_SEELP(void);  //from SX1276.c
extern void DelayMs(uint32_t count);
//start**********************************************
/*---------------------------------------------------------
 Function Name:SX1276_RF_SpiInit
 Description  :
               
 Parameter    :
 Return	      :
*/

unsigned char LoRa_work_channel;   //当前操作的LORA模组1或者2

void SX1276_RF_SpiInit(void)
{ 
			SX1276_RF_UCLK_L;
			SX1276_RF_CSn_H;
			SX1276_RF_MOSI_L;
}

void SX1276_RF_2_SpiInit(void)
{ 
			SX1276_2_RF_UCLK_L;
			SX1276_2_RF_CSn_H;
			SX1276_2_RF_MOSI_L; 
}

void SX1276Reset(void)
{
		 SX1276_RF_RST_L;	
		 DelayMs(200);
		 SX1276_RF_RST_H;
		 DelayMs(300);
}

void SX1276_2_Reset(void)
{
		 SX1276_2_RF_RST_L;	
		 DelayMs(200);
		 SX1276_2_RF_RST_H;
		 DelayMs(300);	
}

void SX1276_RF_SPI_MasterIO(unsigned char out)
{
   unsigned char i;
		for (i=0;i<8;i++){   
			if (out & 0x80){SX1276_RF_MOSI_H;}
			else {SX1276_RF_MOSI_L;}							
			SX1276_RF_UCLK_H;						
			out = (out << 1);					
			SX1276_RF_UCLK_L;						
		} 
}

void SX1276_RF_2_SPI_MasterIO(unsigned char out)
{
  unsigned char i;
		 	for (i=0;i<8;i++){   
			if (out & 0x80){SX1276_2_RF_MOSI_H;}
			else {SX1276_2_RF_MOSI_L;}							
			 
			SX1276_2_RF_UCLK_H;						  /* toggle clock high */
			out = (out << 1);					/* shift 1 place for next bit */
			SX1276_2_RF_UCLK_L;							/* toggle clock low */
		}
}

unsigned char SX1276_RF_SPI_READ_BYTE()
{	 	
  unsigned char j;
  unsigned char i;
  j=0;
		for (i = 0; i < 8; i++){	 
			SX1276_RF_UCLK_H; 
			j = (j << 1);						 
			if(SX1276_RF_MISO_VALUE) j = j | 0x01; 					   
			SX1276_RF_UCLK_L; 							
		}
  return j;	
}


unsigned char SX1276_RF_2_SPI_READ_BYTE()
{	 	
  unsigned char j;
  unsigned char i;
  j=0;
	for (i = 0; i < 8; i++){	 
			SX1276_2_RF_UCLK_H; 
			j = (j << 1);						 // shift 1 place to the left or shift in 0 //
			if(SX1276_2_RF_MISO_VALUE)							 // check to see if bit is high //
				j = j | 0x01; 					         // if high, make bit high //
			SX1276_2_RF_UCLK_L; 							 // toggle clock low //  
		}
  return j;	
}

void cmdSwitchEn(cmdEntype_t cmd)
{
		switch(cmd){
			case enOpen:{
				SX1276_RF_CSn_L;
			}break;
			case enClose:{
				SX1276_RF_CSn_H;
			}break;
			default:break;
		}
}

void cmdSwitchEn_2(cmdEntype_t cmd)
{
		 	switch(cmd)
			 {
				 case enOpen:{
					 SX1276_2_RF_CSn_L;
				 }break;
				 case enClose:{
					 SX1276_2_RF_CSn_H;
				 }break;
				 default:break;
			 }
}


void cmdSwitchPA(cmdpaType_t cmd)
{
			switch(cmd){
				case rxOpen:{
					//SX1276_RXD_OUT;
				}break;
				case txOpen:{
					//SX1276_TXD_OUT;
				}break;
				default:break;
			}
}

void cmdSwitchPA_2(cmdpaType_t cmd)
{
		  switch(cmd)
			 {
				 case rxOpen:{
					 //SX1276_RXD_OUT;
				 }break;
				 case txOpen:{
					 //SX1276_TXD_OUT;
				 }break;
				 
				 default:break;
			 }
}


void fqcRecvData(unsigned char *lpbuf,unsigned short len)
{
    
}

void fqcRecvData_2(unsigned char *lpbuf,unsigned short len)
{
    
}

lpCtrlTypefunc_t  ctrlTypefunc = {
   SX1276_RF_SPI_MasterIO,
   SX1276_RF_SPI_READ_BYTE,
   cmdSwitchEn,
   cmdSwitchPA,
   fqcRecvData
};

lpCtrlTypefunc_t_1  ctrlTypefunc1 = {
   SX1276_RF_2_SPI_MasterIO,
   SX1276_RF_2_SPI_READ_BYTE,
   cmdSwitchEn_2,
   cmdSwitchPA_2,
   fqcRecvData_2
};

void SX1276_PWR_ON(void)
{
  SX1276_PWR_PIN_ON;
  DelayMs(200);
}

void SX1276_2_PWR_ON(void)
{
	SX1276_2_PWR_PIN_ON;
  DelayMs(200);
}

void SX1276_PWR_OFF(void)
{
	SX1276_PWR_PIN_OFF;
	SX1276_RF_CSn_L;
	DelayMs(200);
}

void SX1276_2_PWR_OFF(void)
{
	SX1276_2_PWR_PIN_OFF;
	SX1276_2_RF_CSn_L;
  DelayMs(200);
}

void SX1276_RF_OPEN(void)
{
  SX1276_PWR_ON();
  DelayMs(400); 
  SX1276_RF_SpiInit();
  register_rf_func(&ctrlTypefunc);
  SX1276Reset();
  SX1276Init();  
}


void SX1276_RF_2_OPEN(void)
{
  SX1276_2_PWR_ON();
  DelayMs(400); 
  SX1276_RF_2_SpiInit();
  register_rf_func_1(&ctrlTypefunc1);
  SX1276_2_Reset();
  SX1276Init_2();  
}

void SX1276_RF_CLOSE()
{
    SX1276_PWR_OFF();	
		SX1276_RF_UCLK_L;
		SX1276_RF_CSn_L;
		SX1276_RF_MOSI_L;
		SX1276_RF_RST_L;
}  

void SX1276_RF_2_CLOSE()
{
  SX1276_2_PWR_OFF();
//  DelayMs(600);
		SX1276_2_RF_UCLK_L;
		SX1276_2_RF_CSn_L;
		SX1276_2_RF_MOSI_L;
		SX1276_2_RF_RST_L;
}
