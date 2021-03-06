#pragma once

#include "SequenceFunctions.h"

typedef unsigned long long TUnsignedInt64;

void transferNumFromOneNumberSystemToAnother(int firstNumSystem, int secondNumSystem, const char *originalNumber);

int getPowerOfNum(const char *originalNumber, unsigned int originalNumberSize);

long double getDecimalRepresentationOfNumber(unsigned int originalNumberSize, const char *originalNumber,
                                             int *powerOfNum, int firstNumSystem);

void addInSequenceRepresentationOfNumberIntPartInSecondNumberSystem(
        struct TCharSequence *newNumberIntPartInSecondNumberSystem,
        TUnsignedInt64 *newNumberIntPart, int secondNumSystem);

void addInSequenceRepresentationOfNumberFractionalPartInSecondNumberSystem(long double *newNumberFractionalPart,
                                                                           struct TCharSequence *newNumberFractionalPartInSecondNumberSystem,
                                                                           int secondNumSystem);



