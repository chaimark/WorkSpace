#include "HtmlCode.h"
#include "HttpApp.h"
#include "WifiTask.h"
#include "NumberBaseLic.h"

void pushHttpDatahtml(uint16_t *Len, uint8_t *DataBuff);
int setHttpTaskMain(void);

httpcode UserWeb_Obj;
/*-----------------------------------建立请求网页对象----------------------------------*/
// 新建网页对象 （用户请求的网页对象）
httpcode New_Web_Obj()
{
	httpcode newWebData;
	newWebData.OncePackHtmlCode = _OncePackHtmlCode;				   // 初始化类方法
	newWebData.getClassIDByhttpPath_Http = _getClassIDByhttpPath_Http; // 初始化类方法
	return newWebData;												   // 返回建立的对象数据
}
// （网页类方法）
strnew _OncePackHtmlCode(struct Http_Code This) // 获取分块发送时单包 html code
{
	strnew OncePackCode;
	int StratAddr = This.OncePackStartAddr; // 取出当前包首地址
	OncePackCode.Name._char = &(This.HTMLCode[StratAddr]);
	OncePackCode.MaxLen = ((This.HTMLCodeLen - StratAddr) > ONCE_PACKAGE_LEN ? ONCE_PACKAGE_LEN : (This.HTMLCodeLen - StratAddr) - 1);
	// 检查 OncePackCode.Name._char 的开头是否是结束标记（\r\n）,如果时向后偏移
	while ((OncePackCode.Name._char[0] == '\r') || (OncePackCode.Name._char[0] == '\n'))
	{
		StratAddr++;
		OncePackCode.MaxLen--;
		OncePackCode.Name._char = &This.HTMLCode[StratAddr];
	}
	// 更新首地址 This.OncePackStartAddr
	This.OncePackStartAddr = StratAddr + OncePackCode.MaxLen;
	return OncePackCode;
}
// 使用 http URL 获取 ClassID （类方法）
int _getClassIDByhttpPath_Http(strnew UserPath)
{
	char InputPatt[16] = {0}; // 默认地址 "GET /x HTTP/1.1\0"
	for (int Addr_i = 0; Addr_i < ARR_SIZE(InputPatt); Addr_i++)
	{
		if (Addr_i < 15)
		{
			InputPatt[Addr_i] = UserPath.Name._char[Addr_i]; // copy 传入的 IP
		}
		else
		{
			InputPatt[Addr_i] = 0;
		}
	}
	char *Addr = myStrstr(InputPatt, "GET /", 16);
	switch (Addr[5])
	{
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	default:
		return 0; // 后续添加特殊提示页面
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// 将需要发送的数据 copy 到待发送的空间
void pushHttpDatahtml(uint16_t *Len, uint8_t *DataBuff)
{
	memset(Send_Task.WifiSendAcceptBuff.Name._char, 0, Send_Task.WifiSendAcceptBuff.MaxLen); // 释放 Send_Task.WifiSendAcceptBuff
	Http_AccepSendBuff_Len = 0;

	if (Send_Task.TaskSum == 1)
	{
		catString(Send_Task.WifiSendAcceptBuff.Name._char, ERROR0, WIFI_BUFF_MAXLEN, strlen(ERROR0)); // 响应头 HTTP_HEAD
	}
	else if (Send_Task.TaskID == Send_Task.TaskSum) // 最后一包需要加上 http尾部
	{
		catString(Send_Task.WifiSendAcceptBuff.Name._char, "0\r\n\r\n", WIFI_BUFF_MAXLEN, strlen("0\r\n\r\n")); // 最后一包，追加结束符
	}
	else if (Send_Task.TaskID == 1) // http头部
	{
		catString(Send_Task.WifiSendAcceptBuff.Name._char, HEAD_S, WIFI_BUFF_MAXLEN, strlen(HEAD_S)); // 响应头 HTTP_HEAD
	}
	else // html网页
	{
		strnew OnceHTMLData = UserWeb_Obj.OncePackHtmlCode(&UserWeb_Obj);				 // PackageID 比 TaskID 少一个 请求头 和 http尾
		char Number[16] = {0};															 // ONCE_PACKAGE_LEN 单包数据最大 1024
		int CharLen = doneBaseNumberDataToAsciiStr(Number, 16, OnceHTMLData.MaxLen, 16); // 将 16 进制数字转成数组，并返回长度
		numberArrayToStrArray(Number, Number, CharLen);									 // 数组串转字符串
		swapStr((char *)Number, CharLen);
		// 组织数据
		catString(Send_Task.WifiSendAcceptBuff.Name._char, Number, WIFI_BUFF_MAXLEN, CharLen);								// 组装长度字符串
		catString(Send_Task.WifiSendAcceptBuff.Name._char, "\r\n", WIFI_BUFF_MAXLEN, strlen("\r\n"));						// 结束该字段（\r\n结尾）
		catString(Send_Task.WifiSendAcceptBuff.Name._char, OnceHTMLData.Name._char, WIFI_BUFF_MAXLEN, OnceHTMLData.MaxLen); // 单包html网页
		catString(Send_Task.WifiSendAcceptBuff.Name._char, "\r\n", WIFI_BUFF_MAXLEN, strlen("\r\n"));						// 结束该字段（\r\n结尾）
	}

	Send_Task.TaskID++;
	for ((*Len) = 0; Send_Task.WifiSendAcceptBuff.Name._char[(*Len)] != '\0'; (*Len)++)
	{
		DataBuff[(*Len)] = Send_Task.WifiSendAcceptBuff.Name._char[(*Len)];
	}
	memset(Send_Task.WifiSendAcceptBuff.Name._char, 0, Send_Task.WifiSendAcceptBuff.MaxLen); // 释放 Send_Task.WifiSendAcceptBuff
	Http_AccepSendBuff_Len = 0;
}

_all_web Web_Data_Of_All = { // 增加网页时，需要修改 WebNumberMAX；Web_Data_Of_All  _getClassIDUsehttpPath()
	WebNumberMAX,			 // WebNumber
	{
		{1, {""}, 0}, // WebID + HTML_Code1
		{2, {""}, 0}, // WebID + HTML_Code2
		{3, {""}, 0}, // WebID + HTML_Code3
	}};
/*-----------------------------------公用接口函数----------------------------------*/
// 函数说明
strnew getHtmlCodeUsePathID(int WebID);
// 根据 WebID 找 HTML_Code
strnew getHtmlCodeUsePathID(int WebID)
{
	strnew ResData = {0};
	for (int i = 0; i < Web_Data_Of_All.WebNumber; i++)
	{
		if (Web_Data_Of_All.element_web[i].WebID == WebID)
		{
			ResData.Name._char = Web_Data_Of_All.element_web[i].HTML_Code;
			ResData.MaxLen = Web_Data_Of_All.element_web[i].CodeLen;
			break;
		}
	}
	return ResData;
}
// 根据收到的请求准备响应数据
void prepareHttpData(void)
{
	UserWeb_Obj = NEW_HTTP;																	 // 建立网页对象
	UserWeb_Obj.OncePackStartAddr = 0;														 // 初始化单包首地址
	UserWeb_Obj.PathID = UserWeb_Obj.getClassIDByhttpPath_Http(Send_Task.Client_Request[1]); // 根据请求头提供的IP地址转换成网页ID
	// 根据ID 在网页数据区找对应的网页
	strnew UserAskWeb = getHtmlCodeUsePathID(UserWeb_Obj.PathID);
	UserWeb_Obj.HTMLCode = UserAskWeb.Name._char;
	UserWeb_Obj.HTMLCodeLen = UserAskWeb.MaxLen;
}
int setHttpTaskMain(void)
{
	prepareHttpData(); // 根据收到的请求准备响应数据，建立 UserWeb_Obj
	// 根据 UserWeb_Obj 准备分块指针，设置包数（有几包数据就有多少任务）

	switch (UserWeb_Obj.PathID)
	{
	case 1:
		Send_Task.TaskSum = (sizeof(HTML01) / ONCE_PACKAGE_LEN) + (((sizeof(HTML01) % ONCE_PACKAGE_LEN) > 0) ? 1 : 0);
		break;
	case 2:
		Send_Task.TaskSum = (sizeof(HTML02) / ONCE_PACKAGE_LEN) + (((sizeof(HTML02) % ONCE_PACKAGE_LEN) > 0) ? 1 : 0);
		break;
	case 3:
		Send_Task.TaskSum = (sizeof(HTML03) / ONCE_PACKAGE_LEN) + (((sizeof(HTML03) % ONCE_PACKAGE_LEN) > 0) ? 1 : 0);
		break;
	default:
		UserWeb_Obj.PathID = 1;
		Send_Task.TaskSum = 1;
		return 1;
	}

	Send_Task.TaskID = 1;
	Send_Task.TaskSum += 2;
	return Send_Task.TaskSum; // 加上首位两包数据
}
