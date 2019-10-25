//
// Created by Andrey Valitov on 13.10.2019.
//

#include <stdio.h>
#include <stdlib.h>
#include "ChangingOriginalArray.h"

void swapTwoNumbers(int *firstNum, int *secondNum) {
    int copyOfFirstNum = *firstNum;
    *firstNum = *secondNum;
    *secondNum = copyOfFirstNum;
}

void printArrayOfNumbers(struct ArrayOfNumbers *originalArray) {
    for (unsigned int idx = 0; idx < originalArray->size; ++idx) {
        printf("%d ", originalArray->data[idx]);
    }
}

struct ArrayOfNumbers inputArrayOfNumbers(void) {
    struct ArrayOfNumbers originalArray;
    if (!scanf("%d", &originalArray.size)) {
        // Check for input data
    }
    originalArray.data = (int *) malloc(sizeof(int) * originalArray.size);
    for (unsigned int idx = 0; idx < originalArray.size; ++idx) {
        if (!scanf("%d", &originalArray.data[idx])) {
            // Checking for input data (gitlab bugs)
        }
    }
    return originalArray;
}

void freeArrayMemory(struct ArrayOfNumbers *originalArray) {
    free(originalArray->data);
    originalArray->size = 0;
}

