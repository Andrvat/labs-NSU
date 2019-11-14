#include <stdio.h>
#include <stdlib.h>

#include "TransferSN.h"
#include "CheckInput.h"
#include "globalConsts.h"

int main() {
    int firstNumSystem, secondNumSystem;
    char originalNumber[MAX_SIZE_NUMBER];
    if (!scanf("%d%d", &firstNumSystem, &secondNumSystem)) {
        printInfoAboutIncorrectInput();
        return EXIT_SUCCESS;
    }
    if (!scanf("%13s", originalNumber)) {
        printInfoAboutIncorrectInput();
        return EXIT_SUCCESS;
    }

    if (!checkInputForCorrectData(firstNumSystem, secondNumSystem, originalNumber)) {
        printf("%s", "bad input");
        return EXIT_SUCCESS;
    }
    transferNumFromOneNumberSystemToAnother(firstNumSystem, secondNumSystem, originalNumber);
    return EXIT_SUCCESS;
}

/**
 * @mainpage Лабораторная работа #0, НГУ, ФИТ, 1 курс
 * @author Андрей Валитов
 * @date 19.09.2019
 * @version 1.6
 *
 * @brief Перевод числа из одной системы счисления в другую
 *
 * @attention Удален include(common_lab) из файла CMakeLists.txt. Для лаб необходимо его вернуть в конец файла.
 *
 */