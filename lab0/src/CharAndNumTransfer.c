//
// Created by Andrey Valitov on 19.09.2019.
//
#include "CharAndNumTransfer.h"

int transferCharToNum(char character) {
    if (character <= 'Z' && character >= 'A') {
        character = translateCharacterToLowercase(character);
    }
    if (character >= 'a') {
        return translateBaseNumberSystemLetterToNum(character);
    } else {
        return translateBaseNumberSystemCharNumberToNum(character);
    }
}

char transferNumToChar(const int element) {
    if (element >= 10) {
        return translateLetterToBaseNumberSystem(element);
    } else {
        return translateNumberToBaseNumberSystem(element);
    }
}

char translateCharacterToLowercase(const char character) {
    return (char) (character - 'A' + 'a');
}

int translateBaseNumberSystemLetterToNum(const char character) {
    return (int) character + 10 - 'a';
}

int translateBaseNumberSystemCharNumberToNum(const char character) {
    return (int) character - '0';
}

char translateLetterToBaseNumberSystem(const int element) {
    return (char) (element - 10 + 'A');
}

char translateNumberToBaseNumberSystem(const int element) {
    return (char) (element + '0');
}
