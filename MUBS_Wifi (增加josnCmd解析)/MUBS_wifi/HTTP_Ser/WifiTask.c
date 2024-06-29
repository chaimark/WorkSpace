#include "WifiTask.h"
#include "HtmlCode.h"
#include "HttpApp.h"
#include "JsonApp.h"
#include "import_Wifi_DATA.h"
#include "../Src/Uart.h"

/*-----------------------------------公用参数定义----------------------------------*/
// 初始化 Http_AccepSendBuff
char Http_AccepSendBuff[WIFI_BUFF_MAXLEN];
int Http_AccepSendBuff_Len;

SendTask Send_Task; // wifi 任务表 与 发送buff

void initAllWebDataAndSendAcceptBuff(void)
{
	// 初始化收发 buff
	Send_Task.WifiSendAcceptBuff.Name._char = Http_AccepSendBuff;
	Send_Task.WifiSendAcceptBuff.MaxLen = WIFI_BUFF_MAXLEN;
	// 获取设备参数信息

	memset(&Web_Data_Of_All, 0, sizeof(_all_web)); // 初始化 Web_Data_Of_All
	Web_Data_Of_All.WebNumber = 3;
	Web_Data_Of_All.element_web[0].WebID = 1;
	Web_Data_Of_All.element_web[0].HTML_Code = HTML01;
	Web_Data_Of_All.element_web[0].CodeLen = sizeof(HTML01);
	Web_Data_Of_All.element_web[1].WebID = 2;
	Web_Data_Of_All.element_web[1].HTML_Code = HTML02;
	Web_Data_Of_All.element_web[1].CodeLen = sizeof(HTML02);
	Web_Data_Of_All.element_web[2].WebID = 3;
	Web_Data_Of_All.element_web[2].HTML_Code = HTML03;
	Web_Data_Of_All.element_web[2].CodeLen = sizeof(HTML03);
}

// TCP链接后，设置任务并返回任务数
int setWifiTaskMain(int cmd)
{
	int TaskNumber = 0;

	if (cmd == 0x01)
	{
		TaskNumber = setHttpTaskMain(); // 准备 Html 数据并 建立 HTTP 发送任务
	}
	else
	{
		TaskNumber = setJavaTaskMain(); // 准备 Josn 数据并 建立 HTTP 发送任务
	}
	return TaskNumber;
}

// 查找返回是否存在字串
bool isFoundStr(char * MotherStr, char * SonStr, int MaxSize, int RunTimeMS) {
    char * p_star = NULL;
    for (int i = 0; i < RunTimeMS; i++) {
        if ((p_star = myStrstr(MotherStr, SonStr, MaxSize)) != NULL) // 假设查询一次100ms
            break;
        copyDataForUART();
    }
    return ((p_star == NULL) ? 0 : 1);
}
