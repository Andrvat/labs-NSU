//
// Created by Andrey Valitov on 19.09.2019.
//
#pragma once

#include "SequenceFunctions.h"
typedef unsigned long long TUnsignedInt64;

void transferNumFromOneNumberSystemToAnother(int firstNumSystem, int secondNumSystem, const char *originalNumber);

int getPowerOfNum(const char *originalNumber, unsigned int originalNumberSize);

long double getDecimalRepresentationOfNumber(unsigned int originalNumberSize, const char *originalNumber,
                                             int *powerOfNum, int firstNumSystem);




