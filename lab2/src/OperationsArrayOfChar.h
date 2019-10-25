//
// Created by Andrey Valitov on 23.10.2019.
//

#pragma once

#define MAX_TCHARSEQUENCE_SIZE 10

struct TCharSequence {
    char sequence[MAX_TCHARSEQUENCE_SIZE];
    unsigned int size;
};

void SwapTwoCharacters(char *x, char *y);

int InputTCharSequence(struct TCharSequence *charSequence);

void PrintTCharSequence(const struct TCharSequence *charSequence);

int CheckInputForCorrectSequence(const struct TCharSequence *charSequence);

