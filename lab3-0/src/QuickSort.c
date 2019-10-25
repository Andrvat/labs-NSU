//
// Created by Andrey Valitov on 13.10.2019.
//

#include "QuickSort.h"
#include "ChangingOriginalArray.h"

void QuickSort(struct ArrayOfNumbers *originalArray, int beginOfSorting, int endOfSorting) {
    int leftBorder = beginOfSorting;
    int rightBorder = endOfSorting;
    int pivot = originalArray->data[(beginOfSorting + endOfSorting) / 2];
    while (beginOfSorting <= endOfSorting) {
        while (originalArray->data[beginOfSorting] < pivot) {
            ++beginOfSorting;
        }
        while (originalArray->data[endOfSorting] > pivot) {
            --endOfSorting;
        }
        if (beginOfSorting <= endOfSorting) {
            SwapTwoNumbers(&originalArray->data[endOfSorting], &originalArray->data[beginOfSorting]);
            ++beginOfSorting;
            --endOfSorting;
        }
    }
    if (beginOfSorting < rightBorder) {
        QuickSort(originalArray, beginOfSorting, rightBorder);
    }
    if (endOfSorting > leftBorder) {
        QuickSort(originalArray, leftBorder, endOfSorting);
    }
}
