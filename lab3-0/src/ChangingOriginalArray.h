//
// Created by Andrey Valitov on 13.10.2019.
//

#pragma once

struct ArrayOfNumbers {
    int *data;
    int size;
};

void SwapTwoNumbers(int *firstNum, int *secondNum);

void PrintArrayOfNumbers(struct ArrayOfNumbers *originalArray);

struct ArrayOfNumbers InputArrayOfNumbers();

void FreeArraysMemory(struct ArrayOfNumbers *originalArray);
