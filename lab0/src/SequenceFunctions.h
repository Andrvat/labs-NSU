#pragma once

#define MAX_TCHARSEQUENCE_SIZE 50

struct TCharSequence {
    char data[MAX_TCHARSEQUENCE_SIZE];
    int size;
};

int isEmptySequence(const struct TCharSequence *sequence);

void swapTwoChars(char *firstChar, char *secondChar);

void appendNumToSequence(struct TCharSequence *sequence, char symbol);

void printSequence(const struct TCharSequence *sequence);

void reverseSequence(struct TCharSequence *sequence);

void makeEmptySequence(struct TCharSequence *sequence);