#include <stdio.h>
#include <stdlib.h>

#define MAX_ARRAY_SIZE 17
#define ASCII_TABLE_SIZE 256
#define BUFFER_SIZE 17

/**
 * @struct массив для хранения значений сдвигов каждого символа ASCII таблицы
 */

struct ShiftTable {
    unsigned int shiftIntTable[ASCII_TABLE_SIZE];
    unsigned int shiftTableSize;
};

/**
 * @struct набор параметров, необходимых для работы БМ-поиска
 */

struct BMSearching {
    unsigned char bufferForStorageText[BUFFER_SIZE];
    unsigned char mainArrayForStorageText[MAX_ARRAY_SIZE];
    unsigned int currentIdxForComparison;
    unsigned int idxForNextInput;
    unsigned int countOfNextInputCharacters;
    unsigned int controlForEndOfText;
};

/**
 * @struct шаблон, с которым происходит сравнение
 */

struct OriginPattern {
    unsigned char mainPattern[MAX_ARRAY_SIZE];
    unsigned int patternSize;
};

/**
 * @brief Ввод шаблона
 *
 * @param originPattern
 * @param fileForInputData
 */

static void enterOriginPattern(struct OriginPattern *originPattern, FILE *fileForInputData) {
    unsigned char character = fgetc(fileForInputData);
    originPattern->patternSize = 0;
    while (character != '\n') {
        originPattern->mainPattern[originPattern->patternSize] = character;
        ++originPattern->patternSize;
        character = fgetc(fileForInputData);
    }
    originPattern->mainPattern[originPattern->patternSize + 1] = '\0';
}

/**
 * @brief Создание таблицы сдвигов
 *
 * Счет начинается с предпоследнего элемента шаблона - ему присваивается значение 1.
 * Каждый последующий символ получает 2, 3 и т.д.
 * Но! Если символ уже встречался ранее в шаблоне, то оставляем значение его первого вхождения с конца
 * Всем остальным элементам, не вошедшим в шаблон, присваиваем значение, равное длине шаблона
 *
 * @param shiftTable
 * @param originPattern
 */

static void createShiftTable(struct ShiftTable *shiftTable, const struct OriginPattern *originPattern) {
    shiftTable->shiftTableSize = ASCII_TABLE_SIZE;
    for (unsigned int idx = 0; idx < shiftTable->shiftTableSize; ++idx) {
        shiftTable->shiftIntTable[idx] = originPattern->patternSize;
    }
    int currentIdxForShiftTable = 1;
    for (int idx = (int) originPattern->patternSize - 2; idx >= 0; --idx) {
        if (shiftTable->shiftIntTable[(int) originPattern->mainPattern[idx]] == originPattern->patternSize) {
            shiftTable->shiftIntTable[(int) originPattern->mainPattern[idx]] = currentIdxForShiftTable;
            ++currentIdxForShiftTable;
        }
    }
}

/**
 * @brief Ввод следующих 16 символов
 *
 * Ф-я контролирует получение следующих символов текста
 *
 * @param bmSearching
 * @param fileForInputData
 */

static void inputNextCharacters(struct BMSearching *bmSearching, FILE *fileForInputData) {
    bmSearching->countOfNextInputCharacters = fread(bmSearching->bufferForStorageText, sizeof(unsigned char),
                                                    MAX_ARRAY_SIZE - 1, fileForInputData);
    bmSearching->idxForNextInput = 0;
    bmSearching->bufferForStorageText[bmSearching->countOfNextInputCharacters] = '\0';
}

/**
 * @brief Проверка на наличие пустой строки
 *
 * @param bmSearching
 * @return есть элементы (1) или нет (0)
 */

static int checkForEmptyInput(struct BMSearching *bmSearching) {
    return bmSearching->countOfNextInputCharacters != 0;
}

/**
 * @brief Ввод первых символов текста, чтобы можно было начать сравнение
 *
 * @param bmSearching
 * @param originPattern
 * @param fileForInputData
 */

static void
inputFirstCharacters(struct BMSearching *bmSearching, const struct OriginPattern *originPattern,
                     FILE *fileForInputData) {
    bmSearching->countOfNextInputCharacters = fread(bmSearching->mainArrayForStorageText, sizeof(unsigned char),
                                                    originPattern->patternSize, fileForInputData);
    bmSearching->idxForNextInput = originPattern->patternSize - 1;
}

/**
 * @brief Получение стольких символов, сколько необходимо для следующего сравнения
 *
 * @param bmSearching
 * @param nextStepForInput сколько нужно получить элементов далее из созданного буффера
 * @param originPattern
 * @param fileForInputData
 */

static void getNextCharactersForComparison(struct BMSearching *bmSearching, const unsigned int nextStepForInput,
                                           const struct OriginPattern *originPattern, FILE *fileForInputData) {
    for (unsigned int idx = 0; idx < originPattern->patternSize - nextStepForInput; ++idx) {
        bmSearching->mainArrayForStorageText[idx] = bmSearching->mainArrayForStorageText[idx + nextStepForInput];
    }
    for (int idx = (int) nextStepForInput - 1; idx >= 0; --idx) {
        if (bmSearching->idxForNextInput >= 16) {
            inputNextCharacters(bmSearching, fileForInputData);
            if (bmSearching->countOfNextInputCharacters == 0) {
                bmSearching->controlForEndOfText = 0;
                return;
            }
        }
        if (bmSearching->bufferForStorageText[bmSearching->idxForNextInput] == '\0') {
            bmSearching->controlForEndOfText = 0;
        }
        bmSearching->mainArrayForStorageText[originPattern->patternSize - 1 -
                                             idx] = bmSearching->bufferForStorageText[bmSearching->idxForNextInput];
        bmSearching->idxForNextInput++;
    }
}

/**
 * @brief Инициализация базовых значений параметров структуры BMSearching
 *
 * @param bmSearching
 * @param originPattern
 */

static void makeInitialSetupOfBMSearching(struct BMSearching *bmSearching, const struct OriginPattern *originPattern) {
    bmSearching->currentIdxForComparison = originPattern->patternSize - 1;
    bmSearching->idxForNextInput = MAX_ARRAY_SIZE;
    bmSearching->controlForEndOfText = 1;
}

/**
 * @brief Главная функция поиска шаблона в тексте
 *
 *
 * @param originPattern
 * @param fileForInputData
 */

static void boyerMooreAlgorithm(const struct OriginPattern *originPattern, FILE *fileForInputData) {
    struct BMSearching bmSearching;
    struct ShiftTable shiftTable;
    createShiftTable(&shiftTable, originPattern);
    inputFirstCharacters(&bmSearching, originPattern, fileForInputData);
    if (!checkForEmptyInput(&bmSearching)) {
        return;
    }
    makeInitialSetupOfBMSearching(&bmSearching, originPattern);
    while (bmSearching.controlForEndOfText) {
        unsigned int copyOfCurrentIdxForComparison = bmSearching.currentIdxForComparison;
        for (int idx = (int) originPattern->patternSize - 1; idx >= 0; --idx) {
            printf("%u ", bmSearching.currentIdxForComparison + 1);
            --bmSearching.currentIdxForComparison;
            if (bmSearching.mainArrayForStorageText[idx] != originPattern->mainPattern[idx] &&
                idx == (int) originPattern->patternSize - 1) {
                unsigned int idxOfStep = (unsigned int) bmSearching.mainArrayForStorageText[idx];
                unsigned int nextStepForInput = shiftTable.shiftIntTable[idxOfStep];
                bmSearching.currentIdxForComparison = copyOfCurrentIdxForComparison + nextStepForInput;
                getNextCharactersForComparison(&bmSearching, nextStepForInput, originPattern, fileForInputData);
                break;
            } else if (bmSearching.mainArrayForStorageText[idx] != originPattern->mainPattern[idx] || idx == 0) {
                unsigned int idxOfStep = (unsigned int) originPattern->mainPattern[originPattern->patternSize - 1];
                unsigned int nextStepForInput = shiftTable.shiftIntTable[idxOfStep];
                bmSearching.currentIdxForComparison = copyOfCurrentIdxForComparison + nextStepForInput;
                getNextCharactersForComparison(&bmSearching, nextStepForInput, originPattern, fileForInputData);
                break;
            }
        }
    }
}

int main() {
    FILE *fileForInputData = fopen("in.txt", "r");
    struct OriginPattern originalPattern;
    enterOriginPattern(&originalPattern, fileForInputData);
    boyerMooreAlgorithm(&originalPattern, fileForInputData);
    fclose(fileForInputData);
    return EXIT_SUCCESS;
}

/**
 * @mainpage Лабораторная работа #1-0, НГУ, ФИТ, 1 курс
 * @author Андрей Валитов
 * @date 10.11.2019
 * @version 1.1
 *
 * @brief Поиск подстроки в тексте с использованием алгоритма Бойера-Мура
 *
 * Принцип работы хорошо описан в видео на YouTube.
 * См. https://www.youtube.com/watch?v=KIUHWMwavQg
 *
 * @attention Удален include(common_lab) из файла CMakeLists.txt. Для лаб необходимо его вернуть в конец файла.
 *
 */
