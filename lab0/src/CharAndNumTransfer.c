//
// Created by Andrey Valitov on 19.09.2019.
//

int CharToNum(char element) {
    // переводим в нижний регистр при необходимсоти
    if (element <= 'Z' && element >= 'A') {
        element = element - 'A' + 'a';
    }
    if (element >= 'a') {
        return element + 10 - 'a';
    } else {
        return element - '0';
    }
}

char NumToChar(int element) {
    if (element >= 10) {
        return element - 10 + 'A';
    } else {
        return element + '0';
    }

}
