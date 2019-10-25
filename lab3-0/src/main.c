#include <stdlib.h>
#include <stdio.h>
#include "QuickSort.h"
#include "ChangingOriginalArray.h"


int main(void) {
    struct ArrayOfNumbers originalArray = InputArrayOfNumbers();
    QuickSort(&originalArray, 0, originalArray.size - 1);
    PrintArrayOfNumbers(&originalArray);
    FreeArraysMemory(&originalArray);
    return EXIT_SUCCESS;
}

