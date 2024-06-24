#include "Define.h"

#define TRUE                1
#define FALSE               0
#define EEPROM_DEV_ADDR     0xa0

unsigned char  EEprom_Err_Flage;   //0:正常   1：错误

extern void EEprom_SDA_SET_IN(void);
extern void EEprom_SDA_SET_OUT(void);
extern void DelayUs(uint32_t count);
extern void DelayMs(uint32_t count);

void IIC_Start(void)
{
	EEprom_SDA_SET_OUT();
	DelayUs(4);
	EEprom_SDA_HIGH;
	DelayUs(4);
	EEprom_SCL_HIGH;
	DelayUs(4);
	EEprom_SDA_LOW;
	DelayUs(4);
	EEprom_SCL_LOW;
	DelayUs(4);
}

void IIC_Stop(void)
{
	EEprom_SDA_SET_OUT();
	EEprom_SDA_LOW;
	DelayUs(4);
	EEprom_SCL_HIGH;
	DelayUs(4);
	EEprom_SDA_LOW;
	DelayUs(4);
	EEprom_SDA_HIGH;
}

void IIC_SendAck(void)
{
	EEprom_SDA_SET_OUT();
	EEprom_SDA_LOW;
	DelayUs(4);
	EEprom_SCL_LOW;
	DelayUs(4);
	EEprom_SCL_HIGH;
	DelayUs(4);
	EEprom_SCL_LOW;
	DelayUs(4);
	EEprom_SDA_HIGH;
}

void IIC_SendNoAck(void)
{
	EEprom_SDA_SET_OUT();
	EEprom_SDA_HIGH;
	DelayUs(4);
	EEprom_SCL_LOW;
	DelayUs(4);
	EEprom_SCL_HIGH;
	DelayUs(4);
	EEprom_SCL_LOW;
}

unsigned char IIC_Check_Ack(void)
{
	unsigned char AckStatus;
	EEprom_SDA_SET_IN();
	EEprom_SCL_HIGH;
	DelayUs(4);
	if(EEprom_SDA_VALUE!=0){
		AckStatus = FALSE;
	}else{
		AckStatus = TRUE;
	}
	EEprom_SCL_LOW;
	DelayUs(4);
	EEprom_SDA_SET_OUT();
	return AckStatus;
}

void IIC_SendByte(unsigned char data)
{
	unsigned char tmp;
	EEprom_SDA_SET_OUT();
	for(tmp=0;tmp<8;tmp++){
		if(data & 0x80){
			 EEprom_SDA_HIGH;
		}else{
			EEprom_SDA_LOW;
		}
		DelayUs(4);
		EEprom_SCL_HIGH;
		DelayUs(4);
		EEprom_SCL_LOW;
		DelayUs(4);
		data <<= 1;
	}
	DelayUs(4);
}

unsigned char IIC_RevByte(void)
{
	unsigned char tmp;
	unsigned char DATA=0;
	EEprom_SDA_SET_IN();
	EEprom_SCL_LOW;
	DelayUs(4);
	for(tmp=0;tmp<8;tmp++){
		EEprom_SCL_HIGH;
		DelayUs(4);
		DATA <<= 1;
		if(EEprom_SDA_VALUE!=0){
		 DATA |= 0x01;
		}else{
		 DATA &= 0xfe;
		}
		EEprom_SCL_LOW;
	}
	EEprom_SDA_SET_OUT();
	return DATA;
}

unsigned char EEprom_Byte1Write_No_Check(unsigned char addr,unsigned char data)
{
	IIC_Start();
	IIC_SendByte(EEPROM_DEV_ADDR);
	if(IIC_Check_Ack() == FALSE) goto AAA;
	IIC_SendByte(addr);
	if(IIC_Check_Ack() == FALSE) goto AAA;
	IIC_SendByte(data);
	if(IIC_Check_Ack() == FALSE) goto AAA;
	IIC_Stop();
	DelayMs(5);
	return TRUE;
AAA:	
	IIC_Stop();
	DelayMs(5);
	return FALSE;
}

unsigned char EEprom_Byte1Read_No_Check(unsigned char addr)
{
	unsigned char data;
	IIC_Start();
	IIC_SendByte(EEPROM_DEV_ADDR);
	if(IIC_Check_Ack() == FALSE) goto BBB;
	DelayUs(1);
	IIC_SendByte(addr);
	if(IIC_Check_Ack() == FALSE) goto BBB;
	DelayUs(1);
	IIC_Start();
	IIC_SendByte(EEPROM_DEV_ADDR+1);
	if(IIC_Check_Ack() == FALSE) goto BBB;
	DelayUs(1);
	data = IIC_RevByte();
	IIC_SendNoAck();
	IIC_Stop();
	DelayUs(100);
	return data;
BBB:
  IIC_Stop();
	DelayMs(5);
	return FALSE;
}

unsigned char EEprom_Byte1Read(unsigned char addr)
{
	unsigned char data[3]={0};
	unsigned char i;
	unsigned char r=0;
	EEprom_Err_Flage=0; //清除错误标识
	for(i=0;i<2;i++)data[i]=EEprom_Byte1Read_No_Check(addr);
	if(data[0]==data[1]) r=data[0];
	else{
		data[2]=EEprom_Byte1Read_No_Check(addr);
		if(data[0]==data[2]) r=data[0];
		else if(data[1]==data[2]) r=data[1];
		else{
		 EEprom_Err_Flage=1; //错误标识
		 r=EEprom_Err_Flage;
		}
	}
	return r;
}

unsigned char EEprom_Byte1Write(unsigned char addr,unsigned char data)
{
	unsigned char i;
	EEprom_Err_Flage=0; //清除错误标识
	for(i=0;i<3;i++){
		EEprom_Byte1Write_No_Check(addr,data);
		if(EEprom_Byte1Read(addr)==data) goto CCC;  //写完读出校验正确
	}
	EEprom_Err_Flage=1;       //3次都不正确，标记错误
CCC:	
	return EEprom_Err_Flage;
}

void EEprom_Byte2Write(unsigned char addr,unsigned int data)
{  
	unsigned char i,j;
  unsigned char err[2]={0};
	i=data/256;j=data%256;
	err[0]=EEprom_Byte1Write(addr,i);
	err[1]=EEprom_Byte1Write(addr+1,j);
	if((err[0]==0)&&(err[1]==0)) EEprom_Err_Flage=0; //正常
	else EEprom_Err_Flage=1;                         //故障失败 
}

unsigned int EEprom_Byte2Read(unsigned char addr)
{
	unsigned int EE_Data2Byte;
	unsigned int r=0;
	EE_Data2Byte=EEprom_Byte1Read(addr);
	if(EEprom_Err_Flage!=0) goto DDD;//失败
	EE_Data2Byte=EE_Data2Byte*256;
	EE_Data2Byte=EE_Data2Byte + EEprom_Byte1Read(addr+1);
	if(EEprom_Err_Flage!=0) goto DDD;//失败
	else{
		r=EE_Data2Byte;
		goto EEE;
	}
DDD:	
	EEprom_Err_Flage=1;
	return EEprom_Err_Flage;
EEE:	
	return r;	
}
