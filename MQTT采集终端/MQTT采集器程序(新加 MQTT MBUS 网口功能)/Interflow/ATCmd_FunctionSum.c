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
        {0,1,"AT+ENTM\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//退出AT命令模式，进入透传模式
        {1,2,"+++","a",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//设置进入ATCmd模式
        {2,3,"a","+ok",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//确认进入AT模式
        {3,4,"AT+VER\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询模块固件版本
        {4,5,"AT+E=ON\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置回显使能
        {5,6,"AT+UART2=115200,8,1,NONE,NFC\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置N号串口接口参数
        {6,7,"AT+UARTCLBUF=ON\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置连接前是否清理串口缓存
        {7,8,"AT+DNSTYPE=AUTO\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置模块DNS获取方式
        {8,9,"AT+WANN=STATIC,192.168.1.117,255.255.255.0,192.168.1.1\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置模块获取到的WAN口IP（DHCP/STATIC）
        {9,10,"AT+WEBSOCKPORT1=6432\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置WebSocket端口号
        {10,11,"AT+MAXSK2=1\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置N号串口的最大连接数量
        {11,12,"AT+TCPSE2=KICK\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置N号串口的TCPS超过最大连接数的工作模式
        {12,13,"AT+SOCKA2=TCPC,59.110.170.225,1883\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置N号串口的socket状态（M参数为socket号）
        {13,14,"AT+SOCKLKA1\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询N号串口的socketM的连接状态
        {14,15,"AT+MQTTEN=ON\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置MQTT网关功能状态
        {15,16,"AT+MQTTLPORT=0\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置MQTTsocket连接本地端口号
        {16,17,"AT+MQTTAUTH=ON\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置MQTT连接验证开启状态
        {17,18,"AT+MQTTCID=HY_QL\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置MQTT客户端ID
        {18,19,"AT+MQTTUSER=ql\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置MQTT用户名
        {19,20,"AT+MQTTPSW=ql\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置MQTT用户密码
        {20,21,"AT+MQTTPUBCUSEN=ON\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置MQTT主题发布自定义模式
        {21,22,"AT+MQTTPUB=1,OFF,/hy/gw/02345678903,0,1,0,OFF,2\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置MQTT的预置发布主题信息
        {22,23,"AT+MQTTSUB=1,ON,/hy/gw,0,0,&#44,2\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置MQTT的预置订阅主题信息
        {23,24,"AT+MQTTSER=59.110.170.225,1883\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置MQTT网关功能的服务器IP地址，端口号
        {24,25,"AT+MID=NJHY\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置模块名称
        {25,26,"AT+WEBPOINT=OFF\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//查询/设置WebSocket的方向
        {26,27,"AT+CFGTF\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//设置将当前参数保存为用户默认参数
        {27,-1,"AT+Z\r\n","+OK",true,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},	//设备重启
        {28,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
        {29,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
        {30,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
        {31,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
        {32,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
        {33,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
        {34,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
        {35,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
        {36,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
        {37,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
        {38,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
        {39,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
        {40,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
        {41,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
        {42,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
        {43,-1,"","",false,3,3,false,easyATCmdDataLoadFun,easyATCmdDataDoingFun, NULL, NULL},
    };

    for (int i = 0; i < 43; i++) {
        NetDevice_ATData[i] = Temp[i];
        NetDevice_ATData[i] = New_NetDevAT_Obj(NetDevice_ATData[i]);
    }
    return;
}
