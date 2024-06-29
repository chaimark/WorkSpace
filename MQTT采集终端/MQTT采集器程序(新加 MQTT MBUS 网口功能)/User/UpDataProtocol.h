#ifndef __UpDataProtocol_H
#define __UpDataProtocol_H

#include "main.h"
#include "AT24C0256.h"

#define CONSOLE_UART  UART5
#define CONSOLE_BUF_SIZE 1024  
#define CONSOLE_NEED_BUF       // 需要命令缓存，上下可切换  // 640 Byte
#define CONSOLE_BUF_NUM    10  // 缓存条数
#define CONSOLE_CMD_SIZE    64 // 命令最大长度
#define CONSOLE_PAR_MAX_NUM  3 // 最大参数数量

typedef struct {
	char const* cmd;
	void (*handle)(int argc, char* argv[]);
	char const* help;
}console_cmd_list_t;
extern void UpDataProtocol_Timer_Add(void);
#endif 
