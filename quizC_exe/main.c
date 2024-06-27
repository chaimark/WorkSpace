#include <stdio.h>
#include "StrLib.h"
#include "NumberBaseLic.h"

int main() {
    char gw_id[6] = {0x00,0x23,0x45,0x67,0x89,0x04};
    char IDTemp[13] = {0};
    HEX2ToASCII(gw_id, 6, IDTemp, 12);
    printf("-----------");
    ASCIIToHEX2(IDTemp, 13, gw_id, 6);
    printf("-----------");
    getchar();
    return 0;
}
