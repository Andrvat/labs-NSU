#include <stdio.h>
#include <stdlib.h>
#include "TransferSN.h"
#include "CheckInput.h"
#include "globalConsts.h"

int main() {
    int b1, b2;
    char numberX[MAX_SIZE_NUMBER];
    // fix bugs
    if (scanf("%d%d", &b1, &b2)) {
    }
    if (scanf("%13s", numberX)) {
    }

    if (CheckInput(b1, b2, numberX)) {
        printf("bad input");
        return EXIT_SUCCESS;
    }
    Transfer(b1, b2, numberX);
    return EXIT_SUCCESS;
}
