//
// Created by Andrey Valitov on 23.10.2019.
//

#pragma once

#include "OperationsArrayOfChar.h"

int GetNextPermutation(struct TCharSequence *charSequence);

void PrintMultiplePermutations(struct TCharSequence *charSequence, const unsigned int number);

int GetFirstIdxForReplacement(const struct TCharSequence *charSequence);

int GetSecondIdxForReplacement(const struct TCharSequence *charSequence, const int firstIdxForReplacement);

void ReverseSequenceFromIdxToEnd(struct TCharSequence *charSequence, const int idxReverseFrom);
