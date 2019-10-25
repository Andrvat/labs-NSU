//
// Created by Andrey Valitov on 23.10.2019.
//

#pragma once

#include "OperationsArrayOfChar.h"

int getNextPermutation(struct TCharSequence *charSequence);

void printMultiplePermutations(struct TCharSequence *charSequence, unsigned int number);

int getFirstIdxForReplacement(const struct TCharSequence *charSequence);

unsigned int getSecondIdxForReplacement(const struct TCharSequence *charSequence, int firstIdxForReplacement);

void reverseSequenceFromIdxToEnd(struct TCharSequence *charSequence, int idxReverseFrom);
