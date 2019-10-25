//
// Created by Andrey Valitov on 19.09.2019.
//

#include <math.h>
#include <string.h>
#include <stdio.h>

#include "CharAndNumTransfer.h"
#include "TransferSN.h"
#include "SequenceFunctions.h"
#include "globalConsts.h"

typedef unsigned long long TUnsignedInt64;

int getPowerOfNum(const char *numberX) {
    int powerOfNum = 0;
    while (powerOfNum < (int) strlen(numberX) && numberX[powerOfNum] != '.') {
        powerOfNum++;
    }
    return powerOfNum - 1;
}


void transferNumFromOneNumberSystemToAnother(int firstNumSystem, int secondNumSystem, const char *originalNumber) {
    unsigned int originalNumberSize = strlen(originalNumber);
    // перевод из firstNumSystem в 10-ю СС
    int powerOfNum = getPowerOfNum(originalNumber);
    long double newNumber = 0; // десятичное представление числа X
    for (unsigned int idx = 0; idx < originalNumberSize; ++idx) {
        if (originalNumber[idx] == '.') {
            continue;
        }
        newNumber += transferCharToNum(originalNumber[idx]) * pow(firstNumSystem, powerOfNum);
        powerOfNum--;
    }
    // перевод из 10-й в secondNumSystem СС
    TUnsignedInt64 newNumberIntPart = (TUnsignedInt64) newNumber;
    newNumber -= newNumberIntPart; // newNumber теперь используем как дробную часть
    struct TCharSequence newNumberIntPartInBinarySystem;
    makeEmptySequence(&newNumberIntPartInBinarySystem);
    while (newNumberIntPart > 0) {
        appendNumToSequence(&newNumberIntPartInBinarySystem, transferNumToChar((int) newNumberIntPart % secondNumSystem));
        newNumberIntPart /= secondNumSystem;
    }
    if (isEmptySequence(&newNumberIntPartInBinarySystem)) {
        appendNumToSequence(&newNumberIntPartInBinarySystem, '0');
    }
    struct TCharSequence newNumberFractionalPartInBinarySystem; // массив символов для хранения дробной части в secondNumSystem-й СС
    makeEmptySequence(&newNumberFractionalPartInBinarySystem);
    while (newNumber != 0 && newNumberFractionalPartInBinarySystem.size < MAX_PRECISION_AFTER_DOT) {
        newNumber *= secondNumSystem;
        if (newNumber >= 1) {
            appendNumToSequence(&newNumberFractionalPartInBinarySystem, transferNumToChar((int) newNumber));
            newNumber -= (int) newNumber;
        } else {
            appendNumToSequence(&newNumberFractionalPartInBinarySystem, '0');
        }
    }
    // переворачиваем массив целой части
    reverseSequence(&newNumberIntPartInBinarySystem);
    // вывод числа Х в secondNumSystem-й СС
    printSequence(&newNumberIntPartInBinarySystem);
    if (!isEmptySequence(&newNumberFractionalPartInBinarySystem)) {
        printf("%c", '.');
        printSequence(&newNumberFractionalPartInBinarySystem);
    }
}
