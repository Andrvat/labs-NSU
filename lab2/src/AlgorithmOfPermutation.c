//
// Created by Andrey Valitov on 23.10.2019.
//

#include "AlgorithmOfPermutation.h"
#include "OperationsArrayOfChar.h"

int getNextPermutation(struct TCharSequence *charSequence) {
    int haveNextPermutation = 1;
    int firstIdx = getFirstIdxForReplacement(charSequence, &haveNextPermutation);
    if (!haveNextPermutation) {
        return 0;
    }
    unsigned int secondIdx = getSecondIdxForReplacement(charSequence, firstIdx);
    swapTwoCharacters(&charSequence->sequence[firstIdx],
                      &charSequence->sequence[secondIdx]);
    reverseSequenceFromIdxToEnd(charSequence, firstIdx + 1);
    return 1;
}

void printMultiplePermutations(struct TCharSequence *charSequence, const unsigned int number) {
    unsigned int counterOfPermutations = 0;
    while (counterOfPermutations < number) {
        int haveNextPermutation = getNextPermutation(charSequence);
        if (!haveNextPermutation) {
            return;
        }
        printTCharSequence(charSequence);
        ++counterOfPermutations;
    }
}

int getFirstIdxForReplacement(const struct TCharSequence *charSequence, int *haveNextPermutation) {
    int idxForReplacement = (int) charSequence->size - 2;
    while (charSequence->sequence[idxForReplacement] >= charSequence->sequence[idxForReplacement + 1]) {
        idxForReplacement--;
        // Когда больше перестановок сделать нельзя, индекс выходит за пределы последовательности
        if (idxForReplacement < 0) {
            *haveNextPermutation = 0;
            return -1;
        }
    }
    return idxForReplacement;
}

unsigned int getSecondIdxForReplacement(const struct TCharSequence *charSequence, const int firstIdxForReplacement) {
    unsigned int idxForReplacement = charSequence->size - 1;;
    while (charSequence->sequence[idxForReplacement] <= charSequence->sequence[firstIdxForReplacement]) {
        idxForReplacement--;
    }
    return idxForReplacement;
}

void reverseSequenceFromIdxToEnd(struct TCharSequence *charSequence, const int idxReverseFrom) {
    for (unsigned int idx = idxReverseFrom; idx < charSequence->size - 1; ++idx) {
        swapTwoCharacters(&charSequence->sequence[idx], &charSequence->sequence[idx + 1]);
    }
}
