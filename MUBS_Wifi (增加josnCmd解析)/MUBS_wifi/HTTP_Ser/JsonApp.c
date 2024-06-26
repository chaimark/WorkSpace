#include <stdio.h>
#include "HtmlCode.h"
#include "JsonApp.h"
#include "WifiTask.h"
#include "NumberBaseLic.h"
#include "../Src/protocol.h"
#include "RunComputerCmd.h"

// 输入关键字和值，增加一条 json 数据
int catJsonDataToString(strnew OutString, char * jsonKey, char * jsonForm, void * Data) {
    int NowLenOutStr = strlen(OutString.Name._char);
    if (NowLenOutStr > OutString.MaxLen) {
        return -1;
    }
    /*%d %o %x %u %c %s %f %e %g*/
    enum _FromFlag {
        _INT_Dex,
        _INT_Oex,
        _INT_Hex,
        _UINT_Dex,
        _Char_Ch,
        _Char_Str,
        _Float,
        _Ee,
        _Gg,
    } FromFlag;

    char FromStr[10] = {",%s: "};
    catString(FromStr, jsonForm, 10, strlen(jsonForm));
    catString(FromStr, "}", 10, strlen(jsonForm));
    switch (FromFlag) {
        case _INT_Dex:
        case _INT_Oex:
        case _INT_Hex:sprintf(&OutString.Name._char[NowLenOutStr - 1], FromStr, jsonKey, (*(int *)Data));
            break;
        case _UINT_Dex:sprintf(&OutString.Name._char[NowLenOutStr - 1], FromStr, jsonKey, (*(unsigned int *)Data));
            break;
        case _Char_Ch:sprintf(&OutString.Name._char[NowLenOutStr - 1], FromStr, jsonKey, (*(char *)Data));
            break;
        case _Char_Str:sprintf(&OutString.Name._char[NowLenOutStr - 1], FromStr, jsonKey, (*(char *)Data));
            break;
        case _Float:sprintf(&OutString.Name._char[NowLenOutStr - 1], FromStr, jsonKey, (*(float *)Data));
            break;
        case _Ee:sprintf(&OutString.Name._char[NowLenOutStr - 1], FromStr, jsonKey, (*(double *)Data));
            break;
        case _Gg:sprintf(&OutString.Name._char[NowLenOutStr - 1], FromStr, jsonKey, (*(double *)Data));
            break;
    }
    return strlen(OutString.Name._char);
}

// wifi 外部参数接口
char * WIFI_GatewayFlag;	   // 外部接口引入 WIFI 网关标识		Gateway identification
char * WIFI_WebType;		   // 外部接口引入 WIFI 服务类型		WebService type
char * WIFI_WebServiceAddr; // 外部接口引入 WIFI 服务器地址		WebService address
char * WIFI_WebServicePort; // 外部接口引入 WIFI 服务器端口		WebService port
char * WIFI_MUBS_Baud;	   // 外部接口引入 WIFI MUBS波特率		MUBS Baud rate
char * WIFI_RS485_Baud;	   // 外部接口引入 WIFI RS485波特率		RS485 baud rate
char * WIFI_Uplink;		   // 外部接口引入 WIFI 上行通信		Uplink communication
char * WIFI_Downlink;	   // 外部接口引入 WIFI 下行通信		Downlink communication
char * WIFI_TouchJson;	   // 外部接口引入 WIFI 下行通信		Downlink communication



_JsonWifi_DeviceArgument getDeviceArgumentDATA(void);

// 新建设备参数对象 （返回一个结构体数据）
_JsonWifi_DeviceArgument New_Obj_Json(void) {
    _JsonWifi_DeviceArgument newWebData;
    newWebData.getJsonForWifiDataStr = _getJsonForWifiDataStr;
    return newWebData; // 返回建立的对象数据
}
// 类方法 _getJsonForWifiDataStr 获取json数据
void _getJsonForWifiDataStr(struct Wifi_Argument This) {
    // 后续可扩展该方法，将需要发送的json数据变为可选的，可加上序号做判断
    char jsonData[512] = {0};

    strnew JsonData = NEW_NAME(jsonData);
    catString(JsonData.Name._char, "{}", JsonData.MaxLen, strlen("{}"));

    if ((This.needSendJsonDataFlagBit & 0x01) == 0x01) {
        catJsonDataToString(JsonData, CHAR_GatewayFlag, "%s", This.GatewayFlag.Name._char);
        // catString(jsonForm, CHAR_GatewayFlag, 280, strlen(CHAR_GatewayFlag));
        // catString(jsonForm, This.GatewayFlag.Name._char, sizeof(jsonForm), strlen(This.GatewayFlag.Name._char)); // 替换
        // catString(jsonForm, ",", sizeof(jsonForm), 1);															 // 替换
    }
    // if ((This.needSendJsonDataFlagBit & 0x02) == 0x02)
    // {
    // 	catString(jsonForm, CHAR_WebType, 280, strlen(CHAR_WebType));
    // 	catString(jsonForm, This.WebType.Name._char, sizeof(jsonForm), strlen(This.WebType.Name._char)); // 替换
    // 	catString(jsonForm, ",", sizeof(jsonForm), 1);													 // 替换
    // }
    // if ((This.needSendJsonDataFlagBit & 0x04) == 0x04)
    // {
    // 	catString(jsonForm, CHAR_WebServiceAddr, 280, strlen(CHAR_WebServiceAddr));
    // 	catString(jsonForm, This.WebServiceAddr.Name._char, sizeof(jsonForm), strlen(This.WebServiceAddr.Name._char)); // 替换
    // 	catString(jsonForm, ",", sizeof(jsonForm), 1);																   // 替换
    // }
    // if ((This.needSendJsonDataFlagBit & 0x08) == 0x08)
    // {
    // 	catString(jsonForm, CHAR_WebServicePort, 280, strlen(CHAR_WebServicePort));
    // 	catString(jsonForm, This.WebServicePort.Name._char, sizeof(jsonForm), strlen(This.WebServicePort.Name._char)); // 替换
    // 	catString(jsonForm, ",", sizeof(jsonForm), 1);																   // 替换
    // }
    // if ((This.needSendJsonDataFlagBit & 0x10) == 0x10)
    // {
    // 	catString(jsonForm, CHAR_MUBS_Baud, 280, strlen(CHAR_MUBS_Baud));
    // 	catString(jsonForm, This.MUBS_Baud.Name._char, sizeof(jsonForm), strlen(This.MUBS_Baud.Name._char)); // 替换
    // 	catString(jsonForm, ",", sizeof(jsonForm), 1);														 // 替换
    // }
    // if ((This.needSendJsonDataFlagBit & 0x20) == 0x20)
    // {
    // 	catString(jsonForm, CHAR_RS485_Baud, 280, strlen(CHAR_RS485_Baud));
    // 	catString(jsonForm, This.RS485_Baud.Name._char, sizeof(jsonForm), strlen(This.RS485_Baud.Name._char)); // 替换
    // 	catString(jsonForm, ",", sizeof(jsonForm), 1);														   // 替换
    // }
    // if ((This.needSendJsonDataFlagBit & 0x40) == 0x40)
    // {
    // 	catString(jsonForm, CHAR_Uplink, 280, strlen(CHAR_Uplink));
    // 	catString(jsonForm, This.Uplink.Name._char, sizeof(jsonForm), strlen(This.Uplink.Name._char)); // 替换
    // 	catString(jsonForm, ",", sizeof(jsonForm), 1);												   // 替换
    // }
    // if ((This.needSendJsonDataFlagBit & 0x80) == 0x80)
    // {
    // 	catString(jsonForm, CHAR_Downlink, 280, strlen(CHAR_Downlink));
    // 	catString(jsonForm, This.Downlink.Name._char, sizeof(jsonForm), strlen(This.Downlink.Name._char)); // 替换
    // }
    // if ((This.needSendJsonDataFlagBit & 0x0100) == 0x0100)
    // {
    // 	catString(jsonForm, CHAR_TouchJson, 280, strlen(CHAR_TouchJson));
    // 	catString(jsonForm, This.Touch_Data.Name._char, sizeof(jsonForm), strlen(This.Touch_Data.Name._char)); // 替换
    // }
    // int jsonFormLen = catString(jsonForm, "}", 280, strlen("}"));
    // catString(This.ArgumentStrbuff, jsonForm, sizeof(This.ArgumentStrbuff), jsonFormLen);
}

jsoncode UserJson_Obj;
/*-----------------------------------建立请求json对象----------------------------------*/
// 新建用户请求json对象 （用户请求的网页对象）
jsoncode New_Json_Obj(void) {
    jsoncode ResJson = {0};
    ResJson.OncePackJsonCode = _OncePackJsonCode;					  // 初始化类方法
    ResJson.setClassBitByhttpPath_Json = _setClassBitByhttpPath_Json; // 初始化类方法
    return ResJson;
}
// （ json 类方法）
strnew _OncePackJsonCode(struct Json_Code This) {
    int NowStartAddr = This.OncePackStartAddr;
    strnew OncePackJson;
    OncePackJson.Name._char = &(This.JsonArgument.ArgumentStrbuff[NowStartAddr]);
    OncePackJson.MaxLen = ((This.JsonCodeBuffLen - NowStartAddr) > ONCE_PACKAGE_LEN ? ONCE_PACKAGE_LEN : (This.JsonCodeBuffLen - NowStartAddr) - 1);

    // 更新首地址 This.OncePackStartAddr
    This.OncePackStartAddr = NowStartAddr + OncePackJson.MaxLen;
    return OncePackJson;
}

// 使用 http URL 获取 设置需要发送数据的标记 （类方法）
void _setClassBitByhttpPath_Json(strnew UserPath, struct Json_Code This) {
#define FunactionNumber 2
    // 后续增加多个方法时，可以使用函数，初始化结构体数组 POST_Funaction
    POSTFunaction POST_Funaction[FunactionNumber]; // 创建方法表

    // 方法1
    POST_Funaction[0].FunactionID = getWifiData;
    POST_Funaction[0].FunNameLen = strlen(API_NAME_1);
    copyString(POST_Funaction[0].FunName, API_NAME_1, POST_Funaction[0].FunNameLen, strlen(API_NAME_1));
    // 方法2
    POST_Funaction[1].FunactionID = saveGateway;
    POST_Funaction[1].FunNameLen = strlen(API_NAME_2);
    copyString(POST_Funaction[1].FunName, API_NAME_2, POST_Funaction[1].FunNameLen, strlen(API_NAME_2));

    This.JsonArgument.needSendJsonDataFlagBit = 0; // 初始化需要回复的 json keyWord
    char * Addr = (myStrstr(UserPath.Name._char, "POST /", 16) + strlen("POST /"));

    // 循环匹配方法
    for (int i = 0; i < FunactionNumber; i++) {
        if (myStrstr(Addr, POST_Funaction[i].FunName, POST_Funaction[i].FunNameLen) != NULL) {
            computerjsoncmd JsonCmdData = NEW_JsonComputerCmd; // 新建 JsonCmd 对象
            switch (POST_Funaction[i].FunactionID) {
                case 1:													// api 获取参数 使用
                    This.JsonArgument.needSendJsonDataFlagBit = 0x00ff; // 标记需要发送的 json 数据
                    break;
                case 2: // saveGateway 保存参数
                    JsonCmdData.JsonCmdStr.Name._char = Send_Task.DonwloadJsonStr[0];
                    JsonCmdData.JsonCmdStr.MaxLen = sizeof(Send_Task.DonwloadJsonStr[0]);
                    JsonCmdData.saveGateway(&JsonCmdData);				// 执行下发的 json 命令
                    This.JsonArgument.needSendJsonDataFlagBit = 0x0000; // 标记需要发送的 json 数据
                    break;
                default:
                    return;
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
// 将需要发送的数据 copy 到待发送的空间
void pushDeviceArgJson(uint16_t * Len, uint8_t * DataBuff) {
    char HeadStr[100] = HEAD_S;
    memset(Send_Task.WifiSendAcceptBuff.Name._char, 0, Send_Task.WifiSendAcceptBuff.MaxLen); // 释放 Send_Task.WifiSendAcceptBuff
    Http_AccepSendBuff_Len = 0;

    if (Send_Task.TaskSum == 1) {
        catString(Send_Task.WifiSendAcceptBuff.Name._char, ERROR0, WIFI_BUFF_MAXLEN, strlen(ERROR0)); // 响应头 HTTP_HEAD
    } else if (Send_Task.TaskID == Send_Task.TaskSum) // 最后一包需要加上 http尾部
    {
        catString(Send_Task.WifiSendAcceptBuff.Name._char, "0\r\n\r\n", WIFI_BUFF_MAXLEN, strlen("0\r\n\r\n")); // 最后一包，追加结束符
    } else if (Send_Task.TaskID == 1) // http头部
    {
        // Content-Type: application/json
        char * Addr = myStrstr(HeadStr, "text/html", 100);
        copyString(Addr, "application/json", 100, strlen("application/json"));
        catString(Send_Task.WifiSendAcceptBuff.Name._char, HeadStr, WIFI_BUFF_MAXLEN, strlen(HeadStr)); // 响应头 HTTP_HEAD
        catString(Send_Task.WifiSendAcceptBuff.Name._char, "\r\n\r\n", WIFI_BUFF_MAXLEN, strlen("\r\n\r\n"));
    } else // json 数据
    {
        strnew UserAskJson = UserJson_Obj.OncePackJsonCode(&UserJson_Obj);
        char Number[16] = {0};															// ONCE_PACKAGE_LEN 单包数据最大 1024
        int CharLen = doneBaseNumberDataToAsciiStr(Number, 16, UserAskJson.MaxLen, 16); // 将 16 进制数字转成数组，并返回长度
        numberArrayToStrArray(Number, Number, CharLen);									// 数组串转字符串
        swapStr((char *)Number, CharLen);
        catString(Send_Task.WifiSendAcceptBuff.Name._char, Number, WIFI_BUFF_MAXLEN, CharLen);							  // 组装长度字符串
        catString(Send_Task.WifiSendAcceptBuff.Name._char, "\r\n", WIFI_BUFF_MAXLEN, strlen("\r\n"));					  // 结束该字段（\r\n结尾）
        catString(Send_Task.WifiSendAcceptBuff.Name._char, UserAskJson.Name._char, WIFI_BUFF_MAXLEN, UserAskJson.MaxLen); // 填充 json 数据
        catString(Send_Task.WifiSendAcceptBuff.Name._char, "\r\n", WIFI_BUFF_MAXLEN, strlen("\r\n"));					  // 结束该字段（\r\n结尾）
    }

    Send_Task.TaskID++;
    for ((*Len) = 0; Send_Task.WifiSendAcceptBuff.Name._char[(*Len)] != '\0'; (*Len)++) {
        DataBuff[(*Len)] = Send_Task.WifiSendAcceptBuff.Name._char[(*Len)];
    }
    memset(Send_Task.WifiSendAcceptBuff.Name._char, 0, Send_Task.WifiSendAcceptBuff.MaxLen); // 释放 Send_Task.WifiSendAcceptBuff
    Http_AccepSendBuff_Len = 0;
}

// 根据收到的请求准备响应数据
void prepareJsonData() {
    UserJson_Obj = NEW_JSON_USER;
    UserJson_Obj.JsonArgument = getDeviceArgumentDATA();								 // 获取 json 数据
    UserJson_Obj.setClassBitByhttpPath_Json(Send_Task.Client_Request[1], &UserJson_Obj); // 根据请求头提供的IP地址 设置需要发送的 key flag
    UserJson_Obj.JsonArgument.getJsonForWifiDataStr(&UserJson_Obj.JsonArgument);		 // 根据 json flag 数据选择需要的 json key
    UserJson_Obj.JsonCodeBuff = UserJson_Obj.JsonArgument.ArgumentStrbuff;
    UserJson_Obj.JsonCodeBuffLen = strlen(UserJson_Obj.JsonArgument.ArgumentStrbuff) + 1;
}
int setJavaTaskMain(void) {
    prepareJsonData(); // 根据收到的请求准备响应数据，建立 UserJson_Obj
    if (UserJson_Obj.JsonArgument.needSendJsonDataFlagBit == 0) {
        Send_Task.TaskID = 0;
        Send_Task.TaskSum = 0;
    } else { // 后期如果需要，增加计算json大小，和分块数
        Send_Task.TaskID = 1;
        Send_Task.TaskSum = 3;
    }
    return Send_Task.TaskSum; // 加上首位两包数据
}

// 获取设备参数
_JsonWifi_DeviceArgument getDeviceArgumentDATA(void) {
    /*将指针指向外部参数*/
    WIFI_GatewayFlag = gateway_id;		  // 外部接口引入 WIFI 网关标识
    WIFI_WebType = service_type;		  // 外部接口引入 WIFI 通信服务类型TCP
    WIFI_WebServiceAddr = server_address; // 外部接口引入 WIFI 服务器地址
    WIFI_WebServicePort = server_port;	  // 外部接口引入 WIFI 服务器端口
    WIFI_MUBS_Baud = mbus_baud_rate;	  // 外部接口引入 WIFI MUBS波特率
    WIFI_RS485_Baud = rs485_baud_rate;	  // 外部接口引入 WIFI RS485波特率
    WIFI_Uplink = uplink_comm;			  // 外部接口引入 WIFI 上行通信
    WIFI_Downlink = downlink_comm;		  // 外部接口引入 WIFI 下行通信
    WIFI_TouchJson = touch_Data;		  // 外部接口引入 WIFI 通信报文
    ///////////////////////////////////////////////////////////////

    // 建立独立格式的参数对象
    _JsonWifi_DeviceArgument Device_ArgDATA_Json = NEW_JSON_Argument;
    // 指向字符串
    Device_ArgDATA_Json.GatewayFlag.Name._char = WIFI_GatewayFlag;		 // 外部接口引入 WIFI 网关标识
    Device_ArgDATA_Json.WebType.Name._char = WIFI_WebType;				 // 外部接口引入 WIFI 通信服务类型TCP
    Device_ArgDATA_Json.WebServiceAddr.Name._char = WIFI_WebServiceAddr; // 外部接口引入 WIFI 服务器地址
    Device_ArgDATA_Json.WebServicePort.Name._char = WIFI_WebServicePort; // 外部接口引入 WIFI 服务器端口
    Device_ArgDATA_Json.MUBS_Baud.Name._char = WIFI_MUBS_Baud;			 // 外部接口引入 WIFI MUBS波特率
    Device_ArgDATA_Json.RS485_Baud.Name._char = WIFI_RS485_Baud;		 // 外部接口引入 WIFI RS485波特率
    Device_ArgDATA_Json.Uplink.Name._char = WIFI_Uplink;				 // 外部接口引入 WIFI 上行通信
    Device_ArgDATA_Json.Downlink.Name._char = WIFI_Downlink;			 // 外部接口引入 WIFI 下行通信
    Device_ArgDATA_Json.Touch_Data.Name._char = WIFI_TouchJson;			 // 外部接口引入 WIFI 通信报文

    // 记录字符串长度
    Device_ArgDATA_Json.GatewayFlag.MaxLen = sizeof(gateway_id);
    Device_ArgDATA_Json.WebType.MaxLen = sizeof(service_type);
    Device_ArgDATA_Json.WebServiceAddr.MaxLen = sizeof(server_address);
    Device_ArgDATA_Json.WebServicePort.MaxLen = sizeof(server_port);
    Device_ArgDATA_Json.MUBS_Baud.MaxLen = sizeof(mbus_baud_rate);
    Device_ArgDATA_Json.RS485_Baud.MaxLen = sizeof(rs485_baud_rate);
    Device_ArgDATA_Json.Uplink.MaxLen = sizeof(uplink_comm);
    Device_ArgDATA_Json.Downlink.MaxLen = sizeof(downlink_comm);
    Device_ArgDATA_Json.Touch_Data.MaxLen = sizeof(touch_Data);
    return Device_ArgDATA_Json;
}

