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

int getPowerOfNum(const char *originalNumber, const unsigned int originalNumberSize) {
    int powerOfNum = 0;
    while (powerOfNum < (int) originalNumberSize && originalNumber[powerOfNum] != '.') {
        powerOfNum++;
    }
    return powerOfNum - 1;
}

long double getDecimalRepresentationOfNumber(const unsigned int originalNumberSize, const char *originalNumber,
                                             int *powerOfNum, int firstNumSystem) {
    long double newNumber = 0;
    for (unsigned int idx = 0; idx < originalNumberSize; ++idx) {
        if (originalNumber[idx] == '.') {
            continue;
        }
        newNumber += transferCharToNum(originalNumber[idx]) * pow(firstNumSystem, *powerOfNum);
        *powerOfNum -= 1;
    }
    return newNumber;
}


void transferNumFromOneNumberSystemToAnother(int firstNumSystem, int secondNumSystem,
                                             const char *originalNumber) {
    unsigned int originalNumberSize = strlen(originalNumber);
    int powerOfNum = getPowerOfNum(originalNumber, originalNumberSize);
    long double newNumber = getDecimalRepresentationOfNumber(originalNumberSize, originalNumber, &powerOfNum,
                                                             firstNumSystem);
    TUnsignedInt64 newNumberIntPart = (TUnsignedInt64) newNumber;
    long double newNumberFractionalPart = newNumber - newNumberIntPart;
    struct TCharSequence newNumberIntPartInBinarySystem;
    makeEmptySequence(&newNumberIntPartInBinarySystem);
    while (newNumberIntPart > 0) {
        appendNumToSequence(&newNumberIntPartInBinarySystem, transferNumToChar(newNumberIntPart % secondNumSystem));
        newNumberIntPart /= secondNumSystem;
    }
    if (isEmptySequence(&newNumberIntPartInBinarySystem)) {
        appendNumToSequence(&newNumberIntPartInBinarySystem, '0');
    }
    struct TCharSequence newNumberFractionalPartInBinarySystem; // массив символов для хранения дробной части в secondNumSystem-й СС
    makeEmptySequence(&newNumberFractionalPartInBinarySystem);
    while (newNumberFractionalPart != 0 && newNumberFractionalPartInBinarySystem.size < MAX_PRECISION_AFTER_DOT) {
        newNumberFractionalPart *= secondNumSystem;
        if (newNumberFractionalPart >= 1) {
            appendNumToSequence(&newNumberFractionalPartInBinarySystem,
                                transferNumToChar((int) newNumberFractionalPart));
            newNumberFractionalPart -= (int) newNumberFractionalPart;
        } else {
            appendNumToSequence(&newNumberFractionalPartInBinarySystem, '0');
        }
    }
    reverseSequence(&newNumberIntPartInBinarySystem);
    printSequence(&newNumberIntPartInBinarySystem);
    if (!isEmptySequence(&newNumberFractionalPartInBinarySystem)) {
        printf("%c", '.');
        printSequence(&newNumberFractionalPartInBinarySystem);
    }
}
