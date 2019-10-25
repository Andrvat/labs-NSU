//
// Created by Andrey Valitov on 13.10.2019.
//

#include "ChangingOriginalArray.h"
#include <stdio.h>
#include <stdlib.h>

void SwapTwoNumbers(int *firstNum, int *secondNum) {
    int copyOfFirstNum = *firstNum;
    *firstNum = *secondNum;
    *secondNum = copyOfFirstNum;
}

void PrintArrayOfNumbers(struct ArrayOfNumbers *originalArray) {
    for (int idx = 0; idx < originalArray->size; ++idx) {
        printf("%d ", originalArray->data[idx]);
    }
}

struct ArrayOfNumbers InputArrayOfNumbers() {
    struct ArrayOfNumbers originalArray;
    if (scanf("%d", &originalArray.size)) {
        // Check for input data
    }
    originalArray.data = (int *) malloc(sizeof(int) * originalArray.size);
    for (int idx = 0; idx < originalArray.size; ++idx) {
        if (scanf("%d", &originalArray.data[idx])) {
            // Checking for input data
        }
    }
    return originalArray;
}

void FreeArraysMemory(struct ArrayOfNumbers *originalArray) {
    free(originalArray->data);
    originalArray->size = 0;
}

