#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "OperationsArrayOfChar.h"
#include "AlgorithmOfPermutation.h"

int main(void) {
    struct TCharSequence charSequence;
    int numberOfPermutations;
    int checkingForSequenceSize = InputTCharSequence(&charSequence); // enter your sequence
    if (scanf("%d", &numberOfPermutations) != 1) {
        printf("%s\n", "bad input");
        return EXIT_SUCCESS;
    };
    if (!CheckInputForCorrectSequence(&charSequence) || !checkingForSequenceSize) {
        printf("%s", "bad input");
        return EXIT_SUCCESS;
    }
    PrintMultiplePermutations(&charSequence, numberOfPermutations);
    return EXIT_SUCCESS;
}
