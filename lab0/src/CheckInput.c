#include <string.h>
#include <stdio.h>

#include "CharAndNumTransfer.h"
#include "globalConsts.h"
#include "CheckInput.h"

/**
 * @brief Проверка входной строки - является ли введенная последовательность числом в исходной СС
 *
 * Ф-я проверяет в введенной последовательности наличие посторонних символов, отличных от алфавита
 * 0123456789ABCDEF, а также, меньше ли поразрядные значения основания исходной СС
 *
 * @param firstNumSystem
 * @param secondNumSystem
 * @param originalNumber
 * @return 1, если проверка пройдена и 0, если введенная послед-ть не является числом
 */

int checkInputForCorrectData(const int firstNumSystem, const int secondNumSystem, const char *originalNumber) {
    unsigned int originalNumberSize = strlen(originalNumber);
    if (!(checkNumSystemForCorrectBounds(firstNumSystem, MIN_NUMBER_SYSTEM, MAX_NUMBER_SYSTEM) &&
          checkNumSystemForCorrectBounds(secondNumSystem, MIN_NUMBER_SYSTEM, MAX_NUMBER_SYSTEM))) {
        return 0;
    }
    if (originalNumber[0] == '.' || originalNumber[(int) originalNumberSize - 1] == '.') {
        return 0;
    }
    for (unsigned int idx = 0; idx < originalNumberSize; ++idx) {
        if (!(checkNumSystemForCorrectBounds(originalNumber[idx], '0', '9') ||
              checkNumSystemForCorrectBounds(originalNumber[idx], 'A', 'F') ||
              checkNumSystemForCorrectBounds(originalNumber[idx], 'a', 'f') ||
              originalNumber[idx] == '.')) {
            return 0;
        }
        if (originalNumber[idx] == '.' && originalNumber[idx - 1] == '.') {
            return 0;
        }
        if (transferCharToNum(originalNumber[idx]) >= firstNumSystem) {
            return 0;
        }
    }
    return 1;
}

int checkNumSystemForCorrectBounds(const int currentValue, const int minSymbol, const int maxSymbol) {
    return (currentValue >= minSymbol && currentValue <= maxSymbol);
}

void printInfoAboutIncorrectInput() {
    printf("Incorrect input! Please, enter necessary data before continue...\n");
}
