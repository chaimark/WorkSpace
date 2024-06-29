#include "UpDataProtocol.h"
// 实现 printf
#pragma import(__use_no_semihosting)

struct __FILE {
    int handle;
};
FILE __stdout;

void _sys_exit(int x) {
    x = x;
}

int fputc(int ch, FILE * f) {
    CONSOLE_UART->TXBUF = ch;
    while (!(CONSOLE_UART->ISR & 0x01));
    return (ch);
}

// 串口缓存
static 	unsigned char  console_receiveBuf[CONSOLE_BUF_SIZE];
static 	uint16_t  console_receiveBufLen = 0;
// 超时
static uint32_t console_receive_delay = 0;


// ****命令处理*****************************************************************************
// ****命令处理*****************************************************************************
//命令 *****************************************
static char console_cmd[CONSOLE_CMD_SIZE];
static unsigned char console_cmd_len;
static unsigned char console_cmd_p;

// 缓存 *****************************************
#ifdef CONSOLE_NEED_BUF    
static char console_his[CONSOLE_BUF_NUM][CONSOLE_CMD_SIZE];
static int8_t console_his_r;
static int8_t console_his_w;
#endif

static void Console_buf_clean(void) {
#ifdef CONSOLE_NEED_BUF   
    int8_t i;
    console_his_r = 0;
    console_his_w = 0;
    for (i = 0; i < CONSOLE_BUF_NUM; i++)
        console_his[i][0] = 0;
#endif
}

static void Console_buf_write(char * buf) {
#ifdef CONSOLE_NEED_BUF   
    strcpy(console_his[console_his_w++], buf);
    if (console_his_w >= CONSOLE_BUF_NUM) console_his_w = 0;

    console_his_r = console_his_w;
#endif
}

// + 下n个，-上n个
static void Console_buf_read(bool isdown) {
#ifdef CONSOLE_NEED_BUF   

    int8_t i;

    if (isdown) {
        for (i = 0; i < CONSOLE_BUF_NUM; i++) {
            if (console_his_r == 0)  console_his_r = CONSOLE_BUF_NUM - 1;
            else					 console_his_r--;

            if (console_his[console_his_r][0] != 0) {
                printf("\r%-66s", "");
                printf("\r$ %s", console_his[console_his_r]);
                strcpy(console_cmd, console_his[console_his_r]);
                console_cmd_len = strlen(console_cmd);
                console_cmd_p = console_cmd_len;
                break;
            }
        }
    } else {
        for (i = 0; i < CONSOLE_BUF_NUM; i++) {
            console_his_r++;
            if (console_his_r >= CONSOLE_BUF_NUM)  console_his_r = 0;

            if (console_his[console_his_r][0] != 0) {
                printf("\r%-66s", "");
                printf("\r$ %s", console_his[console_his_r]);
                strcpy(console_cmd, console_his[console_his_r]);
                console_cmd_len = strlen(console_cmd);
                console_cmd_p = console_cmd_len;
                break;
            }
        }
    }
#endif
}

// ******** updata ****************************************
#define UPDATA_OVERTIME  200

static unsigned char updata_sign = 0; // 0：console， 1：update， 2：写SN 
static uint32_t updata_delay = 0;
static uint16_t updata_frame_num = 0;


static void Console_updata_receive() {
    console_receiveBuf[console_receiveBufLen] = 0;

    if (updata_frame_num == 0 && console_receiveBufLen >= 8) {
        if (memcmp((char *)console_receiveBuf, "updata", 6) == 0) {
            updata_frame_num = (((uint16_t)console_receiveBuf[6]) << 8) | console_receiveBuf[7];
            if (updata_frame_num > CONSOLE_BUF_SIZE - 20) {
                updata_frame_num = 0;
                console_receiveBufLen = 0;
                printf("updata sign too long\r\n");
            }
        } else
            console_receiveBufLen = 0;
    }

    // 接收完毕 
    if (updata_frame_num > 0 && console_receiveBufLen >= updata_frame_num + 10) {
        uint16_t line = (((uint16_t)console_receiveBuf[8]) << 8) | console_receiveBuf[9];
        int res = UpData_Receive_Hex(line, console_receiveBuf + 10, console_receiveBufLen - 10);
        if (res == 0) {
            printf("updata sign rec ok\r\n");
        } else if (res == 0x55) {
            printf("updata sign rec complete, and reboot\r\n");
            FL_DelayMs(200);
            NVIC_SystemReset();
        } else if (res == -1) {
            printf("updata sign resend\r\n");
        } else if (res == -2) {
            printf("updata sign error\r\n");
        }

        updata_frame_num = 0;
        console_receiveBufLen = 0;
        updata_delay = 0;
    }

}

// ******** updata ****************************************


// 跳到第一个非 chr 字符
static char * console_filter_chr(char * p, char chr) {
    while (*p != 0 && *p == chr)
        p++;

    if (*p == 0) return NULL;
    else		return p;

}
// 命令处理
static void console_cmd_handle(void) {
    // 参数
    int i;
    char par[CONSOLE_PAR_MAX_NUM + 1][32];
    int argc = 0;
    //	char *argv[CONSOLE_PAR_MAX_NUM + 1];
    //	for (i = 0; i < CONSOLE_PAR_MAX_NUM + 1; i++) 
    //		argv[i] = par[i];
        //找命令和参数
    char * p_next;
    char * p = console_cmd;
    console_cmd[console_cmd_len] = 0;
    int len;

    // 分析输入字符串
    if (console_cmd_len == 0) return;
    for (i = 0; i < CONSOLE_PAR_MAX_NUM + 1; i++) {
        p = console_filter_chr(p, ' ');
        // 有非空字符
        if (p != NULL) {
            p_next = strchr(p, ' ');

            if (p_next != NULL) {
                len = p_next - p;
                strncpy(par[argc], p, len);
                par[argc][len] = 0;
                argc++;

                p = p_next + 1;
            } else {
                strcpy(par[argc++], p);
                break;
            }
        } else {
            break;
        }
    }

    if (argc == 0) {
        printf("\r\nThe cmd is empty\r\n");
        return;
    }

    //	// 找对应命令的函数
    //	for (i = 0; i < sizeof(console_cmd_list)/sizeof(console_cmd_list[0]); i++) 
    //	{
    //		if (strcmp(par[0], console_cmd_list[i].cmd) == 0) 
    //		{
    //			console_cmd_list[i].handle(argc, argv);
    //			return;
    //		}
    //	}

    printf("\r\ncmd not found\r\n");
}

// ***屏幕操作******************************************************************************

// 重新开始
static void console_reset(void) {
    memset(console_cmd, 0, CONSOLE_CMD_SIZE);
    console_cmd_len = 0;
    console_cmd_p = 0;
    printf("\r\n$ ");
}
// 退格
static void console_bs(void) {
    int num;
    int i;

    if (console_cmd_p > 0 && console_cmd_p <= console_cmd_len) {
        console_cmd_p--;
        printf("\b");

        num = console_cmd_len - console_cmd_p;
        memcpy(console_cmd + console_cmd_p, console_cmd + console_cmd_p + 1, num);  // 拷贝字符串
        // printf("%s", console_cmd + console_cmd_p);    // 字符串后移
        // printf(" ");
        // for (i = 0; i < num; i++)    //回到删除
        // 	printf("\b");
        console_cmd_len--;

        printf("\r$ %s ", console_cmd);
        for (i = 0; i < console_cmd_len - console_cmd_p + 1; i++)
            printf("\b");
    }
}


void console_rx(unsigned char * recByte, unsigned char size) {
    int i, j;

    if (size == 3) {

        if (recByte[0] == 0x1b && recByte[1] == 0x5b && recByte[2] == 0x41) {
            // 上 
            Console_buf_read(true);
            return;
        } else if (recByte[0] == 0x1b && recByte[1] == 0x5b && recByte[2] == 0x42) {
            // 下
            Console_buf_read(false);
            return;
        } else if (recByte[0] == 0x1b && recByte[1] == 0x5b && recByte[2] == 0x43) {
            // 右
            if (console_cmd_p < console_cmd_len) {
                printf("\x1b\x5b\x43");
                console_cmd_p++;
            }

            return;
        } else if (recByte[0] == 0x1b && recByte[1] == 0x5b && recByte[2] == 0x44) {
            // 左
            if (console_cmd_p > 0) {
                printf("\x1b\x5b\x44");
                console_cmd_p--;
            }
            return;
        }
    }

    for (i = 0; i < size; i++) {
        if (recByte[i] >= 0x20) {
            console_cmd_len++;
            if (console_cmd_len >= CONSOLE_CMD_SIZE) {
                console_cmd_len = 0;
                console_cmd_p = 0;
                printf("\r%-66s", "");
                printf("\r$ ");
                break;
            }

            for (j = console_cmd_len - 1; j >= console_cmd_p; j--) {
                console_cmd[j + 1] = console_cmd[j];
            }
            console_cmd[console_cmd_p++] = recByte[i];

            printf("\r$ %s", console_cmd);
            for (j = 0; j < console_cmd_len - console_cmd_p; j++)
                printf("\b");
        } else if (recByte[i] == '\r') {
            Console_buf_write(console_cmd);  // 保存命令
            console_cmd_handle();
            console_reset();
        } else if (recByte[i] == '\b') {
            console_bs(); // 退格
        }
    }
}

void UpDataProtocol_Timer_Add(void) {
    console_receive_delay++;
    updata_delay++;
}

extern int compatible_sn(unsigned char * recByte, uint16_t * Size);

void console_task(void) {
    if (console_receive_delay > 4) {
        console_receive_delay = 0;
        if (UART4Ddata.RxLen != 0) {
            NVIC_DisableIRQ(UART4_IRQn);
            memcpy(console_receiveBuf + console_receiveBufLen, UART4Ddata.RxBuf, UART4Ddata.RxLen);
            console_receiveBufLen += UART4Ddata.RxLen;
            UART4Ddata.RxLen = 0;
            NVIC_EnableIRQ(UART4_IRQn);
            if (updata_sign == 0) {
                // 兼容处理
                if (compatible_sn(console_receiveBuf, &console_receiveBufLen) < 0) {
                    console_rx(console_receiveBuf, console_receiveBufLen);
                    console_receiveBufLen = 0;
                }
            } else {// 进入升级模式
                Console_updata_receive();
            }
        }
        // 未收到数据
        else {
            if (updata_sign > 0 && updata_delay > UPDATA_OVERTIME) {
                printf("updata sign quit\r\n");
                console_receiveBufLen = 0;
                updata_sign = 0;
            } else if (updata_sign == 1) {
                updata_sign = 2;
                printf("updata sign ready\r\n");
            }
        }
    }
}

void console_init(void) {
    printf("\r\n/** console app begin **/\r\n");
    Console_buf_clean();
    console_reset();
}


// *****************************************************
#define PROTOCOL_BEGIN			  0x7E // 包头
#define PROTOCOL_DATABEG          0x02 // 数据开始
#define PROTOCOL_REC_DATAEND      0x05 // 接收数据结束
#define PROTOCOL_SEND_DATAEND     0x03 // 发送数据结束

#define PROTOCOL_NODE_ADDR_OFFSET 3    // 网关地址 偏移
#define PROTOCOL_LEN_OFFSET       14   // 长度     偏移
#define PROTOCOL_DATABEG_OFFSET   16   // 数据开始 偏移
#define PROTOCOL_VALUE_OFFSET     17   // 有效数据 偏移
#define PROTOCOL_CMD_OFFSET       12   // 命令 偏移
#define PROTOCOL_LORA_NODE_ADDR_OFFSET 8  // 控制器地址 偏移

#define PROTOCOL_FIX_LEN          20   //  除了数据的固定长度

#define PROTOCOL_SUCESS_FLAG      0x22   // 成功标志
#define PROTOCOL_FAIL_FLAG        0x11   // 失败标志

uint16_t crc16(unsigned char * buf, uint16_t len) {
    uint16_t crc = 0xffff;
    uint16_t polynomial = 0xa001;
    unsigned char i, j;

    for (i = 0; i < len; i++) {
        crc ^= buf[i];
        for (j = 0; j < 8; j++) {
            if ((crc & 0x0001) != 0) {
                crc >>= 1;
                crc ^= polynomial;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

int compatible_sn(unsigned char * recByte, uint16_t * Size) {
    int len = 0;
    unsigned char size = *Size;
    if (recByte[0] != PROTOCOL_BEGIN)return -1;
    if (size > 1 && recByte[1] != PROTOCOL_BEGIN)return -1;

    if (size >= PROTOCOL_LEN_OFFSET + 2) {
        len = (((uint16_t)recByte[PROTOCOL_LEN_OFFSET]) << 8) | recByte[PROTOCOL_LEN_OFFSET + 1];
        if (len != 1 && len != 5)
            return -1;
    }

    if (size >= PROTOCOL_FIX_LEN + len) {
        *Size = 0;
        if (recByte[12] != 3 && recByte[12] != 4)return 0;
        // 处理报文
        unsigned char crc_len = PROTOCOL_FIX_LEN + len - 2;
        uint16_t crc = crc16(recByte, crc_len);
        if ((crc & 0xff) == recByte[crc_len + 1] && (crc >> 8) == recByte[crc_len]) {
            crc_len = 0;
            unsigned char cbuf[30];
            cbuf[crc_len++] = 0x7E;
            cbuf[crc_len++] = 0x7E;
            cbuf[crc_len++] = 0x01;
            cbuf[crc_len++] = AT24CXX_Manager.gw_id[0];
            cbuf[crc_len++] = AT24CXX_Manager.gw_id[1];
            cbuf[crc_len++] = AT24CXX_Manager.gw_id[2];
            cbuf[crc_len++] = AT24CXX_Manager.gw_id[3];
            cbuf[crc_len++] = AT24CXX_Manager.gw_id[4];
            cbuf[crc_len++] = 0x00;
            cbuf[crc_len++] = 0x00;
            cbuf[crc_len++] = 0x00;
            cbuf[crc_len++] = 0x00;
            cbuf[crc_len++] = recByte[12];
            cbuf[crc_len++] = 0x00;
            if (recByte[12] == 03) {
                cbuf[crc_len++] = 0x00;
                cbuf[crc_len++] = 0x05;
                cbuf[crc_len++] = 0x02;
                cbuf[crc_len++] = AT24CXX_Manager.gw_id[0];
                cbuf[crc_len++] = AT24CXX_Manager.gw_id[1];
                cbuf[crc_len++] = AT24CXX_Manager.gw_id[2];
                cbuf[crc_len++] = AT24CXX_Manager.gw_id[3];
                cbuf[crc_len++] = AT24CXX_Manager.gw_id[4];
                cbuf[crc_len++] = 0x03;
            } else {
                AT24CXX_Manager.gw_id[0] = recByte[PROTOCOL_VALUE_OFFSET];
                AT24CXX_Manager.gw_id[1] = recByte[PROTOCOL_VALUE_OFFSET + 1];
                AT24CXX_Manager.gw_id[2] = recByte[PROTOCOL_VALUE_OFFSET + 2];
                AT24CXX_Manager.gw_id[3] = recByte[PROTOCOL_VALUE_OFFSET + 3];
                AT24CXX_Manager.gw_id[4] = recByte[PROTOCOL_VALUE_OFFSET + 4];
                EEprom_JSON_Write((unsigned char *)(&AT24CXX_Manager.gw_id), 5);
                cbuf[crc_len++] = 0x00;
                cbuf[crc_len++] = 0x01;
                cbuf[crc_len++] = 0x02;
                cbuf[crc_len++] = 0x22; //
                cbuf[crc_len++] = 0x03;
            }
            crc = crc16(cbuf, crc_len);
            cbuf[crc_len++] = crc >> 8;
            cbuf[crc_len++] = crc & 0xff;
            Uart5Send(cbuf, crc_len);
        }
    }
    return 0;
}
