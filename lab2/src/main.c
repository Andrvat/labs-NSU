#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "OperationsArrayOfChar.h"
#include "AlgorithmOfPermutation.h"

// Принцип работы алгоритма см. на https://ru.wikibooks.org/wiki/Реализации_алгоритмов/Алгоритм_Нарайаны

int main(void) {
    struct TCharSequence charSequence;
    int numberOfPermutations;
    int isLengthAcceptable = 1;
    inputCharSequence(&charSequence, &isLengthAcceptable);
    if (!scanf("%d", &numberOfPermutations)) {
        printf("%s\n", "bad input");
        return EXIT_SUCCESS;
    }
    if (!checkInputForCorrectSequence(&charSequence) || !isLengthAcceptable) {
        printf("%s", "bad input");
        return EXIT_SUCCESS;
    }
    printMultiplePermutations(&charSequence, numberOfPermutations);
    return EXIT_SUCCESS;
}
