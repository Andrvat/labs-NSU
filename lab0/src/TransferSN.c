//
// Created by Andrey Valitov on 19.09.2019.
//
#include "CharAndNumTransfer.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "TransferSN.h"
#include "SequenceFunctions.h"

typedef unsigned long long TUnsignedInt64;

int GetPowerOfNum(const char *numberX) {
    int powerOfNum = 0;
    while (powerOfNum < (int) strlen(numberX) && numberX[powerOfNum] != '.') {
        powerOfNum++;
    }
    return powerOfNum - 1;
}


void Transfer(int b1, int b2, const char *numberX) {
    // перевод из b1 в 10-ю СС
    int powerOfNum = GetPowerOfNum(numberX);
    long double newX = 0; // десятичное представление числа X
    for (int i = 0; i < (int) strlen(numberX); ++i) {
        if (numberX[i] == '.') {
            continue;
        }
        newX += CharToNum(numberX[i]) * pow(b1, powerOfNum);
        powerOfNum--;
    }
    // перевод из 10-й в b2 СС
    TUnsignedInt64 newXIntPart = (TUnsignedInt64) newX;
    newX -= newXIntPart; // newX теперь используем как дробную часть
    struct TCharSequence numXB2IntPart;
    MakeEmptySequence(&numXB2IntPart);
    while (newXIntPart > 0) {
        AppendNumToSequence(&numXB2IntPart, NumToChar(newXIntPart % b2));
        newXIntPart /= b2;
    }
    if (IsEmptySequence(&numXB2IntPart)) {
        AppendNumToSequence(&numXB2IntPart, '0');
    }
    struct TCharSequence numXB2Fractional; // массив символов для хранения дробной части в b2-й СС
    MakeEmptySequence(&numXB2Fractional);
    while (newX != 0 && numXB2Fractional.Count < 12) {
        newX *= b2;
        if (newX >= 1) {
            AppendNumToSequence(&numXB2Fractional, NumToChar((int) newX));
            newX -= (int) newX;
        } else {
            AppendNumToSequence(&numXB2Fractional, '0');
        }
    }
    // переворачиваем массив целой части
    ReverseSequence(&numXB2IntPart);
    // вывод числа Х в b2-й СС
    PrintSequence(&numXB2IntPart);
    if (!IsEmptySequence(&numXB2Fractional)) {
        printf("%c", '.');
        PrintSequence(&numXB2Fractional);
    }
}
