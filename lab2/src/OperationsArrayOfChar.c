#include <stdio.h>

#include "OperationsArrayOfChar.h"

void swapTwoCharacters(char *firstChar, char *secondChar) {
    char copyOfFirstChar = *firstChar;
    *firstChar = *secondChar;
    *secondChar = copyOfFirstChar;
}

/**
 * @brief ввод начальной последовательности
 *
 * @param charSequence
 * @param isLengthAcceptable параметр, проверяющий длину входной последовательности
 * Если он больше допустимой, до изменяем его, нет - программа продолжается
 */

void inputCharSequence(struct TCharSequence *charSequence, int *isLengthAcceptable) {
    char currentCharacter = (char) getchar();
    charSequence->size = 0;
    while (currentCharacter != '\n') {
        charSequence->sequence[charSequence->size] = currentCharacter;
        ++charSequence->size;
        currentCharacter = (char) getchar();
        int isCurrentSizeGreaterThanPossible = (charSequence->size > MAX_TCHARSEQUENCE_SIZE);
        if (isCurrentSizeGreaterThanPossible) {
            *isLengthAcceptable = 0;
        }
    }
    *isLengthAcceptable = 1;
}

/**
 * @brief Проверка входной последовательности на наличие посторонних символов
 *
 * @param charSequence
 * @return true или false
 */

int checkInputForCorrectSequence(const struct TCharSequence *charSequence) {
    int isElementInSequenceOnlyOne = (charSequence->size == 1);
    if (isElementInSequenceOnlyOne) {
        return 1;
    }
    for (unsigned int idx = 0; idx < charSequence->size; ++idx) {
        int hasSequenceExtraneousCharacters = (charSequence->sequence[idx] < '0' || charSequence->sequence[idx] > '9');
        int isThereRepetitionOfNums = ((idx < charSequence->size - 1) &&
                                       (charSequence->sequence[idx] == charSequence->sequence[idx + 1]));
        if (isThereRepetitionOfNums || hasSequenceExtraneousCharacters) {
            return 0;
        }
    }
    return 1;
}

void printTCharSequence(const struct TCharSequence *charSequence) {
    for (unsigned int idx = 0; idx < charSequence->size; ++idx) {
        printf("%c", charSequence->sequence[idx]);
    }
    printf("\n");
}
