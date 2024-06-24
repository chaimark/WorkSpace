#include "main.h"
#include "protocol.h"
#include "define.h"
#include "StrLib.h"
#include "AcceptAnalyze.h"

/*模拟定义设备的外部参数, 后期修改 Data_Interaction.h 从外部引入参数*/

extern UARTOpStruct UART0Ddata;
extern void Uart0Send(uint8_t *txBuffer, uint16_t length);

// 获取的参数
char wifi_ver[30] = {0}; // 许2021.11.10改******  给协议

// private ***************************************************************************
typedef bool (*wifi_cmd)(char *data);
typedef void (*wifi_send_data)(uint8_t *, uint16_t);
typedef struct wifi_process_s
{
	const char *cmd;	// 命令
	const char *sucess; // 成功返回

	const uint32_t overtime_10ms; // 超时时间
	const int8_t overtime_num;	  // 超时重试次数
	const int8_t sucess_add;	  // 成功后前进步数, 0 结束配置

	wifi_cmd fun_cmd; // 自定义命令， 如果cmd == NULL
	char *read;		  // 读数据的缓存

} wifi_process_t;
static uint8_t wifi_tcp_ch = 0; // tcp 最后一次接收数据的通道

// 发送缓存
static char *wifi_sendBuf = (char *)UART0Ddata.TxBuf;
static uint16_t wifi_sendBufLen = 0;

// 发送函数
static wifi_send_data wifi_send = Uart0Send;

// 超时处理
// #define WIFI_REBOOT_INV_MAX  8640000  // 24h  最大重启间隔
#define WIFI_REBOOT_INV_MAX 360000 // 1h   最大重启间隔
#define WIFI_READ_ISSI_INV 360000  // 1h   读取信号强度间隔
// #define WIFI_READ_ISSI_INV  2000     // 20s  读取信号强度间隔 调试
#define WIFI_HEART_INV 6000			// 60s  心跳间隔
#define WIFI_HEART_REBOOT_INV 18000 // 180s

// 流程处理
#define WIFI_PROCESS_MAX sizeof(wifi_process) / sizeof(wifi_process_t) // 总条数

#define WIFI_DISCONN_INDEX 8 // 断开连接, 索引位置
#define WIFI_SNED_INDEX 9	 // 发送数据等待索引位置

static uint32_t wifi_receive_delay = 0;		  // 40ms 一次执行此任务
static int32_t wifi_process_delay = 0;		  // 用于控制AT指令模式下，指令超时
static uint32_t wifi_heart_inv_delay = 0;	  // 用于透传模式下控制心跳，1min 无有效报文，则发一次心跳
static uint32_t wifi_heart_reboot_inv = 0;	  // 多长时间收不到有效报文，重启
static uint8_t WIFI_connect_flag = 0;		  // 是否连接上
static uint8_t wifi_process_index = 0xff;	  // 当前 步骤
static uint8_t wifi_process_repeat_count = 0; // 重试次数
static uint8_t wifi_process_reboot_count = 0; // 重启次数

// static void wifi_up_lora_cmd(uint8_t cmd, lora_node_t *lora_node, uint8_t dataLen, uint8_t *data, uint8_t flag);
static bool wifi_set_hotspot(char *cmd);
// static bool wifi_nopfun(char* data);
static bool wifi_data_cmd(char *cmd);

void wifi_set_param(uint8_t *host_confirm);

uint8_t send_data[16] = {0x68, 0x20, 0x11, 0x11, 0x11, 0X11, 0X00, 0X11, 0X11, 0X81, 0X03, 0X90, 0X1F, 0X00, 0X28, 0X16};

// 没有 cmd  通过  fun_cmd 返回 cmd命令
// 没有sucess,通过 fun_cmd 判断返回
static wifi_process_t wifi_process[] =
	{
		// 重启流程
		{"AT\r\n", "OK", 200, 3, 1, NULL, NULL},
		{"ATE0\r\n", "OK", 200, 3, 1, NULL, NULL},				  // 关闭回显
		{"AT+GMR\r\n", "version:", 200, 3, 1, NULL, wifi_ver},	  // 获取版本号
		{"AT+CWMODE_DEF=2\r\n", "OK", 200, 3, 1, NULL, NULL},	  // 设置为AP
		{NULL, "OK", 200, 3, 1, wifi_set_hotspot, NULL},		  // 设置热点信息
		{"AT+CIPMUX=1\r\n", "OK", 200, 3, 1, NULL, NULL},		  // 设置为多连接
		{"AT+CIPSERVER=0\r\n", "OK", 200, 3, 1, NULL, NULL},	  // 关闭TCP服务器
		{"AT+CIPSERVER=1,8888\r\n", "OK", 200, 3, 0, NULL, NULL}, // 打开TCP服务器
																  //{"AT+CIPCLOSE=0\r\n"      , NULL     , 200 , 3, 0 , wifi_nopfun      , NULL     } , // 关闭连接
		{"AT+CIPCLOSE=0\r\n", "UNLINK", 200, 3, 0, NULL, NULL},	  // 关闭连接

		// 发送数据
		{NULL, "SEND OK", 2000, 3, 0, wifi_data_cmd, NULL}, // 发送数据
};

// 设置热点信息
static bool wifi_set_hotspot(char *cmd)
{
	char WIFI_NAME_ID[12] = {0};
	CopyString(WIFI_NAME_ID,&gateway_id[1],12,11);
	WIFI_NAME_ID[11] = '\0';
	sprintf(cmd, "AT+CWSAP_DEF=\"hyMQTT_%s\",\"njhy1234\",5,4,1,0\r\n",	WIFI_NAME_ID);
	return true;
}

// static bool wifi_nopfun(char* data)
//{
//	return true;
// }

// 不发数据，只等待
static bool wifi_data_cmd(char *cmd)
{
	return false;
}

// 返回下一次重启间隔， 3次以后，2min,每次间隔增加一倍
// 最多间隔 24h
static uint32_t wifi_set_reboot_inv(void)
{
	uint32_t temp = 12000;

	temp = wifi_process_reboot_count < 3 ? 1200 : (temp << wifi_process_reboot_count - 3);

	if (temp >= WIFI_REBOOT_INV_MAX)
		temp = WIFI_REBOOT_INV_MAX;
	else
		wifi_process_reboot_count++;

	return temp;
}

void wifi_send_protocol(uint8_t *txBuffer, uint16_t length)
{
	uint8_t wifi_sendcmd[32] = {0};
	sprintf((char *)wifi_sendcmd, "AT+CIPSEND=%d,%d\r\n", wifi_tcp_ch, length);
	wifi_send(wifi_sendcmd, strlen((char *)wifi_sendcmd));
	if (isFoundStr(HTTPBuff_p, ">", HTTPBuff_MaxLen, 40) == 1)
	{
		wifi_send(txBuffer, length); // 查到 ">" 发下一条，没查到延时4s
		wifi_sendBufLen = length;
	}
}

// public **********************************************************************************
static protocol_manager_t protocol_wifi;
void wifi_init(void)
{
	// 下行
	protocol_manager_init(&protocol_wifi, UART0Ddata.TxBuf, UARTMAX, wifi_send_protocol);
	//	wifi_set_param(lora_host_confirm());
	//	// 上行
	//	lora_set_wifi_fun(wifi_up_lora_cmd); // 设置上传的函数
}

void wifi_timer_add(void)
{
	protocol_tick_add(&protocol_wifi);
	wifi_receive_delay++;
	wifi_process_delay--;
	wifi_heart_inv_delay++;
	wifi_heart_reboot_inv++;
}

// 一段时间无数据，则开始处理，wifi无法透传，所以得靠时间来处理
void wifi_timer_clean(void)
{
	wifi_receive_delay = 0;
}

// 处理多个请求（网络任务）
void doClientRequest(void)
{
	char *p_star = NULL; // 开始
	do
	{
		if (((p_star = myStrstr(HTTPBuff_p, "+IPD,", HTTPBuff_MaxLen)) != NULL) || getCmdTaskNumberOrLinkID(1))
		{
			if (getCmdTaskNumberOrLinkID(1) == 0)
				wifi_tcp_ch = atoi(p_star + strlen("+IPD,")); // tcp 通道
			else
				wifi_tcp_ch = (uint8_t)getCmdTaskNumberOrLinkID(2);
			// 保存 http 请求，并返回客户端的提供的方法，选择对应的服务
			int CmdID = saveHttpDataAndanalyze(wifi_tcp_ch); // 服务1:发送静态html	// 服务2:发送josn数据
			int TaskNumber = setWifiTaskMain(CmdID);
			for (int i = 0; i < TaskNumber; i++)
			{
				protocol_wifi.protocol_send_data(&protocol_wifi, CmdID, 16, send_data, 0); // wifi_send_protocol  数据发送	html
				if (isFoundStr(HTTPBuff_p, "SEND OK", HTTPBuff_MaxLen, 40) == 0)
				{
					if (isFoundStr(HTTPBuff_p, "ERROR", HTTPBuff_MaxLen, 40) == 1)
						break;
				}
			}
		}
		deleteCmdTask(); // 发一次，就清除一次请求，每次只清一条
		DelayMs(1000);	 // 等待一段时间，防止DMA收不全
		if (UART0Ddata.RxLen != 0)
		{
			copyDataForUART();
		}
	} while (isFoundStr(HTTPBuff_p, "+IPD", HTTPBuff_MaxLen, 1) != 0); // 发完数据后，在查询上位机返回时没有收到数据即可推出
	memset(HTTPBuff_p, 0, HTTPBuff_MaxLen); // 释放 HTTPBuff_p
	HTTPBuff_NowLen = 0;
}

void wifi_task(void)
{
	char *p_star = NULL; // 开始
	char *p_end = NULL;	 // 结束
	uint16_t temp;
	uint16_t tempLen;
	// 30ms check 一次
	if (wifi_receive_delay > 3)
	{
		wifi_receive_delay = 0;
		initAllWebDataAndSendAcceptBuff(); // 初始化 web 参数与 wifibuff
		if (UART0Ddata.RxLen != 0)
		{
			// 搬运串口数据
			copyDataForUART();
			if ((p_star = myStrstr(HTTPBuff_p, "+IPD,", HTTPBuff_MaxLen)) != NULL)
			{
				// 长度太短，则不处理
				tempLen = HTTPBuff_NowLen - (p_star - HTTPBuff_p); // 有效数据长度
				if (tempLen < 12)
					return;

				// 提取报文信息
				wifi_tcp_ch = atoi(p_star + strlen("+IPD,")); // tcp 通道
				temp = atoi(p_star + strlen("+IPD,0,"));	  // 数据长度

				// 找有效数据开头
				if ((p_star = strchr(p_star, ':')) == NULL)
					return;
				p_star += 1;

				// 数据太短也不处理
				tempLen = HTTPBuff_NowLen - (p_star - HTTPBuff_p); // 有效数据长度
				if (tempLen < temp)
					return;

				protocol_wifi.protocol_receive(&protocol_wifi, (uint8_t *)p_star, &temp); // 拷贝数据到缓存
				if (protocol_wifi.protocol_analysis(&protocol_wifi, true))				  // 收到完整数据，清心跳
				{
					wifi_heart_inv_delay = 0;
					wifi_heart_reboot_inv = 0;
					WIFI_connect_flag = 1; // TCP 连接上
				}
				doClientRequest(); // 处理网络任务
			}
			else if (strstr(HTTPBuff_p, "STA_DISCONNECTED") != NULL)
			{
				// 热点断开， 需要手动断开tcp
				wifi_process_index = WIFI_DISCONN_INDEX;
				wifi_process_delay = 100; // 延时1.0s 后 退出透传
			}
			else if ((p_star = strstr((char *)HTTPBuff_p, ",CONNECT")) != NULL)
			{
				// TCP 连接
				if (p_star > HTTPBuff_p)
					wifi_tcp_ch = *(p_star - 1) - '0';

				wifi_heart_inv_delay = 0;
				wifi_heart_reboot_inv = 0;
				WIFI_connect_flag = 1; // TCP 连接上
			}
			else if ((p_star = strstr(HTTPBuff_p, ",CLOSED")) != NULL)
			{
				// tcp 断开
				WIFI_connect_flag = 0; // TCP 断开
			}
			else if (wifi_sendBufLen != 0 && (p_star = strstr(HTTPBuff_p, "OK\r\n>")) != NULL)
			{
				// 发送数据的异步处理
				wifi_send((uint8_t *)wifi_sendBuf, wifi_sendBufLen);
				wifi_sendBufLen = 0;
				wifi_process_index = WIFI_SNED_INDEX;
			}
			if (isFoundStr(HTTPBuff_p, "UNLINK", HTTPBuff_MaxLen, 40) == 1)
			{
				wifi_process_index = 0xff; // 重启模块
			}
			else if (wifi_process_index < WIFI_PROCESS_MAX)
			{
				// 与反馈的字符一致，则进入下一个命令
				// 没有反馈字符，则通过函数来判断
				if ((wifi_process[wifi_process_index].sucess != NULL && (p_star = strstr(HTTPBuff_p, wifi_process[wifi_process_index].sucess)) != NULL) ||
					(wifi_process[wifi_process_index].sucess == NULL && wifi_process[wifi_process_index].fun_cmd(HTTPBuff_p)))
				{
					// 如果是获取的数据，则拷贝到目标地址
					if (wifi_process[wifi_process_index].read != NULL && p_star != NULL)
					{
						// 拷贝数据
						p_star += strlen(wifi_process[wifi_process_index].sucess);
						p_end = strchr(p_star, '\r');
						if (p_end != NULL)
						{
							temp = p_end - p_star;
							strncpy(wifi_process[wifi_process_index].read, p_star, temp);
							wifi_process[wifi_process_index].read[temp] = 0;
						}
					}

					// 一条at指令处理完成
					if (wifi_process[wifi_process_index].sucess_add != 0)
					{
						wifi_process_index += wifi_process[wifi_process_index].sucess_add;
					}
					else
					{
						// ********************这里跳到透传模式*********************************************************************
						wifi_process_reboot_count = 0; // 重启次数清零
						wifi_heart_inv_delay = 0;
						wifi_heart_reboot_inv = 0;
						wifi_process_index = 0x55; // 随便填一个 >  WIFI_PROCESS_MAX 即可
					}

					wifi_process_repeat_count = 0;
					wifi_process_delay = 0; // 在AT模式下, 用于计算命令间的延时时间
				}
			}
		}
		else if (getCmdTaskNumberOrLinkID(1) > 0)
		{
			doClientRequest(); // 处理网络任务 // 有请求还没处理
		}
		else
		{
			protocol_wifi.protocol_analysis(&protocol_wifi, false);

			// 1 min 无有效报文，发送心跳
			if (wifi_heart_inv_delay > WIFI_HEART_INV && WIFI_connect_flag == 1)
			{
				//				protocol_wifi.send_heart(&protocol_wifi, ec200t_rssi, ec200t_ccid);
				wifi_heart_inv_delay = 0;
			}
			else if (wifi_heart_reboot_inv > WIFI_HEART_REBOOT_INV && WIFI_connect_flag == 1)
			{
				// 3 min 无数据连接，则重启模块（TCP 连接上，但是无通信， 很可能是服务器挂了）
				// 正常通信断开，应该在检查 "NO CARRIER" 时就检测到了
				wifi_heart_reboot_inv = 0;

				// 重启模块
				WIFI_connect_flag = 0;
				wifi_process_index = 0xff; // 重新连接
			}
			else if (wifi_process_index == 0xff)
			{
				// 重启模块
				wifi_power_off();
				DelayMs(200);
				wifi_power_on();

				wifi_process_index = 0;
				wifi_process_repeat_count = 0;
				wifi_process_delay = wifi_set_reboot_inv(); // 计算下一次重启间隔
			}
			else if (wifi_process_index < WIFI_PROCESS_MAX && wifi_process_delay <= 0)
			{
				if (wifi_process_repeat_count >= wifi_process[wifi_process_index].overtime_num)	// 三次失败重启模块
				{
					wifi_process_index = 0xff; // 重启模块
				}
				else
				{
					wifi_process_repeat_count++;
					wifi_process_delay = wifi_process[wifi_process_index].overtime_10ms;

					// 发送命令分两种：1,直接发送命令，2,命令较复杂通过函数返回命令
					if (wifi_process[wifi_process_index].cmd != NULL)
					{
						strcpy(wifi_sendBuf, wifi_process[wifi_process_index].cmd);

						wifi_sendBufLen = strlen(wifi_sendBuf);
						wifi_send((uint8_t *)wifi_sendBuf, wifi_sendBufLen);
						wifi_sendBufLen = 0;
					}
					else
					{
						if (wifi_process[wifi_process_index].fun_cmd(wifi_sendBuf))
						{
							// 获取命令成功
							wifi_sendBufLen = strlen(wifi_sendBuf);
							wifi_send((uint8_t *)wifi_sendBuf, wifi_sendBufLen);
							wifi_sendBufLen = 0;
						}
					}
				}
			}
		}
	}
}
// void wifi_task_quiz(void)
// {
// 	// 启动 wifi 模块

// 	//
// }
//// **** host 与 lora 协议对接 ************************************
//// 上行收到 lora 数据， 转化为wifi命令， lora 回调
// static void wifi_up_lora_cmd(uint8_t cmd, lora_node_t *lora_node, uint8_t dataLen, uint8_t *data, uint8_t flag)
//{
//	if (flag == 2)  // 抄表自然结束，特殊处理
//	{
//		protocol_wifi.protocol_send_lora(&protocol_wifi, 0x34, lora_node, (uint16_t)dataLen, data, flag);
//	}
//	else if ((cmd & 0x7f) == 0x01)
//	{
//		protocol_wifi.protocol_send_lora(&protocol_wifi, 0x33, lora_node, (uint16_t)dataLen, data, flag);
//	}
//	else if ((cmd & 0x7f) == 0x02)
//	{
//		protocol_wifi.protocol_send_lora(&protocol_wifi, 0x31, lora_node, (uint16_t)dataLen, data, flag);
//	}
// }

//// 下行收到 lora 命令，转化为lora命令， host回调
// static void wifi_down_lora_cmd(uint8_t cmd, lora_node_t *lora_node, uint8_t dataLen, uint8_t *data)
//{
////	// 读控制器的值
////	if (cmd == 0x33)
////		lora_host_send_cmd(0x01, lora_node, dataLen, data, wifi_up_lora_cmd);
////	//控制
////	else if (cmd == 0x31)
////		lora_host_send_cmd(0x02, lora_node, dataLen, data, wifi_up_lora_cmd);
////	// 启动一次抄表
////	else if (cmd == 0x32)
////		lora_meter_begin(wifi_up_lora_cmd);  // 开始抄表
////	// 停止抄表
////	else if (cmd == 0x34)
////		lora_meter_end();  // 停止抄表
//}

// 初始化完成后设置的一些参数
void wifi_set_param(uint8_t *host_confirm)
{
	//	protocol_wifi.protocol_set_fun(&protocol_wifi, AT24C02_write, wifi_down_lora_cmd);
	//	protocol_wifi.protocol_set_gw_addr(&protocol_wifi, AT24C02_manager.gw_addr);
	//	protocol_wifi.protocol_set_param(&protocol_wifi, AT24C02_manager.gsm_ip,    // wifi 还是发 4G的ip 和 周期
	//			&AT24C02_manager.gsm_auto_time, host_confirm, 1);
}

uint8_t wifi_is_connect(void)
{
	return WIFI_connect_flag;
}
