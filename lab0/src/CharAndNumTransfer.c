//
// Created by Andrey Valitov on 19.09.2019.
//

int transferCharToNum(char element) {
    // переводим в нижний регистр при необходимсоти
    if (element <= 'Z' && element >= 'A') {
        element = (char) (element - 'A' + 'a');
    }
    if (element >= 'a') {
        return element + 10 - 'a';
    } else {
        return element - '0';
    }
}

char transferNumToChar(int element) {
    if (element >= 10) {
        return (char) (element - 10 + 'A');
    } else {
        return (char) (element + '0');
    }

}
