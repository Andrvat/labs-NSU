//
// Created by Andrey Valitov on 13.10.2019.
//

#pragma once

struct ArrayOfNumbers {
    int *data;
    int size;
};

void swapTwoNumbers(int *firstNum, int *secondNum);

void printArrayOfNumbers(struct ArrayOfNumbers *originalArray);

struct ArrayOfNumbers inputArrayOfNumbers(void);

void freeArrayMemory(struct ArrayOfNumbers *originalArray);
