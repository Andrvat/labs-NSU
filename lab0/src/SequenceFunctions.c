//
// Created by Andrey Valitov on 28.09.2019.
//

#include "SequenceFunctions.h"
#include <stdio.h>

int IsEmptySequence(const struct TCharSequence *sequence) {
    return sequence->Count == 0;
}


void AppendNumToSequence(struct TCharSequence *sequence, char symbol) {
    sequence->Data[sequence->Count] = symbol;
    ++sequence->Count;
}

void PrintSequence(const struct TCharSequence *sequence) {
    for (int j = 0; j < sequence->Count; ++j) {
        printf("%c", sequence->Data[j]);
    }
}

void ReverseSequence(struct TCharSequence *sequence) {
    for (int idx = 0; idx < sequence->Count / 2; idx++) {
        char copyElement = sequence->Data[sequence->Count - idx - 1];
        sequence->Data[sequence->Count - idx - 1] = sequence->Data[idx];
        sequence->Data[idx] = copyElement;
    }
}

void MakeEmptySequence(struct TCharSequence *sequence) {
    sequence->Count = 0;
}
