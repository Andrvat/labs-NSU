#include <stdlib.h>
#include <stdio.h>

/**
 * @struct Массив чисел
 */

struct IntArray {
    int *array;
    unsigned int arraySize;
};

/**
 * @brief Вывод ошибки при неудачном scanf
 */

static void printError() {
    fprintf(stderr, "Check your input data!\n");
}

static void destroyIntArray(struct IntArray *numArray) {
    free(numArray->array);
}

static void swapTwoNums(int *firstNum, int *secondNum) {
    int copyOfFirst = *firstNum;
    *firstNum = *secondNum;
    *secondNum = copyOfFirst;
}

static void printArrayElements(const struct IntArray *numArray) {
    for (unsigned int i = 0; i < numArray->arraySize; ++i) {
        printf("%d ", numArray->array[i]);
    }
}

static unsigned int getArraySize(const struct IntArray *numArray) {
    return numArray->arraySize;
}

/**
 * @brief Выделение памяти под массив
 *
 * @param numArray
 * @param inputFile
 */

static void makeIntArrayFromFile(struct IntArray *numArray, FILE *inputFile) {
    if (!fscanf(inputFile, "%u", &numArray->arraySize)) {
        printError();
    }
    numArray->array = malloc(sizeof(int) * numArray->arraySize);
}

/**
 * @brief Просеивание текущего элемента
 *
 * @param numArray
 * @param currentIndex
 */

static void raiseElementToTheTop(struct IntArray *numArray, unsigned int currentIndex) {
    while ((currentIndex + 1) / 2 != 0 && numArray->array[currentIndex] > numArray->array[(currentIndex - 1) / 2]) {
        swapTwoNums(&numArray->array[currentIndex], &numArray->array[(currentIndex - 1) / 2]);
        currentIndex = (currentIndex - 1) / 2;
    }
}

/**
 * @brief Построение кучи из введенного массива
 *
 * @param numArray
 * @param inputFile
 */

static void makeHeap(struct IntArray *numArray, FILE *inputFile) {
    unsigned int currentIndex = 0;
    if (!fscanf(inputFile, "%d", &numArray->array[currentIndex])) {
        printError();
        return;
    }
    ++currentIndex;
    while (currentIndex < numArray->arraySize) {
        if (!fscanf(inputFile, "%d", &numArray->array[currentIndex])) {
            printError();
            return;
        }
        raiseElementToTheTop(numArray, currentIndex);
        ++currentIndex;
    }
    fclose(inputFile);
}

/**
 * @brief Пирамидальная сортировка
 *
 * @param numArray
 */

static void doHeapSort(struct IntArray *numArray) {
    unsigned int currentIdx = numArray->arraySize - 1;
    while (currentIdx != 0) {
        if (currentIdx == 1 && numArray->array[0] < numArray->array[currentIdx]) {
            return;
        }
        swapTwoNums(&numArray->array[0], &numArray->array[currentIdx]);
        --currentIdx;
        unsigned int movementIdx = 0;
        unsigned int firstChildIdx = 1;
        unsigned int secondChildIdx = 2;
        while (firstChildIdx <= currentIdx &&
               (numArray->array[movementIdx] < numArray->array[firstChildIdx] ||
                numArray->array[movementIdx] < numArray->array[secondChildIdx])) {
            // Если secondChildIdx указывает на currentIdx + 1, то его уже нельзя рассматривать,
            // т.к. currentIdx - индекс последне возможного для рассмотрения элемента
            if (secondChildIdx == currentIdx + 1) {
                if (numArray->array[movementIdx] < numArray->array[firstChildIdx]) {
                    swapTwoNums(&numArray->array[movementIdx], &numArray->array[firstChildIdx]);
                }
                movementIdx = firstChildIdx;
            } else if (numArray->array[firstChildIdx] > numArray->array[secondChildIdx]) {
                swapTwoNums(&numArray->array[movementIdx], &numArray->array[firstChildIdx]);
                movementIdx = firstChildIdx;
            } else {
                swapTwoNums(&numArray->array[movementIdx], &numArray->array[secondChildIdx]);
                movementIdx = secondChildIdx;
            }
            firstChildIdx = movementIdx * 2 + 1;
            secondChildIdx = movementIdx * 2 + 2;
        }
    }
}

int main(void) {
    struct IntArray numArray;
    FILE *inputFile = fopen("in.txt", "r");
    makeIntArrayFromFile(&numArray, inputFile);
    if (getArraySize(&numArray) == 0) {
        printError();
        fclose(inputFile);
        return EXIT_SUCCESS;
    }
    makeHeap(&numArray, inputFile);
    doHeapSort(&numArray);
    printArrayElements(&numArray);
    destroyIntArray(&numArray);
    return EXIT_SUCCESS;
}

/**
 * @mainpage Лабораторная работа #3-1, НГУ, ФИТ, 1 курс
 * @author Андрей Валитов
 * @date 21.11.2019
 * @version 1.2
 *
 * @brief Реализация пирамидальной сортировки
 *
 * @attention Удален include(common_lab) из файла CMakeLists.txt. Для лаб необходимо его вернуть в конец файла.
 */
