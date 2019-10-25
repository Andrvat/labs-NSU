#include <stdlib.h>
#include <stdio.h>
#include "QuickSort.h"
#include "ChangingOriginalArray.h"


int main(void) {
    struct ArrayOfNumbers originalArray = inputArrayOfNumbers();
    quickSort(&originalArray, 0, originalArray.size - 1);
    printArrayOfNumbers(&originalArray);
    freeArrayMemory(&originalArray);
    return EXIT_SUCCESS;
}

