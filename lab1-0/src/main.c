#include <stdio.h>

#define MAX_ARRAY_SIZE 17
#define ASCII_TABLE_SIZE 256
#define BUFFER_SIZE 17

struct ShiftTable {
    unsigned int shiftIntTable[ASCII_TABLE_SIZE];
    unsigned int shiftTableSize;
};

struct BMSearching {
    unsigned char bufferForStorageText[BUFFER_SIZE];
    unsigned char mainArrayForStorageText[MAX_ARRAY_SIZE];
    unsigned int currentIdxForComparison;
    unsigned int idxForNextInput;
    unsigned int countOfNextInputCharacters;
    unsigned int controlForEndOfText;
};

struct OriginPattern {
    unsigned char mainPattern[MAX_ARRAY_SIZE];
    unsigned int patternSize;
};

void enterOriginPattern(struct OriginPattern *originPattern, FILE *file) {
    unsigned char character = fgetc(file);
    originPattern->patternSize = 0;
    while (character != '\n') {
        originPattern->mainPattern[originPattern->patternSize] = character;
        ++originPattern->patternSize;
        character = fgetc(file);
    }
    originPattern->mainPattern[originPattern->patternSize + 1] = '\0';
}

void createShiftTable(struct ShiftTable *shiftTable, const struct OriginPattern *originPattern) {
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

void inputNextCharacters(struct BMSearching *bmSearching, FILE *file) {
    bmSearching->countOfNextInputCharacters = fread(bmSearching->bufferForStorageText, sizeof(unsigned char),
                                                    MAX_ARRAY_SIZE - 1, file);
    bmSearching->idxForNextInput = 0;
    bmSearching->bufferForStorageText[bmSearching->countOfNextInputCharacters] = '\0';
}

int checkForEmptyInput(struct BMSearching *bmSearching) {
    return bmSearching->countOfNextInputCharacters != 0;
}

void inputFirstCharacters(struct BMSearching *bmSearching, const struct OriginPattern *originPattern, FILE *file) {
    bmSearching->countOfNextInputCharacters = fread(bmSearching->mainArrayForStorageText, sizeof(unsigned char),
                                                    originPattern->patternSize, file);
    bmSearching->idxForNextInput = originPattern->patternSize - 1;
}

void getNextCharactersForComparison(struct BMSearching *bmSearching, const unsigned int nextStepForInput,
                                    const struct OriginPattern *originPattern, FILE *file) {
    for (unsigned int idx = 0; idx < originPattern->patternSize - nextStepForInput; ++idx) {
        bmSearching->mainArrayForStorageText[idx] = bmSearching->mainArrayForStorageText[idx + nextStepForInput];
    }
    for (int idx = (int) nextStepForInput - 1; idx >= 0; --idx) {
        if (bmSearching->idxForNextInput >= 16) {
            inputNextCharacters(bmSearching, file);
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

void makeInitialSetupOfBMSearching(struct BMSearching *bmSearching, const struct OriginPattern *originPattern) {
    bmSearching->currentIdxForComparison = originPattern->patternSize - 1;
    bmSearching->idxForNextInput = MAX_ARRAY_SIZE;
    bmSearching->controlForEndOfText = 1;
}

void boyerMooreAlgorithm(const struct OriginPattern *originPattern, FILE *file) {
    struct BMSearching bmSearching;
    struct ShiftTable shiftTable;
    createShiftTable(&shiftTable, originPattern);
    inputFirstCharacters(&bmSearching, originPattern, file);
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
                getNextCharactersForComparison(&bmSearching, nextStepForInput, originPattern, file);
                break;
            } else if (bmSearching.mainArrayForStorageText[idx] != originPattern->mainPattern[idx] || idx == 0) {
                unsigned int idxOfStep = (unsigned int) originPattern->mainPattern[originPattern->patternSize - 1];
                unsigned int nextStepForInput = shiftTable.shiftIntTable[idxOfStep];
                bmSearching.currentIdxForComparison = copyOfCurrentIdxForComparison + nextStepForInput;
                getNextCharactersForComparison(&bmSearching, nextStepForInput, originPattern, file);
                break;
            }
        }
    }
}

int main() {
    FILE *file = fopen("in.txt", "r");;
    struct OriginPattern originalPattern;
    enterOriginPattern(&originalPattern, file);
    boyerMooreAlgorithm(&originalPattern, file);
    fclose(file);
}
