#include "ATCmd_FunctionSum.h"
#include "main.h"
#include "NumberBaseLic.h"
#include "AT24C0256.h"

// 函数说明
void easyATCmdDataLoad(strnew OutStr, NetDevATCmd NowATCmd); // 发送时的指令装载函数
bool easyATCmdDataDoing(NetDevATCmd NowATCmd);				 // 接受模组反馈的信息，判断是否发送成功，或执行其他操作
bool ReBootDevATCmdDoing(NetDevATCmd NowATCmd);				 // 重启指令特殊处理

//======================================发送时的指令装载函数 ================================================//
void easyATCmdDataLoad(strnew OutStr, NetDevATCmd NowATCmd) {
    copyString(OutStr.Name._char, NowATCmd.ATCmd, OutStr.MaxLen, strlen(NowATCmd.ATCmd));
}
// 波特率设置特殊处理
void setDevBandCmsDataLoad(strnew OutStr, NetDevATCmd NowATCmd) {
    // AT+UART=9600,8,1,NONE,NFC\r\n
    char BandData[16] = {"2000000"};
    int ArrayLen = doneBaseNumberDataToAsciiStr(BandData, 16, UART0_BAND, 16);
    BandData[(ArrayLen < 16) ? ArrayLen : (ArrayLen--)] = '\0';
    memset(OutStr.Name._char, 0, OutStr.MaxLen);
    sprintf(OutStr.Name._char, "AT+UART=%s,8,1,NONE,NFC\r\n", BandData);
}
// 网口设置特殊处理
void setWannCmdDataLoad(strnew OutStr, NetDevATCmd NowATCmd) {
    // AT+WANN=STATIC,192.168.0.7,255.255.255.0,192.168.0.1\r\n
    memset(OutStr.Name._char, 0, OutStr.MaxLen);
    sprintf(OutStr.Name._char, "AT+WANN=STATIC,%s,%s,%s\r\n", AT24CXX_Manager.NET_Local_IP, AT24CXX_Manager.NET_Local_MASK, AT24CXX_Manager.NET_Local_GATEWAY);
    int ArrayLen = strlen(OutStr.Name._char);
    OutStr.Name._char[(ArrayLen < OutStr.MaxLen) ? ArrayLen : (OutStr.MaxLen - 1)] = '\0';
}
void setRemoteIPCmdDataLoad(strnew OutStr, NetDevATCmd NowATCmd) {
    // AT+SOCK=TCPC,169.254.68.235,8234\r\n
    memset(OutStr.Name._char, 0, OutStr.MaxLen);
    sprintf(OutStr.Name._char, "AT+SOCK=TCPC,%s,%d\r\n", AT24CXX_Manager.NET4G_Remote_Url, AT24CXX_Manager.NET_Remote_Port);
    int ArrayLen = strlen(OutStr.Name._char);
    OutStr.Name._char[(ArrayLen < OutStr.MaxLen) ? ArrayLen : (OutStr.MaxLen - 1)] = '\0';
}
void setUserAndPassWordDataLoad(strnew OutStr, NetDevATCmd NowATCmd) {
    // AT+WEBU=admin,admin\r\n
    memset(OutStr.Name._char, 0, OutStr.MaxLen);
    sprintf(OutStr.Name._char, "AT+WEBU=%s,%s\r\n", "admin", "admin");
    int ArrayLen = strlen(OutStr.Name._char);
    OutStr.Name._char[(ArrayLen < OutStr.MaxLen) ? ArrayLen : (OutStr.MaxLen - 1)] = '\0';
}

#warning "增加网口 ATCmd 装载函数"

//========================接受模组反馈的信息，判断是否发送成功，或执行其他操作===================================//
bool easyATCmdDataDoing(NetDevATCmd NowATCmd) {
    if (myStrstr(Now_NetDevParameter.NetDataBuff, NowATCmd.SucessStr, Now_NetDevParameter.NetDataBuff_NowLen) != NULL)
        return true;
    else
        return false;
}
// 重启指令特殊处理
bool ReBootDevATCmdDoing(NetDevATCmd NowATCmd) {
    bool ReFalg = easyATCmdDataDoing(NowATCmd);
    FL_DelayMs(1000); // 延时等待重启结束
    return ReFalg;
}
#warning "增加网口 ATCmd 处理函数"

// 定义AT指令
NetDevParameter Now_NetDevParameter;
NetDevATCmd NetDevice_ATData[ATCMD_MAXNUMBER] = {0};

// 设置或查询 AT指令 的装载方法
void _DataInstallation(strnew OutStr, struct _NetDevATCmdData This) {
    // 调用实际装载的函数
    switch (This.LoadATSendFunName) {
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
    }
}
// 处理指令返回的方法
bool _DoingATCmdResData(struct _NetDevATCmdData This) {
    bool ResFlag = false;
    // 调用实际处理回复的函数
    switch (This.DoingATReceiveFunName) {
        case easyATCmdDataDoingFun:
            ResFlag = easyATCmdDataDoing(This);
            break;
        case ReBootDevFun:
            ResFlag = ReBootDevATCmdDoing(This);
            break;
        case ResetTrueFlagDoingFun:
            ResFlag = true;
            break;
    }
    return ResFlag;
}
// 建立对象的函数
NetDevATCmd New_NetDevAT_Obj(NetDevATCmd DataInit) {
    DataInit.DataInstallation = _DataInstallation;
    DataInit.DoingATCmdResData = _DoingATCmdResData;
    return DataInit;
}

void MQTT_NetAT_Init(void) {
    NetDevATCmd Temp[ATCMD_MAXNUMBER] = {
        {0, 1, "AT+E=ON\r\n", "\r\n+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {1, 2, "AT+VER\r\n", "\r\n+OK=", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {2, 3, "AT+RFCEN=OFF\r\n", "\r\n+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {3, 4, "AT+MAC\r\n", "\r\n+OK=", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {4, 5, "AT+DNS\r\n", "\r\n+OK=", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {5, 6, "AT+UARTSET=ON\r\n", "\r\n+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {6, 7, "AT+UART=115200,8,1,NONE,NFC\r\n", "\r\n+OK", true, 3, 3, false, setBandDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {7, 8, "AT+WANN=STATIC,192.168.0.7,255.255.255.0,192.168.0.1\r\n", "\r\n+OK", true, 3, 3, false, setWannDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {8, 9, "AT+WEBPORT=80\r\n", "\r\n+OK", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {9, 10, "AT+SOCKPORT=20108\r\n", "\r\n+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {10, 11, "AT+TCPSE=KICK\r\n", "\r\n+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {11, 12, "AT+WEBU=admin,admin\r\n", "\r\n+OK", true, 3, 3, false, setUserAndPassWordDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {12, 13, "AT+SOCK=TCPC,169.254.68.235,8234\r\n", "\r\n+OK", true, 3, 3, false, setRemoteIPDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {13, 14, "AT+SOCKLK\r\n", "\r\n+OK=", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {14, 15, "AT+SCSLINK=ON\r\n", "\r\n+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {15, 16, "AT+CLIENTRST=ON\r\n", "\r\n+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {16, 17, "AT+UARTCLBUF=ON\r\n", "\r\n+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {17, 18, "AT+RSTIM\r\n", "\r\n+OK=", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {18, 19, "AT+MAXSK=1\r\n", "\r\n+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {19, 20, "AT+MID=NJHY\r\n", "\r\n+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {20, 21, "AT+Z\r\n", "\r\n+OK", true, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {21, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {22, -1, "", "", false, 3, 3, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {23, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {24, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {25, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {26, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {27, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {28, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {29, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {30, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {31, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {32, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {33, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {34, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {35, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {36, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {37, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {38, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {39, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {40, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {41, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {42, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
        {43, -1, "", "", false, 0, 0, false, easyATCmdDataLoadFun, easyATCmdDataDoingFun, NULL, NULL},
    };

    for (int i = 0; i < 43; i++) {
        NetDevice_ATData[i] = Temp[i];
        NetDevice_ATData[i] = New_NetDevAT_Obj(NetDevice_ATData[i]);
    }
    return;
}
