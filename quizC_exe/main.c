#include <stdio.h>
#include "StrLib.h"
#include "NumberBaseLic.h"


void main(void) {
    unsigned char TempHexId[6] = {0x00,0x23,0x45,0x67,0x89,0x04};
    unsigned char TempAscIIId[12] = {0};
    int AscLen = 12;
    int HexLen = 6;
    memset(TempAscIIId, 0, AscLen);
    HEX2ToASCII(TempHexId, HexLen, TempAscIIId, AscLen);
    printf("%s\t", TempAscIIId);
    printf("\n-----------\n");

    memset(TempHexId, 0, HexLen);
    ASCIIToHEX2(TempAscIIId, AscLen, TempHexId, HexLen);
    for (int i = 0; i < HexLen; i++) {
        printf("%x\t", TempHexId[i]);
    }
    printf("\n-----------\n");
}
