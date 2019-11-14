#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ARRAY_SIZE 16
#define HASH_NUM 3
#define TRUE 1
#define END_OF_RING_ARRAY 15

/**
 * @struct Кольцевой массив для хранения текущей последовательности сравнения
 */

struct RingArray {
    unsigned char array[MAX_ARRAY_SIZE];
    unsigned int firstIdx;
    unsigned int secondIdx;
};

/**
 * @struct Массив для хранения считываемых данных
 */

struct BufferForStorageText {
    unsigned char buffer[MAX_ARRAY_SIZE];
    unsigned int currentBufferIdx;
};

/**
 * @struct Шаблон
 */

struct MainPattern {
    unsigned char patternArray[MAX_ARRAY_SIZE];
    unsigned int sizeOfPattern;
    unsigned int patternHashValue;
};

/**
 * @struct Параметры, необходимые для работы алгоритма Рабина-Карпа
 */

struct RKSearching {
    struct RingArray ringArray;
    struct BufferForStorageText bufferForStorageText;
    unsigned int idxForPrintProtocol;
    unsigned int controlEndOfText;
    unsigned int currentHashValue;
    unsigned int controlForFirstCycle;
};

/**
 * @brief Ввод следующих символов
 *
 * Функция вводит следующие MAX_ARRAY_SIZE символов в созданый буффер
 *
 * Если далее в файле нет символов, то фунция меняет значение controlEndOfText
 * И более символы считаться из файла не будут
 *
 * Если считалось меньше MAX_ARRAY_SIZE символов, то указываем в буффере конец - '\0'
 *
 * @param rkSearching
 * @param fileForInputData
 */

static void inputNextBuffer(struct RKSearching *rkSearching, FILE *fileForInputData) {
    unsigned int countOfNextElements = fread(rkSearching->bufferForStorageText.buffer, sizeof(unsigned char),
                                             MAX_ARRAY_SIZE, fileForInputData);
    unsigned int areThereNextElements = countOfNextElements > 0;
    if (!areThereNextElements) {
        rkSearching->controlEndOfText = 0;
    }
    if (countOfNextElements < END_OF_RING_ARRAY) {
        rkSearching->bufferForStorageText.buffer[countOfNextElements] = '\0';
    }
    rkSearching->bufferForStorageText.currentBufferIdx = 0;
}

/**
 *
 * @brief Подсчет текущего хэш-значения
 *
 * Функция считает хэш-значение для текущей последовательности символов
 * Которую необходимо сравнить с шаблоном pattern
 *
 * @param rkSearching
 * @param patternSize
 */

static void calculateHashValue(struct RKSearching *rkSearching, unsigned int patternSize) {
    rkSearching->currentHashValue = 0;
    unsigned int multiplicationOfHashNum = 1;
    unsigned int maximumOfHashInPower = (unsigned int) pow(HASH_NUM, patternSize - 1);
    unsigned int currentIdxForCalcNewValue = rkSearching->ringArray.firstIdx;
    while (TRUE) {
        if (currentIdxForCalcNewValue > END_OF_RING_ARRAY) {
            currentIdxForCalcNewValue = 0;
        }
        if (multiplicationOfHashNum > maximumOfHashInPower) {
            break;
        }
        rkSearching->currentHashValue +=
                ((unsigned int) rkSearching->ringArray.array[currentIdxForCalcNewValue] % HASH_NUM) *
                multiplicationOfHashNum;
        multiplicationOfHashNum *= HASH_NUM;
        currentIdxForCalcNewValue++;
    }
}

/**
 * @brief Получение следующего символа
 *
 * Функция берет следующий символ из созданного буффера
 * И заменяет последний символ из текущей строки сравнения на полученное значение
 *
 * Если все символы из буффера были использованы, то ф-я вводит новый буффер
 *
 * @param rkSearching
 * @param fileForInputData
 */

static void replaceEndCharacterInMainArray(struct RKSearching *rkSearching, FILE *fileForInputData) {
    if (rkSearching->bufferForStorageText.currentBufferIdx > END_OF_RING_ARRAY) {
        inputNextBuffer(rkSearching, fileForInputData);
    }
    rkSearching->ringArray.array[rkSearching->ringArray.secondIdx]
            = rkSearching->bufferForStorageText.buffer[rkSearching->bufferForStorageText.currentBufferIdx];
    rkSearching->bufferForStorageText.currentBufferIdx++;
}

/**
 * @brief Получение следующего символа
 *
 * Функция берет следующий символ из созданного буффера
 * И заменяет первый символ из текущей строки сравнения на полученное значение
 *
 * Если все символы из буффера были использованы, то ф-я вводит новый буффер
 *
 * @param rkSearching
 * @param fileForInputData
 */

static void replaceBeginCharacterInMainArray(struct RKSearching *rkSearching, FILE *fileForInputData) {
    if (rkSearching->bufferForStorageText.currentBufferIdx > END_OF_RING_ARRAY) {
        inputNextBuffer(rkSearching, fileForInputData);
    }
    rkSearching->ringArray.array[rkSearching->ringArray.firstIdx]
            = rkSearching->bufferForStorageText.buffer[rkSearching->bufferForStorageText.currentBufferIdx];
    rkSearching->bufferForStorageText.currentBufferIdx++;
}

/**
 * @brief Инициализация начальных параметров структуры RKSearching
 *
 * @param rkSearching
 */

static void initBasicValuesOfRKSearching(struct RKSearching *rkSearching) {
    rkSearching->controlEndOfText = 1;
    rkSearching->idxForPrintProtocol = 1;
    rkSearching->controlForFirstCycle = 0;
}

/**
 * @brief Проверка наличия текста для сравнения с шаблоном
 *
 * @param rkSearching
 */

static void checkForNonemptyTextInput(struct RKSearching *rkSearching) {
    if (rkSearching->ringArray.array[0] == '\0') {
        rkSearching->controlEndOfText = 0;
    }
}

/**
 * @brief Получение первых символов
 *
 * Функция копирует первые полученные символа текста из созданного буффера
 * В текущую строку для сравнения с шаблоном
 *
 * @param rkSearching
 * @param pattern
 */

static void getCharactersFromBufferWithPatternSize(struct RKSearching *rkSearching, struct MainPattern *pattern) {
    rkSearching->ringArray.firstIdx = 0;
    for (unsigned int idx = 0; idx < MAX_ARRAY_SIZE; ++idx) {
        rkSearching->ringArray.array[idx] = rkSearching->bufferForStorageText.buffer[idx];
        rkSearching->bufferForStorageText.currentBufferIdx++;
    }
    rkSearching->ringArray.secondIdx = pattern->sizeOfPattern;
    checkForNonemptyTextInput(rkSearching);
}

/**
 * @brief Сравнение текущей последовательности с шаблоном
 *
 * Функция сравнивает хэш-значение шаблона и текущей последовательности
 *
 * Если оно совпало, то ф-я печатает протокол работы (индекс текущего элемента в тексте)
 * И продолжает работу, пока символы двух последовательностей совпадают
 *
 * Если нет, то ф-я завершается
 *
 * @param rkSearching
 * @param pattern
 */

static void comparePatternWithCurrentString(struct RKSearching *rkSearching, struct MainPattern *pattern) {
    unsigned int idxForPattern = 0;
    unsigned int currentIdxForPrintProtocol = rkSearching->ringArray.firstIdx;
    while (TRUE) {
        if (idxForPattern == pattern->sizeOfPattern) {
            break;
        }
        if (currentIdxForPrintProtocol > END_OF_RING_ARRAY) {
            currentIdxForPrintProtocol = 0;
        }
        printf("%u ", rkSearching->idxForPrintProtocol);
        rkSearching->idxForPrintProtocol++;
        if (rkSearching->ringArray.array[currentIdxForPrintProtocol] == pattern->patternArray[idxForPattern]) {
            idxForPattern++;
            currentIdxForPrintProtocol++;
            continue;
        } else {
            break;
        }
    }
}

/**
 * @brief Проверка текущей строки на наличие переноса '\n'
 *
 * Функция проверяет наличие символа переноса строки '\n'
 * Если такой был найден, то происходит проверка - возможно ли сравнить текущую строку с шаблоном
 * Если да, то происходит сравнение
 * Если нет, то далее будет рассматриваться уже другая строка из текста
 *
 * @param rkSearching
 * @param pattern
 * @param fileForInputData
 */

static void
checkCurrentStringForSuitableLength(struct RKSearching *rkSearching, struct MainPattern *pattern,
                                    FILE *fileForInputData) {
    unsigned int counterOfMovement = 0;
    unsigned int controlForEndOfRingArray = 0;
    while (counterOfMovement != pattern->sizeOfPattern) {
        rkSearching->ringArray.secondIdx++;
        rkSearching->ringArray.firstIdx++;
        if (rkSearching->ringArray.secondIdx > END_OF_RING_ARRAY) {
            rkSearching->ringArray.secondIdx = 0;
            controlForEndOfRingArray = 1;
            rkSearching->idxForPrintProtocol++;
            replaceEndCharacterInMainArray(rkSearching, fileForInputData);
            counterOfMovement++;
            continue;
        }
        if (rkSearching->ringArray.firstIdx > END_OF_RING_ARRAY) {
            rkSearching->ringArray.firstIdx = 0;
            replaceEndCharacterInMainArray(rkSearching, fileForInputData);
            counterOfMovement++;
            if (counterOfMovement == pattern->sizeOfPattern) {
                rkSearching->ringArray.secondIdx++;
            }
            rkSearching->idxForPrintProtocol++;
            continue;
        }
        if (controlForEndOfRingArray) {
            replaceEndCharacterInMainArray(rkSearching, fileForInputData);
        }
        rkSearching->idxForPrintProtocol++;
        counterOfMovement++;
    }
}

/**
 * @brief Выполнение алгоритма Рабина-Карпа
 *
 * Главная функция, которая по алгоритму Рабина-Карпа ищет вхождение шаблона в тексте
 * И выводит протокол его работы
 *
 * @param rkSearching
 * @param pattern
 * @param fileForInputData
 */

static void rabinKarpAlgorithm(struct RKSearching *rkSearching, struct MainPattern *pattern, FILE *fileForInputData) {
    initBasicValuesOfRKSearching(rkSearching);
    inputNextBuffer(rkSearching, fileForInputData);
    getCharactersFromBufferWithPatternSize(rkSearching, pattern);
    while (rkSearching->controlEndOfText) {
        if (rkSearching->ringArray.secondIdx > 0) {
            if (rkSearching->ringArray.array[rkSearching->ringArray.secondIdx - 1] == '\n') {
                checkCurrentStringForSuitableLength(rkSearching, pattern, fileForInputData);
                continue;
            }
        }
        calculateHashValue(rkSearching, pattern->sizeOfPattern);
        unsigned int copyOfCurrentIdxForPrintProtocol = rkSearching->idxForPrintProtocol;
        if (rkSearching->currentHashValue == pattern->patternHashValue) {
            comparePatternWithCurrentString(rkSearching, pattern);
        }
        rkSearching->idxForPrintProtocol = ++copyOfCurrentIdxForPrintProtocol;
        rkSearching->ringArray.firstIdx++;
        rkSearching->ringArray.secondIdx++;
        if (rkSearching->ringArray.secondIdx > END_OF_RING_ARRAY) {
            rkSearching->ringArray.secondIdx = 0;
            rkSearching->controlForFirstCycle = 1;
            replaceEndCharacterInMainArray(rkSearching, fileForInputData);
            continue;
        }
        if (rkSearching->ringArray.firstIdx > END_OF_RING_ARRAY) {
            rkSearching->ringArray.firstIdx = 0;
            replaceBeginCharacterInMainArray(rkSearching, fileForInputData);
            continue;
        }
        if (rkSearching->controlForFirstCycle) {
            replaceEndCharacterInMainArray(rkSearching, fileForInputData);
        }
    }
}

/**
 * @brief Ввод шаблона
 *
 * @param pattern
 * @param fileForInputData
 */

static void inputMainPattern(struct MainPattern *pattern, FILE *fileForInputData) {
    pattern->sizeOfPattern = 0;
    unsigned char character = getc(fileForInputData);
    while (character != '\n') {
        pattern->patternArray[pattern->sizeOfPattern] = character;
        ++pattern->sizeOfPattern;
        character = getc(fileForInputData);
    }
}

/**
 * @brief Подсчет хэш-значения шаблона
 *
 * @param pattern
 */

static void calcPatternHashValue(struct MainPattern *pattern) {
    pattern->patternHashValue = 0;
    unsigned int multiplicationOfHashNum = 1;
    for (unsigned int idx = 0; idx < pattern->sizeOfPattern; ++idx) {
        pattern->patternHashValue += ((unsigned int) pattern->patternArray[idx] % HASH_NUM) * multiplicationOfHashNum;
        multiplicationOfHashNum *= HASH_NUM;
    }
}

/**
 * @brief Печать хэш-значения шаблона
 *
 * @param pattern
 */

static void printPatternHashValue(const struct MainPattern *pattern) {
    printf("%u ", pattern->patternHashValue);
}

int main(void) {
    struct RKSearching rkSearching;
    struct MainPattern pattern;
    FILE *fileForInputData = fopen("in.txt", "r");
    inputMainPattern(&pattern, fileForInputData);
    calcPatternHashValue(&pattern);
    printPatternHashValue(&pattern);
    rabinKarpAlgorithm(&rkSearching, &pattern, fileForInputData);
    fclose(fileForInputData);
    return EXIT_SUCCESS;
}

/**
 * @mainpage Лабораторная работа #2, НГУ, ФИТ, 1 курс
 * @author Андрей Валитов
 * @date 12.11.2019
 * @version 1.2
 *
 * @brief Поиск шаблона в тексте по алгоритму Рабина-Карпа
 *
 * @attention Удален include(common_lab) из файла CMakeLists.txt. Для лаб необходимо его вернуть в конец файла.
 */
