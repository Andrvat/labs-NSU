//
// Created by Andrey Valitov on 23.10.2019.
//

#include "AlgorithmOfPermutation.h"
#include "OperationsArrayOfChar.h"

int GetNextPermutation(struct TCharSequence *charSequence) {
    int firstIdx = GetFirstIdxForReplacement(charSequence);
    // Если указывает на -1, то нельзя получить следующую перестановку
    if (firstIdx == -1) {
        return 0;
    }
    int secondIdx = GetSecondIdxForReplacement(charSequence, firstIdx);
    SwapTwoCharacters(&charSequence->sequence[firstIdx],
                      &charSequence->sequence[secondIdx]);
    ReverseSequenceFromIdxToEnd(charSequence, firstIdx + 1);
    return 1;
}

void PrintMultiplePermutations(struct TCharSequence *charSequence, const unsigned int number) {
    unsigned int counter = 0;
    // Получаем number перестановок
    while (counter < number) {
        int haveNextPermutation = GetNextPermutation(charSequence);
        // Проверка на то, что возможно получить следующую перестановку
        if (!haveNextPermutation) {
            break;
        }
        PrintTCharSequence(charSequence);
        ++counter;
    }
}

int GetFirstIdxForReplacement(const struct TCharSequence *charSequence) {
    int idxForReplacement = charSequence->size - 2;
    while (charSequence->sequence[idxForReplacement] >= charSequence->sequence[idxForReplacement + 1]) {
        idxForReplacement--;
        // Когда больше перестановок сделать нельзя, индекс выходит за пределы последовательности
        if (idxForReplacement < 0) {
            return -1;
        }
    }
    return idxForReplacement;
}

int GetSecondIdxForReplacement(const struct TCharSequence *charSequence, const int firstIdxForReplacement) {
    int idxForReplacement = charSequence->size - 1;;
    while (charSequence->sequence[idxForReplacement] <= charSequence->sequence[firstIdxForReplacement]) {
        idxForReplacement--;
    }
    return idxForReplacement;
}

void ReverseSequenceFromIdxToEnd(struct TCharSequence *charSequence, const int idxReverseFrom) {
    for (unsigned int idx = idxReverseFrom; idx < charSequence->size - 1; ++idx) {
        SwapTwoCharacters(&charSequence->sequence[idx], &charSequence->sequence[idx + 1]);
    }
}
