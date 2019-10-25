//
// Created by Andrey Valitov on 23.10.2019.
//

#include "AlgorithmOfPermutation.h"
#include "OperationsArrayOfChar.h"

int getNextPermutation(struct TCharSequence *charSequence) {
    int firstIdx = getFirstIdxForReplacement(charSequence);
    // Если указывает на -1, то нельзя получить следующую перестановку
    if (firstIdx == -1) {
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
    // Получаем number перестановок
    while (counterOfPermutations < number) {
        int haveNextPermutation = getNextPermutation(charSequence);
        // Проверка на то, что возможно получить следующую перестановку
        if (!haveNextPermutation) {
            return;
        }
        printTCharSequence(charSequence);
        ++counterOfPermutations;
    }
}

int getFirstIdxForReplacement(const struct TCharSequence *charSequence) {
    int idxForReplacement = (int) charSequence->size - 2;
    while (charSequence->sequence[idxForReplacement] >= charSequence->sequence[idxForReplacement + 1]) {
        idxForReplacement--;
        // Когда больше перестановок сделать нельзя, индекс выходит за пределы последовательности
        if (idxForReplacement < 0) {
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
