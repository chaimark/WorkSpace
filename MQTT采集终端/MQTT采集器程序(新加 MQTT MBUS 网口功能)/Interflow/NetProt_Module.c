#include "NetProt_Module.h"
#include "ATCmd_FunctionSum.h"
#include "NumberBaseLic.h"
#include "Uart.h"
#include "RTC.h"
#include "AT24C0256.h"
#include "MQTT_JSON.h"
#include "Gpio.h"
#include "main.h"
#include "StrLib.h"
#include "Define.h"

bool SetDevATCMDModel_ThroughSendData(void);
void setNetArgumentInit(void);
void MQTT_NET_10mS_Timers_Add(void);
bool copyDataForUART(void);

// 判断是否连接上MQTT 发送心跳等待回复，如果有回复则 MQQT 连接成功
bool isMQTTLinkOnleng(void) {
    return true;
}
// 注册上线
void publishUpDataForMQTT(void) {
    memset(Now_NetDevParameter.NetDataBuff, 0, sizeof(Now_NetDevParameter.NetDataBuff));
    //登录: 0x10, 0x16, 0x00, 0x04, 0x4D, 0x51, 0x4 ,0x54, 0x04, 0xC2, 0x00, 0x64, 0x00, 0x02, 0x71, 0x6C, 0x00, 0x02, 0x71, 0x6C, 0x00, 0x02, 0x71, 0x6C
    char LoginStr[24] = {0x10, 0x16, 0x00, 0x04, 0x4D, 0x51, 0x04 ,0x54, 0x04, 0xC2, 0x00, 0x64, 0x00, 0x02, 0x71, 0x6C, 0x00, 0x02, 0x71, 0x6C, 0x00, 0x02, 0x71, 0x6C};
    //主题: 0x82, 0x0C, 0x00, 0x0A, 0x00, 0x07, 0x68, 0x79, 0x2F, 0x67, 0x77, 0x2F, 0x23, 0x00 
    char Tille[14] = {0x82, 0x0C, 0x00, 0x0A, 0x00, 0x07, 0x68, 0x79, 0x2F, 0x67, 0x77, 0x2F, 0x23, 0x00,};
    sendDataByNetProt((unsigned char *)LoginStr, 24);
    FL_DelayMs(2000); // 延时等待重启结束
    sendDataByNetProt((unsigned char *)Tille, 14);
    char JsonData[] = {"hy/gw/get/02345678903{\"gw\":{\"msg_id\":16,\"msg_gw_pr\":0,\"gw_id\":\"02345678903\"},\"data\":{\"update_gw_id\":\"02345678904\"}}"};
    FL_DelayMs(2000); // 延时等待重启结束
    sendDataByNetProt((unsigned char *)JsonData, strlen(JsonData));
}
// 处理用户请求/命令
bool doingUserRequest(void) {
    return true;
}
// 处理模组回复,判断指令是否发送成功，或者模组再忙等待返回等操作
bool doingATCmdResult(NetDevATCmd NowATCmd) {
    return NowATCmd.DoingATCmdResData(&NowATCmd);
}
// 发送ATCmd
bool sendATCmdData(NetDevATCmd NowATCmd) {
    char Temp[500] = {0};
    strnew ATCmd_SendBUFF = NEW_NAME(Temp);

    for (int SendCount_i = 0; SendCount_i < NowATCmd.CmdSendCount; SendCount_i++) {
        // 清理接收缓存
        memset(Now_NetDevParameter.NetDataBuff, 0, sizeof(Now_NetDevParameter.NetDataBuff));
        // 装载指令
        NowATCmd.DataInstallation(ATCmd_SendBUFF, &NowATCmd);
        // 发送指令
        sendDataByNetProt((unsigned char *)ATCmd_SendBUFF.Name._char, strlen(ATCmd_SendBUFF.Name._char));
        FL_DelayMs(1000);
        for (int ResCount_i = 0; ResCount_i < NowATCmd.CmsResCount; ResCount_i++) {
            // 模组是否回复
            if (copyDataForUART() == false) {
                // 判断是否可以跳过
                if ((NowATCmd.DoingATReceiveFunName == ResetTrueFlagDoingFun) && ((ResCount_i + 1) == NowATCmd.CmsResCount)) {
                    return true; // 发送结束也没收到回复，直接跳过
                }
                continue;
            }
            // 处理模组的回复
            if (doingATCmdResult(NowATCmd) == false) {
                return false; // AT 指令发送失败
            }
            // 该指令是否需要上位机回复，如果不需要直接回复 true
            if (NowATCmd.IsGetComputerResFlag == false) {
                return true; // 上位机回复成功
            }
            return doingUserRequest(); // 上位机回复成功
        }
    }
    return true; // AT 指令发送限制为0,直接跳过该指令
}

void MOTT_Net_Task(void) {
    char * p_star = NULL;

    if (Now_NetDevParameter.CheckTCPLinkFlag == true) { // 检查当前设备 TCP 是否连接
        if (TCP_Link_OnlineFlga != 0) {                 // 检查当前设备 TCP 是否连接，低电平有效
            Now_NetDevParameter.NowTCPLinkFlag = false; // 设备 TCP 是否连接
        }

        Now_NetDevParameter.ReBootCount++;
    } else if (Now_NetDevParameter.CheckOnlineFlag == true) { // 检查当前设备 network 是否在线
        if (isMQTTLinkOnleng() == false) {
            Now_NetDevParameter.NowNetOnlineFlag = false; // 设备 network 是否在线
            Now_NetDevParameter.ReBootCount++;
        }
    }
    // 不在线或无连接，重新联网通信
    if ((Now_NetDevParameter.NowNetOnlineFlag == false) || (Now_NetDevParameter.NowTCPLinkFlag == false)) {
        setNetArgumentInit();
        // 设置模组进入 AT 模式
        if (SetDevATCMDModel_ThroughSendData() == false)
            return;
        int NowStep = 0;
        while (NowStep != -1) {
            // 一个是否执行的开关，后续可扩展为指令控制
            if (NetDevice_ATData[NowStep].RunFlag == true) {
                for (int i = 0; i < 3; i++) {
                    if (sendATCmdData(NetDevice_ATData[NowStep])) {
                        break;
                    }
                    // 联网遇到问题，中断联网进程
                    if (i == 2) {
                        Now_NetDevParameter.CheckTCPLinkFlag = false; // TCP 连接失败，不需要检查，直接准备重启
                        Now_NetDevParameter.CheckOnlineFlag = false;  // TCP 连接失败，不需要检查，直接准备重启
                        Now_NetDevParameter.NowNetOnlineFlag = false; // 标记不在线，下次运行这个任务时重启
                        Now_NetDevParameter.NowTCPLinkFlag = false;	  // 标记不在线，下次运行这个任务时重启
                        Now_NetDevParameter.ReBootCount++;			  // 重连一次
                        return;
                    }
                }
            }
            NowStep = NetDevice_ATData[NowStep].Next_CmdID;
        }
        // 检查当前设备 TCP 是否连接，低电平有效
        if (TCP_Link_OnlineFlga == 0) {
            Now_NetDevParameter.CheckTCPLinkFlag = false;   // TCP 连接成功后暂时不需要检查
            Now_NetDevParameter.CheckOnlineFlag = false;    // TCP 连接成功后暂时不需要检查
            Now_NetDevParameter.ReBootCount = 0;            // 复位重启计数器
            if (Switch_4G_Flag == 0) {                      // 只有再关闭4G 的情况下 才可以使用接口判断tcp是否连接成功
                Now_NetDevParameter.NowTCPLinkFlag = true;  // TCP 已连接
            }
            // 判断 MQTT 是否在线
            if (isMQTTLinkOnleng()) {
                Now_NetDevParameter.NowNetOnlineFlag = true; // 设备已在线
                Now_NetDevParameter.ReBootCount = 0;		 // 连接成功，重器计数清零
                publishUpDataForMQTT();// 注册上线
            }
        }
        memset(Now_NetDevParameter.NetDataBuff, 0, sizeof(Now_NetDevParameter.NetDataBuff));
    } else { // 设备在线进入数据透传模式
        if (Now_NetDevParameter.MQTT_NET_Receive_checkTime == 0) { // 20ms check 一次，后获取数据
            Now_NetDevParameter.MQTT_NET_Receive_checkTime = 2;
            if (copyDataForUART()) { // 从 UART0Ddata 获取数据
                // 解析收到的数据，找到 josn 数据
                if (myStrstr(Now_NetDevParameter.NetDataBuff, "+QMTRECV:", sizeof(Now_NetDevParameter.NetDataBuff)) != NULL) {
                    if (((p_star = myStrstr(Now_NetDevParameter.NetDataBuff, "hy/gw/get/", sizeof(Now_NetDevParameter.NetDataBuff))) != NULL) && ((p_star = strstr(p_star, "\",")) != NULL)) {
                        MQTT_JSON_Analysis(p_star);
                    }
                }
            }
        }
    }
}

void MQTT_NET_10mS_Timers_Add(void) {
    static int CheckTime = 100000; // 每 100 s 后运行任务
    if (!(--CheckTime)) {
        CheckTime = 100000;							 // 100 秒后，判断你是否处于连接状态
        Now_NetDevParameter.CheckOnlineFlag = true;	 // 检查网络在线标记
        Now_NetDevParameter.CheckTCPLinkFlag = true; // 检查TCP连接标记
    }
    // 每 10ms 计数一次
    ((Now_NetDevParameter.MQTT_NET_Receive_checkTime != 0) ? (Now_NetDevParameter.MQTT_NET_Receive_checkTime--) : 0);
}
// 搬运串口数据
bool copyDataForUART(void) {
    if (UART0Ddata.RxLen != 0) {
        NVIC_DisableIRQ(UART0_IRQn); // 关闭中断使能
        if (Now_NetDevParameter.NetDataBuff_NowLen + UART0Ddata.RxLen <= sizeof(Now_NetDevParameter.NetDataBuff)) { // 追加到 Now_NetDevParameter.NetDataBuff
            Now_NetDevParameter.NetDataBuff_NowLen = catString(Now_NetDevParameter.NetDataBuff, (char *)UART0Ddata.RxBuf, sizeof(Now_NetDevParameter.NetDataBuff), UART0Ddata.RxLen);
        } else {
            memset(Now_NetDevParameter.NetDataBuff, 0, sizeof(Now_NetDevParameter.NetDataBuff));											  // 释放 HTTPBuff_p
            copyString(Now_NetDevParameter.NetDataBuff, (char *)UART0Ddata.RxBuf, sizeof(Now_NetDevParameter.NetDataBuff), UART0Ddata.RxLen); // 接收数据
            Now_NetDevParameter.NetDataBuff_NowLen = UART0Ddata.RxLen;
        }
        UART0Ddata.RxLen = 0;
        NVIC_EnableIRQ(UART0_IRQn); // 开启中断使能
        return true;
    } else {
        return false;
    }
}
void MQTT_NetProt_Init(void) {
    // EC20_WAKEUP_IN_L;
    // NET_CFG_ON;
    V33_PWR_CTL_ON;
    NET_RST_OFF;
    FL_DelayMs(500); // 延时等待重启结束
    NET_RST_ON;

    // 寻找正确的band
    // int Count = 0;
    // int TempBand[3] = {2400, 9600, 115200};
    // for (Count = 0; Count < 3; Count++)
    // {
    // 	Select_OFF_NET_To_UART0;
    // 	UART0_BAND = TempBand[Count];
    // 	MF_NET_To_UART0_Init(); // 以太网
    // 	MF_NET_To_UART0_Interrupt_Init();
    // 	Select_ON_NET_To_UART0;

    // 	FL_DelayMs(1000);
    // 	sendDataByNetProt("+++", 3);
    // 	if (copyDataForUART())
    // 	{
    // 		if (myStrstr(Now_NetDevParameter.NetDataBuff, "a", Now_NetDevParameter.NetDataBuff_NowLen) != NULL)
    // 		{
    // 			UART0_BAND = TempBand[Count];
    // 			MF_NET_To_UART0_Init();
    // 			break;
    // 		}
    // 	}
    // }
    // if (Count >= 3)
    // {
    // 		Select_OFF_NET_To_UART0;
    // 		UART0_BAND = 9600;
    // 		MF_NET_To_UART0_Init(); // 以太网
    // 		MF_NET_To_UART0_Interrupt_Init();
    // 		Select_ON_NET_To_UART0;
    // }

    MQTT_NetAT_Init();
    return;
}

// 设置模组处于AT模式
bool SetDevATCMDModel_ThroughSendData(void) {
    // 先复位模组 ==========
    NET_RST_OFF;
    FL_DelayMs(500); // 拉低 500 ms
    NET_RST_ON;
    FL_DelayMs(2000); // 延时等待重启结束
    copyDataForUART();
    bool ATConfig_Flag = true;
    // 进入AT模式 ==========
    char cmdStr[3][2][20] = {{"AT+ENTM\r\n", "\r\n+OK"}, {"+++", "a"}, {"a", "+ok"}};
    for (int i = 0; i < 3; i++) {
        memset(Now_NetDevParameter.NetDataBuff, 0, sizeof(Now_NetDevParameter.NetDataBuff));	// 释放 HTTPBuff_p
        sendDataByNetProt((unsigned char *)cmdStr[i][0], strlen(cmdStr[i][0]));
        FL_DelayMs(2000);
        if (i != 0) {
            if (copyDataForUART() == true) {
                if (myStrstr(Now_NetDevParameter.NetDataBuff, cmdStr[i][1], Now_NetDevParameter.NetDataBuff_NowLen) != NULL)
                    continue;
                else {
                    ATConfig_Flag = false; // 无法进入AT模式，直接退出，等待下一次重启
                    break;
                }
            }
        } else {
            copyDataForUART();
        }
    }
    if (ATConfig_Flag == true) {
        sendDataByNetProt("AT+VER\r\n", strlen("AT+VER\r\n"));
        FL_DelayMs(2000);
        ATConfig_Flag = copyDataForUART();
    }
    return ATConfig_Flag;
}
// AT 参数初始化
void setNetArgumentInit(void) {
    Now_NetDevParameter.MQTT_NET_Receive_checkTime = 2;
    return;
}
