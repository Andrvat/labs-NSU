//
// Created by Andrey Valitov on 19.09.2019.
//

#include <string.h>

#include "CharAndNumTransfer.h"
#include "globalConsts.h"
#include "CheckInput.h"

int checkNumSystemForCorrectBounds(int currentValue, int minSymbol, int maxSymbol) {
    return (currentValue >= minSymbol && currentValue <= maxSymbol);
}

int checkInputForCorrectData(const int firstNumSystem, const int secondNumSystem, const char *originalNumber) {
    unsigned int originalNumberSize = strlen(originalNumber);
    if (!(checkNumSystemForCorrectBounds(firstNumSystem, MIN_NUMBER_SYSTEM, MAX_NUMBER_SYSTEM) &&
          checkNumSystemForCorrectBounds(secondNumSystem, MIN_NUMBER_SYSTEM, MAX_NUMBER_SYSTEM))) {
        return -1;
    }
    if (originalNumber[0] == '.' || originalNumber[(int) originalNumberSize - 1] == '.') {
        return -1;
    }
    for (unsigned int idx = 0; idx < originalNumberSize; ++idx) {
        if (!(checkNumSystemForCorrectBounds(originalNumber[idx], '0', '9') ||
              checkNumSystemForCorrectBounds(originalNumber[idx], 'A', 'F') ||
              checkNumSystemForCorrectBounds(originalNumber[idx], 'a', 'f') ||
              originalNumber[idx] == '.')) {
            return -1;
        }
        if (originalNumber[idx] == '.' && originalNumber[idx - 1] == '.') {
            return -1;
        }
        if (transferCharToNum(originalNumber[idx]) >= firstNumSystem) {
            return -1;
        }
    }
    return 0;
}
