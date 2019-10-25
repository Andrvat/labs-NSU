//
// Created by Andrey Valitov on 19.09.2019.
//

#include <string.h>
#include "CharAndNumTransfer.h"
#include "globalConsts.h"
#include "CheckInput.h"

int CheckBounds(int value, int min, int max) {
    return (value >= min && value <= max);
}

int CheckInput(int b1, int b2, const char *numberX) {
    if (!(CheckBounds(b1, MIN_SN, MAX_SN) && CheckBounds(b2, MIN_SN, MAX_SN))) {
        return -1;
    }
    if (numberX[0] == '.' || numberX[(int) strlen(numberX) - 1] == '.') {
        return -1;
    }
    for (unsigned int i = 0; i < strlen(numberX); ++i) {
        if (!(CheckBounds(numberX[i], '0', '9') ||
              CheckBounds(numberX[i], 'A', 'F') ||
              CheckBounds(numberX[i], 'a', 'f') ||
              numberX[i] == '.')) {
            return -1;
        }
        if (numberX[i] == '.' && numberX[i - 1] == '.') {
            return -1;
        }
        if (CharToNum(numberX[i]) >= b1) {
            return -1;
        }
    }
    return 0;
}
