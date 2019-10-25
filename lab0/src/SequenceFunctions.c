//
// Created by Andrey Valitov on 28.09.2019.
//

#include <stdio.h>

#include "SequenceFunctions.h"

int isEmptySequence(const struct TCharSequence *sequence) {
    return sequence->size == 0;
}

void swapTwoChars(char *firstChar, char *secondChar) {
    char copyOfFirstChar = *firstChar;
    *firstChar = *secondChar;
    *secondChar = copyOfFirstChar;
}

void appendNumToSequence(struct TCharSequence *sequence, const char symbol) {
    sequence->data[sequence->size] = symbol;
    ++sequence->size;
}

void printSequence(const struct TCharSequence *sequence) {
    for (unsigned int idx = 0; idx < sequence->size; ++idx) {
        printf("%c", sequence->data[idx]);
    }
}

void reverseSequence(struct TCharSequence *sequence) {
    for (unsigned int idx = 0; idx < sequence->size / 2; idx++) {
        swapTwoChars(&sequence->data[sequence->size - idx - 1], &sequence->data[idx]);
    }
}

void makeEmptySequence(struct TCharSequence *sequence) {
    sequence->size = 0;
}
