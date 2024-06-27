#include "NumberBaseLic.h"

/*
// 该库中提到的所有进制数全是直接表示，比如
// hex(0x125)   //表示 dec(125)
// hex(0x1010)  //表示 bin(1010)
// hex(0x1f)    //表示 hex(0x1f)
*/
// int ASCIIToHEX2(char *asc, char *hex, unsigned char len);	// 外用接口 双字符串 转 单数组串 0x31 0x32 ==> 0x12
void numberArrayToStrArray(char StrArray[], char NumberArray[], int ArrayMinLen);									// 将数组串转字符串 0x01 0x02 ==> 0x31 0x32
void strArrayToNumberArray(char NumberArray[], char StrArray[], int ArrayMinLen);									// 将字符串转数组串 0x31 0x32 ==> 0x01 0x02
uint64_t anyBaseToAnyBase(uint64_t Number, int IntputBase, int OutputBase);											// 任意进制互转
int anyBaseNumberToAnyBaseArray(uint64_t Number, int IntputBase, int OutputBase, char OutArray[], int ArrayMinLen); // 任意进制数 转 任意进制数组
int64_t anyBaseArrayToAnyBaseNumber(char IntArray[], int ArrayMinLen, int IntputBase, int OutputBase);				// 任意进制数组 转 任意进制数
int shortChStrToDoubleChStr(strnew inputArray, strnew OutputArray);													// 单字节数组 转 双字节数组 0x23 --> 0x02 0x03
int doubleChStrToShortChStr(strnew inputArray, strnew OutputArray);													// 双字节数组 转 单字节数组 0x02 0x03 --> 0x23
int64_t doneAsciiStrToAnyBaseNumberData(char AscArray[], int ArrayLen, int OutputBase);								// 字符串 转 任意进制数
int doneBaseNumberDataToAsciiStr(char AscArray[], int ArrayLen, int NumberData, int IntputBase);					// 任意进制数 转 字符串

/*
0x61 == 'a' => 0x0a;
0x41 == 'A' => 0x0A;
0x31 == '1' => 0x01;
*/
// 将数组串转字符串
void numberArrayToStrArray(char StrArray[], char NumberArray[], int ArrayMinLen) {
    int Addr = 0;
    do {
        if (NumberArray[Addr] < 10) {
            StrArray[Addr] = NumberArray[Addr] + '0';
        } else if ((NumberArray[Addr] >= 0x0A) && (NumberArray[Addr] <= 0x0F)) {
            StrArray[Addr] = NumberArray[Addr] + 0x37;
        }
        Addr++;
    } while (Addr < ArrayMinLen);
}
// 将字符串转数组串
void strArrayToNumberArray(char NumberArray[], char StrArray[], int ArrayMinLen) {
    int Addr = 0;
    do {
        if ((StrArray[Addr] >= '0') && (StrArray[Addr] <= '9')) {
            NumberArray[Addr] = StrArray[Addr] - '0';
        } else if ((StrArray[Addr] >= 'A') && (StrArray[Addr] <= 'Z')) {
            NumberArray[Addr] = StrArray[Addr] - 0x37;
        } else if ((StrArray[Addr] >= 'a') && (StrArray[Addr] <= 'z')) {
            NumberArray[Addr] = StrArray[Addr] - 0x57;
        }
        Addr++;
    } while (Addr < ArrayMinLen);
}

// 任意进制数 转 对应进制数组 返回长度
uint64_t anyBaseNumberToSameArray(uint8_t * Output, int StrSize, uint64_t InputNumber) {
    int8_t Add_i = 0;
    uint8_t TempArray[16] = {0}; // uint64_t也只有16位数 0x0000000000000001
    uint64_t OutLen = 0;

    // 将数据转为字符串
    for (Add_i = 0; (Add_i < 16) && (Add_i < StrSize); Add_i++) {
        TempArray[Add_i] = InputNumber & 0x000000000000000F; // 先存低位
        InputNumber = InputNumber >> 4;
    }
    // 计算长度
    OutLen = StrSize;
    Add_i = StrSize;
    while ((TempArray[--Add_i] == 0) && (Add_i >= 0)) {
        OutLen--;
    }
    swapStr((char *)TempArray, OutLen);
    for (Add_i = 0; (Add_i < 16) && (Add_i < OutLen); Add_i++) {
        Output[Add_i] = TempArray[Add_i];
    }
    return OutLen;
}
// 任意进制数组 转 对应进制数 返回数
uint64_t anyArrayToSameBaseNumber(int NumberBase, uint8_t * InputStr, int StrSize) {
    uint8_t TempArray[16] = {0}; // uint64_t也只有16位数 0x0000000000000001
    uint64_t ResNumber = 0;
    // 输入的数组长度大于16直接返回
    if (StrSize > 16) {
        return ResNumber;
    }
    // copy 输入的数据
    for (int Add_i = 0; Add_i < StrSize; Add_i++) {
        TempArray[Add_i] = InputStr[Add_i];
    }
    // 将数组串组装成直接表示的数
    // 如：hex(0x125) ==> dec(125)
    for (int Add_i = 15; Add_i >= 0; Add_i--) {
        ResNumber = ResNumber << 4;
        uint64_t Number = TempArray[Add_i] & 0x0F; // 0b:00001111
        ResNumber = ResNumber | Number;
    }
    return ResNumber;
}

// 任意进制互转
uint64_t anyBaseToAnyBase(uint64_t Number, int IntputBase, int OutputBase) {
    uint64_t ResNumber = Number;
    uint8_t TempArray[16] = {0}; // uint64_t也只有16位数 0x0000000000000001

    // ============================先转到16进制============================ //
    memset(TempArray, 0, 16); // 清空数组
    // 16进制直接返回
    if (IntputBase != 16) {
        ResNumber = 0;
        // 先将数据转为数组串
        for (int Add_i = 0; Add_i < 16; Add_i++) {
            TempArray[Add_i] = Number & 0x000000000000000F; // 先存低位
            Number = Number >> 4;
        }
        // 根据数组串和该数原来的进制 转换为16进制 如：Dex(0x31)==>Hex(0x1f)
        for (int Add_i = 0; Add_i < 16; Add_i++) {
            uint64_t TempNumber = TempArray[Add_i];
            // 单独计算每个空间对应的数的大小
            for (int i = 0; i < Add_i; i++) {
                TempNumber = TempNumber * IntputBase;
            }
            ResNumber += TempNumber;
        }
        Number = ResNumber; // 先转到16进制
    }

    // ============================再转到任意进制============================ //
    memset(TempArray, 0, 16); // 清空数组
    // 16进制直接返回
    if (OutputBase != 16) {
        ResNumber = 0;
        // 先将数据根据目标进制转为数组串
        for (int Add_i = 0; Add_i < 16; Add_i++) {
            TempArray[Add_i] = Number % OutputBase; // 先存低位
            Number /= OutputBase;
        }
        // 再将数组串组装成直接表示的数
        // 如：hex(0x125) ==> dec(125)
        for (int Add_i = 15; Add_i >= 0; Add_i--) {
            ResNumber = ResNumber << 4;
            Number = TempArray[Add_i] & 0x0F; // 0b:00001111
            ResNumber = ResNumber | Number;
        }
        Number = ResNumber; // 再转到任意进制
    }
    ResNumber = Number;
    return ResNumber;
}

// 任意进制数 转 任意进制数组
int anyBaseNumberToAnyBaseArray(uint64_t Number, int IntputBase, int OutputBase, char OutArray[], int ArrayMinLen) {
    int ResLen = 0;
    uint64_t TempNum = anyBaseToAnyBase(Number, IntputBase, OutputBase);
    char TempArray[16] = {0};
    int ArrayLen = anyBaseNumberToSameArray((uint8_t *)TempArray, 16, TempNum);
    if ((ResLen = copyString(OutArray, TempArray, ArrayMinLen, ArrayLen)) == -1) {
        ResLen = ArrayMinLen;
    } else {
        ResLen = ArrayLen;
    }
    return ResLen;
}

// 任意进制数组 转 任意进制数
int64_t anyBaseArrayToAnyBaseNumber(char IntArray[], int ArrayMinLen, int IntputBase, int OutputBase) {
    int64_t TempNum = anyArrayToSameBaseNumber(IntputBase, (uint8_t *)IntArray, ArrayMinLen);
    TempNum = anyBaseToAnyBase(TempNum, IntputBase, OutputBase);
    return TempNum;
}

// 单字节数组 转 双字节数组 0x23 --> 0x02 0x03
int shortChStrToDoubleChStr(strnew inputArray, strnew OutputArray) {
    if (&(inputArray.Name._char) != &(OutputArray.Name._char)) {
        int ResLen = 0;
        char TempChar[2] = {0};
        if (OutputArray.MaxLen < (inputArray.MaxLen * 2)) {
            return 0;
        }
        for (int i = 0; i < inputArray.MaxLen; i++) {
            memset(TempChar, 0, 2);
            if (anyBaseNumberToSameArray((uint8_t *)TempChar, 2, inputArray.Name._char[i]) == 1) {
                swapStr(TempChar, 2);
            }
            ResLen += 2;  // 单字节转两字节
            OutputArray.Name._char[i * 2 + 0] = TempChar[0];
            OutputArray.Name._char[i * 2 + 1] = TempChar[1];
        }
        return ResLen;
    }
    return 0;
}
// 双字节数组 转 单字节数组 0x02 0x03 --> 0x23
int doubleChStrToShortChStr(strnew inputArray, strnew OutputArray) {
    if (inputArray.Name._char != OutputArray.Name._char) {
        int ResLen = 0;
        if (OutputArray.MaxLen < (inputArray.MaxLen / 2)) {
            return 0;
        }
        for (ResLen = 0; ResLen < (inputArray.MaxLen / 2); ResLen++) {
            OutputArray.Name._char[ResLen] = ((inputArray.Name._char[ResLen * 2] & 0x0F) << 4) | (inputArray.Name._char[ResLen * 2 + 1] & 0x0F);
        }
        return ResLen;
    }
    return 0;
}

// 字符串转 任意进制数
int64_t doneAsciiStrToAnyBaseNumberData(char AscArray[], int ArrayLen, int OutputBase) {
    swapStr(AscArray, ArrayLen);		// 先将数组从从大端模式改为小端
    strArrayToNumberArray(AscArray, AscArray, ArrayLen);						   // 去掉 0x30
    int NumTemp = anyBaseArrayToAnyBaseNumber(AscArray, ArrayLen, 10, OutputBase); // 组合成 任意进制数
    numberArrayToStrArray(AscArray, AscArray, ArrayLen);						   // 复原 0x30
    return NumTemp;
}
// 任意进制数 转 字符串
int doneBaseNumberDataToAsciiStr(char AscArray[], int ArrayLen, int NumberData, int IntputBase) {
    uint64_t TempNum = anyBaseToAnyBase(NumberData, IntputBase, 10);					// 先转到 10进制
    int AscArrayLen = anyBaseNumberToSameArray((uint8_t *)AscArray, ArrayLen, TempNum); // 10进制 转对应数组
    numberArrayToStrArray(AscArray, AscArray, AscArrayLen);								// 数组串 转 字符串
    return AscArrayLen;
}

// // 外用接口
// int ASCIIToHEX2(char *asc, char *hex, unsigned char len)
// {
// 	strnew inputArray;
// 	strnew OutputArray;

// 	inputArray.Name._char = asc;
// 	OutputArray.Name._char = hex;

// 	inputArray.MaxLen = len * 2;
// 	OutputArray.MaxLen = len;
// 	// 0x32 0x33 --> 0x02 0x03
// 	strArrayToNumberArray(inputArray.Name._char, inputArray.Name._char,inputArray.MaxLen);
// 	// 0x02 0x03 --> 0x23
// 	doubleChStrToShortChStr(inputArray, OutputArray);
// }

// 外用接口
void HEX2ToASCII(char * hex, int hex_len, char * asc, int asc_len) {
    // HEX TO STR-------------------
    strnew IDStr;
    strnew IDHex;
    IDHex.Name._char = hex;
    IDHex.MaxLen = hex_len;
    IDStr.Name._char = asc;
    IDStr.MaxLen = asc_len;
    shortChStrToDoubleChStr(IDHex, IDStr);
    numberArrayToStrArray(IDStr.Name._char, IDStr.Name._char, IDStr.MaxLen);
    // HEX TO STR-------------------
}
