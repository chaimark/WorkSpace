#include "main.h"
#include "protocol.h"
#include "HttpApp.h"
#include "JsonApp.h"

extern uint8_t wifi_bumber[4];
extern unsigned char Cault_CS(unsigned char *Temp_Bufferf,unsigned char start_num,unsigned char stop_num);

/*模拟定义设备的外部参数, 后期修改 Data_Interaction.h 从外部引入参数*/
char gateway_id[40] = "\"02345678903\"";
char service_type[40] = "\"TCPMQTT\"";
char server_address[40] = "\"hysmartmeter.com\"";
char server_port[40] = "\"1883\"";
char mbus_baud_rate[40] = "\"2400,E,8,1\"";
char rs485_baud_rate[40] = "\"9600,N,8,1\"";
char uplink_comm[40] = "\"4G+USB+WIFI\"";
char downlink_comm[40] = "\"MBUS+RS485*2\"";
char touch_Data[500] = TOUCH_JSON;

static void protocol20_receive_clean(protocol_manager_t *manager)
{
	manager->receiveBufLen = 0;
	manager->frameLen = -1;
}

// 心跳包返回
static void protocol_cmd_2f(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
}

// 定时包返回
static void protocol_cmd_2b(protocol_manager_t *manager, uint8_t *data, uint16_t len)
{
	*manager->host_confirm =  *data;
}
// 读取网关时间
static void protocol_cmd_01(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
//	manager->sendBuf[PROTOCOL_VALUE_OFFSET + 0] = RTC_DATA.year;
//	manager->sendBuf[PROTOCOL_VALUE_OFFSET + 1] = RTC_DATA.month;
//	manager->sendBuf[PROTOCOL_VALUE_OFFSET + 2] = RTC_DATA.day;
//	manager->sendBuf[PROTOCOL_VALUE_OFFSET + 3] = RTC_DATA.hour;
//	manager->sendBuf[PROTOCOL_VALUE_OFFSET + 4] = RTC_DATA.minute;
//	manager->sendBuf[PROTOCOL_VALUE_OFFSET + 5] = RTC_DATA.second;

	manager->protocol_send(manager, 0x01, 6);
}
// 网关时间矫正（对时）
static void protocol_cmd_02(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
//	FL_RTC_InitTypeDef rtc;

//	rtc.second = data[5];
//	rtc.minute = data[4];
//	rtc.hour   = data[3];
//	rtc.day    = data[2];
//	rtc.month  = data[1];
//	rtc.year   = data[0];

//	R8025_set_rtc(&rtc);

	manager->sendBuf[PROTOCOL_VALUE_OFFSET] = PROTOCOL_SUCESS_FLAG; // 回复成功
	manager->protocol_send(manager, 0x02, 1);
}

// 读网关号码
static void protocol_cmd_03(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
	int i;

	if (manager->gw_addr != NULL)
	{
		for (i = 0; i < 5; i++) 
			manager->sendBuf[PROTOCOL_VALUE_OFFSET + i] = manager->gw_addr[i];
	}
	else
	{
		for (i = 0; i < 5; i++) 
			manager->sendBuf[PROTOCOL_VALUE_OFFSET + i] = 0;
	}

	manager->protocol_send(manager, 0x03, 5);
}

// 修改网关号码
static void protocol_cmd_04(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
	int i;

	if (manager->gw_addr != NULL)
	{
		for (i = 0; i < 5; i++) 
			manager->gw_addr[i] = data[i];

		manager->data_save(manager->gw_addr, 5);
		manager->sendBuf[PROTOCOL_VALUE_OFFSET] = PROTOCOL_SUCESS_FLAG; // 回复成功
	}
	else
	{
		manager->sendBuf[PROTOCOL_VALUE_OFFSET] = PROTOCOL_FAIL_FLAG; // 回复失败
	}

	manager->protocol_send(manager, 0x04, 1);
}

// 读取IP/端口
static void protocol_cmd_05(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
	int i;
	if (manager->ip != NULL) 
	{
		for (i = 0; i < 4; i++) 
			manager->sendBuf[PROTOCOL_VALUE_OFFSET + i] = manager->ip[i];

		manager->sendBuf[PROTOCOL_VALUE_OFFSET + 4] = manager->ip[5];
		manager->sendBuf[PROTOCOL_VALUE_OFFSET + 5] = manager->ip[4];
		manager->protocol_send(manager, 0x05, 6);
	}
}

// 修改IP/端口
static void protocol_cmd_06(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
	int i;
	if (manager->ip != NULL) 
	{
		for (i = 0; i < 4; i++) 
			manager->ip[i] = data[i];

		manager->ip[4] = data[5];
		manager->ip[5] = data[4];  // mcu是小端

		manager->data_save(manager->ip, 6);
		manager->sendBuf[PROTOCOL_VALUE_OFFSET] = PROTOCOL_SUCESS_FLAG; // 回复成功
	}
	else
	{
		manager->sendBuf[PROTOCOL_VALUE_OFFSET] = PROTOCOL_FAIL_FLAG; // 回复失败
	}

	manager->protocol_send(manager, 0x06, 1);
}

// 读上报周期
static void protocol_cmd_07(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
	if (manager->auto_time != NULL) 
	{
		uint8_t *time = (uint8_t * )(manager->auto_time);
		manager->sendBuf[PROTOCOL_VALUE_OFFSET + 0] = time[1];
		manager->sendBuf[PROTOCOL_VALUE_OFFSET + 1] = time[0];

		manager->protocol_send(manager, 0x07, 2);
	}
}

// 设置自动上报周期
static void protocol_cmd_08(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
	if (manager->auto_time != NULL) 
	{
		uint8_t *time = (uint8_t * )(manager->auto_time);
		time[1] = data[0];
		time[0] = data[1];

		manager->data_save(time, 6);
		manager->sendBuf[PROTOCOL_VALUE_OFFSET] = PROTOCOL_SUCESS_FLAG; // 回复成功
	}
	else
	{
		manager->sendBuf[PROTOCOL_VALUE_OFFSET] = PROTOCOL_FAIL_FLAG; // 回复失败
	}

	manager->protocol_send(manager, 0x08, 1);
}

// 读控制器地址
static void protocol_cmd_09(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
//	uint8_t group = data[0]; // 第几组
//	uint8_t temp_u8;

//	if((group >= 1) && (group <= 12))
//	{
//		temp_u8 = (group - 1) * 20;  // 从第几个地址开始

//		manager->sendBuf[PROTOCOL_VALUE_OFFSET] = lora_equ.lora_node_num;
//		manager->sendBuf[PROTOCOL_VALUE_OFFSET + 1] = group;

//		memcpy(manager->sendBuf + PROTOCOL_VALUE_OFFSET + 2, &lora_equ.lora_node[temp_u8], 80);
//		manager->protocol_send(manager, 0x09, 82);
//	}
}

// 写控制器地址
static void protocol_cmd_0A(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
//	uint8_t num = data[0];   // 总数量
//	uint8_t group = data[1]; // 第几组
//	uint8_t temp_u8;

//	if(((group >= 1) || (group <= 12)) && num <= 240)  // 表示第几组，每组20个地址，80字节
//	{  
//		temp_u8 = (group - 1) * 20;  // 从第几个地址开始
//		memcpy(&lora_equ.lora_node[temp_u8], data + 2, 80);

//		// 数量
//		lora_equ.lora_node_num = num;

//		// 填充
//		if (num < 240)
//			memset(&lora_equ.lora_node[num], 0xff, 4);

//		// 保存结果
//		lora_write_equ();

//		manager->sendBuf[PROTOCOL_VALUE_OFFSET] = PROTOCOL_SUCESS_FLAG; // 回复成功
//		manager->protocol_send(manager, 0x0A, 1);
//	}		       
}

// 控制开关阀
static void protocol_cmd_31(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
//	manager->lora_cmd(0x31, (lora_node_t *)(manager->receiveBuf + PROTOCOL_LORA_NODE_ADDR_OFFSET), len, data);
}
// 启动一次抄收数据并上传， 启动一次抄表
static void protocol_cmd_32(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
	manager->lora_cmd(0x32, NULL, len, data);

	manager->sendBuf[PROTOCOL_VALUE_OFFSET] = PROTOCOL_SUCESS_FLAG; // 回复成功
	manager->protocol_send(manager, 0x32, 1);

}

// 停止抄表
static void protocol_cmd_34(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
	manager->lora_cmd(0x34, NULL, len, data);

	manager->sendBuf[PROTOCOL_VALUE_OFFSET] = PROTOCOL_SUCESS_FLAG; // 回复成功
	manager->protocol_send(manager, 0x34, 1);

}

// 网关复位
static void protocol_cmd_40(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
	manager->sendBuf[PROTOCOL_VALUE_OFFSET] = PROTOCOL_SUCESS_FLAG; // 回复成功
	manager->protocol_send(manager, 0x40, 1);

	DelayMs(1000);
	// reset
//	FL_RCC_SetSoftReset();
}



// 读阀门控制器数据
static void protocol_cmd_33(protocol_manager_t *manager, uint8_t *data, uint16_t len) 
{
//	manager->lora_cmd(0x33, (lora_node_t *)(manager->receiveBuf + PROTOCOL_LORA_NODE_ADDR_OFFSET), len, data);
}

static protocol_fun_cmd_t protocol_fun_cmd[] = 
{
	{ 0x2f, protocol_cmd_2f, 01 } ,  //*心跳回复 // 主动发的回复
	{ 0x2b, protocol_cmd_2b, 01 } ,  //*周期回复 // 主动发的回复

	{ 0x01, protocol_cmd_01, 01 } ,  //*读取网关时间
	{ 0x02, protocol_cmd_02, 06 } ,  //*网关时间矫正（对时）

	{ 0x03, protocol_cmd_03, 01 } ,  //*读网关号码
	{ 0x04, protocol_cmd_04, 05 } ,  //*修改网关号码

	{ 0x05, protocol_cmd_05, 01 } ,  //*读取IP/端口
	{ 0x06, protocol_cmd_06, 06 } ,  //*修改IP/端口

	{ 0x07, protocol_cmd_07, 01 } ,  //*读上报周期
	{ 0x08, protocol_cmd_08, 02 } ,  //*设置自动上报周期

	{ 0x09, protocol_cmd_09, 1 } ,  //*读控制器地址
	{ 0x0A, protocol_cmd_0A, 82 } , //*写控制器地址


	{ 0x31, protocol_cmd_31, 02 } ,  //*开关阀
	{ 0x32, protocol_cmd_32, 01 } ,  //*启动一次抄表
	{ 0x40, protocol_cmd_40, 01 } ,  //*系统复位
	{ 0x33, protocol_cmd_33, 01 } ,  //*读阀门控制器数据
	{ 0x34, protocol_cmd_34, 01 } ,  //*停止抄表
	{ 0x00, NULL                } ,

	// 0xB1：上传开关阀门记录
	// 0X33 / 0x2B（0xB3 / 0xAB）：主动上传阀门控制器数据
};

static void protocol_receive(protocol_manager_t *manager, uint8_t *receive, uint16_t *length)
{
	// 放到缓存中
	if (manager->receiveBufLen + *length > PROTOCOL_REC_BUF)  // 溢出
	{
		if (*length <= PROTOCOL_REC_BUF)    //
		{
			memcpy(manager->receiveBuf, receive, *length);
			manager->receiveBufLen = *length;
		}
		else
		{
			protocol20_receive_clean(manager);
		}
	}
	else
	{
		memcpy(manager->receiveBuf + manager->receiveBufLen, receive, *length);
		manager->receiveBufLen += *length;
	}

	*length = 0;
	manager->tick = 0;
}

// 完整的接收一帧报文，返回true 
static bool protocol_analysis(protocol_manager_t *manager, bool isRef)
{
	uint16_t  check_len;
	uint16_t  crc_u16;
	int complete = 0;
	int i;

	if (!isRef && manager->receiveBufLen > 0) // 如果没有新数据，并且接收缓冲器不为空
	{
		if (manager->tick >= 200) // 2s 
			protocol20_receive_clean(manager);

		return false;
	}

	// 没收到报文长度
	if (manager->frameLen == -1) 
	{
		if (manager->receiveBufLen >= 1)
		{
			if (manager->receiveBuf[0] != PROTOCOL_BEGIN)
				goto error;
		}

		if (manager->receiveBufLen >= 2)
		{
			if (manager->receiveBuf[1] != PROTOCOL_BEGIN)
				goto error;
		}
	}

	// 报文长度
	if (manager->receiveBufLen >= PROTOCOL_LEN_OFFSET + 2)
		manager->frameLen = (((uint16_t)manager->receiveBuf[PROTOCOL_LEN_OFFSET]) << 8) | manager->receiveBuf[PROTOCOL_LEN_OFFSET + 1];
	else 
		return false;
	
	//数据头
	if (manager->receiveBufLen >= PROTOCOL_DATABEG_OFFSET + 1)
	{
		if (manager->receiveBuf[PROTOCOL_DATABEG_OFFSET] != PROTOCOL_DATABEG)
			goto error;
	}
	else 
		return false;

	// 数据尾部
	check_len = PROTOCOL_DATABEG_OFFSET + manager->frameLen + 2;
	if (manager->frameLen != -1 && manager->receiveBufLen >= check_len)
	{
		if (manager->receiveBuf[check_len - 1] != PROTOCOL_REC_DATAEND)
			goto error;
	}
	else 
		return false;

	// CRC
	if (manager->receiveBufLen >= check_len + 2)
	{
//		crc_u16 = check_crc16(manager->receiveBuf, check_len);	

		if ( (crc_u16 >> 8) == manager->receiveBuf[check_len] || (crc_u16 & 0xff) == manager->receiveBuf[check_len + 1]) 
			complete = 1;
		else
			goto error;

	}
	else 
		return false;
	
	// 完成
	if (complete == 1) 
	{
		//  如果不校验地址，或者地址匹配，则可以解析报文了
		if (manager->gw_addr == NULL ||
				(manager->gw_addr[0] == manager->receiveBuf[PROTOCOL_NODE_ADDR_OFFSET] &&
					manager->gw_addr[1] == manager->receiveBuf[PROTOCOL_NODE_ADDR_OFFSET + 1] &&
					manager->gw_addr[2] == manager->receiveBuf[PROTOCOL_NODE_ADDR_OFFSET + 2] &&
					manager->gw_addr[3] == manager->receiveBuf[PROTOCOL_NODE_ADDR_OFFSET + 3] &&
					manager->gw_addr[4] == manager->receiveBuf[PROTOCOL_NODE_ADDR_OFFSET + 4])   ||
				(0xff == manager->receiveBuf[PROTOCOL_NODE_ADDR_OFFSET] &&
					0xff == manager->receiveBuf[PROTOCOL_NODE_ADDR_OFFSET + 1] &&
					0xff == manager->receiveBuf[PROTOCOL_NODE_ADDR_OFFSET + 2] &&
					0xff == manager->receiveBuf[PROTOCOL_NODE_ADDR_OFFSET + 3] &&
					0xff == manager->receiveBuf[PROTOCOL_NODE_ADDR_OFFSET + 4]))
		{
			for (i = 0; protocol_fun_cmd[i].cmd != 0; i++) 
			{
				if (protocol_fun_cmd[i].cmd == manager->receiveBuf[PROTOCOL_CMD_OFFSET] &&
						manager->frameLen ==  protocol_fun_cmd[i].data_len)
				{
					protocol_fun_cmd[i].fun_cmd(manager, manager->receiveBuf + PROTOCOL_VALUE_OFFSET, manager->frameLen);
				}
			}
		}
	}

	// 把剩余的拷贝到前面，准备继续解析
	if (manager->receiveBufLen > check_len + 2)
	{
		memcpy(manager->receiveBuf, manager->receiveBuf + check_len + 2, manager->receiveBufLen - check_len - 2);
		manager->receiveBufLen = manager->receiveBufLen - check_len - 2;
		manager->frameLen = -1;
	}
	else
		protocol20_receive_clean(manager);

	return true;

error:
	protocol20_receive_clean(manager);
	return false;
}

void protocol_tick_add(protocol_manager_t *manager)
{
	manager->tick ++;
}


static void protocol_set_gw_addr(protocol_manager_t *manager, uint8_t *gw_addr)
{
	manager->gw_addr = gw_addr;
}

// 回调函数
static void protocol_set_fun(protocol_manager_t *manager, uint8_t (* dataSave)(uint8_t*, uint8_t),
		void (* loraCmd)(uint8_t, lora_node_t *, uint8_t, uint8_t *))
{
	manager->data_save = dataSave;
	manager->lora_cmd = loraCmd;
}

static void protocol_set_param(protocol_manager_t *manager, uint8_t *addr, uint16_t *auto_time, uint8_t *host_confirm, uint8_t com_sign)
{
	manager->ip = addr;
	manager->auto_time = auto_time;
	manager->host_confirm = host_confirm;
	manager->com_sign = com_sign;
}

static void protocol_send(protocol_manager_t *manager, uint8_t cmd, uint16_t len)
{
	uint16_t crc; 

	if (len + PROTOCOL_FIX_LEN > manager->sendBufMax)  // 超过发送缓冲区
		return;

	manager->sendBufLen = 0;
	manager->sendBuf[manager->sendBufLen++] = PROTOCOL_BEGIN;
	manager->sendBuf[manager->sendBufLen++] = PROTOCOL_BEGIN; // 头
	manager->sendBuf[manager->sendBufLen++] = 0x01;           // 中心站地址

	// 设备地址
	if (manager->gw_addr == NULL)
		memset(manager->sendBuf + manager->sendBufLen, 0xff, 5);
	else
		memcpy(manager->sendBuf + manager->sendBufLen, manager->gw_addr, 5);
	manager->sendBufLen += 5;

	manager->sendBuf[manager->sendBufLen++] = 0x00;              // 中继地址
	manager->sendBuf[manager->sendBufLen++] = 0x00;              // 节点地址 高
	manager->sendBuf[manager->sendBufLen++] = 0x00;              // 节点地址 低
	manager->sendBuf[manager->sendBufLen++] = 0x00;              // 设备类型
	manager->sendBuf[manager->sendBufLen++] = cmd;               // 命令
	manager->sendBuf[manager->sendBufLen++] = 0x00;              // 上行
	manager->sendBuf[manager->sendBufLen++] = (len >> 8) & 0xff; // 数据长度
	manager->sendBuf[manager->sendBufLen++] = len & 0xff;        // 数据长度

	manager->sendBuf[manager->sendBufLen++] = PROTOCOL_DATABEG; // 数据开始         

	//memcpy(manager->sendBuf + manager->sendBufLen, data, len); // 有效数据
	manager->sendBufLen += len;  // 跳过有效数据

	manager->sendBuf[manager->sendBufLen++] = PROTOCOL_SEND_DATAEND; // 数据结束


//	crc = check_crc16(manager->sendBuf, manager->sendBufLen);
	
	manager->sendBuf[manager->sendBufLen++] = crc >> 8;
	manager->sendBuf[manager->sendBufLen++] = crc & 0xff;

	manager->send_data(manager->sendBuf, manager->sendBufLen);
}

// 主动上行发送
// flag == 0x80: 表示重发，直接发送 sendBuf 缓存即可
static void protocol_send_lora(protocol_manager_t *manager, uint8_t cmd, lora_node_t *lora_node, uint16_t len, uint8_t *data, uint8_t flag)
{
	uint16_t crc; 

	// 重发
	if (flag == 0x80) 
	{
		manager->send_data(manager->sendBuf, manager->sendBufLen);
		return;
	}

	// 超过发送缓冲区
	if (len + 8 + PROTOCOL_FIX_LEN > manager->sendBufMax)  
		return;

	// 有些lora上来的数据 需要 添加时间
	if (cmd == 0x33)
	{
//		data[len++] = RTC_DATA.year;
//		data[len++] = RTC_DATA.month;
//		data[len++] = RTC_DATA.day;
//		data[len++] = RTC_DATA.hour;
//		data[len++] = RTC_DATA.minute;
//		data[len++] = RTC_DATA.second;  // 时间
		// // 特殊处理
		// if (cmd == 0x31)
		// 	data[len++] = 0x00;
	}

	// 协议开始 ***********
	manager->sendBufLen = 0;
	manager->sendBuf[manager->sendBufLen++] = PROTOCOL_BEGIN;
	manager->sendBuf[manager->sendBufLen++] = PROTOCOL_BEGIN; // 头
	manager->sendBuf[manager->sendBufLen++] = 0x01;           // 中心站地址

	// 设备地址
	if (manager->gw_addr == NULL)
		memset(manager->sendBuf + manager->sendBufLen, 0xff, 5);
	else
		memcpy(manager->sendBuf + manager->sendBufLen, manager->gw_addr, 5);
	manager->sendBufLen += 5;

	if (lora_node != NULL) 
	{
		uint8_t *p =(uint8_t *)lora_node; 
		manager->sendBuf[manager->sendBufLen++] = p[0];  // 中继地址
		manager->sendBuf[manager->sendBufLen++] = p[1];  // 
		manager->sendBuf[manager->sendBufLen++] = p[2];  // 节点地址, 直接转发，否则大小端不一致
		manager->sendBuf[manager->sendBufLen++] = p[3];  // 设备类型
	}
	else
	{
		manager->sendBuf[manager->sendBufLen++] = 0x00;              // 中继地址
		manager->sendBuf[manager->sendBufLen++] = 0x00;              // 节点地址 高
		manager->sendBuf[manager->sendBufLen++] = 0x00;              // 节点地址 低
		manager->sendBuf[manager->sendBufLen++] = 0x00;              // 设备类型
	}

	manager->sendBuf[manager->sendBufLen++] = cmd;               // 命令
	manager->sendBuf[manager->sendBufLen++] = 0x00;              // 上行
	manager->sendBuf[manager->sendBufLen++] = (len >> 8) & 0xff; // 数据长度
	manager->sendBuf[manager->sendBufLen++] = len & 0xff;        // 数据长度

	manager->sendBuf[manager->sendBufLen++] = PROTOCOL_DATABEG; // 数据开始         

	memcpy(manager->sendBuf + manager->sendBufLen, data, len); // 有效数据
	manager->sendBufLen += len;

	manager->sendBuf[manager->sendBufLen++] = PROTOCOL_SEND_DATAEND; // 数据结束


//	crc = check_crc16(manager->sendBuf, manager->sendBufLen);
	
	manager->sendBuf[manager->sendBufLen++] = crc >> 8;
	manager->sendBuf[manager->sendBufLen++] = crc & 0xff;

	manager->send_data(manager->sendBuf, manager->sendBufLen);
}

static void send_heart(protocol_manager_t *manager, uint8_t rssi, char *ccid)
{
//	int i;
//	int temp = PROTOCOL_VALUE_OFFSET;

//	manager->sendBuf[temp++] = RTC_DATA.year;
//	manager->sendBuf[temp++] = RTC_DATA.month;
//	manager->sendBuf[temp++] = RTC_DATA.day;
//	manager->sendBuf[temp++] = RTC_DATA.hour;
//	manager->sendBuf[temp++] = RTC_DATA.minute;
//	manager->sendBuf[temp++] = RTC_DATA.second;  // 时间

//	manager->sendBuf[temp++] = rssi;  // 信号强度

//	for (i = 0; i < 10; i++) 
//		manager->sendBuf[temp++] = ccid[i];
//	
//	manager->sendBuf[temp++] = VERSION;

//	manager->protocol_send(manager, 0x2f, temp - PROTOCOL_VALUE_OFFSET);
}

//unsigned char Cault_CS(unsigned char *Temp_Bufferf,unsigned char start_num,unsigned char stop_num)//
//{
//  unsigned char temp,cs;
//  cs=0;
//  for(temp=start_num;temp<stop_num;temp++){
//    cs=cs+*(Temp_Bufferf+temp);
//  }
//  return cs;
//}
// 主动上行发送
// flag == 0x80: 表示重发，直接发送 sendBuf 缓存即可
static void protocol_send_data(protocol_manager_t *manager, uint8_t cmd, uint16_t len, uint8_t *data, uint8_t flag)
{
//	uint16_t crc; 

	// 重发
	if (flag == 0x80) 
	{
		manager->send_data(manager->sendBuf, manager->sendBufLen);
		return;
	}

	// 超过发送缓冲区
	if (len + 8 + PROTOCOL_FIX_LEN > manager->sendBufMax)  
		return;
	
	manager->sendBufLen = 0;
	memset(manager->sendBuf, 0, UARTMAX); // 释放 HTTPBuff_ps
	if(cmd == 1)
	{
		pushHttpDatahtml(&manager->sendBufLen, manager->sendBuf);
	}
	else
	{
		pushDeviceArgJson(&manager->sendBufLen, manager->sendBuf);
	}
	manager->send_data(manager->sendBuf, manager->sendBufLen);   //UART0Ddata.TxBuf    // wifi_send_protocol  数据发送()
}


// 对外接口
void protocol_manager_init(protocol_manager_t *manager,  uint8_t *sendBuf, uint16_t sendBufMax, void (* sendData)(uint8_t*, uint16_t))
{
	manager->receiveBufLen = 0;
	manager->frameLen = -1;
	
	manager->sendBuf = sendBuf;
	manager->sendBufLen = 0;
	manager->sendBufMax = sendBufMax;  // 最大帧长度
                                          
	manager->gw_addr = NULL;
	manager->ip = NULL;
	manager->auto_time = NULL;

	manager->tick = 0;


	if (sendData != NULL) 
		manager->send_data = sendData;			// wifi_send_protocol  数据发送

	manager->protocol_send = protocol_send;

	manager->protocol_receive = protocol_receive ;
	manager->protocol_analysis = protocol_analysis;
	manager->send_heart = send_heart;

	// 参数
	manager->protocol_set_gw_addr = protocol_set_gw_addr;
	manager->protocol_set_fun = protocol_set_fun;
	manager->protocol_set_param = protocol_set_param;
	manager->protocol_send_lora = protocol_send_lora;
	manager->protocol_send_data = protocol_send_data;
}

