//
// Created by Andrey Valitov on 23.10.2019.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "OperationsArrayOfChar.h"

void swapTwoCharacters(char *x, char *y) {
    char copyOfX = *x;
    *x = *y;
    *y = copyOfX;
}

int inputCharSequence(struct TCharSequence *charSequence) {
    char currentCharacter = (char) getchar();
    charSequence->size = 0;
    while (currentCharacter != '\n') {
        charSequence->sequence[charSequence->size] = currentCharacter;
        ++charSequence->size;
        currentCharacter = (char) getchar();
        // Если длина текущей последовательности стала больше максимально возможной длины,
        // которую может хранить массив charSequence->sequence, то ввод считается неккоректным
        if (charSequence->size > MAX_TCHARSEQUENCE_SIZE) {
            return 0;
        }
    }
    return 1;
}

int checkInputForCorrectSequence(const struct TCharSequence *charSequence) {
    // Если элемент всего 1, то проверку на то,
    // что последовательность состоит из одинаковых символов, совершать не нужно
    if (charSequence->size == 1) {
        return 1;
    }
    // Проверка на то, что из данной последовательности можно составить перестановку и
    // Проверка на наличие посторонних символов
    for (unsigned int idx = 0; idx < charSequence->size; ++idx) {
        if (charSequence->sequence[idx] < '0' || charSequence->sequence[idx] > '9') {
            return 0;
        }
        if (idx < charSequence->size - 1) {
            if (charSequence->sequence[idx] == charSequence->sequence[idx + 1]) {
                return 0;
            }
        }
    }
    return 1;
}

void printTCharSequence(const struct TCharSequence *charSequence) {
    for (unsigned int idx = 0; idx < charSequence->size; ++idx) {
        printf("%c", charSequence->sequence[idx]);
    }
    printf("\n");
}
