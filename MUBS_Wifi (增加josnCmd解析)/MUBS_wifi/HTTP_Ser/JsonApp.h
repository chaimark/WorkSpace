#ifndef JSONAPP_H
#define JSONAPP_H

#include <string.h>
#include <stdint.h>
#include "StrLib.h"
#include "import_Wifi_DATA.h"

/*-----------------------------------定义json参数类----------------------------------*/
// wifi 外部参数接口
extern char *WIFI_GatewayFlag;	  // 外部接口引入 WIFI 网关标识		Gateway identification
extern char *WIFI_WebType;		  // 外部接口引入 WIFI 服务类型		WebService type
extern char *WIFI_WebServiceAddr; // 外部接口引入 WIFI 服务器地址	WebService address
extern char *WIFI_WebServicePort; // 外部接口引入 WIFI 服务器端口	WebService port
extern char *WIFI_MUBS_Baud;	  // 外部接口引入 WIFI MUBS波特率	MUBS Baud rate
extern char *WIFI_RS485_Baud;	  // 外部接口引入 WIFI RS485波特率	RS485 baud rate
extern char *WIFI_Uplink;		  // 外部接口引入 WIFI 上行通信		Uplink communication
extern char *WIFI_Downlink;		  // 外部接口引入 WIFI 下行通信		Downlink communication
extern char *WIFI_TouchJson;	  // 外部接口引入 WIFI 下行通信		Downlink communication

// 类定义 json数据类
typedef struct Wifi_Argument
{
	strnew GatewayFlag;
	strnew WebType;
	strnew WebServiceAddr;
	strnew WebServicePort;

	strnew MUBS_Baud;
	strnew RS485_Baud;
	strnew Uplink;
	strnew Downlink;
	strnew Touch_Data;
	uint32_t needSendJsonDataFlagBit;

	char ArgumentStrbuff[512];								  // json参数越多，需要的空间就越大
	void (*getJsonForWifiDataStr)(struct Wifi_Argument This); // 将数据整理成json字符串
} _JsonWifi_DeviceArgument;
// 类方法
void _getJsonForWifiDataStr(struct Wifi_Argument This);
// 建立对象
#define NEW_JSON_Argument New_Obj_Json()
extern _JsonWifi_DeviceArgument New_Obj_Json(void); // 建立对象的函数

/*-----------------------------------定义请求json类----------------------------------*/
// 类定义 用户需要的json
typedef struct Json_Code
{
	char *JsonCodeBuff;
	int JsonCodeBuffLen;
	_JsonWifi_DeviceArgument JsonArgument;
	int OncePackStartAddr;
	strnew (*OncePackJsonCode)(struct Json_Code This);							// 获取分块发送时单包 Json code
	void (*setClassBitByhttpPath_Json)(strnew UserPath, struct Json_Code This); // 使用 http URL 获取 ClassID
} jsoncode;
// 类方法
strnew _OncePackJsonCode(struct Json_Code This);
void _setClassBitByhttpPath_Json(strnew UserPath, struct Json_Code This);
// 建立对象
#define NEW_JSON_USER New_Json_Obj()
extern jsoncode New_Json_Obj(void); // 建立对象的函数

/*-----------------------------------外部函数接口----------------------------------*/
extern _JsonWifi_DeviceArgument getDeviceArgumentDATA(void);
extern void pushDeviceArgJson(uint16_t *Len, uint8_t *DataBuff);
extern int setJavaTaskMain(void);
extern int catJsonDataToString(strnew OutString, char * jsonKey, char * jsonForm, void * Data);
#endif
