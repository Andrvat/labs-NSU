#include <math.h>
#include <string.h>
#include <stdio.h>

#include "CharAndNumTransfer.h"
#include "TransferSN.h"
#include "SequenceFunctions.h"
#include "globalConsts.h"

/**
 * @brief Главная функция перевода числа из одной СС в другую
 *
 * Перевод осуществляется через десятичную СС
 * Перевод разделяется на два этапа:
 * Этап 1: перевод целой части
 * Этап 2: перевод дробной части
 * См. названия переменных и функций - они делают ровно то, что должны по названию
 *
 * @param firstNumSystem
 * @param secondNumSystem
 * @param originalNumber введенное число
 */

void transferNumFromOneNumberSystemToAnother(const int firstNumSystem, const int secondNumSystem,
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

/**
 * @brief Подсчет степени
 *
 * @param originalNumber
 * @param originalNumberSize
 * @return степень десятки самого старшего разряда
 */

int getPowerOfNum(const char *originalNumber, const unsigned int originalNumberSize) {
    int powerOfNum = 0;
    while (powerOfNum < (int) originalNumberSize && originalNumber[powerOfNum] != '.') {
        powerOfNum++;
    }
    return powerOfNum - 1;
}

/**
 * @brief Перевод числа в 10-ю СС
 *
 * @param originalNumberSize
 * @param originalNumber
 * @param powerOfNum
 * @param firstNumSystem
 * @return введенное число в десятичной СС
 */

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

/**
 * @brief Добавление в последовательность вывода целой части исходного числа в той СС, в которую осуществляется перевод
 *
 * @param newNumberIntPartInSecondNumberSystem
 * @param newNumberIntPart
 * @param secondNumSystem
 */

void addInSequenceRepresentationOfNumberIntPartInSecondNumberSystem(
        struct TCharSequence *newNumberIntPartInSecondNumberSystem,
        TUnsignedInt64 *newNumberIntPart, const int secondNumSystem) {
    while (*newNumberIntPart > 0) {
        appendNumToSequence(newNumberIntPartInSecondNumberSystem,
                            transferNumToChar(*newNumberIntPart % secondNumSystem));
        *newNumberIntPart = *newNumberIntPart / secondNumSystem;
    }
}

/**
 * @brief Добавление в последовательность вывода дробной части исходного числа в той СС, в которую осуществляется перевод
 *
 * @param newNumberFractionalPart
 * @param newNumberFractionalPartInSecondNumberSystem
 * @param secondNumSystem
 */

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


