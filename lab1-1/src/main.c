#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ARRAY_SIZE 16
#define HASH_NUM 3
#define TRUE 1
#define END_OF_RING_ARRAY 15

struct RingArray {
    unsigned char array[MAX_ARRAY_SIZE];
    unsigned int firstIdx;
    unsigned int secondIdx;
};

struct BufferForStorageText {
    unsigned char buffer[MAX_ARRAY_SIZE];
    unsigned int currentBufferIdx;
};

struct MainPattern {
    unsigned char patternArray[MAX_ARRAY_SIZE];
    unsigned int sizeOfPattern;
    unsigned int patternHashValue;
};

struct RKSearching {
    struct RingArray ringArray;
    struct BufferForStorageText bufferForStorageText;
    unsigned int idxForPrintProtocol;
    unsigned int controlEndOfText;
    unsigned int currentHashValue;
    unsigned int controlForFirstCycle;
};

static void inputNextBuffer(struct RKSearching *rkSearching, FILE *file) {
    unsigned int countOfNextElements = fread(rkSearching->bufferForStorageText.buffer, sizeof(unsigned char),
                                             MAX_ARRAY_SIZE, file);
    unsigned int areThereNextElements = countOfNextElements > 0;
    if (!areThereNextElements) {
        rkSearching->controlEndOfText = 0;
    }
    if (countOfNextElements < END_OF_RING_ARRAY) {
        rkSearching->bufferForStorageText.buffer[countOfNextElements] = '\0';
    }
    rkSearching->bufferForStorageText.currentBufferIdx = 0;
}

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

static void replaceEndCharacterInMainArray(struct RKSearching *rkSearching, FILE *file) {
    if (rkSearching->bufferForStorageText.currentBufferIdx > END_OF_RING_ARRAY) {
        inputNextBuffer(rkSearching, file);
    }
    rkSearching->ringArray.array[rkSearching->ringArray.secondIdx]
            = rkSearching->bufferForStorageText.buffer[rkSearching->bufferForStorageText.currentBufferIdx];
    rkSearching->bufferForStorageText.currentBufferIdx++;
}

static void replaceBeginCharacterInMainArray(struct RKSearching *rkSearching, FILE *file) {
    if (rkSearching->bufferForStorageText.currentBufferIdx > END_OF_RING_ARRAY) {
        inputNextBuffer(rkSearching, file);
    }
    rkSearching->ringArray.array[rkSearching->ringArray.firstIdx]
            = rkSearching->bufferForStorageText.buffer[rkSearching->bufferForStorageText.currentBufferIdx];
    rkSearching->bufferForStorageText.currentBufferIdx++;
}

static void initBasicValuesOfRKSearching(struct RKSearching *rkSearching) {
    rkSearching->controlEndOfText = 1;
    rkSearching->idxForPrintProtocol = 1;
    rkSearching->controlForFirstCycle = 0;
}

static void checkForNonemptyTextInput(struct RKSearching *rkSearching) {
    if (rkSearching->ringArray.array[0] == '\0') {
        rkSearching->controlEndOfText = 0;
    }
}

static void getCharactersFromBufferWithPatternSize(struct RKSearching *rkSearching, struct MainPattern *pattern) {
    rkSearching->ringArray.firstIdx = 0;
    for (unsigned int idx = 0; idx < MAX_ARRAY_SIZE; ++idx) {
        rkSearching->ringArray.array[idx] = rkSearching->bufferForStorageText.buffer[idx];
        rkSearching->bufferForStorageText.currentBufferIdx++;
    }
    rkSearching->ringArray.secondIdx = pattern->sizeOfPattern;
    checkForNonemptyTextInput(rkSearching);
}

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

static void
checkCurrentStringForSuitableLength(struct RKSearching *rkSearching, struct MainPattern *pattern, FILE *file) {
    unsigned int counterOfMovement = 0;
    unsigned int controlForEndOfRingArray = 0;
    while (counterOfMovement != pattern->sizeOfPattern) {
        rkSearching->ringArray.secondIdx++;
        rkSearching->ringArray.firstIdx++;
        if (rkSearching->ringArray.secondIdx > END_OF_RING_ARRAY) {
            rkSearching->ringArray.secondIdx = 0;
            controlForEndOfRingArray = 1;
            rkSearching->idxForPrintProtocol++;
            replaceEndCharacterInMainArray(rkSearching, file);
            counterOfMovement++;
            continue;
        }
        if (rkSearching->ringArray.firstIdx > END_OF_RING_ARRAY) {
            rkSearching->ringArray.firstIdx = 0;
            replaceEndCharacterInMainArray(rkSearching, file);
            counterOfMovement++;
            if (counterOfMovement == pattern->sizeOfPattern) {
                rkSearching->ringArray.secondIdx++;
            }
            rkSearching->idxForPrintProtocol++;
            continue;
        }
        if (controlForEndOfRingArray) {
            replaceEndCharacterInMainArray(rkSearching, file);
        }
        rkSearching->idxForPrintProtocol++;
        counterOfMovement++;
    }
}

static void rabinKarpAlgorithm(struct RKSearching *rkSearching, struct MainPattern *pattern, FILE *file) {
    initBasicValuesOfRKSearching(rkSearching);
    inputNextBuffer(rkSearching, file);
    getCharactersFromBufferWithPatternSize(rkSearching, pattern);
    while (rkSearching->controlEndOfText) {
        if (rkSearching->ringArray.secondIdx > 0) {
            if (rkSearching->ringArray.array[rkSearching->ringArray.secondIdx - 1] == '\n') {
                checkCurrentStringForSuitableLength(rkSearching, pattern, file);
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
            replaceEndCharacterInMainArray(rkSearching, file);
            continue;
        }
        if (rkSearching->ringArray.firstIdx > END_OF_RING_ARRAY) {
            rkSearching->ringArray.firstIdx = 0;
            replaceBeginCharacterInMainArray(rkSearching, file);
            continue;
        }
        if (rkSearching->controlForFirstCycle) {
            replaceEndCharacterInMainArray(rkSearching, file);
        }
    }
}

static void inputMainPattern(struct MainPattern *pattern, FILE *file) {
    pattern->sizeOfPattern = 0;
    unsigned char character = getc(file);
    while (character != '\n') {
        pattern->patternArray[pattern->sizeOfPattern] = character;
        ++pattern->sizeOfPattern;
        character = getc(file);
    }
}

static void calcPatternHashValue(struct MainPattern *pattern) {
    pattern->patternHashValue = 0;
    unsigned int multiplicationOfHashNum = 1;
    for (unsigned int idx = 0; idx < pattern->sizeOfPattern; ++idx) {
        pattern->patternHashValue += ((unsigned int) pattern->patternArray[idx] % HASH_NUM) * multiplicationOfHashNum;
        multiplicationOfHashNum *= HASH_NUM;
    }
}

static void printPatternHashValue(const struct MainPattern *pattern) {
    printf("%u ", pattern->patternHashValue);
}

int main(void) {
    struct RKSearching rkSearching;
    struct MainPattern pattern;
    FILE *file = fopen("in.txt", "r");
    inputMainPattern(&pattern, file);
    calcPatternHashValue(&pattern);
    printPatternHashValue(&pattern);
    rabinKarpAlgorithm(&rkSearching, &pattern, file);
    fclose(file);
    return EXIT_SUCCESS;
}
