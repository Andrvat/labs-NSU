#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#define MAX_CHAR_NUM 256
#define ZIP_TEXT 'c'
#define UNZIP_TEXT 'd'
#define BUFFER_SIZE 20
#define BYTE_LENGTH 8
#define FILE_EMPTY -1

typedef unsigned char ui8;

typedef struct huffmanTree {
    struct huffmanTree *leftSubtree;
    struct huffmanTree *rightSubtree;
    ui8 character;
    int encodingTreeFrequency;
} THuffmanTree;

typedef struct binaryCodesTable {
    ui8 *bits;
    ui8 character;
    ui8 bitsLen; // <= 8
} TCodesTable;

typedef struct readWriteFileBits {
    FILE *outputStream;
    FILE *inputStream;
    ui8 bufferData[BUFFER_SIZE];
    size_t bufferIdx; // <= BUFFER_SIZE
    ui8 bitsContainer[BYTE_LENGTH];
    size_t bitsIdx;
} TRWFileBits;

bool FileDoesntEnd(const size_t bufferIdx, const size_t currentBufferSize) {
    return bufferIdx != currentBufferSize || currentBufferSize == BUFFER_SIZE;
}

bool IsBufferOverflow(const TRWFileBits *fileProcessing) {
    return fileProcessing->bufferIdx == BUFFER_SIZE;
}

void UpdateBuffer(size_t *inputSize, TRWFileBits *fileProcessing) {
    *inputSize = fread(fileProcessing->bufferData, sizeof(ui8), BUFFER_SIZE, fileProcessing->inputStream);
    fileProcessing->bufferIdx = 0;
}

int GetElementByItsBinaryCode(const ui8 *binCodeSequence) {
    int element = 0;
    for (int i = 0; i < BYTE_LENGTH; i++) {
        element = (element << 1) | binCodeSequence[i];
    }
    return element;
}

ui8 GetElementsBit(const ui8 element, const int bitIdx) {
    return (bool) ((1 << bitIdx) & element);
}

int ComparatorForPriorityQueueElements(const void *a, const void *b) {
    return (((THuffmanTree *) a)->encodingTreeFrequency) - (((THuffmanTree *) b)->encodingTreeFrequency);
}

void InitHuffmanPriorityQueue(const size_t initialSize, THuffmanTree *priorityQueue) {
    for (size_t i = 0; i < initialSize; i++) {
        priorityQueue[i].character = (ui8) i;
        priorityQueue[i].encodingTreeFrequency = INT_MAX;
        priorityQueue[i].leftSubtree = NULL;
        priorityQueue[i].rightSubtree = NULL;
    }
}

bool IsLeafYetNotVisited(const THuffmanTree treeLeaf) {
    return treeLeaf.encodingTreeFrequency == INT_MAX;
}

void FillLeafsDataToPriorityQueue(size_t *leavesCount, TRWFileBits *fileProcessing,
                                  THuffmanTree *priorityQueue) {
    ui8 element = fileProcessing->bufferData[fileProcessing->bufferIdx];
    fileProcessing->bufferIdx += 1;
    if (IsLeafYetNotVisited(priorityQueue[element])) {
        priorityQueue[element].encodingTreeFrequency = 0;
        *leavesCount += 1;
    }
    priorityQueue[element].encodingTreeFrequency++;
}

THuffmanTree *InputTextToPriorityQueue(TRWFileBits *fileProcessing, size_t *realSize) {
    THuffmanTree *priorityQueue = malloc(sizeof(THuffmanTree) * MAX_CHAR_NUM);
    InitHuffmanPriorityQueue(MAX_CHAR_NUM, priorityQueue);
    size_t inputSize = fread(fileProcessing->bufferData, sizeof(ui8), BUFFER_SIZE, fileProcessing->inputStream);
    if (inputSize == 0) {
        *realSize = 0;
        free(priorityQueue);
        return NULL;
    }
    fileProcessing->bufferIdx = 0;
    FillLeafsDataToPriorityQueue(realSize, fileProcessing, priorityQueue);
    while (FileDoesntEnd(fileProcessing->bufferIdx, inputSize)) {
        FillLeafsDataToPriorityQueue(realSize, fileProcessing, priorityQueue);
        if (IsBufferOverflow(fileProcessing)) {
            UpdateBuffer(&inputSize, fileProcessing);
        }
    }
    qsort(priorityQueue, MAX_CHAR_NUM, sizeof(THuffmanTree), ComparatorForPriorityQueueElements);
    fclose(fileProcessing->inputStream);
    return priorityQueue;
}

void SwapPriorityQueueElements(THuffmanTree *first, THuffmanTree *second) {
    THuffmanTree saveCopy = *first;
    *first = *second;
    *second = saveCopy;
}

void RaiseElementToTheTopInBinHeap(size_t currentIndex, THuffmanTree *priorityQueue) {
    while ((currentIndex + 1) / 2 != 0 &&
           priorityQueue[currentIndex].encodingTreeFrequency < priorityQueue[(currentIndex - 1) / 2].encodingTreeFrequency) {
        SwapPriorityQueueElements(&priorityQueue[currentIndex], &priorityQueue[(currentIndex - 1) / 2]);
        currentIndex = (currentIndex - 1) / 2;
    }
}

bool IsNotThereNextDataToHeapify(const int idxQueue) {
    return idxQueue <= 0;
}

void CreateCodesTable(int dynamicCodesIdx, THuffmanTree *root, size_t *tableIdx,
                      TCodesTable *codesTable, ui8 *dynamicSymbolsBinaryCode) {
    if (root->leftSubtree != NULL) {
        dynamicCodesIdx++;
        dynamicSymbolsBinaryCode[dynamicCodesIdx - 1] = 0;
        CreateCodesTable(dynamicCodesIdx, root->leftSubtree, tableIdx, codesTable,
                         dynamicSymbolsBinaryCode);
        dynamicCodesIdx--;
    }
    if (root->rightSubtree != NULL) {
        dynamicCodesIdx++;
        dynamicSymbolsBinaryCode[dynamicCodesIdx - 1] = 1;
        CreateCodesTable(dynamicCodesIdx, root->rightSubtree, tableIdx, codesTable,
                         dynamicSymbolsBinaryCode);
        dynamicCodesIdx--;
    }
    if (root->leftSubtree == NULL && root->rightSubtree == NULL) {
        if (dynamicCodesIdx == 0) {
            dynamicSymbolsBinaryCode[0] = 0;
            dynamicCodesIdx++;
        }
        codesTable[*tableIdx].character = root->character;
        codesTable[*tableIdx].bits = malloc(sizeof(ui8) * dynamicCodesIdx);
        memcpy(codesTable[*tableIdx].bits, dynamicSymbolsBinaryCode, sizeof(ui8) * dynamicCodesIdx);
        codesTable[*tableIdx].bitsLen = dynamicCodesIdx;
        *tableIdx += 1;
    }
}

THuffmanTree *CopyHuffmanTree(const THuffmanTree copyFrom) {
    THuffmanTree *tree = malloc(sizeof(THuffmanTree));
    tree->character = copyFrom.character;
    tree->rightSubtree = copyFrom.rightSubtree;
    tree->leftSubtree = copyFrom.leftSubtree;
    tree->encodingTreeFrequency = copyFrom.encodingTreeFrequency;
    return tree;
}

void HeapifyBinPriorityQueue(const int idxQueue, THuffmanTree *priorityQueue) {
    if (IsNotThereNextDataToHeapify(idxQueue)) {
        return;
    }
    int movementIdx = 0;
    int firstChildIdx = 1;
    int secondChildIdx = 2;
    while (firstChildIdx <= idxQueue &&
           (priorityQueue[movementIdx].encodingTreeFrequency > priorityQueue[firstChildIdx].encodingTreeFrequency ||
            priorityQueue[movementIdx].encodingTreeFrequency > priorityQueue[secondChildIdx].encodingTreeFrequency)) {
        if (secondChildIdx == idxQueue + 1) {
            if (priorityQueue[movementIdx].encodingTreeFrequency > priorityQueue[firstChildIdx].encodingTreeFrequency) {
                SwapPriorityQueueElements(&priorityQueue[movementIdx], &priorityQueue[firstChildIdx]);
            }
            movementIdx = firstChildIdx;
        } else if (priorityQueue[firstChildIdx].encodingTreeFrequency <= priorityQueue[secondChildIdx].encodingTreeFrequency) {
            SwapPriorityQueueElements(&priorityQueue[movementIdx], &priorityQueue[firstChildIdx]);
            movementIdx = firstChildIdx;
        } else {
            SwapPriorityQueueElements(&priorityQueue[movementIdx], &priorityQueue[secondChildIdx]);
            movementIdx = secondChildIdx;
        }
        firstChildIdx = movementIdx * 2 + 1;
        secondChildIdx = movementIdx * 2 + 2;
    }
}

THuffmanTree GetTopFromPriorityQueue(THuffmanTree *priorityQueue, int *idxQueue) {
    THuffmanTree saveTop = priorityQueue[0];
    SwapPriorityQueueElements(&priorityQueue[0], &priorityQueue[*idxQueue]);
    *idxQueue -= 1;
    HeapifyBinPriorityQueue(*idxQueue, priorityQueue);
    return saveTop;
}

void PutTreeToPriorityQueue(int *idxQueue, THuffmanTree *unionLeaves, THuffmanTree *priorityQueue) {
    *idxQueue += 1;
    SwapPriorityQueueElements(&priorityQueue[*idxQueue], unionLeaves);
    free(unionLeaves);
    RaiseElementToTheTopInBinHeap(*idxQueue, priorityQueue);
}

THuffmanTree *BuildHuffmanCodingTree(const size_t realSize, THuffmanTree *priorityQueue) {
    int idxQueue = (int) (realSize - 1);
    if (idxQueue == 0) {
        priorityQueue[idxQueue].encodingTreeFrequency = 0;
    }
    while (idxQueue != 0) {
        THuffmanTree testOne = GetTopFromPriorityQueue(priorityQueue, &idxQueue);
        THuffmanTree testTwo = GetTopFromPriorityQueue(priorityQueue, &idxQueue);
        THuffmanTree *unionLeaves = malloc(sizeof(THuffmanTree));
        unionLeaves->encodingTreeFrequency = testOne.encodingTreeFrequency + testTwo.encodingTreeFrequency;
        THuffmanTree *leftChild = CopyHuffmanTree(testOne);
        THuffmanTree *rightChild = CopyHuffmanTree(testTwo);
        unionLeaves->leftSubtree = leftChild;
        unionLeaves->rightSubtree = rightChild;
        PutTreeToPriorityQueue(&idxQueue, unionLeaves, priorityQueue);
    }
    return CopyHuffmanTree(priorityQueue[0]);
}

void CopyElementToBitsContainer(const ui8 element, TRWFileBits *fileProcessing) {
    fileProcessing->bitsContainer[fileProcessing->bitsIdx] = element;
    fileProcessing->bitsIdx++;
    if (fileProcessing->bitsIdx == BYTE_LENGTH) {
        ui8 elementToWrite = (ui8) GetElementByItsBinaryCode(fileProcessing->bitsContainer);
        fwrite(&elementToWrite, sizeof(ui8), 1, fileProcessing->outputStream);
        fileProcessing->bitsIdx = 0;
    }
}

void WriteHuffmanTreeToFile(THuffmanTree *root, TRWFileBits *fileProcessing) {
    if (root->leftSubtree != NULL) {
        CopyElementToBitsContainer(1, fileProcessing);
        WriteHuffmanTreeToFile(root->leftSubtree, fileProcessing);
    }
    if (root->rightSubtree != NULL) {
        WriteHuffmanTreeToFile(root->rightSubtree, fileProcessing);
    }
    if (root->leftSubtree == NULL && root->rightSubtree == NULL) {
        CopyElementToBitsContainer(0, fileProcessing);
        for (int i = 0; i < BYTE_LENGTH; i++) {
            ui8 currentBit = GetElementsBit(root->character, BYTE_LENGTH - i - 1);
            CopyElementToBitsContainer(currentBit, fileProcessing);
        }
    }
}

bool IsNeededToWriteMissingBits(const size_t bitsIdx) {
    return bitsIdx != 0 && bitsIdx <= BYTE_LENGTH;
}

void WriteMissingBits(TRWFileBits *fileProcessing, ui8 *neededBits) {
    for (int i = fileProcessing->bitsIdx; i < BYTE_LENGTH; i++) {
        fileProcessing->bitsContainer[i] = 0;
        *neededBits += 1;
    }
    ui8 elementToWrite = (ui8) GetElementByItsBinaryCode(fileProcessing->bitsContainer);
    fwrite(&elementToWrite, sizeof(ui8), 1, fileProcessing->outputStream);
}

void EncodeTextWithNewCharacters(const size_t realSize, TCodesTable *codesTable,
                                 TRWFileBits *fileProcessing, ui8 *neededBits) {
    fseek(fileProcessing->inputStream, 3, SEEK_CUR);
    size_t inputSize = fread(fileProcessing->bufferData, sizeof(ui8), BUFFER_SIZE, fileProcessing->inputStream);
    fileProcessing->bufferIdx = 0;
    while (FileDoesntEnd(fileProcessing->bufferIdx, inputSize)) {
        for (size_t tableIdx = 0; tableIdx < realSize; tableIdx++) {
            if (codesTable[tableIdx].character == fileProcessing->bufferData[fileProcessing->bufferIdx]) {
                for (int i = 0; i < codesTable[tableIdx].bitsLen; i++) {
                    CopyElementToBitsContainer(codesTable[tableIdx].bits[i], fileProcessing);
                }
                fileProcessing->bufferIdx++;
                break;
            }
        }
        if (IsBufferOverflow(fileProcessing)) {
            UpdateBuffer(&inputSize, fileProcessing);
        }
    }
    if (IsNeededToWriteMissingBits(fileProcessing->bitsIdx)) {
        WriteMissingBits(fileProcessing, neededBits);
    }
}

void WriteNewCodesToFile(const size_t realSize, THuffmanTree *huffmanTree,
                         TCodesTable *codesTable,
                         TRWFileBits *fileProcessing, ui8 *neededBits) {
    fileProcessing->inputStream = fopen("in.txt", "rb");
    fileProcessing->bitsIdx = 0;
    WriteHuffmanTreeToFile(huffmanTree, fileProcessing);
    EncodeTextWithNewCharacters(realSize, codesTable, fileProcessing, neededBits);
}

bool IsBitsContainerOverflow(const TRWFileBits *fileProcessing) {
    return fileProcessing->bitsIdx == BYTE_LENGTH;
}

void UpdateBitsContainer(TRWFileBits *fileProcessing, size_t *inputSize) {
    fileProcessing->bitsIdx = 0;
    fileProcessing->bufferIdx++;
    if (IsBufferOverflow(fileProcessing)) {
        UpdateBuffer(inputSize, fileProcessing);
    }
    for (int copyIdx = 0; copyIdx < BYTE_LENGTH; copyIdx++) {
        fileProcessing->bitsContainer[copyIdx] = GetElementsBit(fileProcessing->bufferData[fileProcessing->bufferIdx],
                                                                BYTE_LENGTH - copyIdx - 1);
    }
}

void GetElementBits(ui8 *elementBits, TRWFileBits *fileProcessing, size_t *inputSize) {
    for (int i = 0; i < BYTE_LENGTH; i++) {
        fileProcessing->bitsIdx += 1;
        if (IsBitsContainerOverflow(fileProcessing)) {
            UpdateBitsContainer(fileProcessing, inputSize);
        }
        elementBits[i] = fileProcessing->bitsContainer[fileProcessing->bitsIdx];
    }
}

void FillDataToHuffmanDecodingTreeLeaf(THuffmanTree *huffmanTree,
                                       TRWFileBits *fileProcessing,
                                       size_t *leavesCount, size_t *inputSize) {
    huffmanTree->leftSubtree = NULL;
    huffmanTree->rightSubtree = NULL;
    ui8 elementBits[BYTE_LENGTH];
    GetElementBits(elementBits, fileProcessing, inputSize);
    fileProcessing->bitsIdx++;
    if (IsBitsContainerOverflow(fileProcessing)) {
        UpdateBitsContainer(fileProcessing, inputSize);
    }
    ui8 element = GetElementByItsBinaryCode(elementBits);
    *leavesCount += 1;
    huffmanTree->character = element;
}

void GrowHuffmanDecodingTree(int dynamicBinCodesIdx, THuffmanTree *huffmanTree,
                             TRWFileBits *fileProcessing,
                             size_t *leavesCount, size_t *inputSize,
                             ui8 *dynamicSymbolsBinaryCode) {
    if (fileProcessing->bitsContainer[fileProcessing->bitsIdx] == 1) {
        huffmanTree->leftSubtree = malloc(sizeof(THuffmanTree));
        huffmanTree->rightSubtree = malloc(sizeof(THuffmanTree));
        fileProcessing->bitsIdx++;
        if (IsBitsContainerOverflow(fileProcessing)) {
            UpdateBitsContainer(fileProcessing, inputSize);
        }
        dynamicBinCodesIdx++;
        dynamicSymbolsBinaryCode[dynamicBinCodesIdx - 1] = 0;
        GrowHuffmanDecodingTree(dynamicBinCodesIdx, huffmanTree->leftSubtree, fileProcessing,
                                leavesCount, inputSize, dynamicSymbolsBinaryCode);
        dynamicSymbolsBinaryCode[dynamicBinCodesIdx - 1] = 1;
        GrowHuffmanDecodingTree(dynamicBinCodesIdx, huffmanTree->rightSubtree, fileProcessing,
                                leavesCount, inputSize, dynamicSymbolsBinaryCode);
    } else {
        FillDataToHuffmanDecodingTreeLeaf(huffmanTree, fileProcessing, leavesCount, inputSize);
    }
}

THuffmanTree *BuildHuffmanDecodingTree(int dynamicBinCodesIdx, THuffmanTree *huffmanTree,
                                       TRWFileBits *fileProcessing,
                                       size_t *leavesCount, size_t *inputSize,
                                       ui8 *dynamicSymbolsBinaryCode) {
    huffmanTree = malloc(sizeof(THuffmanTree));
    if (fileProcessing->bitsContainer[fileProcessing->bitsIdx] == 0) {
        FillDataToHuffmanDecodingTreeLeaf(huffmanTree, fileProcessing, leavesCount, inputSize);
    } else {
        GrowHuffmanDecodingTree(dynamicBinCodesIdx, huffmanTree, fileProcessing, leavesCount,
                                inputSize, dynamicSymbolsBinaryCode);
    }
    return huffmanTree;
}

bool IsVertexLeaf(const THuffmanTree *huffmanTree) {
    return huffmanTree->leftSubtree == NULL
           && huffmanTree->rightSubtree == NULL;
}

void FindNextDecodingTreesLeaf(THuffmanTree *huffmanTree, ui8 *currentCode,
                               TRWFileBits *fileProcessing, size_t *inputSize) {
    fileProcessing->bitsIdx = fileProcessing->bitsIdx + 1;
    if (IsBitsContainerOverflow(fileProcessing)) {
        UpdateBitsContainer(fileProcessing, inputSize);
    }
    if (IsVertexLeaf(huffmanTree)) {
        *currentCode = (ui8) huffmanTree->character;
    } else {
        if (fileProcessing->bitsContainer[fileProcessing->bitsIdx] == 0) {
            FindNextDecodingTreesLeaf(huffmanTree->leftSubtree, currentCode, fileProcessing, inputSize);
        } else {
            FindNextDecodingTreesLeaf(huffmanTree->rightSubtree, currentCode, fileProcessing, inputSize);
        }
    }
}

bool AreMissingBitsReached(const size_t inputSize, const char howManyBitsMissing, TRWFileBits *fileProcessing) {
    return fileProcessing->bufferIdx == inputSize - 1 &&
           fileProcessing->bitsIdx + howManyBitsMissing == BYTE_LENGTH &&
           inputSize < BUFFER_SIZE;
}

void PrintNativeTextByHuffmanTree(const size_t leavesCount, const char howManyBitsMissing, THuffmanTree *huffmanTree,
                                  TRWFileBits *fileProcessing, size_t *inputSize) {
    if (IsBitsContainerOverflow(fileProcessing)) {
        UpdateBitsContainer(fileProcessing, inputSize);
    }
    while (FileDoesntEnd(fileProcessing->bufferIdx, *inputSize)) {
        ui8 code;
        if (fileProcessing->bitsContainer[fileProcessing->bitsIdx] == 0) {
            if (AreMissingBitsReached(*inputSize, howManyBitsMissing, fileProcessing)) {
                break;
            }
            if (leavesCount == 1) {
                code = (ui8) huffmanTree->character;
                fwrite(&code, sizeof(ui8), 1, fileProcessing->outputStream);
                fileProcessing->bitsIdx++;
                if (IsBitsContainerOverflow(fileProcessing)) {
                    UpdateBitsContainer(fileProcessing, inputSize);
                }
                continue;
            }
            FindNextDecodingTreesLeaf(huffmanTree->leftSubtree, &code, fileProcessing, inputSize);
            fwrite(&code, sizeof(ui8), 1, fileProcessing->outputStream);
        } else {
            FindNextDecodingTreesLeaf(huffmanTree->rightSubtree, &code, fileProcessing, inputSize);
            fwrite(&code, sizeof(ui8), 1, fileProcessing->outputStream);
        }
        if (IsBitsContainerOverflow(fileProcessing)) {
            UpdateBitsContainer(fileProcessing, inputSize);
        }
    }
}

void DeleteCodesTable(const size_t realSize, TCodesTable *codesTable) {
    for (size_t i = 0; i < realSize; i++) {
        free(codesTable[i].bits);
    }
    free(codesTable);
}

void CloseStreams(TRWFileBits *fileProcessing) {
    fclose(fileProcessing->inputStream);
    fclose(fileProcessing->outputStream);
    free(fileProcessing);
}

void _deleteHuffmanTree(THuffmanTree *tree) {
    if (tree->leftSubtree) {
        _deleteHuffmanTree(tree->leftSubtree);
        free(tree->leftSubtree);
    }
    if (tree->rightSubtree) {
        _deleteHuffmanTree(tree->rightSubtree);
        free(tree->rightSubtree);
    }
}

void DeleteHuffmanTree(THuffmanTree *huffmanTree) {
    if (huffmanTree) {
        _deleteHuffmanTree(huffmanTree);
        free(huffmanTree);
    }
}

void GetFirstBitsForDecodingTree(TRWFileBits *fileProcessing, size_t *inputSize) {
    fileProcessing->bufferIdx = 0;
    *inputSize = fread(fileProcessing->bufferData, sizeof(ui8), BUFFER_SIZE,
                       fileProcessing->inputStream);
    fileProcessing->bitsIdx = 0;
    for (int copyIdx = 0; copyIdx < BYTE_LENGTH; copyIdx++) {
        fileProcessing->bitsContainer[copyIdx] = GetElementsBit(fileProcessing->bufferData[fileProcessing->bufferIdx],
                                                                BYTE_LENGTH - copyIdx - 1);
    }
}

void WriteInfoAboutEmptyFile(TRWFileBits *fileProcessing) {
    fseek(fileProcessing->outputStream, 0, SEEK_SET);
    char emptyFileValue = FILE_EMPTY;
    fwrite(&emptyFileValue, sizeof(char), 1, fileProcessing->outputStream);
}

bool IsNativeFileWasEmpty(char controlValue) {
    return controlValue == FILE_EMPTY;
}

int main(void) {
    TRWFileBits *fileProcessing = malloc(sizeof(TRWFileBits));
    fileProcessing->inputStream = fopen("in.txt", "rb");
    fileProcessing->outputStream = fopen("out.txt", "wb");
    ui8 *workingDirection = malloc(sizeof(ui8) * 3);
    if (fread(workingDirection, sizeof(ui8), 3, fileProcessing->inputStream) != 3) {}
    if (workingDirection[0] == ZIP_TEXT) {
        fseek(fileProcessing->outputStream, 1, SEEK_CUR);
        size_t realTextElementsNumber = 0;
        THuffmanTree *priorityQueue = InputTextToPriorityQueue(fileProcessing, &realTextElementsNumber);
        if (realTextElementsNumber == 0) {
            WriteInfoAboutEmptyFile(fileProcessing);
            CloseStreams(fileProcessing);
            free(priorityQueue);
            free(workingDirection);
            return EXIT_SUCCESS;
        }
        THuffmanTree *huffmanTree = BuildHuffmanCodingTree(realTextElementsNumber, priorityQueue);
        free(priorityQueue);
        TCodesTable *codesTable = malloc(sizeof(TCodesTable) * realTextElementsNumber);
        size_t codesTableIdx = 0;
        ui8 neededBits = 0;
        ui8 dynamicSymbolsBinaryCode[MAX_CHAR_NUM];
        CreateCodesTable(0, huffmanTree, &codesTableIdx, codesTable,
                         dynamicSymbolsBinaryCode);
        WriteNewCodesToFile(realTextElementsNumber, huffmanTree, codesTable, fileProcessing, &neededBits);
        fseek(fileProcessing->outputStream, 0, SEEK_SET);
        fwrite(&neededBits, sizeof(ui8), 1, fileProcessing->outputStream);
        DeleteHuffmanTree(huffmanTree);
        DeleteCodesTable(realTextElementsNumber, codesTable);
        CloseStreams(fileProcessing);
    }
    if (workingDirection[0] == UNZIP_TEXT) {
        char howManyBitsAdded;
        if (!fread(&howManyBitsAdded, sizeof(char), 1, fileProcessing->inputStream)) {
        }
        if (IsNativeFileWasEmpty(howManyBitsAdded)) {
            free(workingDirection);
            CloseStreams(fileProcessing);
            return EXIT_SUCCESS;
        }
        THuffmanTree *huffmanTree = NULL;
        size_t currentInputSize;
        GetFirstBitsForDecodingTree(fileProcessing, &currentInputSize);
        size_t leavesCount = 0;
        ui8 dynamicSymbolsBinaryCode[MAX_CHAR_NUM];
        huffmanTree = BuildHuffmanDecodingTree(0, huffmanTree, fileProcessing,
                                               &leavesCount, &currentInputSize, dynamicSymbolsBinaryCode);
        PrintNativeTextByHuffmanTree(leavesCount, howManyBitsAdded, huffmanTree, fileProcessing,
                                     &currentInputSize);
        CloseStreams(fileProcessing);
        DeleteHuffmanTree(huffmanTree);
    }
    free(workingDirection);
    return EXIT_SUCCESS;
}
