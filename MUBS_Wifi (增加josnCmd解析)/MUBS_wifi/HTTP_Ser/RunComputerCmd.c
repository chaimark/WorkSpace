#include "RunComputerCmd.h"
#include "WifiTask.h"

// 新建数组对象 （返回一个结构体数据）
computerjsoncmd New_JsonCmd_Obj(void)
{
	computerjsoncmd JsonCMD;
	JsonCMD.JsonCmdStr.Name._char = NULL;
	JsonCMD.JsonCmdStr.MaxLen = 0;
	JsonCMD.saveGateway = _saveGateway;
	return JsonCMD;
}
// 类方法
bool _saveGateway(struct _ComputerJsonCMD This)
{
	// myStrstr();
	// This.JsonCmdStr.Name._char;
	return true;
}
