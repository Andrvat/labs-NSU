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

void transferNumFromOneNumberSystemToAnother(int firstNumSystem, int secondNumSystem,
                                             const char *originalNumber) {
    unsigned int originalNumberSize = strlen(originalNumber);
    int powerOfNum = getPowerOfNum(originalNumber, originalNumberSize);
    long double originalNumberInDecimalSystem = getDecimalRepresentationOfNumber(originalNumberSize, originalNumber,
                                                                                 &powerOfNum,
                                                                                 firstNumSystem);
    TUnsignedInt64 newNumberIntPart = (TUnsignedInt64) originalNumberInDecimalSystem;
    long double newNumberFractionalPart = originalNumberInDecimalSystem - newNumberIntPart;
    struct TCharSequence newNumberIntPartInSecondNumberSystem;
    makeEmptySequence(&newNumberIntPartInSecondNumberSystem);
    addInSequenceRepresentationOfNumberIntPartInSecondNumberSystem(&newNumberIntPartInSecondNumberSystem,
                                                                   &newNumberIntPart,
                                                                   secondNumSystem);
    if (isEmptySequence(&newNumberIntPartInSecondNumberSystem)) {
        appendNumToSequence(&newNumberIntPartInSecondNumberSystem, '0');
    }
    struct TCharSequence newNumberFractionalPartInSecondNumberSystem;
    makeEmptySequence(&newNumberFractionalPartInSecondNumberSystem);
    addInSequenceRepresentationOfNumberFractionalPartInSecondNumberSystem(&newNumberFractionalPart,
                                                                          &newNumberFractionalPartInSecondNumberSystem,
                                                                          secondNumSystem);
    reverseSequence(&newNumberIntPartInSecondNumberSystem);
    printSequence(&newNumberIntPartInSecondNumberSystem);
    if (!isEmptySequence(&newNumberFractionalPartInSecondNumberSystem)) {
        printf("%c", '.');
        printSequence(&newNumberFractionalPartInSecondNumberSystem);
    }
}

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

void addInSequenceRepresentationOfNumberIntPartInSecondNumberSystem(
        struct TCharSequence *newNumberIntPartInSecondNumberSystem,
        TUnsignedInt64 *newNumberIntPart, const int secondNumSystem) {
    while (*newNumberIntPart > 0) {
        appendNumToSequence(newNumberIntPartInSecondNumberSystem,
                            transferNumToChar(*newNumberIntPart % secondNumSystem));
        *newNumberIntPart = *newNumberIntPart / secondNumSystem;
    }
}

void addInSequenceRepresentationOfNumberFractionalPartInSecondNumberSystem(long double *newNumberFractionalPart,
                                                                           struct TCharSequence *newNumberFractionalPartInSecondNumberSystem,
                                                                           const int secondNumSystem) {
    while (*newNumberFractionalPart != 0 &&
           newNumberFractionalPartInSecondNumberSystem->size < MAX_PRECISION_AFTER_DOT) {
        *newNumberFractionalPart = *newNumberFractionalPart * secondNumSystem;
        if (*newNumberFractionalPart >= 1) {
            appendNumToSequence(newNumberFractionalPartInSecondNumberSystem,
                                transferNumToChar((int) *newNumberFractionalPart));
            *newNumberFractionalPart = *newNumberFractionalPart - (int) *newNumberFractionalPart;
        } else {
            appendNumToSequence(newNumberFractionalPartInSecondNumberSystem, '0');
        }
    }
}


