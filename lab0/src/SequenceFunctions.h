//
// Created by Andrey Valitov on 28.09.2019.
//
#pragma once

struct TCharSequence {
    char Data[50];
    int Count;
};

int IsEmptySequence(const struct TCharSequence *sequence);

void AppendNumToSequence(struct TCharSequence *sequence, char symbol);

void PrintSequence(const struct TCharSequence *sequence);

void ReverseSequence(struct TCharSequence *sequence);

void MakeEmptySequence(struct TCharSequence *sequence);
