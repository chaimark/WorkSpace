#ifndef RUNCOMPUTERCMD_H
#define RUNCOMPUTERCMD_H

#include <stdbool.h>
#include "StrLib.h"

/*-----------------------------------定义请求josnCmd类----------------------------------*/
typedef struct _ComputerJsonCMD
{
	strnew JsonCmdStr;
	bool (*saveGateway)(struct _ComputerJsonCMD This);	// 方法1：保存网关编号

	//..... 可以有多种方法处理 josn 后续扩展不同的类方法即可
} computerjsoncmd;
// 类方法...
bool _saveGateway(struct _ComputerJsonCMD This);
// 建立对象
#define NEW_JsonComputerCmd New_JsonCmd_Obj()
extern computerjsoncmd New_JsonCmd_Obj(void); // 建立对象的函数

#endif
