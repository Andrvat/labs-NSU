//
// Created by Andrey Valitov on 23.10.2019.
//

#pragma once

#define MAX_TCHARSEQUENCE_SIZE 10

struct TCharSequence {
    char sequence[MAX_TCHARSEQUENCE_SIZE];
    unsigned int size;
};

void swapTwoCharacters(char *firstChar, char *secondChar);

void inputCharSequence(struct TCharSequence *charSequence, int *isLengthAcceptable);

void printTCharSequence(const struct TCharSequence *charSequence);

int checkInputForCorrectSequence(const struct TCharSequence *charSequence);

