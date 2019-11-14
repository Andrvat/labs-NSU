#include <stdlib.h>
#include <stdio.h>

#include "OperationsArrayOfChar.h"
#include "AlgorithmOfPermutation.h"

int main(void) {
    struct TCharSequence charSequence;
    int numberOfPermutations;
    int isLengthAcceptable = 1;
    inputCharSequence(&charSequence, &isLengthAcceptable);
    if (!scanf("%d", &numberOfPermutations)) {
        printf("%s\n", "bad input");
        return EXIT_SUCCESS;
    }
    if (!checkInputForCorrectSequence(&charSequence) || !isLengthAcceptable) {
        printf("%s", "bad input");
        return EXIT_SUCCESS;
    }
    printMultiplePermutations(&charSequence, numberOfPermutations);
    return EXIT_SUCCESS;
}


/**
 * @mainpage Лабораторная работа #2, НГУ, ФИТ, 1 курс
 * @author Андрей Валитов
 * @date 23.10.2019
 * @version 1.1
 *
 * @brief Получение следующих n перестановок (или сколько возможно)
 *
 * Принцип работы алгоритма можно посмотреть на
 * https://ru.wikibooks.org/wiki/Реализации_алгоритмов/Алгоритм_Нарайаны
 * P.S. Алгоритм Дейкстры работает похожим образом.
 * Данный код является реализацией алгоритма Нарайаны, работающим в среднем за О(n)
 *
 * @attention Удален include(common_lab) из файла CMakeLists.txt. Для лаб необходимо его вернуть в конец файла.
 *
 */
