#include <stdio.h>
#include <stdlib.h>

#include "TransferSN.h"
#include "CheckInput.h"
#include "globalConsts.h"

int main() {
    int firstNumSystem, secondNumSystem;
    char originalNumber[MAX_SIZE_NUMBER];
    // fix bugs
    if (scanf("%d%d", &firstNumSystem, &secondNumSystem)) {
    }
    if (scanf("%13s", originalNumber)) {
    }

    if (checkInputForCorrectData(firstNumSystem, secondNumSystem, originalNumber)) {
        printf("%s", "bad input");
        return EXIT_SUCCESS;
    }
    transferNumFromOneNumberSystemToAnother(firstNumSystem, secondNumSystem, originalNumber);
    return EXIT_SUCCESS;
}
