#include "SX1276.h"
#include "Define.h"

const unsigned char   SpreadingFactor=11;    //7-12
const unsigned char   CodingRate=2;          //1-4
const unsigned char   Bw_Frequency=7;        //6-9
const unsigned char   powerValue=7;
const unsigned char   power_data[8]={0X80,0X80,0X80,0X83,0X86,0x89,0x8c,0x8f};

unsigned char SX1276_RF_RXBuffer[32];
unsigned char SX1276_2_RF_RXBuffer[32];
unsigned char Frequency[3]={0x6C,0x80,0x00}; //433
unsigned char RF_RXBuffer[50];  
unsigned char RF_TXBuffer[30]; 
unsigned char RF_2_RXBuffer[50];  
unsigned char RF_2_TXBuffer[30]; 
unsigned char RF_ReceiveFig= 0x01; 
unsigned char RF_2_ReceiveFig= 0x01; 
unsigned char RF_EX0_STATUS;
unsigned char RF_EX1_STATUS;
unsigned char CRC_Value;
unsigned char SX1278_RLEN;

unsigned char SingalStatus=0;

extern unsigned char METERID[2];
extern unsigned char Responed_RF0_TX_In_Main_Flage;  
extern unsigned char Responed_RF1_TX_In_Main_Flage;

extern void RECEIVE_DATA(void);
extern void Delay_ms(unsigned int temp);
extern unsigned char LoRa_work_channel;   //当前操作的LORA模组1或者2
extern void RECEIVE_2_DATA(void);

lpCtrlTypefunc_t lpTypefunc = {0,0,0,0,0};
lpCtrlTypefunc_t_1 lpTypefunc1 = {0,0,0,0,0};

void register_rf_func(lpCtrlTypefunc_t *func)
{
	if(func->lpByteWritefunc != 0){
		lpTypefunc.lpByteWritefunc = func->lpByteWritefunc;
	}
	if(func->lpByteReadfunc != 0){
		lpTypefunc.lpByteReadfunc = func->lpByteReadfunc;
	}
	if(func->lpSwitchEnStatus != 0){
		lpTypefunc.lpSwitchEnStatus = func->lpSwitchEnStatus;
	}
	if(func->paSwitchCmdfunc != 0){
		lpTypefunc.paSwitchCmdfunc = func->paSwitchCmdfunc;
	}
	if(func->lpRecvDataTousr != 0){
		lpTypefunc.lpRecvDataTousr = func->lpRecvDataTousr;
	}
}

void register_rf_func_1(lpCtrlTypefunc_t_1 *func)
{
	if(func->lpByteWritefunc_1 != 0){
		lpTypefunc1.lpByteWritefunc_1 = func->lpByteWritefunc_1;
	}
	if(func->lpByteReadfunc_1 != 0){
		lpTypefunc1.lpByteReadfunc_1 = func->lpByteReadfunc_1;
	}
	if(func->lpSwitchEnStatus_1 != 0){
		lpTypefunc1.lpSwitchEnStatus_1 = func->lpSwitchEnStatus_1;
	}
	if(func->paSwitchCmdfunc_1 != 0){
		lpTypefunc1.paSwitchCmdfunc_1 = func->paSwitchCmdfunc_1;
	}
	if(func->lpRecvDataTousr_1 != 0){
		lpTypefunc1.lpRecvDataTousr_1 = func->lpRecvDataTousr_1;
	}
}

void FSK_SEND_PACKET(void)
{
	SX1276LoRaSetOpMode( Stdby_mode );
	SX1276WriteBuffer(REG_LR_DIOMAPPING1,0x01);
	SX1276WriteBuffer(REG_LR_DIOMAPPING2,0x20);
}

void FSK_SEND_PACKET_2(void)
{
	SX1276LoRaSetOpMode_2( Stdby_mode );
	SX1276WriteBuffer_2(REG_LR_DIOMAPPING1,0x01);
	SX1276WriteBuffer_2(REG_LR_DIOMAPPING2,0x20);
}
	
void SX1276WriteBuffer( unsigned char addr, unsigned char buffer)
{ 
	lpTypefunc.lpSwitchEnStatus(enOpen); //NSS = 0;
	lpTypefunc.lpByteWritefunc( addr | 0x80 );
	lpTypefunc.lpByteWritefunc( buffer);
	lpTypefunc.lpSwitchEnStatus(enClose); //NSS = 1;
}

void SX1276WriteBuffer_2( unsigned char addr, unsigned char buffer)
{ 
	lpTypefunc1.lpSwitchEnStatus_1(enOpen); //NSS = 0;
	lpTypefunc1.lpByteWritefunc_1( addr | 0x80 );
	lpTypefunc1.lpByteWritefunc_1( buffer);
	lpTypefunc1.lpSwitchEnStatus_1(enClose); //NSS = 1;
}
	
	
unsigned char SX1276ReadBuffer(unsigned char addr)
{
  unsigned char Value;
  lpTypefunc.lpSwitchEnStatus(enOpen); //NSS = 0;
  lpTypefunc.lpByteWritefunc( addr & 0x7f  );
  Value = lpTypefunc.lpByteReadfunc();
  lpTypefunc.lpSwitchEnStatus(enClose);//NSS = 1;
  return Value; 
}
unsigned char SX1276ReadBuffer_2(unsigned char addr)
{
  unsigned char Value;
  lpTypefunc1.lpSwitchEnStatus_1(enOpen); //NSS = 0;
  lpTypefunc1.lpByteWritefunc_1( addr & 0x7f  );
  Value = lpTypefunc1.lpByteReadfunc_1();
  lpTypefunc1.lpSwitchEnStatus_1(enClose);//NSS = 1;
  return Value; 
}
	
void SX1276LoRaSetOpMode( RFMode_SET opMode )
{
	unsigned char opModePrev;
	opModePrev=SX1276ReadBuffer(REG_LR_OPMODE);
	opModePrev &=0xf8;
	opModePrev |= (unsigned char)opMode;
	SX1276WriteBuffer( REG_LR_OPMODE, opModePrev);		
}

void SX1276LoRaSetOpMode_2( RFMode_SET opMode )
{
	unsigned char opModePrev;
	opModePrev=SX1276ReadBuffer_2(REG_LR_OPMODE);
	opModePrev &=0xf8;
	opModePrev |= (unsigned char)opMode;
	SX1276WriteBuffer_2( REG_LR_OPMODE, opModePrev);		
}

void SX1276LoRaFsk( Debugging_fsk_ook opMode )
{
	unsigned char opModePrev;
	opModePrev=SX1276ReadBuffer(REG_LR_OPMODE);
	opModePrev &=0x7F;
	opModePrev |= (unsigned char)opMode;
	SX1276WriteBuffer( REG_LR_OPMODE, opModePrev);		
}

void SX1276LoRaFsk_2( Debugging_fsk_ook opMode )
{
	unsigned char opModePrev;
	opModePrev=SX1276ReadBuffer_2(REG_LR_OPMODE);
	opModePrev &=0x7F;
	opModePrev |= (unsigned char)opMode;
	SX1276WriteBuffer_2( REG_LR_OPMODE, opModePrev);		
}
	
void SX1276LoRaSetRFFrequency(void)
{
  unsigned char RFFrequency=0x00;
  unsigned int RFFrequencyTemp=0;
	if((RFFrequency>0x00) && (RFFrequency<=0x25)) {
		RFFrequencyTemp=((METERID[0]&0xF0)>>4)*10+(METERID[0]&0x0F);
	}
	if((RFFrequency>0x25) && (RFFrequency<=0x50)) {
		RFFrequencyTemp=((METERID[0]&0xF0)>>4)*10+(METERID[0]&0x0F)-25;
	}
	if((RFFrequency>0x50) && (RFFrequency<=0x75)) {
		RFFrequencyTemp=((METERID[0]&0xF0)>>4)*10+(METERID[0]&0x0F)-50;
	}
	if((RFFrequency>0x75) && (RFFrequency<=0x99)) {
		RFFrequencyTemp=((METERID[0]&0xF0)>>4)*10+(METERID[0]&0x0F)-75;
	}
  switch(RFFrequencyTemp){  //Frequency[3]={0x6c,0x80,0x00};
	 case 0:
		 Frequency[0] =0x6c;  //433.99  7110656+16384
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
	 case 1:
		 Frequency[0] =0x6c;  //434.99  
		 Frequency[1] =0xC0;
		 Frequency[2] =0x00;
		 break;
	 case 2:
		 Frequency[0] =0x6D;  //435.99
		 Frequency[1] =0x00;
		 Frequency[2] =0x00;
		 break;
	 case 3:
		 Frequency[0] =0x6D;  //436.99
		 Frequency[1] =0x40;
		 Frequency[2] =0x00;
		 break;
	 case 4:
		 Frequency[0] =0x6D;  //437.99
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
	 case 5:
		 Frequency[0] =0x6D;  //438.99
		 Frequency[1] =0xC0;
		 Frequency[2] =0x00;
		 break;
	 case 6:
		 Frequency[0] =0x6E;  //439.99
		 Frequency[1] =0x00;
		 Frequency[2] =0x00;
		 break;
	 case 7:
		 Frequency[0] =0x6E;  //440.99
		 Frequency[1] =0x40;
		 Frequency[2] =0x00;
		 break;
	 case 8:
		 Frequency[0] =0x6E;  //441.99
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
	 case 9:
		 Frequency[0] =0x6E;  //442.99
		 Frequency[1] =0xC0;
		 Frequency[2] =0x00;
		 break;
	 case 10:
		 Frequency[0] =0x6F;  //443.99
		 Frequency[1] =0x00;
		 Frequency[2] =0x00;
		 break;
	 case 11:
		 Frequency[0] =0x6F;  //444.99  
		 Frequency[1] =0x40;
		 Frequency[2] =0x00;
		 break;
	 case 12:
		 Frequency[0] =0x6F;  //445.99  
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
	 case 13:
		 Frequency[0] =0x6F;  //446.99
		 Frequency[1] =0xC0;
		 Frequency[2] =0x00;
		 break;
	 case 14:
		 Frequency[0] =0x70;  //447.99
		 Frequency[1] =0x00;
		 Frequency[2] =0x00;
		 break;
	 case 15:
		 Frequency[0] =0x70;  //448.99
		 Frequency[1] =0x40;
		 Frequency[2] =0x00;
		 break;
	 case 16:
		 Frequency[0] =0x70;  //449.99
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
	 case 17:
		 Frequency[0] =0x70;  //450.99
		 Frequency[1] =0xC0;
		 Frequency[2] =0x00;
		 break;
	 case 18:
		 Frequency[0] =0x71;  //451.99
		 Frequency[1] =0x00;
		 Frequency[2] =0x00;
		 break;
	 case 19:
		 Frequency[0] =0x71;  //452.99
		 Frequency[1] =0x40;
		 Frequency[2] =0x00;
		 break;
	 case 20:
		 Frequency[0] =0x71;  //453.99
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
	 case 21:
		 Frequency[0] =0x71;  //455.99  
		 Frequency[1] =0xC0;
		 Frequency[2] =0x00;
		 break;
	 case 22:
		 Frequency[0] =0x72;  //456.99  
		 Frequency[1] =0x00;
		 Frequency[2] =0x00;
		 break;
	 case 23:
		 Frequency[0] =0x72;  //457.99
		 Frequency[1] =0x40;
		 Frequency[2] =0x00;
		 break;
	 case 24:
		 Frequency[0] =0x72;  //458.99
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
	 case 25:
		 Frequency[0] =0x72;  //459.99  
		 Frequency[1] =0xC0;
		 Frequency[2] =0x00;
		 break;
	 default: 
		 Frequency[0] =0x6c;  //433.99  7110656+16384
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
  }
  SX1276WriteBuffer( REG_LR_FRFMSB, Frequency[0]);
  SX1276WriteBuffer( REG_LR_FRFMID, Frequency[1]);
  SX1276WriteBuffer( REG_LR_FRFLSB, Frequency[2]);
}

void SX1276LoRaSetRFFrequency_2(void)
{
  unsigned char RFFrequency=0x00;
  unsigned int RFFrequencyTemp=0;
	if((RFFrequency>0x00) && (RFFrequency<=0x25)) {
		RFFrequencyTemp=((METERID[0]&0xF0)>>4)*10+(METERID[0]&0x0F);
	}
	if((RFFrequency>0x25) && (RFFrequency<=0x50)) {
		RFFrequencyTemp=((METERID[0]&0xF0)>>4)*10+(METERID[0]&0x0F)-25;
	}
	if((RFFrequency>0x50) && (RFFrequency<=0x75)) {
		RFFrequencyTemp=((METERID[0]&0xF0)>>4)*10+(METERID[0]&0x0F)-50;
	}
	if((RFFrequency>0x75) && (RFFrequency<=0x99)) {
		RFFrequencyTemp=((METERID[0]&0xF0)>>4)*10+(METERID[0]&0x0F)-75;
	}
	RFFrequencyTemp=RFFrequencyTemp+3; //模组2
  switch(RFFrequencyTemp){  //Frequency[3]={0x6c,0x80,0x00};
	 case 0:
		 Frequency[0] =0x6c;  //433.99  7110656+16384
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
	 case 1:
		 Frequency[0] =0x6c;  //434.99  
		 Frequency[1] =0xC0;
		 Frequency[2] =0x00;
		 break;
	 case 2:
		 Frequency[0] =0x6D;  //435.99
		 Frequency[1] =0x00;
		 Frequency[2] =0x00;
		 break;
	 case 3:
		 Frequency[0] =0x6D;  //436.99
		 Frequency[1] =0x40;
		 Frequency[2] =0x00;
		 break;
	 case 4:
		 Frequency[0] =0x6D;  //437.99
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
	 case 5:
		 Frequency[0] =0x6D;  //438.99
		 Frequency[1] =0xC0;
		 Frequency[2] =0x00;
		 break;
	 case 6:
		 Frequency[0] =0x6E;  //439.99
		 Frequency[1] =0x00;
		 Frequency[2] =0x00;
		 break;
	 case 7:
		 Frequency[0] =0x6E;  //440.99
		 Frequency[1] =0x40;
		 Frequency[2] =0x00;
		 break;
	 case 8:
		 Frequency[0] =0x6E;  //441.99
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
	 case 9:
		 Frequency[0] =0x6E;  //442.99
		 Frequency[1] =0xC0;
		 Frequency[2] =0x00;
		 break;
	 case 10:
		 Frequency[0] =0x6F;  //443.99
		 Frequency[1] =0x00;
		 Frequency[2] =0x00;
		 break;
	 case 11:
		 Frequency[0] =0x6F;  //444.99  
		 Frequency[1] =0x40;
		 Frequency[2] =0x00;
		 break;
	 case 12:
		 Frequency[0] =0x6F;  //445.99  
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
	 case 13:
		 Frequency[0] =0x6F;  //446.99
		 Frequency[1] =0xC0;
		 Frequency[2] =0x00;
		 break;
	 case 14:
		 Frequency[0] =0x70;  //447.99
		 Frequency[1] =0x00;
		 Frequency[2] =0x00;
		 break;
	 case 15:
		 Frequency[0] =0x70;  //448.99
		 Frequency[1] =0x40;
		 Frequency[2] =0x00;
		 break;
	 case 16:
		 Frequency[0] =0x70;  //449.99
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
	 case 17:
		 Frequency[0] =0x70;  //450.99
		 Frequency[1] =0xC0;
		 Frequency[2] =0x00;
		 break;
	 case 18:
		 Frequency[0] =0x71;  //451.99
		 Frequency[1] =0x00;
		 Frequency[2] =0x00;
		 break;
	 case 19:
		 Frequency[0] =0x71;  //452.99
		 Frequency[1] =0x40;
		 Frequency[2] =0x00;
		 break;
	 case 20:
		 Frequency[0] =0x71;  //453.99
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
	 case 21:
		 Frequency[0] =0x71;  //455.99  
		 Frequency[1] =0xC0;
		 Frequency[2] =0x00;
		 break;
	 case 22:
		 Frequency[0] =0x72;  //456.99  
		 Frequency[1] =0x00;
		 Frequency[2] =0x00;
		 break;
	 case 23:
		 Frequency[0] =0x72;  //457.99
		 Frequency[1] =0x40;
		 Frequency[2] =0x00;
		 break;
	 case 24:
		 Frequency[0] =0x72;  //458.99
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
	 case 25:
		 Frequency[0] =0x72;  //459.99  
		 Frequency[1] =0xC0;
		 Frequency[2] =0x00;
		 break;
	 default: 
		 Frequency[0] =0x6c;  //433.99  7110656+16384
		 Frequency[1] =0x80;
		 Frequency[2] =0x00;
		 break;
  }
  SX1276WriteBuffer_2( REG_LR_FRFMSB, Frequency[0]);
  SX1276WriteBuffer_2( REG_LR_FRFMID, Frequency[1]);
  SX1276WriteBuffer_2( REG_LR_FRFLSB, Frequency[2]);
}


void SX1276LoRaSetRFPower(unsigned char power )
{
	SX1276WriteBuffer( REG_LR_PADAC, 0x87);
	SX1276WriteBuffer( REG_LR_PACONFIG,  power_data[power] );
}

void SX1276LoRaSetRFPower_2(unsigned char power )
{
	SX1276WriteBuffer_2( REG_LR_PADAC, 0x87);
	SX1276WriteBuffer_2( REG_LR_PACONFIG,  power_data[power] );
}
	
void SX1276LoRaSetSpreadingFactor(unsigned char factor )
{
	unsigned char RECVER_DAT;
	SX1276LoRaSetNbTrigPeaks( 3 );
	RECVER_DAT=SX1276ReadBuffer( REG_LR_MODEMCONFIG2);	  
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG2_SF_MASK ) | ( factor << 4 );
	SX1276WriteBuffer( REG_LR_MODEMCONFIG2, RECVER_DAT );	 
}

void SX1276LoRaSetSpreadingFactor_2(unsigned char factor )
{
	unsigned char RECVER_DAT;
	SX1276LoRaSetNbTrigPeaks_2( 3 );
	RECVER_DAT=SX1276ReadBuffer_2( REG_LR_MODEMCONFIG2);	  
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG2_SF_MASK ) | ( factor << 4 );
	SX1276WriteBuffer_2( REG_LR_MODEMCONFIG2, RECVER_DAT );	 
}
	
void SX1276LoRaSetNbTrigPeaks(unsigned char value )
{
	unsigned char RECVER_DAT;
	RECVER_DAT = SX1276ReadBuffer( 0x31);
	RECVER_DAT = ( RECVER_DAT & 0xF8 ) | value;
	SX1276WriteBuffer( 0x31, RECVER_DAT );
}
void SX1276LoRaSetNbTrigPeaks_2(unsigned char value )
{
	unsigned char RECVER_DAT;
	RECVER_DAT = SX1276ReadBuffer_2( 0x31);
	RECVER_DAT = ( RECVER_DAT & 0xF8 ) | value;
	SX1276WriteBuffer_2( 0x31, RECVER_DAT );
}
	
void SX1276LoRaSetErrorCoding(unsigned char value )
{	
	unsigned char RECVER_DAT;
	RECVER_DAT=SX1276ReadBuffer( REG_LR_MODEMCONFIG1);
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG1_CODINGRATE_MASK ) | ( value << 1 );
	SX1276WriteBuffer( REG_LR_MODEMCONFIG1, RECVER_DAT);
	// LoRaSettings.ErrorCoding = value;
}

void SX1276LoRaSetErrorCoding_2(unsigned char value )
{	
	unsigned char RECVER_DAT;
	RECVER_DAT=SX1276ReadBuffer_2( REG_LR_MODEMCONFIG1);
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG1_CODINGRATE_MASK ) | ( value << 1 );
	SX1276WriteBuffer_2( REG_LR_MODEMCONFIG1, RECVER_DAT);
	// LoRaSettings.ErrorCoding = value;
}
	
void SX1276LoRaSetPacketCrcOn(BOOL enable )
{	
	unsigned char RECVER_DAT;
	RECVER_DAT= SX1276ReadBuffer( REG_LR_MODEMCONFIG2);
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK ) | ( enable << 2 );
	SX1276WriteBuffer( REG_LR_MODEMCONFIG2, RECVER_DAT );
}

void SX1276LoRaSetPacketCrcOn_2(BOOL enable )
{	
	unsigned char RECVER_DAT;
	RECVER_DAT= SX1276ReadBuffer_2( REG_LR_MODEMCONFIG2);
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK ) | ( enable << 2 );
	SX1276WriteBuffer_2( REG_LR_MODEMCONFIG2, RECVER_DAT );
}
	
void SX1276LoRaSetSignalBandwidth(unsigned char bw )
{
	unsigned char RECVER_DAT;
	RECVER_DAT=SX1276ReadBuffer( REG_LR_MODEMCONFIG1);
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG1_BW_MASK ) | ( bw << 4 );
	SX1276WriteBuffer( REG_LR_MODEMCONFIG1, RECVER_DAT );
	// LoRaSettings.SignalBw = bw;
}
void SX1276LoRaSetSignalBandwidth_2(unsigned char bw )
{
	unsigned char RECVER_DAT;
	RECVER_DAT=SX1276ReadBuffer_2( REG_LR_MODEMCONFIG1);
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG1_BW_MASK ) | ( bw << 4 );
	SX1276WriteBuffer_2( REG_LR_MODEMCONFIG1, RECVER_DAT );
	// LoRaSettings.SignalBw = bw;
}
	
void SX1276LoRaSetImplicitHeaderOn(BOOL enable )
{
	unsigned char RECVER_DAT;
	RECVER_DAT=SX1276ReadBuffer( REG_LR_MODEMCONFIG1 );
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK ) | ( enable );
	SX1276WriteBuffer( REG_LR_MODEMCONFIG1, RECVER_DAT );
}
void SX1276LoRaSetImplicitHeaderOn_2(BOOL enable )
{
	unsigned char RECVER_DAT;
	RECVER_DAT=SX1276ReadBuffer_2( REG_LR_MODEMCONFIG1 );
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK ) | ( enable );
	SX1276WriteBuffer_2( REG_LR_MODEMCONFIG1, RECVER_DAT );
}
	
void SX1276LoRaSetSymbTimeout(unsigned int value )
{
	unsigned char RECVER_DAT[2];
	RECVER_DAT[0]=SX1276ReadBuffer( REG_LR_MODEMCONFIG2 );
	RECVER_DAT[1]=SX1276ReadBuffer( REG_LR_SYMBTIMEOUTLSB );
	RECVER_DAT[0] = ( RECVER_DAT[0] & RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK ) | ( ( value >> 8 ) & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK );
	RECVER_DAT[1] = value & 0xFF;
	SX1276WriteBuffer( REG_LR_MODEMCONFIG2, RECVER_DAT[0]);
	SX1276WriteBuffer( REG_LR_SYMBTIMEOUTLSB, RECVER_DAT[1]);
}

void SX1276LoRaSetSymbTimeout_2(unsigned int value )
{
	unsigned char RECVER_DAT[2];
	RECVER_DAT[0]=SX1276ReadBuffer_2( REG_LR_MODEMCONFIG2 );
	RECVER_DAT[1]=SX1276ReadBuffer_2( REG_LR_SYMBTIMEOUTLSB );
	RECVER_DAT[0] = ( RECVER_DAT[0] & RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK ) | ( ( value >> 8 ) & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK );
	RECVER_DAT[1] = value & 0xFF;
	SX1276WriteBuffer_2( REG_LR_MODEMCONFIG2, RECVER_DAT[0]);
	SX1276WriteBuffer_2( REG_LR_SYMBTIMEOUTLSB, RECVER_DAT[1]);
}
	
void SX1276LoRaSetPayloadLength(unsigned char value )
{
  SX1276WriteBuffer( REG_LR_PAYLOADLENGTH, value );
} 
void SX1276LoRaSetPayloadLength_2(unsigned char value )
{
  SX1276WriteBuffer_2( REG_LR_PAYLOADLENGTH, value );
} 
	
void SX1276LoRaSetPreamLength(unsigned int value )
{
	unsigned char a[2];
	a[0]=(value&0xff00)>>8;
	a[1]=value&0xff;
	SX1276WriteBuffer( REG_LR_PREAMBLEMSB, a[0] );
	SX1276WriteBuffer( REG_LR_PREAMBLELSB, a[1] );
}
void SX1276LoRaSetPreamLength_2(unsigned int value )
{
	unsigned char a[2];
	a[0]=(value&0xff00)>>8;
	a[1]=value&0xff;
	SX1276WriteBuffer_2( REG_LR_PREAMBLEMSB, a[0] );
	SX1276WriteBuffer_2( REG_LR_PREAMBLELSB, a[1] );
}

	
void SX1276LoRaSetMobileNode(BOOL enable )
{	 
	unsigned char RECVER_DAT;
	RECVER_DAT=SX1276ReadBuffer( REG_LR_MODEMCONFIG3 );
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG3_MOBILE_NODE_MASK ) | ( enable << 3 );
	SX1276WriteBuffer( REG_LR_MODEMCONFIG3, RECVER_DAT );
}
void SX1276LoRaSetMobileNode_2(BOOL enable )
{	 
	unsigned char RECVER_DAT;
	RECVER_DAT=SX1276ReadBuffer_2( REG_LR_MODEMCONFIG3 );
	RECVER_DAT = ( RECVER_DAT & RFLR_MODEMCONFIG3_MOBILE_NODE_MASK ) | ( enable << 3 );
	SX1276WriteBuffer_2( REG_LR_MODEMCONFIG3, RECVER_DAT );
}


void SX1276Init(void)
{
	SX1276LoRaSetOpMode(Sleep_mode);  
	SX1276LoRaFsk(LORA_mode);	      
	SX1276LoRaSetOpMode(Stdby_mode);   
	SX1276WriteBuffer( REG_LR_DIOMAPPING1,GPIO_VARE_1);
	SX1276WriteBuffer( REG_LR_DIOMAPPING2,GPIO_VARE_2);
	SX1276LoRaSetRFFrequency();
	SX1276LoRaSetRFPower(powerValue);
	SX1276LoRaSetSpreadingFactor(SpreadingFactor);	 
	SX1276LoRaSetErrorCoding(CodingRate);		 
	SX1276LoRaSetPacketCrcOn(TRUE);			 
	SX1276LoRaSetSignalBandwidth( Bw_Frequency );	 
	SX1276LoRaSetImplicitHeaderOn(FLASE);		 
	SX1276LoRaSetPayloadLength( 0xff);
	SX1276LoRaSetSymbTimeout( 0x3FF );
	SX1276LoRaSetMobileNode(TRUE); 			  
	SX1276_RF_RECEIVE();
	//RF_SEELP();
}
	
void SX1276Init_2(void)
{
	SX1276LoRaSetOpMode_2(Sleep_mode);  
	SX1276LoRaFsk_2(LORA_mode);	      
	SX1276LoRaSetOpMode_2(Stdby_mode);   
	SX1276WriteBuffer_2( REG_LR_DIOMAPPING1,GPIO_VARE_1);
	SX1276WriteBuffer_2( REG_LR_DIOMAPPING2,GPIO_VARE_2);
	SX1276LoRaSetRFFrequency_2();
	SX1276LoRaSetRFPower_2(powerValue);
	SX1276LoRaSetSpreadingFactor_2(SpreadingFactor);	 
	SX1276LoRaSetErrorCoding_2(CodingRate);		 
	SX1276LoRaSetPacketCrcOn_2(TRUE);			 
	SX1276LoRaSetSignalBandwidth_2( Bw_Frequency );	 
	SX1276LoRaSetImplicitHeaderOn_2(FLASE);		 
	SX1276LoRaSetPayloadLength_2( 0xff);
	SX1276LoRaSetSymbTimeout_2( 0x3FF );
	SX1276LoRaSetMobileNode_2(TRUE); 			  
	SX1276_RF_RECEIVE_2();
	//RF_SEELP();
}
	
void SX1276_RF_SENDPACKET(unsigned char *RF_TRAN_P,unsigned char LEN)
{
	unsigned char ASM_i;
	lpTypefunc.paSwitchCmdfunc(txOpen);
	SX1276LoRaSetOpMode( Stdby_mode );
	SX1276WriteBuffer( REG_LR_HOPPERIOD, 0 );	          
	SX1276WriteBuffer( REG_LR_IRQFLAGSMASK,IRQN_TXD_Value); 
	SX1276WriteBuffer( REG_LR_PAYLOADLENGTH, LEN);	  
	SX1276WriteBuffer( REG_LR_FIFOTXBASEADDR, 0);
	SX1276WriteBuffer( REG_LR_FIFOADDRPTR, 0 );
	lpTypefunc.lpSwitchEnStatus(enOpen);
	lpTypefunc.lpByteWritefunc( 0x80 );
	for( ASM_i = 0; ASM_i < LEN; ASM_i++ ){
	  lpTypefunc.lpByteWritefunc( *RF_TRAN_P );
		RF_TRAN_P++;
	}
	lpTypefunc.lpSwitchEnStatus(enClose);
	SX1276WriteBuffer(REG_LR_DIOMAPPING1,0x40);
	SX1276WriteBuffer(REG_LR_DIOMAPPING2,0x00);
	SX1276LoRaSetOpMode( Transmitter_mode );
}

void SX1276_RF_SENDPACKET_2(unsigned char *RF_TRAN_P,unsigned char LEN)
{
	unsigned char ASM_i;
	lpTypefunc1.paSwitchCmdfunc_1(txOpen);
	SX1276LoRaSetOpMode_2( Stdby_mode );
	SX1276WriteBuffer_2( REG_LR_HOPPERIOD, 0 );	          
	SX1276WriteBuffer_2( REG_LR_IRQFLAGSMASK,IRQN_TXD_Value); 
	SX1276WriteBuffer_2( REG_LR_PAYLOADLENGTH, LEN);	  
	SX1276WriteBuffer_2( REG_LR_FIFOTXBASEADDR, 0);
	SX1276WriteBuffer_2( REG_LR_FIFOADDRPTR, 0 );
	lpTypefunc1.lpSwitchEnStatus_1(enOpen);
	lpTypefunc1.lpByteWritefunc_1( 0x80 );
	for( ASM_i = 0; ASM_i < LEN; ASM_i++ ){
	  lpTypefunc1.lpByteWritefunc_1( *RF_TRAN_P );
		RF_TRAN_P++;
	}
	lpTypefunc1.lpSwitchEnStatus_1(enClose);
	SX1276WriteBuffer_2(REG_LR_DIOMAPPING1,0x40);
	SX1276WriteBuffer_2(REG_LR_DIOMAPPING2,0x00);
	SX1276LoRaSetOpMode_2( Transmitter_mode );
}

void SX1276_RF_RECEIVE (void)
{
	SX1276LoRaSetOpMode(Stdby_mode );
	SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);  
	SX1276WriteBuffer(REG_LR_HOPPERIOD,	PACKET_MIAX_Value );
	SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X00 );
	SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0X00 );	
	SX1276LoRaSetOpMode( Receiver_mode );
	lpTypefunc.paSwitchCmdfunc(rxOpen);
}

void SX1276_RF_RECEIVE_2 (void)
{
	SX1276LoRaSetOpMode_2(Stdby_mode );
	SX1276WriteBuffer_2(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);  
	SX1276WriteBuffer_2(REG_LR_HOPPERIOD,	PACKET_MIAX_Value );
	SX1276WriteBuffer_2( REG_LR_DIOMAPPING1, 0X00 );
	SX1276WriteBuffer_2( REG_LR_DIOMAPPING2, 0X00 );	
	SX1276LoRaSetOpMode_2( Receiver_mode );
	lpTypefunc1.paSwitchCmdfunc_1(rxOpen);
}
	
void SX1276_RF_CAD_RECEIVE (void)
{
	SX1276LoRaSetOpMode( Stdby_mode );
	SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,  IRQN_CAD_Value);	
	SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X80 );
	SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0X00 );	
	SX1276LoRaSetOpMode( CAD_mode );
	lpTypefunc.paSwitchCmdfunc(rxOpen);
}
	
void SX1276_RF_CAD_RECEIVE_2 (void)
{
	SX1276LoRaSetOpMode_2( Stdby_mode );
	SX1276WriteBuffer_2(REG_LR_IRQFLAGSMASK,  IRQN_CAD_Value);	
	SX1276WriteBuffer_2( REG_LR_DIOMAPPING1, 0X80 );
	SX1276WriteBuffer_2( REG_LR_DIOMAPPING2, 0X00 );	
	SX1276LoRaSetOpMode_2( CAD_mode );
	lpTypefunc1.paSwitchCmdfunc_1(rxOpen);
}
	

void SX1276_RF_SEELP(void)
{
	SX1276LoRaSetOpMode( Stdby_mode );
	SX1276WriteBuffer(REG_LR_IRQFLAGSMASK,  IRQN_SEELP_Value);  
	SX1276WriteBuffer( REG_LR_DIOMAPPING1, 0X00 );
	SX1276WriteBuffer( REG_LR_DIOMAPPING2, 0X00 );	
	SX1276LoRaSetOpMode( Sleep_mode );
}

void SX1276_RF_SEELP_2(void)
{
	SX1276LoRaSetOpMode_2( Stdby_mode );
	SX1276WriteBuffer_2(REG_LR_IRQFLAGSMASK,  IRQN_SEELP_Value);  
	SX1276WriteBuffer_2( REG_LR_DIOMAPPING1, 0X00 );
	SX1276WriteBuffer_2( REG_LR_DIOMAPPING2, 0X00 );	
	SX1276LoRaSetOpMode_2( Sleep_mode );
}

void SX1276_Interupt(void)
{
  unsigned int	i;  
  unsigned char RF_REC_RLEN_i;
  RF_EX0_STATUS=SX1276ReadBuffer( REG_LR_IRQFLAGS ); 
  if((RF_EX0_STATUS&0x08)==0x08){	
    SX1276_RF_RECEIVE();
		SX1276_RXD_LED_L;
  }
  if((RF_EX0_STATUS&0x40)==0x40){
    CRC_Value=SX1276ReadBuffer( REG_LR_MODEMCONFIG2 );
    if((CRC_Value&0x04)==0x04){
      SX1276WriteBuffer (REG_LR_FIFOADDRPTR,0x00);
      SX1278_RLEN = SX1276ReadBuffer(REG_LR_NBRXBYTES);
      lpTypefunc.lpSwitchEnStatus(enOpen);
      lpTypefunc.lpByteWritefunc( 0x00 );
      for(RF_REC_RLEN_i=0;RF_REC_RLEN_i<SX1278_RLEN;RF_REC_RLEN_i++){
        if (RF_REC_RLEN_i<32) SX1276_RF_RXBuffer[RF_REC_RLEN_i]=lpTypefunc.lpByteReadfunc();		    
      }
      lpTypefunc.lpSwitchEnStatus(enClose);
    }
    for(i=0;i<SX1278_RLEN;i++){
      if (i<32) RF_RXBuffer[i]=SX1276_RF_RXBuffer[i];
    }
    SX1276LoRaSetOpMode( Stdby_mode );
    RECEIVE_DATA();
    Responed_RF0_TX_In_Main_Flage=1;//
    SX1276_RXD_LED_H;		
  }
//  SX1276WriteBuffer( REG_LR_IRQFLAGS, 0xff  );	
//  Responed_RF0_TX_In_Main_Flage=1;//	SX1276_RF_RECEIVE();
}



//*********************************************
void SX1276_2_Interupt(void)
{
  unsigned int	i;  
  unsigned char RF_REC_RLEN_i;
  RF_EX1_STATUS=SX1276ReadBuffer_2( REG_LR_IRQFLAGS ); 
  if((RF_EX1_STATUS&0x08)==0x08)
  {	
    SX1276_RF_RECEIVE_2();
		SX1276_2_RXD_LED_L;
  }
  if((RF_EX1_STATUS&0x40)==0x40)//receive
  {
    CRC_Value=SX1276ReadBuffer_2( REG_LR_MODEMCONFIG2 );
    if(CRC_Value&0x04==0x04)
    {
      SX1276WriteBuffer_2(REG_LR_FIFOADDRPTR,0x00);
      SX1278_RLEN = SX1276ReadBuffer_2(REG_LR_NBRXBYTES);
      lpTypefunc1.lpSwitchEnStatus_1(enOpen);
      lpTypefunc1.lpByteWritefunc_1( 0x00 );
      for(RF_REC_RLEN_i=0;RF_REC_RLEN_i<SX1278_RLEN;RF_REC_RLEN_i++){
        if (RF_REC_RLEN_i<32) SX1276_2_RF_RXBuffer[RF_REC_RLEN_i]=lpTypefunc1.lpByteReadfunc_1();		    
      }
      lpTypefunc1.lpSwitchEnStatus_1(enClose);
    }
    for(i=0;i<SX1278_RLEN;i++)
    {
      if (i<32) RF_2_RXBuffer[i]=SX1276_2_RF_RXBuffer[i];
    }
    SX1276LoRaSetOpMode_2( Stdby_mode );
    RECEIVE_2_DATA();
		SingalStatus=0x01;        //信号
		Responed_RF1_TX_In_Main_Flage=1;//
		SX1276_2_RXD_LED_H;
  }
//  SX1276WriteBuffer( REG_LR_IRQFLAGS, 0xff  );
	//Responed_RF0_TX_In_Main_Flage=1;//SX1276_RF_RECEIVE();
}
