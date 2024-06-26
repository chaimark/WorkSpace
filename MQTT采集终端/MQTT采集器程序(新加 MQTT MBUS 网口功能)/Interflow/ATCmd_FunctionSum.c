#include "ATCmd_FunctionSum.h"
#include "main.h"
#include "NumberBaseLic.h"
#include "AT24C0256.h"

// 函数说明
void easyATCmdDataLoad(strnew OutStr, NetDevATCmd NowATCmd); // 发送时的指令装载函数
bool easyATCmdDataDoing(NetDevATCmd NowATCmd);               // 接受模组反馈的信息，判断是否发送成功，或执行其他操作
bool ReBootDevATCmdDoing(NetDevATCmd NowATCmd);              // 重启指令特殊处理

// 定义AT指令
NetDevParameter Now_NetDevParameter;
NetDevATCmd NetDevice_ATData[ATCMD_MAXNUMBER] = {
    {0, 1, "AT+E=ON\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                                  // 查询/设置回显使能
    {1, 2, "AT+UART2=115200,8,1,NONE,NFC\r\n", "+OK", false, 3, 3, false, setBandDataLoadFun, ResetTrueFlagDoingFun, NULL, NULL},                              // 查询/设置N号串口接口参数
    {2, 3, "AT+UARTCLBUF=ON\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                          // 查询/设置连接前是否清理串口缓存
    {3, 4, "AT+DNSTYPE=AUTO\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                          // 查询/设置模块DNS获取方式
    {4, 5, "AT+WANN=STATIC,192.168.1.117,255.255.255.0,192.168.1.1\r\n", "+OK", true, 3, 3, false, setWannDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},     // 查询/设置模块获取到的WAN口IP（DHCP/STATIC）
    {5, 6, "AT+WEBSOCKPORT1=6432\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                     // 查询/设置WebSocket端口号
    {6, 7, "AT+MAXSK2=1\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                              // 查询/设置N号串口的最大连接数量
    {7, 8, "AT+TCPSE2=KICK\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                           // 查询/设置N号串口的TCPS超过最大连接数的工作模式
    {8, 9, "AT+SOCKA2=TCPC,59.110.170.225,1883\r\n", "+OK", true, 3, 3, false, setRemoteIPDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                     // 查询/设置N号串口的socket状态（M参数为socket号）
    {9, 10, "AT+SOCKLKA2\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                             // 查询N号串口的socketM的连接状态
    {10, 11, "AT+MQTTEN=ON\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                           // 查询/设置MQTT网关功能状态
    {11, 12, "AT+MQTTLPORT=0\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                         // 查询/设置MQTTsocket连接本地端口号
    {12, 13, "AT+MQTTAUTH=ON\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                         // 查询/设置MQTT连接验证开启状态
    {13, 14, "AT+MQTTCID=HY_QL\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                       // 查询/设置MQTT客户端ID
    {14, 15, "AT+MQTTUSER=ql\r\n", "+OK", true, 3, 3, false, setUserAndPassWordDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                // 查询/设置MQTT用户名
    {15, 16, "AT+MQTTPSW=ql\r\n", "+OK", true, 3, 3, false, setUserAndPassWordDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                 // 查询/设置MQTT用户密码
    {16, 17, "AT+MQTTPUBCUSEN=ON\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                     // 查询/设置MQTT主题发布自定义模式
    {17, 18, "AT+MQTTPUB=1,ON,hy/gw/get/02345678903,0,MY_PUB,0,OFF,2\r\n", "+OK", true, 3, 3, false, setMQTTMassagePUBFun, easyATCmdDataDoingFun, NULL, NULL}, // 查询/设置MQTT的预置发布主题信息
    {18, 19, "AT+MQTTSUB=1,ON,/hy/gw/#,0,0,&#44,2\r\n", "+OK", true, 3, 3, false, setMQTTMassageSUBFun, easyATCmdDataDoingFun, NULL, NULL},                    // 查询/设置MQTT的预置订阅主题信息
    {19, 20, "AT+MQTTSER=59.110.170.225,1883\r\n", "+OK", true, 3, 3, false, setRemoteIPDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                       // 查询/设置MQTT网关功能的服务器IP地址，端口号
    {20, 21, "AT+MID=NJHY\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                            // 查询/设置模块名称
    {21, 22, "AT+WEBPOINT=OFF\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                        // 查询/设置WebSocket的方向
    {22, 23, "AT+CFGTF\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                               // 设置将当前参数保存为用户默认参数
    {23, -1, "AT+Z\r\n", "+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},                                                   // 设备重启
    {24, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {25, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {26, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {27, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {28, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {29, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {30, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {31, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {32, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {33, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {34, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {35, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {36, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {37, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {38, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {39, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {40, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {41, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {42, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    {43, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
};

//======================================发送时的指令装载函数 ================================================//
void easyATCmdDataLoad(strnew OutStr, NetDevATCmd NowATCmd)
{
    copyString(OutStr.Name._char, NowATCmd.ATCmd, OutStr.MaxLen, strlen(NowATCmd.ATCmd));
}
// 需要替换指令，并执行简单装载的指令
void flagChangeUartIDCmdDataLoad(strnew OutStr, NetDevATCmd NowATCmd)
{
    char AtCmd[100] = {0};
    copyString(AtCmd, NowATCmd.ATCmd, 100, strlen(NowATCmd.ATCmd));

    char *pAddr = NULL;
    if ((pAddr = myStrstr(AtCmd, "SOCKLKA", strlen(AtCmd))) != NULL)
    {
        pAddr += strlen("SOCKLKA") + 1;
    }
    else if ((pAddr = myStrstr(AtCmd, "MAXSK", strlen(AtCmd))) != NULL)
    {
        pAddr += strlen("MAXSK") + 1;
    }
    else if ((pAddr = myStrstr(AtCmd, "TCPSE", strlen(AtCmd))) != NULL)
    {
        pAddr += strlen("TCPSE") + 1;
    }
    (*pAddr) = '2'; // 修改为使用串口2
    copyString(OutStr.Name._char, AtCmd, OutStr.MaxLen, strlen(AtCmd));
}
// 波特率设置特殊处理
void setDevBandCmsDataLoad(strnew OutStr, NetDevATCmd NowATCmd)
{
    // AT+UART2=9600,8,1,NONE,NFC\r\n
    char BandData[16] = {"2000000"};
    int ArrayLen = doneBaseNumberDataToAsciiStr(BandData, 16, UART0_BAND, 16);
    BandData[(ArrayLen < 16) ? ArrayLen : (ArrayLen--)] = '\0';
    memset(OutStr.Name._char, 0, OutStr.MaxLen);
    sprintf(OutStr.Name._char, "AT+UART2=%s,8,1,NONE,NFC\r\n", BandData);
}
// 网口设置特殊处理
void setWannCmdDataLoad(strnew OutStr, NetDevATCmd NowATCmd)
{
    // AT+WANN=STATIC,192.168.0.7,255.255.255.0,192.168.0.1\r\n
    memset(OutStr.Name._char, 0, OutStr.MaxLen);
    sprintf(OutStr.Name._char, "AT+WANN=STATIC,%s,%s,%s\r\n", AT24CXX_Manager.NET_Local_IP, AT24CXX_Manager.NET_Local_MASK, AT24CXX_Manager.NET_Local_GATEWAY);
    int ArrayLen = strlen(OutStr.Name._char);
    OutStr.Name._char[(ArrayLen < OutStr.MaxLen) ? ArrayLen : (OutStr.MaxLen - 1)] = '\0';
}
void setRemoteIPCmdDataLoad(strnew OutStr, NetDevATCmd NowATCmd)
{
    // AT+SOCKA2=TCPC,59.110.170.225,1883\r\n
    memset(OutStr.Name._char, 0, OutStr.MaxLen);
    if (myStrstr(NowATCmd.ATCmd, "SOCKA2", strlen(NowATCmd.ATCmd)) != NULL)
    {
        sprintf(OutStr.Name._char, "AT+SOCKA2=TCPC,%s,%d\r\n", AT24CXX_Manager.NET4G_Remote_Url, AT24CXX_Manager.NET_Remote_Port);
    }
    else if (myStrstr(NowATCmd.ATCmd, "MQTTSER", strlen(NowATCmd.ATCmd)) != NULL)
    {
        sprintf(OutStr.Name._char, "AT+MQTTSER=%s,%d\r\n", AT24CXX_Manager.NET4G_Remote_Url, AT24CXX_Manager.NET_Remote_Port);
    }
    else
    {
        copyString(OutStr.Name._char, NowATCmd.ATCmd, OutStr.MaxLen, strlen(NowATCmd.ATCmd));
    }
    int ArrayLen = strlen(OutStr.Name._char);
    OutStr.Name._char[(ArrayLen < OutStr.MaxLen) ? ArrayLen : (OutStr.MaxLen - 1)] = '\0';
}
void setUserAndPassWordDataLoad(strnew OutStr, NetDevATCmd NowATCmd)
{
    memset(OutStr.Name._char, 0, OutStr.MaxLen);
    if (strcmp(NowATCmd.ATCmd, "AT+MQTTUSER=ql\r\n") == 0)
    {
        sprintf(OutStr.Name._char, "AT+MQTTUSER=%s\r\n", "ql");
    }
    else if (strcmp(NowATCmd.ATCmd, "AT+MQTTPSW=ql\r\n") == 0)
    {
        sprintf(OutStr.Name._char, "AT+MQTTPSW=%s\r\n", "ql");
    }
    else
    {
        copyString(OutStr.Name._char, NowATCmd.ATCmd, OutStr.MaxLen, strlen(NowATCmd.ATCmd));
    }
    int ArrayLen = strlen(OutStr.Name._char);
    OutStr.Name._char[(ArrayLen < OutStr.MaxLen) ? ArrayLen : (OutStr.MaxLen - 1)] = '\0';
}
void setMQTTMassagePUBCmdDataLoad(strnew OutStr, NetDevATCmd NowATCmd)
{
    static uint8_t My_PUSName_I = 0;
    // AT+MQTTPUB=1,ON,hy/gw/get/02345678903,0,1,0,OFF,2
    memset(OutStr.Name._char, 0, OutStr.MaxLen);
    char IDTemp[13] = {0};
    HEX2ToASCII((char *)AT24CXX_Manager.gw_id, 6, IDTemp, 12);
    sprintf(OutStr.Name._char, "AT+MQTTPUB=1,ON,hy/gw/set/%s,0,%d,0,OFF,2\r\n", &IDTemp[1], My_PUSName_I);
    int ArrayLen = strlen(OutStr.Name._char);
    OutStr.Name._char[(ArrayLen < OutStr.MaxLen) ? ArrayLen : (OutStr.MaxLen - 1)] = '\0';
}
void setMQTTMassageSUBCmdDataLoad(strnew OutStr, NetDevATCmd NowATCmd)
{
    // AT+MQTTSUB=1,ON,/hy/gw/#,0,0,&#44,2
    memset(OutStr.Name._char, 0, OutStr.MaxLen);
    sprintf(OutStr.Name._char, "AT+MQTTSUB=1,ON,/hy/gw/get/02345678903,0,0,&#44,2\r\n");
    int ArrayLen = strlen(OutStr.Name._char);
    OutStr.Name._char[(ArrayLen < OutStr.MaxLen) ? ArrayLen : (OutStr.MaxLen - 1)] = '\0';
}
#warning "增加网口 ATCmd 装载函数"

//========================接受模组反馈的信息，判断是否发送成功，或执行其他操作===================================//
bool easyATCmdDataDoing(NetDevATCmd NowATCmd)
{
    if (myStrstr(Now_NetDevParameter.NetDataBuff, NowATCmd.SucessStr, Now_NetDevParameter.NetDataBuff_NowLen) != NULL)
        return true;
    else
        return false;
}
// 重启指令特殊处理
bool ReBootDevATCmdDoing(NetDevATCmd NowATCmd)
{
    bool ReFalg = easyATCmdDataDoing(NowATCmd);
    FL_DelayMs(1000); // 延时等待重启结束
    return ReFalg;
}
#warning "增加网口 ATCmd 处理函数"

// 设置或查询 AT指令 的装载方法
void _DataInstallation(strnew OutStr, struct _NetDevATCmdData This)
{
    // 调用实际装载的函数
    switch (This.LoadATSendFunName)
    {
    case flagChangeUartIDFun:
        flagChangeUartIDCmdDataLoad(OutStr, This);
        break;
    case easyATCmdDataLoadFun:
        easyATCmdDataLoad(OutStr, This);
        break;
    case setBandDataLoadFun:
        setDevBandCmsDataLoad(OutStr, This);
        break;
    case setWannDataLoadFun:
        setWannCmdDataLoad(OutStr, This);
        break;
    case setRemoteIPDataLoadFun:
        setRemoteIPCmdDataLoad(OutStr, This);
        break;
    case setUserAndPassWordDataLoadFun:
        setUserAndPassWordDataLoad(OutStr, This);
        break;
    case setMQTTMassagePUBFun:
        setMQTTMassagePUBCmdDataLoad(OutStr, This);
        break;
    case setMQTTMassageSUBFun:
        setMQTTMassageSUBCmdDataLoad(OutStr, This);
        break;
    }
}
// 处理指令返回的方法
bool _DoingATCmdResData(struct _NetDevATCmdData This)
{
    bool ResFlag = false;
    // 调用实际处理回复的函数
    switch (This.DoingATReceiveFunName)
    {
    case ResetTrueFlagDoingFun:
        ResFlag = true;
        break;
    case easyATCmdDataDoingFun:
        ResFlag = easyATCmdDataDoing(This);
        break;
    case ReBootDevFun:
        ResFlag = ReBootDevATCmdDoing(This);
        break;
    }
    return ResFlag;
}
// 建立对象的函数
NetDevATCmd New_NetDevAT_Obj(NetDevATCmd DataInit)
{
    DataInit.DataInstallation = _DataInstallation;
    DataInit.DoingATCmdResData = _DoingATCmdResData;
    return DataInit;
}

void MQTT_NetAT_Init(void)
{
    for (int i = 0; i < ATCMD_MAXNUMBER; i++)
    {
        NetDevice_ATData[i] = New_NetDevAT_Obj(NetDevice_ATData[i]);
    }
    return;
}
