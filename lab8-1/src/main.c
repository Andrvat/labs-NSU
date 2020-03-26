#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define MAX_VERTEX_NUM 5000


typedef enum checkData {
    success,
    noInput,
    badVertex,
    badLength,
    noSpanningTree,
    badNumOfLines,
    badNumOfVertices,
    badNumOfEdges
} workingResult;

typedef struct adjacencyMatrix {
    size_t size;
    short **data;
} TMatrix;

typedef struct intPair {
    short first;
    short second;
} TPair;


TMatrix *initMatrix(const int userSize) {
    TMatrix *matrix = malloc(sizeof(TMatrix));
    matrix->size = userSize;
    matrix->data = malloc(userSize * sizeof(short *));
    for (size_t i = 0; i < matrix->size; i++) {
        matrix->data[i] = malloc(sizeof(short) * userSize);
        for (size_t j = 0; j < matrix->size; j++) {
            matrix->data[i][j] = SHRT_MAX;
        }
    }
    return matrix;
}

void freeMatrix(TMatrix *matrix) {
    for (size_t i = 0; i < matrix->size; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}


void printIOException() {
    printf("There is no input. Check your file to data");
}

int checkNumberOfEdges(const int numberOfEdges, const int numberOfVertices) {
    return (numberOfEdges <= numberOfVertices * (numberOfVertices + 1) / 2);
}

int inputInitialValues(int *numberOfVertices, int *numberOfEdges) {
    int haveNextInput = 0;
    if (!scanf("%d", numberOfVertices)) {
        printIOException();
    }
    haveNextInput = scanf("%d", numberOfEdges);
    return haveNextInput == 1;
}

int isBetweenBoundaries(const long long int checkingValue, const int leftBoundary, const int rightBoundary) {
    return (checkingValue >= leftBoundary && checkingValue <= rightBoundary);
}

TMatrix *inputEdges(const int numberOfVertices, const int numberOfEdges,
                    workingResult *controlValueOfInputVertexes) {
    TMatrix *matrix = initMatrix(numberOfVertices);
    workingResult checkForCorrectData = success;
    for (int idx = 0; idx < numberOfEdges; ++idx) {
        int begin, end;
        int haveNextInput = 0;
        if (!scanf("%d%d", &begin, &end)) {
            printIOException();
        }
        long long int currentWeight;
        haveNextInput = scanf("%lld", &currentWeight);
        // если вводится число - то haveNextInput обязан равняться единице
        if (haveNextInput != 1) {
            *controlValueOfInputVertexes = noInput;
            return matrix;
        }
        if (!isBetweenBoundaries(currentWeight, 0, INT_MAX)) {
            checkForCorrectData = badLength;
            continue;
        }
        if (!(isBetweenBoundaries(begin, 1, numberOfVertices) &&
              isBetweenBoundaries(end, 1, numberOfVertices))) {
            checkForCorrectData = badVertex;
            continue;
        }
        if (currentWeight == INT_MAX) {
            currentWeight = SHRT_MAX - 1;
        }
        // индекс = номер вершины - 1
        begin--;
        end--;
        matrix->data[begin][end] = (short) currentWeight;
        matrix->data[end][begin] = (short) currentWeight;
    }
    *controlValueOfInputVertexes = checkForCorrectData;

    return matrix;
}

TPair *initMinDistList(const int numOfVertices) {
    TPair *minDist = malloc(sizeof(TPair) * numOfVertices);
    for (int i = 0; i < numOfVertices; i++) {
        minDist[i].first = SHRT_MAX;
        minDist[i].second = SHRT_MAX;
    }
    return minDist;
}

TPair *findMinimumSpanningTree(const int numberOfEdges, const int numberOfVertices,
                               workingResult *status, TMatrix *matrix) {
    if (numberOfVertices == 0 || (numberOfEdges == 0 && numberOfVertices != 1)) {
        *status = noSpanningTree;
        return NULL;
    }
    TPair *listOfEdges = malloc(sizeof(TPair) * (numberOfVertices - 1));
    bool *visitingList = malloc(sizeof(bool) * numberOfVertices);
    memset(visitingList, false, sizeof(bool) * numberOfVertices);
    TPair *minDist = initMinDistList(numberOfVertices);
    short movingStartVertex = 0;
    short countOfVisitedVertices = 0;
    for (size_t strIdx = 0; strIdx < matrix->size; strIdx++) {
        short idxOfMinValueInDistList = 0;
        for (size_t columnIdx = 0; columnIdx < matrix->size; columnIdx++) {
            if (matrix->data[movingStartVertex][columnIdx] < minDist[columnIdx].second && !visitingList[columnIdx]) {
                minDist[columnIdx].first = movingStartVertex;
                minDist[columnIdx].second = matrix->data[movingStartVertex][columnIdx];
            }
            if (minDist[columnIdx].second < minDist[idxOfMinValueInDistList].second) {
                idxOfMinValueInDistList = columnIdx;
            }
        }
        if (visitingList[movingStartVertex] == false) {
            countOfVisitedVertices++;
        }
        visitingList[movingStartVertex] = true;
        if (countOfVisitedVertices == numberOfVertices) {
            break;
        }
        if (!visitingList[idxOfMinValueInDistList]) {
            listOfEdges[strIdx].first = (short) (minDist[idxOfMinValueInDistList].first + 1);
            listOfEdges[strIdx].second = (short) (idxOfMinValueInDistList + 1);
        }
        minDist[idxOfMinValueInDistList].first = SHRT_MAX;
        minDist[idxOfMinValueInDistList].second = SHRT_MAX;
        movingStartVertex = idxOfMinValueInDistList;
    }
    if (countOfVisitedVertices != numberOfVertices) {
        *status = noSpanningTree;
    }
    free(visitingList);
    free(minDist);
    return listOfEdges;
}

void printMessageByStatus(const workingResult status) {
    switch (status) {
        case noInput:
            printf("bad number of lines");
            break;
        case badVertex:
            printf("bad vertex");
            break;
        case badLength:
            printf("bad length");
            break;
        case badNumOfLines:
            printf("bad number of lines");
            break;
        case badNumOfVertices:
            printf("bad number of vertices");
            break;
        case badNumOfEdges:
            printf("bad number of edges");
            break;
        case noSpanningTree:
            printf("no spanning tree");
            break;
        default:
            break;
    }
}

void printSpanningTree(const int numberOfVertices, TPair *listOfEdges) {
    for (int i = 0; i < numberOfVertices - 1; i++) {
        printf("%d %d\n", listOfEdges[i].first, listOfEdges[i].second);
    }
}

int main(void) {
    int numberOfVertices;
    int numberOfEdges;
    workingResult status = success;
    if (!inputInitialValues(&numberOfVertices, &numberOfEdges)) {
        status = badNumOfLines;
    } else if (!isBetweenBoundaries(numberOfVertices, 0, MAX_VERTEX_NUM)) {
        status = badNumOfVertices;
    } else if (!checkNumberOfEdges(numberOfEdges, numberOfVertices)) {
        status = badNumOfEdges;
    }
    TMatrix *matrix = NULL;
    TPair *listOfEdges = NULL;
    if (status == success) {
        matrix = inputEdges(numberOfVertices, numberOfEdges, &status);
    }
    if (status == success) {
        listOfEdges = findMinimumSpanningTree(numberOfEdges, numberOfVertices, &status, matrix);
    }
    if (status == success) {
        printSpanningTree(numberOfVertices, listOfEdges);
    } else {
        printMessageByStatus(status);
    }
    if (matrix != NULL) {
        freeMatrix(matrix);
    }
    if (listOfEdges != NULL) {
        free(listOfEdges);
    }
    return EXIT_SUCCESS;
}

