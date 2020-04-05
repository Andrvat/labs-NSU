#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_CHAR_NUM 256
#define ZIP_TEXT 'c'
#define UNZIP_TEXT 'd'
#define BUFFER_SIZE 20
#define BYTE_LENGTH 8
#define FILE_EMPTY -1

typedef unsigned char unChar;

unChar dynamicSymbolsBinaryCode[256];

typedef struct unsignedCharBuffer {
    unChar data[BUFFER_SIZE];
    short movingIdx; // <= BUFFER_SIZE
} TUnCharBuffer;

typedef struct huffmanDecodingTree {
    struct huffmanDecodingTree *leftSubtree;
    struct huffmanDecodingTree *rightSubtree;
    unChar characterCode;
    unChar characterValue; // <= MAX_CHAR_NUM
    bool isLeaf;
} THuffmanDecodingTree;


typedef struct huffmanCodingTree {
    struct huffmanCodingTree *leftSubtree;
    struct huffmanCodingTree *rightSubtree;
    unChar character;
    int frequenciesSum;
} THuffmanCodingTree;

typedef struct binaryCodesTable {
    unChar *bits;
    unChar character;
    unChar bitsLen; // <= 8
} TCodesTable;

typedef struct fileHuffmanTreeSequence {
    unChar sequenceContainer[MAX_CHAR_NUM * 3];
    unsigned short sequenceCount;
} TTreeSequence;

typedef struct readWriteFileBits {
    unChar bitsContainer[BYTE_LENGTH];
    FILE *outputStream;
    FILE *inputStream;
    short bitsIdx;
} TRWFileBits;

void printIOException() {
    fprintf(stderr, "Check your file to have correct data! The file is empty\n");
}

bool fileDidntEnd(const short movingIdx, const short inputSize) {
    return movingIdx != inputSize || inputSize == BUFFER_SIZE;
}

bool isBufferOverflow(TUnCharBuffer *buffer) {
    return buffer->movingIdx == BUFFER_SIZE;
}

void updateBuffer(TUnCharBuffer *buffer, short *inputSize, TRWFileBits *fileProcessing) {
    if ((*inputSize = (short) fread(buffer->data, sizeof(unChar), BUFFER_SIZE, fileProcessing->inputStream)) < 0) {
        printIOException();
    }
    buffer->movingIdx = 0;
}

int getElementByBinaryCode(const unChar *buffer) {
    int element = 0;
    for (int i = 0; i < BYTE_LENGTH; i++) {
        element = (element << 1) | (buffer[i] - '0');
    }
    return element;
}

unChar *getElementsBinaryCode(unChar element) {
    unChar *bitsCode = malloc(sizeof(unChar) * BYTE_LENGTH);
    for (int j = 0; j < BYTE_LENGTH; j++) {
        bitsCode[j] = '0';
    }
    short i = 0;
    while (element != 0) {
        bitsCode[BYTE_LENGTH - i - 1] = element % 2 + '0';
        element = (element) >> 1;
        i++;
    }
    return bitsCode;
}

int leavesComparator(const void *a, const void *b) {
    return (((THuffmanCodingTree *) a)->frequenciesSum) - (((THuffmanCodingTree *) b)->frequenciesSum);
}

bool isCountZero(const size_t count) {
    return count == 0;
}

void initHuffmanCodingTreeLeaves(THuffmanCodingTree *tree) {
    for (int i = 0; i < MAX_CHAR_NUM; i++) {
        tree[i].character = (unChar) i;
        tree[i].frequenciesSum = INT_MAX;
        tree[i].leftSubtree = NULL;
        tree[i].rightSubtree = NULL;
    }
}

bool isLeafYetNotVisited(THuffmanCodingTree leaf) {
    return leaf.frequenciesSum == INT_MAX;
}

void fillLeafsData(size_t *leavesCount, TUnCharBuffer *buffer,
                   THuffmanCodingTree *tree) {
    unChar element = buffer->data[buffer->movingIdx];
    buffer->movingIdx += 1;
    if (isLeafYetNotVisited(tree[element])) {
        tree[element].frequenciesSum = 0;
        *leavesCount += 1;
    }
    tree[element].frequenciesSum++;
}

void copyCountTreesLeafs(const short count, THuffmanCodingTree *copyTo, THuffmanCodingTree *copyFrom) {
    for (int i = 0; i < (int) count; i++) {
        copyTo[i] = copyFrom[i];
    }
}

THuffmanCodingTree *inputTextToHuffmanLeaves(TRWFileBits *fileProcessing, size_t *realSize) {
    THuffmanCodingTree *oldTree = malloc(sizeof(THuffmanCodingTree) * MAX_CHAR_NUM);
    initHuffmanCodingTreeLeaves(oldTree);
    TUnCharBuffer *buffer = malloc(sizeof(TUnCharBuffer));
    short inputSize;
    if ((inputSize = (short) fread(buffer->data, sizeof(unChar), BUFFER_SIZE, fileProcessing->inputStream)) < 0) {
        printIOException();
    }
    if (isCountZero(inputSize)) {
        *realSize = 0;
        free(buffer);
        free(oldTree);
        return NULL;
    }
    buffer->movingIdx = 0;
    fillLeafsData(realSize, buffer, oldTree);
    while (fileDidntEnd(buffer->movingIdx, inputSize)) {
        fillLeafsData(realSize, buffer, oldTree);
        if (isBufferOverflow(buffer)) {
            updateBuffer(buffer, &inputSize, fileProcessing);
        }
    }
    free(buffer);
    qsort(oldTree, MAX_CHAR_NUM, sizeof(THuffmanCodingTree), leavesComparator);
    THuffmanCodingTree *realTree = malloc(sizeof(THuffmanCodingTree) * (*realSize));
    copyCountTreesLeafs(*realSize, realTree, oldTree);
    free(oldTree);
    fclose(fileProcessing->inputStream);
    return realTree;
}

void swapTwoVertices(THuffmanCodingTree *first, THuffmanCodingTree *second) {
    THuffmanCodingTree saveCopy = *first;
    *first = *second;
    *second = saveCopy;
}

void raiseVertexToTheTop(THuffmanCodingTree *tree, size_t currentIndex) {
    while ((currentIndex + 1) / 2 != 0 &&
           tree[currentIndex].frequenciesSum < tree[(currentIndex - 1) / 2].frequenciesSum) {
        swapTwoVertices(&tree[currentIndex], &tree[(currentIndex - 1) / 2]);
        currentIndex = (currentIndex - 1) / 2;
    }
}

bool isItRoot(const size_t currentIdx) {
    return currentIdx == 0;
}

void getTwoMinLeavesByFrequencies(THuffmanCodingTree *tree, unsigned int currentIdx) {
    for (int i = 0; i < 2; i++) {
        if (isItRoot(currentIdx)) {
            return;
        }
        swapTwoVertices(&tree[0], &tree[currentIdx]);
        currentIdx--;
        size_t movementIdx = 0;
        size_t firstChildIdx = 1;
        size_t secondChildIdx = 2;
        while (firstChildIdx <= currentIdx &&
               (tree[movementIdx].frequenciesSum > tree[firstChildIdx].frequenciesSum ||
                tree[movementIdx].frequenciesSum > tree[secondChildIdx].frequenciesSum)) {
            if (secondChildIdx == currentIdx + 1) {
                if (tree[movementIdx].frequenciesSum > tree[firstChildIdx].frequenciesSum) {
                    swapTwoVertices(&tree[movementIdx], &tree[firstChildIdx]);
                }
                movementIdx = firstChildIdx;
            } else if (tree[firstChildIdx].frequenciesSum <= tree[secondChildIdx].frequenciesSum) {
                swapTwoVertices(&tree[movementIdx], &tree[firstChildIdx]);
                movementIdx = firstChildIdx;
            } else {
                swapTwoVertices(&tree[movementIdx], &tree[secondChildIdx]);
                movementIdx = secondChildIdx;
            }
            firstChildIdx = movementIdx * 2 + 1;
            secondChildIdx = movementIdx * 2 + 2;
        }
    }
}

void copyCountElements(const size_t count, const unChar *copyFrom, unChar *copyTo) {
    for (size_t i = 0; i < count; i++) {
        copyTo[i] = copyFrom[i];
    }
}

void createCodesTableAndTreeSequence(THuffmanCodingTree *root, int dynamicCodesIdx, size_t *tableIdx,
                                     TCodesTable *codesTable, TTreeSequence *sequence) {
    if (root->leftSubtree != NULL) {
        sequence->sequenceContainer[sequence->sequenceCount] = '1'; // T in sequence
        sequence->sequenceCount++;
        dynamicCodesIdx++;
        dynamicSymbolsBinaryCode[dynamicCodesIdx - 1] = '0';
        createCodesTableAndTreeSequence(root->leftSubtree, dynamicCodesIdx, tableIdx, codesTable, sequence);
        dynamicCodesIdx--;
    }
    if (root->rightSubtree != NULL) {
        dynamicCodesIdx++;
        dynamicSymbolsBinaryCode[dynamicCodesIdx - 1] = '1';
        createCodesTableAndTreeSequence(root->rightSubtree, dynamicCodesIdx, tableIdx, codesTable, sequence);
        dynamicCodesIdx--;
    }
    if (root->leftSubtree == NULL && root->rightSubtree == NULL) {
        if (isCountZero(dynamicCodesIdx)) {
            dynamicSymbolsBinaryCode[0] = '0';
            dynamicCodesIdx++;
        }
        codesTable[*tableIdx].character = root->character;
        sequence->sequenceContainer[sequence->sequenceCount] = '0'; // L in sequence
        sequence->sequenceCount++;
        sequence->sequenceContainer[sequence->sequenceCount] = codesTable[*tableIdx].character;
        sequence->sequenceCount++;
        codesTable[*tableIdx].bits = malloc(sizeof(unChar) * dynamicCodesIdx);
        copyCountElements(dynamicCodesIdx, dynamicSymbolsBinaryCode, codesTable[*tableIdx].bits);
        codesTable[*tableIdx].bitsLen = dynamicCodesIdx;
        *tableIdx += 1;
    }
}

THuffmanCodingTree *createNewVertex(THuffmanCodingTree copyFrom) {
    THuffmanCodingTree *vertex = malloc(sizeof(THuffmanCodingTree));
    vertex->character = copyFrom.character;
    vertex->rightSubtree = copyFrom.rightSubtree;
    vertex->leftSubtree = copyFrom.leftSubtree;
    vertex->frequenciesSum = copyFrom.frequenciesSum;
    return vertex;
}

void buildHuffmanCodingTree(const size_t realSize, THuffmanCodingTree *tree) {
    size_t currentIdx = realSize - 1;
    if (isItRoot(currentIdx)) {
        tree[currentIdx].frequenciesSum = 0;
    }
    while (currentIdx != 0) {
        getTwoMinLeavesByFrequencies(tree, currentIdx);
        THuffmanCodingTree *unionLeaves = malloc(sizeof(THuffmanCodingTree));
        unionLeaves->frequenciesSum = tree[currentIdx].frequenciesSum + tree[currentIdx - 1].frequenciesSum;
        THuffmanCodingTree *leftChild = createNewVertex(tree[currentIdx]);
        THuffmanCodingTree *rightChild = createNewVertex(tree[currentIdx - 1]);
        unionLeaves->leftSubtree = leftChild;
        unionLeaves->rightSubtree = rightChild;
        currentIdx--;
        swapTwoVertices(&tree[currentIdx], unionLeaves);
        free(unionLeaves);
        raiseVertexToTheTop(tree, currentIdx);
    }
}

void copyElementToBitsContainer(const unChar element, TRWFileBits *fileProcessing) {
    fileProcessing->bitsContainer[fileProcessing->bitsIdx] = element;
    fileProcessing->bitsIdx++;
    if (fileProcessing->bitsIdx == BYTE_LENGTH) {
        unChar elementToWrite = (unChar) getElementByBinaryCode(fileProcessing->bitsContainer);
        fwrite(&elementToWrite, sizeof(unChar), 1, fileProcessing->outputStream);
        fileProcessing->bitsIdx = 0;
    }
}

bool isSequenceElementAlphabetSymbol(const size_t seqIdx, TTreeSequence *sequence) {
    return (sequence->sequenceContainer[seqIdx] != '0' && sequence->sequenceContainer[seqIdx] != '1') ||
           (sequence->sequenceContainer[seqIdx - 1] == '0' && sequence->sequenceContainer[seqIdx - 2] != '0' &&
            (sequence->sequenceContainer[seqIdx] == '0' || sequence->sequenceContainer[seqIdx] == '1'));
}

void writeTreeSequenceToFile(TTreeSequence *treeSequence, TRWFileBits *fileProcessing) {
    fileProcessing->bitsIdx = 0;
    for (size_t seqIdx = 0; seqIdx < treeSequence->sequenceCount; seqIdx++) {
        if (!isItRoot(seqIdx)) {
            if (isSequenceElementAlphabetSymbol(seqIdx, treeSequence)) {
                unChar *binCode = getElementsBinaryCode(treeSequence->sequenceContainer[seqIdx]);
                for (int i = 0; i < BYTE_LENGTH; i++) {
                    copyElementToBitsContainer(binCode[i], fileProcessing);
                }
                free(binCode);
                continue;
            }
        }
        copyElementToBitsContainer(treeSequence->sequenceContainer[seqIdx], fileProcessing);
    }
}

bool isNeededToWriteMissingBits(const short bitsIdx) {
    return bitsIdx != 0 && bitsIdx <= BYTE_LENGTH;
}

void writeMissingBits(TRWFileBits *fileProcessing, unChar *neededBits) {
    for (int i = fileProcessing->bitsIdx; i < BYTE_LENGTH; i++) {
        fileProcessing->bitsContainer[i] = '0';
        *neededBits += 1;
    }
    unChar elementToWrite = (unChar) getElementByBinaryCode(fileProcessing->bitsContainer);
    fwrite(&elementToWrite, sizeof(unChar), 1, fileProcessing->outputStream);
}

void encodeTextOfNewCharacters(const size_t realSize, TCodesTable *codesTable, TRWFileBits *fileProcessing,
                               unChar *neededBits) {
    TUnCharBuffer *buffer = malloc(sizeof(TUnCharBuffer));
    unChar *tmpContainer = malloc(sizeof(unChar) * 3);
    if (!fread(tmpContainer, sizeof(unChar), 3, fileProcessing->inputStream)) {
        printIOException();
    }
    free(tmpContainer);
    short inputSize;
    if ((inputSize = (short) fread(buffer->data, sizeof(unChar), BUFFER_SIZE, fileProcessing->inputStream)) < 0) {
        printIOException();
    }
    buffer->movingIdx = 0;
    while (fileDidntEnd(buffer->movingIdx, inputSize)) {
        for (size_t tableIdx = 0; tableIdx < realSize; tableIdx++) {
            if (codesTable[tableIdx].character == buffer->data[buffer->movingIdx]) {
                for (int i = 0; i < codesTable[tableIdx].bitsLen; i++) {
                    copyElementToBitsContainer(codesTable[tableIdx].bits[i], fileProcessing);
                }
                buffer->movingIdx++;
                break;
            }
        }
        if (isBufferOverflow(buffer)) {
            updateBuffer(buffer, &inputSize, fileProcessing);
        }
    }
    free(buffer);
    if (isNeededToWriteMissingBits(fileProcessing->bitsIdx)) {
        writeMissingBits(fileProcessing, neededBits);
    }
}

void writeNewCodesToFile(const size_t realSize, TCodesTable *codesTable,
                         TTreeSequence *sequence, TRWFileBits *fileProcessing, unChar *neededBits) {
    fileProcessing->inputStream = fopen("in.txt", "rb");
    writeTreeSequenceToFile(sequence, fileProcessing);
    encodeTextOfNewCharacters(realSize, codesTable, fileProcessing, neededBits);
}

bool isBitsContainerOverflow(TRWFileBits *fileProcessing) {
    return fileProcessing->bitsIdx == BYTE_LENGTH;
}

void updateBitsContainer(TRWFileBits *fileProcessing, TUnCharBuffer *buffer, short *inputSize) {
    fileProcessing->bitsIdx = 0;
    buffer->movingIdx = (short) (buffer->movingIdx + 1);
    if (isBufferOverflow(buffer)) {
        updateBuffer(buffer, inputSize, fileProcessing);
    }
    unChar *newElement = getElementsBinaryCode(buffer->data[buffer->movingIdx]);
    for (int copyIdx = 0; copyIdx < BYTE_LENGTH; copyIdx++) {
        fileProcessing->bitsContainer[copyIdx] = newElement[copyIdx];
    }
    free(newElement);
}

THuffmanDecodingTree *buildHuffmanDecodingTree(THuffmanDecodingTree *huffmanTree, int dynamicBinCodesIdx,
                                               TRWFileBits *fileProcessing,
                                               TUnCharBuffer *buffer,
                                               bool *isItStartBuilding, short *countOfLeaves, short *inputSize) {
    if (fileProcessing->bitsContainer[fileProcessing->bitsIdx] == '0' && *isItStartBuilding == true) {
        *isItStartBuilding = false;
        huffmanTree = malloc(sizeof(THuffmanDecodingTree));
        huffmanTree->leftSubtree = NULL;
        huffmanTree->rightSubtree = NULL;
        huffmanTree->isLeaf = true;
        unChar elementBits[BYTE_LENGTH];
        for (int i = 0; i < BYTE_LENGTH; i++) {
            fileProcessing->bitsIdx += 1;
            if (isBitsContainerOverflow(fileProcessing)) {
                updateBitsContainer(fileProcessing, buffer, inputSize);
            }
            elementBits[i] = fileProcessing->bitsContainer[fileProcessing->bitsIdx];
        }
        unChar element = getElementByBinaryCode(elementBits);
        huffmanTree->characterCode = 0;
        *countOfLeaves = 1;
        huffmanTree->characterValue = element;
        fileProcessing->bitsIdx += 1;
        return huffmanTree;
    } else if (fileProcessing->bitsContainer[fileProcessing->bitsIdx] == '1' && *isItStartBuilding == true) {
        *isItStartBuilding = false;
        huffmanTree = malloc(sizeof(THuffmanDecodingTree));
        huffmanTree->leftSubtree = malloc(sizeof(THuffmanDecodingTree));
        huffmanTree->rightSubtree = malloc(sizeof(THuffmanDecodingTree));
        huffmanTree->isLeaf = false;
        fileProcessing->bitsIdx += 1;
        if (isBitsContainerOverflow(fileProcessing)) {
            updateBitsContainer(fileProcessing, buffer, inputSize);
        }
        dynamicBinCodesIdx++;
        dynamicSymbolsBinaryCode[dynamicBinCodesIdx - 1] = '0';
        buildHuffmanDecodingTree(huffmanTree->leftSubtree, dynamicBinCodesIdx, fileProcessing, buffer,
                                 isItStartBuilding,
                                 countOfLeaves, inputSize);
        dynamicSymbolsBinaryCode[dynamicBinCodesIdx - 1] = '1';
        buildHuffmanDecodingTree(huffmanTree->rightSubtree, dynamicBinCodesIdx, fileProcessing, buffer,
                                 isItStartBuilding,
                                 countOfLeaves, inputSize);
        return huffmanTree;
    }

    if (fileProcessing->bitsContainer[fileProcessing->bitsIdx] == '1') {
        huffmanTree->leftSubtree = malloc(sizeof(THuffmanDecodingTree));
        huffmanTree->rightSubtree = malloc(sizeof(THuffmanDecodingTree));
        huffmanTree->isLeaf = false;
        fileProcessing->bitsIdx += 1;
        if (isBitsContainerOverflow(fileProcessing)) {
            updateBitsContainer(fileProcessing, buffer, inputSize);
        }
        dynamicBinCodesIdx++;
        dynamicSymbolsBinaryCode[dynamicBinCodesIdx - 1] = '0';
        buildHuffmanDecodingTree(huffmanTree->leftSubtree, dynamicBinCodesIdx, fileProcessing, buffer,
                                 isItStartBuilding,
                                 countOfLeaves, inputSize);
        dynamicSymbolsBinaryCode[dynamicBinCodesIdx - 1] = '1';
        buildHuffmanDecodingTree(huffmanTree->rightSubtree, dynamicBinCodesIdx, fileProcessing, buffer,
                                 isItStartBuilding,
                                 countOfLeaves, inputSize);
    } else {
        huffmanTree->leftSubtree = NULL;
        huffmanTree->rightSubtree = NULL;
        huffmanTree->isLeaf = true;
        unChar elementBits[BYTE_LENGTH];
        for (int i = 0; i < BYTE_LENGTH; i++) {
            fileProcessing->bitsIdx += 1;
            if (isBitsContainerOverflow(fileProcessing)) {
                updateBitsContainer(fileProcessing, buffer, inputSize);
            }
            elementBits[i] = fileProcessing->bitsContainer[fileProcessing->bitsIdx];
        }
        fileProcessing->bitsIdx += 1;
        if (isBitsContainerOverflow(fileProcessing)) {
            updateBitsContainer(fileProcessing, buffer, inputSize);
        }
        unChar element = getElementByBinaryCode(elementBits);
        *countOfLeaves += 1;
        huffmanTree->characterCode = 0;
        for (int i = dynamicBinCodesIdx - 1; i >= 0; i--) {
            huffmanTree->characterCode = (element << 1) | (dynamicSymbolsBinaryCode[i] - '0');
        }
        huffmanTree->characterValue = element;
    }
    return NULL;
}

void findNextDecodingTreesLeaf(THuffmanDecodingTree *huffmanTree, unChar *code,
                               TRWFileBits *fileProcessing, TUnCharBuffer *buffer, short *inputSize) {
    fileProcessing->bitsIdx = (short) (fileProcessing->bitsIdx + 1);
    if (isBitsContainerOverflow(fileProcessing)) {
        updateBitsContainer(fileProcessing, buffer, inputSize);
    }
    if (huffmanTree->isLeaf == false) {
        if (fileProcessing->bitsContainer[fileProcessing->bitsIdx] == '0') {
            findNextDecodingTreesLeaf(huffmanTree->leftSubtree, code, fileProcessing, buffer, inputSize);
        } else {
            findNextDecodingTreesLeaf(huffmanTree->rightSubtree, code, fileProcessing, buffer, inputSize);
        }
    } else {
        *code = (unChar) huffmanTree->characterValue;
    }
}

bool areMissingBitsReached(const short inputSize, const char howManyBitsMissing, TRWFileBits *fileProcessing,
                           TUnCharBuffer *buffer) {
    return buffer->movingIdx == inputSize - 1 && fileProcessing->bitsIdx + howManyBitsMissing == BYTE_LENGTH &&
           inputSize < BUFFER_SIZE;
}

void printNativeTextByHuffmanTree(const char howManyBitsMissing, THuffmanDecodingTree *huffmanTree,
                                  TRWFileBits *fileProcessing,
                                  TUnCharBuffer *buffer, short countOfLeaves, short *inputSize) {
    if (isBitsContainerOverflow(fileProcessing)) {
        updateBitsContainer(fileProcessing, buffer, inputSize);
    }
    while (fileDidntEnd(buffer->movingIdx, *inputSize)) {
        if (fileProcessing->bitsContainer[fileProcessing->bitsIdx] == '0') {
            if (areMissingBitsReached(*inputSize, howManyBitsMissing, fileProcessing, buffer)) {
                break;
            }
            unChar code;
            if (countOfLeaves == 1) {
                code = (unChar) huffmanTree->characterValue;
                fwrite(&code, sizeof(unChar), 1, fileProcessing->outputStream);
                fileProcessing->bitsIdx++;
                if (isBitsContainerOverflow(fileProcessing)) {
                    updateBitsContainer(fileProcessing, buffer, inputSize);
                }
                continue;
            }
            findNextDecodingTreesLeaf(huffmanTree->leftSubtree, &code, fileProcessing, buffer, inputSize);
            fwrite(&code, sizeof(unChar), 1, fileProcessing->outputStream);
        } else {
            unChar code;
            findNextDecodingTreesLeaf(huffmanTree->rightSubtree, &code, fileProcessing, buffer, inputSize);
            fwrite(&code, sizeof(unChar), 1, fileProcessing->outputStream);
        }
        if (isBitsContainerOverflow(fileProcessing)) {
            updateBitsContainer(fileProcessing, buffer, inputSize);
        }
    }
}

void deleteCodesTable(const size_t realSize, TCodesTable *codesTable) {
    for (size_t i = 0; i < realSize; i++) {
        free(codesTable[i].bits);
    }
    free(codesTable);
}

void _deleteDecodingTree(THuffmanDecodingTree *vertex) {
    if (vertex->leftSubtree) {
        _deleteDecodingTree(vertex->leftSubtree);
        free(vertex->leftSubtree);
    }
    if (vertex->rightSubtree) {
        _deleteDecodingTree(vertex->rightSubtree);
        free(vertex->rightSubtree);
    }
}

void deleteDecodingTree(THuffmanDecodingTree *root) {
    if (root) {
        _deleteDecodingTree(root);
        free(root);
    }
}

void closeStreams(TRWFileBits *fileProcessing) {
    fclose(fileProcessing->inputStream);
    fclose(fileProcessing->outputStream);
    free(fileProcessing);
}

void _deleteCodingTree(THuffmanCodingTree *vertex) {
    if (vertex->leftSubtree) {
        _deleteCodingTree(vertex->leftSubtree);
        free(vertex->leftSubtree);
    }
    if (vertex->rightSubtree) {
        _deleteCodingTree(vertex->rightSubtree);
        free(vertex->rightSubtree);
    }
}

void deleteCodingTree(THuffmanCodingTree *root) {
    if (root) {
        _deleteCodingTree(root);
        free(root);
    }
}

void getFirstBitsForDecodingTree(TRWFileBits *fileProcessing, TUnCharBuffer *buffer, short *inputSize) {
    buffer->movingIdx = 0;
    if ((*inputSize = (short) fread(buffer->data, sizeof(unChar), BUFFER_SIZE, fileProcessing->inputStream)) < 0) {
        printIOException();
    }
    fileProcessing->bitsIdx = 0;
    unChar *newElement = getElementsBinaryCode(buffer->data[buffer->movingIdx]);
    for (int i = 0; i < BYTE_LENGTH; i++) {
        fileProcessing->bitsContainer[i] = newElement[i];
    }
    free(newElement);
}

void writeInfoAboutEmptyFile(TRWFileBits *fileProcessing) {
    fseek(fileProcessing->outputStream, 0, SEEK_SET);
    char emptyFileValue = FILE_EMPTY;
    fwrite(&emptyFileValue, sizeof(char), 1, fileProcessing->outputStream);
}

bool isNativeFileWasEmpty(char controlValue) {
    return controlValue == FILE_EMPTY;
}

int main(void) {
    TRWFileBits *fileProcessing = malloc(sizeof(TRWFileBits));
    fileProcessing->inputStream = fopen("in.txt", "rb");
    fileProcessing->outputStream = fopen("out.txt", "wb");
    unChar *workingDirection = malloc(sizeof(unChar) * 3);
    if (fread(workingDirection, sizeof(unChar), 3, fileProcessing->inputStream) != 3) {
        printIOException();
    }
    if (workingDirection[0] == ZIP_TEXT) {
        fseek(fileProcessing->outputStream, 1, SEEK_CUR);
        size_t realTextElementsNumber = 0;
        THuffmanCodingTree *huffmanTree = inputTextToHuffmanLeaves(fileProcessing, &realTextElementsNumber);
        if (isCountZero(realTextElementsNumber)) {
            writeInfoAboutEmptyFile(fileProcessing);
            closeStreams(fileProcessing);
            free(workingDirection);
            return EXIT_SUCCESS;
        }
        buildHuffmanCodingTree(realTextElementsNumber, huffmanTree);
        TCodesTable *codesTable = malloc(sizeof(TCodesTable) * realTextElementsNumber);
        size_t codesTableIdx = 0;
        unChar neededBits = 0;
        TTreeSequence *treeSequence = malloc(sizeof(TTreeSequence));
        treeSequence->sequenceCount = 0;
        createCodesTableAndTreeSequence(huffmanTree, 0, &codesTableIdx, codesTable, treeSequence);
        writeNewCodesToFile(realTextElementsNumber, codesTable, treeSequence, fileProcessing, &neededBits);
        fseek(fileProcessing->outputStream, 0, SEEK_SET);
        fwrite(&neededBits, sizeof(unChar), 1, fileProcessing->outputStream);
        free(treeSequence);
        deleteCodingTree(huffmanTree);
        deleteCodesTable(realTextElementsNumber, codesTable);
        closeStreams(fileProcessing);
    }
    if (workingDirection[0] == UNZIP_TEXT) {
        char howManyBitsAdded;
        if (!fread(&howManyBitsAdded, sizeof(char), 1, fileProcessing->inputStream)) {
            printIOException();
        }
        if (isNativeFileWasEmpty(howManyBitsAdded)) {
            free(workingDirection);
            return EXIT_SUCCESS;
        }
        THuffmanDecodingTree *huffmanTree = NULL;
        TUnCharBuffer *buffer = malloc(sizeof(TUnCharBuffer));
        short currentInputSize;
        getFirstBitsForDecodingTree(fileProcessing, buffer, &currentInputSize);
        bool isItStartBuilding = true;
        short leavesCount = 0;
        huffmanTree = buildHuffmanDecodingTree(huffmanTree, 0, fileProcessing, buffer, &isItStartBuilding,
                                               &leavesCount, &currentInputSize);
        printNativeTextByHuffmanTree(howManyBitsAdded, huffmanTree, fileProcessing, buffer, leavesCount,
                                     &currentInputSize);
        free(buffer);
        closeStreams(fileProcessing);
        deleteDecodingTree(huffmanTree);
    }
    free(workingDirection);
    return EXIT_SUCCESS;
}
