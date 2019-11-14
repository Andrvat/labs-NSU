#include <stdlib.h>
#include "QuickSort.h"
#include "ChangingOriginalArray.h"


int main(void) {
    struct ArrayOfNumbers originalArray = inputArrayOfNumbers();
    quickSort(&originalArray, 0, originalArray.size - 1);
    printArrayOfNumbers(&originalArray);
    freeArrayMemory(&originalArray);
    return EXIT_SUCCESS;
}


/**
 * @mainpage Лабораторная работа #3-0, НГУ, ФИТ, 1 курс
 * @author Андрей Валитов
 * @date 13.10.2019
 * @version 1.4
 *
 * @brief Реализация быстрой сортировки
 *
 * @attention Удален include(common_lab) из файла CMakeLists.txt. Для лаб необходимо его вернуть в конец файла.
 */


