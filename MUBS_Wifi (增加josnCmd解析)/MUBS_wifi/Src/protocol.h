#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include <stdbool.h>

#define PROTOCOL_REC_BUF  600


#define PROTOCOL_BEGIN			  0x7E // 包头
#define PROTOCOL_DATABEG          0x02 // 数据开始
#define PROTOCOL_REC_DATAEND      0x05 // 接收数据结束
#define PROTOCOL_SEND_DATAEND     0x03 // 发送数据结束

#define PROTOCOL_NODE_ADDR_OFFSET 3    // 网关地址 偏移
#define PROTOCOL_LEN_OFFSET       14   // 长度     偏移
#define PROTOCOL_DATABEG_OFFSET   16   // 数据开始 偏移
#define PROTOCOL_VALUE_OFFSET     17   // 有效数据 偏移
#define PROTOCOL_CMD_OFFSET       12   // 命令 偏移
#define PROTOCOL_LORA_NODE_ADDR_OFFSET 8  // 控制器地址 偏移

#define PROTOCOL_FIX_LEN          20   //  除了数据的固定长度

#define PROTOCOL_SUCESS_FLAG      0x22   // 成功标志
#define PROTOCOL_FAIL_FLAG        0x11   // 失败标志
#define PROTOCOL_LOST_FLAG        0x33   // 中继失联

#define LORA_ADDR_MAX    50

#include "HtmlCode.h"

/*模拟定义设备的外部参数, 后期修改 Data_Interaction.h 从外部引入参数*/
extern char gateway_id[40];
extern char service_type[40];
extern char server_address[40];
extern char server_port[40];
extern char mbus_baud_rate[40];
extern char rs485_baud_rate[40];
extern char uplink_comm[40];
extern char downlink_comm[40];
extern char touch_Data[500];

#pragma pack (1)
typedef struct lora_node_s
{
	uint8_t relay_addr;
	uint16_t node_addr;
	uint8_t node_type;
} lora_node_t;

typedef struct lora_equ_s
{
	lora_node_t lora_node[LORA_ADDR_MAX];
	uint16_t lora_node_num;
} lora_equ_t;
#pragma pack ()

typedef struct protocol_manager_s
{
	//接收
	uint8_t receiveBuf[PROTOCOL_REC_BUF];
	uint32_t receiveBufLen; // 接收长度
	int frameLen;  //读取帧的长度

	//发送
	uint8_t *sendBuf;
	uint16_t sendBufLen;
	uint16_t sendBufMax;  // 最大帧长度

	// 网关地址
	uint8_t *gw_addr;
	// ip地址
	uint8_t *ip;  // Ip 和 port
	uint16_t *auto_time; // 自动上报时间, 字节对齐问题，不能直接用
	// 收到的读lora数据回复
	uint8_t *host_confirm;

	// 设备号
	uint8_t com_sign; // 用于标记不通的通信接口

	uint32_t tick;


	void (* protocol_send)(struct protocol_manager_s *, uint8_t, uint16_t); // 发送报文
	void (* send_data)(uint8_t *, uint16_t);
	uint8_t (* data_save)(uint8_t *, uint8_t);  // 数据保存
	void (* lora_cmd)(uint8_t, lora_node_t *, uint8_t, uint8_t *);  // 收到控制器命令

	// public
	void (* protocol_receive)(struct protocol_manager_s *, uint8_t *, uint16_t *); // 拷贝数据到缓存
	bool (* protocol_analysis)(struct protocol_manager_s *, bool);                // 处理报文
	void (* send_heart)(struct protocol_manager_s *, uint8_t, char *);         // 发送心跳包, 外部任务调用，保证在透传模式下发送

	// 设置一些参数
	void (* protocol_set_gw_addr)(struct protocol_manager_s *, uint8_t *);         // 设置 gw_addr
	void (* protocol_set_fun)(struct protocol_manager_s *, uint8_t (* dataSave)(uint8_t*, uint8_t),
	void (* loraCmd)(uint8_t, lora_node_t *, uint8_t, uint8_t *));  // 向 lora 转发数据

	void (* protocol_set_param)(struct protocol_manager_s *, uint8_t *, uint16_t *, uint8_t *, uint8_t); // 保存的函数

	// 外部调用发送报文
	void (* protocol_send_lora)(struct protocol_manager_s *, uint8_t, lora_node_t *, uint16_t, uint8_t *, uint8_t);
	
	void (* protocol_send_data)(struct protocol_manager_s *, uint8_t, uint16_t, uint8_t *, uint8_t);


}protocol_manager_t;


typedef void (* protocol_cmd_deal)(protocol_manager_t *manager, uint8_t *, uint16_t);

typedef struct {
    uint8_t  cmd;
    protocol_cmd_deal fun_cmd;
    uint16_t  data_len;
} protocol_fun_cmd_t;


void protocol_manager_init(protocol_manager_t *manager,  uint8_t *sendBuf, uint16_t sendBufMax, void (* sendData)(uint8_t*, uint16_t));
void protocol_tick_add(protocol_manager_t *manager);


#endif




