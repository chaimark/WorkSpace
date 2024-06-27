#ifndef PUBLIC_H
#define PUBLIC_H

#include <stdint.h>
#include "StrLib.h"

// 接受HTTP请求并解析HTTP

// GET /index.html HTTP/1.1\r\n
// Host: 192.168.4.1:8888\r\n
// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.93 Safari/537.36\r\n
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n
// Accept-Language: en-US,en;q=0.5\r\n
// Accept-Encoding: gzip, deflate, br\r\n
// Connection: keep-alive\r\n
// Upgrade-Insecure-Requests: 1\r\n
// \r\n

// 上位机可能会下发的方法定义
#define API_NAME_1 "getWifiData"
#define API_NAME_2 "saveGateway"

// 查表对比获取到的方法，并返回方法对应的ID
typedef struct _POST_Funaction
{
	enum _FunactionID
	{
		getWifiData,
		saveGateway,
	} FunactionID;
	char FunName[10];
	int FunNameLen;
} POSTFunaction;
/*-----------------------------------WiFi数据发送任务表----------------------------------*/
#define CMDBUFFMAXLEN 100
#define JSONDATAMAXLEN 500
typedef struct _SendTask
{
	int TaskID;								 // 1 - n
	int TaskSum;							 // 3 - n
	strnew WifiSendAcceptBuff;				 // wifi缓存接受buff
	strnew Client_Request[3];				 // HttpBuff[1] = GET /index.html HTTP/1.1\r\n   //HttpBuff[2] = Host: 192.168.1.1\r\n
	char CmdTask[3][2][CMDBUFFMAXLEN];		 // 用户存储客户端请求的命令
	char DonwloadJsonStr[3][JSONDATAMAXLEN]; // 保存尚未下发的josn 数据
	char CmdNumber;							 // 是否存在已保存的请求总数
	int FrontTcpLinkID;						 // 上次连接的 link ID
} SendTask;
extern SendTask Send_Task; // wifi 任务表 与 发送buff

/*-----------------------------------公用接口参数与函数----------------------------------*/

extern char Http_AccepSendBuff[WIFI_BUFF_MAXLEN];
extern int Http_AccepSendBuff_Len;

// web数据的解析与方法

extern void initAllWebDataAndSendAcceptBuff(void); // 初始化
extern int setWifiTaskMain(int cmd);			   // TCP链接后，设置任务并返回任务数
extern bool isFoundStr(char * MotherStr, char * SonStr, int MaxSize, int RunTimeMS);
#endif
