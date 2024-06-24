#include "StrLib.h"
#include "AcceptAnalyze.h"

// 函数说明
int saveHttpDataAndanalyze(char FrontTcpLinkID);
void deleteCmdTask(void);
int getCmdTaskNumberOrLinkID(int UserCase);
// 判断当前时候有保存但未响应的请求
int getCmdTaskNumberOrLinkID(int UserCase)
{
    if ((Send_Task.TaskID > Send_Task.TaskSum) || (Send_Task.TaskSum == 0))
    {
        Send_Task.TaskID = 0;
        Send_Task.TaskSum = 0;
    }
    if (UserCase == 2)
        return Send_Task.FrontTcpLinkID;
    else
        return Send_Task.CmdNumber;
}
// 获取某行的长度
int getHttpStrToBuffLen(char *StartAdd)
{
    char *Tempch = StartAdd;
    int Addr_i = 0;
    while ((Tempch[Addr_i] != '\r') && (Tempch[Addr_i + 1] != '\n'))
    {
        Addr_i++;
    }
    return Addr_i;
}
// 分析转载指令的方法
int AndanalyzeFunaction()
{
    char *TempStartAdd = NULL;
    int Len = 0;
    for (int Cmd_i = 0; Cmd_i < 3; Cmd_i++) // 查询是否下发 josn 数据
    {
        if ((TempStartAdd = myStrstrCont(Send_Task.WifiSendAcceptBuff.Name._char, "\r\n{", Send_Task.WifiSendAcceptBuff.MaxLen, (Cmd_i + 1))) != NULL)
        {
            TempStartAdd += sizeof("\r\n{");
            // 保存 josn 行
            Len = getHttpStrToBuffLen(TempStartAdd); // 获取某行的长度 遇到 \r\n 停止
            // 记录第 josn 行
            copyString(Send_Task.DonwloadJsonStr[Cmd_i], TempStartAdd, sizeof(Send_Task.DonwloadJsonStr), Len);
        }
    }
    // 数据已获取完毕，释放缓存
    memset(Send_Task.WifiSendAcceptBuff.Name._char, 0, Send_Task.WifiSendAcceptBuff.MaxLen); // 释放 Send_Task.WifiSendAcceptBuff
    Http_AccepSendBuff_Len = 0;
    // 分析转载指令的方法
    if (myStrstr(Send_Task.Client_Request[1].Name._char, "GET", Send_Task.Client_Request[1].MaxLen) != NULL) // 不需要要等待，查询时间可以短一点
        return 1;   // 回复 html
    else if (myStrstr(Send_Task.Client_Request[1].Name._char, "POST", Send_Task.Client_Request[1].MaxLen) != NULL) // 不需要要等待，查询时间可以短一点
    {
        return 2;   // 回复 josn
    }
    else
        return 0;   // 不回复
}
// 记录需要解析的数据
int saveHttpDataAndanalyze(char FrontTcpLinkID)
{
    char *TempStartAdd = NULL;
    int Len = 0;
    // 初始化 Send_Taskss
    memset(&Send_Task.Client_Request, 0, 3 * sizeof(strnew)); // 初始化 Send_Task.Client_Request[3]
    Send_Task.TaskID = 0;
    Send_Task.TaskSum = 0;

    // 保存此次通信的 link ID
    Send_Task.FrontTcpLinkID = FrontTcpLinkID;
    Send_Task.Client_Request[0] = Send_Task.WifiSendAcceptBuff;

    // 没有已保存的命令，需要从报文中读取，否则直接装载请求数据
    if (getCmdTaskNumberOrLinkID(1) == 0)
    {
        for (int Cmd_i = 0; Cmd_i < 3; Cmd_i++)
        {
            // 找到第 Cmd_i+1 条指令的地址  +IPD,0,494:......
            if ((TempStartAdd = myStrstrCont(Send_Task.Client_Request[0].Name._char, "+IPD", Send_Task.Client_Request[0].MaxLen, (Cmd_i + 1))) != NULL)
            {
                Send_Task.CmdNumber++;
                for (int i = 0; i < ((Send_Task.Client_Request[0].Name._char + Send_Task.Client_Request[0].MaxLen) - TempStartAdd); i++)
                {
                    if (TempStartAdd[i] == ':')
                    {
                        TempStartAdd = &TempStartAdd[i] + 1;
                        break;
                    }
                }
                /*********************保存不同请求行，GET/POST等方法***************************/
                // 保存请求行
                Len = getHttpStrToBuffLen(TempStartAdd); // 获取某行的长度 遇到 \r\n 停止
                // 记录第一行
                Send_Task.Client_Request[1].Name._char = TempStartAdd;
                Send_Task.Client_Request[1].MaxLen = Len;
                copyString(Send_Task.CmdTask[Cmd_i][0], Send_Task.Client_Request[1].Name._char, CMDBUFFMAXLEN, Send_Task.Client_Request[1].MaxLen);

                // 找到第 Cmd_i+1 条Host指令的地址  Host: 192.168.4.1:8888
                if ((TempStartAdd = myStrstrCont(Send_Task.Client_Request[0].Name._char, "Host: ", Send_Task.Client_Request[0].MaxLen, (Cmd_i + 1))) != NULL)
                {
                    /*********************保存用户提供的域名了，可以扩展***************************/
                    // 保存Host行
                    Len = getHttpStrToBuffLen(TempStartAdd); // 获取某行的长度 遇到 \r\n 停止
                    // 记录第二行
                    Send_Task.Client_Request[2].Name._char = TempStartAdd;
                    Send_Task.Client_Request[2].MaxLen = Len;

                    copyString(Send_Task.CmdTask[Cmd_i][1], Send_Task.Client_Request[2].Name._char, CMDBUFFMAXLEN, Send_Task.Client_Request[2].MaxLen);
                }
            }
        }
    }
    // 装载命令
    Send_Task.Client_Request[1].Name._char = Send_Task.CmdTask[0][0];
    Send_Task.Client_Request[1].MaxLen = CMDBUFFMAXLEN;
    Send_Task.Client_Request[2].Name._char = Send_Task.CmdTask[0][1];
    Send_Task.Client_Request[2].MaxLen = CMDBUFFMAXLEN;

    return AndanalyzeFunaction(); // 1 回复网页； 2 回复 josn
}
// 清除一条请求记录
void deleteCmdTask(void)
{
    Send_Task.TaskID = 0;
    Send_Task.TaskSum = 0;

    if (Send_Task.CmdNumber > 0)
    {
        // 运行一条指令后，减少一条
        copyString(Send_Task.CmdTask[0][0], Send_Task.CmdTask[1][0], CMDBUFFMAXLEN, CMDBUFFMAXLEN);
        copyString(Send_Task.CmdTask[0][1], Send_Task.CmdTask[1][1], CMDBUFFMAXLEN, CMDBUFFMAXLEN);
        copyString(Send_Task.CmdTask[1][0], Send_Task.CmdTask[2][0], CMDBUFFMAXLEN, CMDBUFFMAXLEN);
        copyString(Send_Task.CmdTask[1][1], Send_Task.CmdTask[2][1], CMDBUFFMAXLEN, CMDBUFFMAXLEN);
        memset(Send_Task.CmdTask[2][0], 0, CMDBUFFMAXLEN);
        memset(Send_Task.CmdTask[2][1], 0, CMDBUFFMAXLEN);
        // 指令对应的 josn 数据区也要清理
        copyString(Send_Task.DonwloadJsonStr[0], Send_Task.DonwloadJsonStr[1], JSONDATAMAXLEN, JSONDATAMAXLEN);
        copyString(Send_Task.DonwloadJsonStr[1], Send_Task.DonwloadJsonStr[2], JSONDATAMAXLEN, JSONDATAMAXLEN);
        memset(Send_Task.DonwloadJsonStr[2], 0, JSONDATAMAXLEN);
        Send_Task.CmdNumber = ((Send_Task.CmdNumber > 0) ? Send_Task.CmdNumber - 1 : 0);
    }
    else
    {
        memset(Send_Task.CmdTask, 0, sizeof(Send_Task.CmdTask));
        memset(Send_Task.DonwloadJsonStr, 0, sizeof(Send_Task.DonwloadJsonStr));
    }
}
