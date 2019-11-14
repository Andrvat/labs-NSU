#include "QuickSort.h"
#include "ChangingOriginalArray.h"

/**
 * @brief рекурсивный алгоритм быстрой сортировки
 *
 * За pivot берется серединный элемент
 * Происходит перестановка элементов массива так,
 * Чтобы справа от pivot остались элементы, большие его
 * А слева - меньшие
 * Далее, если есть, что сортировать, рекурсивно продолжаем сортировку,
 * Пока массив не будет полностью отсортиован
 *
 * @param originalArray массив для сортировки
 * @param beginOfSorting индекс, с которого начинааем сортировку
 * @param endOfSorting индекс, каким заканчиваем сортировку
 */

void quickSort(struct ArrayOfNumbers *originalArray, int beginOfSorting, int endOfSorting) {
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
            swapTwoNumbers(&originalArray->data[endOfSorting], &originalArray->data[beginOfSorting]);
            ++beginOfSorting;
            --endOfSorting;
        }
    }
    if (beginOfSorting < rightBorder) {
        quickSort(originalArray, beginOfSorting, rightBorder);
    }
    if (endOfSorting > leftBorder) {
        quickSort(originalArray, leftBorder, endOfSorting);
    }
}
