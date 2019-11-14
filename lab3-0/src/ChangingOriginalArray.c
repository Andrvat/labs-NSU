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

/**
 * @brief Ввод неотсортированного массива
 *
 * @return структруа ArrayOfNumbers
 * (см. реализацию в *.h)
 */

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

/**
 * @brief Освобождение памяти, выделенное под массив
 *
 * @param originalArray
 */

void freeArrayMemory(struct ArrayOfNumbers *originalArray) {
    free(originalArray->data);
    originalArray->size = 0;
}

