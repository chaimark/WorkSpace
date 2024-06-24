#ifndef HTTPAPP_H
#define HTTPAPP_H

#include <string.h>
#include <stdint.h>
#include "StrLib.h"

/*-----------------------------------网页数据类----------------------------------*/
#define WebNumberMAX 3
// 类定义 所有网页的集合
typedef struct All_Web
{
	int WebNumber;
	struct _element_web
	{
		int WebID;
		char *HTML_Code;
		int CodeLen;
	} element_web[WebNumberMAX];
} _all_web;
// web对象
extern _all_web Web_Data_Of_All; // 所有的网页集合
/*-----------------------------------定义网页类----------------------------------*/
// 类定义 用户需要的网页
typedef struct Http_Code
{
	int PathID;
	char *HTMLCode;
	int HTMLCodeLen;
	int OncePackStartAddr;
	strnew (*OncePackHtmlCode)(struct Http_Code This); // 获取分块发送时单包 html code
	int (*getClassIDByhttpPath_Http)(strnew UserPath); // 使用 http URL 获取 ClassID
} httpcode;
// 类方法
strnew _OncePackHtmlCode(struct Http_Code This);
int _getClassIDByhttpPath_Http(strnew UserPath);
// 建立对象
#define NEW_HTTP New_Web_Obj()
extern httpcode New_Web_Obj(void); // 建立对象的函数

/*-----------------------------------外部函数接口----------------------------------*/
extern void pushHttpDatahtml(uint16_t *Len, uint8_t *DataBuff);
extern int setHttpTaskMain(void);

/*
//分块数据1
HTTP/1.1 200 OK\r\n
Transfer-Encoding: chunked\r\n
Content-Type: text/plain\r\n
\r\n

//分块数据2
5\r\n
Hello\r\n

//分块数据3
7\r\n
World!\r\n

//分块数据4
0\r\n
\r\n
*/
#endif
