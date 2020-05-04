#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define MAX_VERTEX_NUM 5000
#define INFINITY 4294967295

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
    unsigned int **data;
} TMatrix;

typedef struct distanceToAdjacentVertex {
    unsigned int vertexNum;
    unsigned int minEdgeWeight;
} TDistToVertex;

TMatrix *initMatrix(const int userSize) {
    TMatrix *matrix = malloc(sizeof(TMatrix));
    matrix->size = userSize;
    matrix->data = malloc(userSize * sizeof(unsigned int *));
    for (size_t i = 0; i < matrix->size; i++) {
        matrix->data[i] = malloc(sizeof(unsigned int) * userSize);
        for (size_t j = 0; j < matrix->size; j++) {
            matrix->data[i][j] = INFINITY;
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
        // индекс = номер вершины - 1
        begin--;
        end--;
        matrix->data[begin][end] = (unsigned int) currentWeight;
        matrix->data[end][begin] = (unsigned int) currentWeight;
    }
    *controlValueOfInputVertexes = checkForCorrectData;

    return matrix;
}

TDistToVertex *initMinDistArray(const unsigned int numOfVertices) {
    TDistToVertex *minDist = malloc(sizeof(TDistToVertex) * numOfVertices);
    for (unsigned int i = 0; i < numOfVertices; i++) {
        minDist[i].vertexNum = INFINITY;
        minDist[i].minEdgeWeight = INFINITY;
    }
    return minDist;
}

unsigned int getNextVertexWithMinEdgeWeight(const size_t matrixSize, const bool *isVisited,
                                            const TDistToVertex *minDists, bool *isThereNextEdge) {
    unsigned int nextVertexWithMinEdgeWeight = 0;
    for (size_t columnIdx = 0; columnIdx < matrixSize; columnIdx++) {
        if (minDists[columnIdx].minEdgeWeight < minDists[nextVertexWithMinEdgeWeight].minEdgeWeight &&
            !isVisited[columnIdx]) {
            *isThereNextEdge = true;
            nextVertexWithMinEdgeWeight = columnIdx;
        }
    }
    return nextVertexWithMinEdgeWeight;
}

void updateMinDestinationsToUnvisitedVertices(const TMatrix *matrix, const unsigned int newMSTVertex,
                                              const bool *isVisited, TDistToVertex *minDists) {
    for (size_t columnIdx = 0; columnIdx < matrix->size; columnIdx++) {
        if (matrix->data[newMSTVertex][columnIdx] < minDists[columnIdx].minEdgeWeight && !isVisited[columnIdx]) {
            minDists[columnIdx].vertexNum = newMSTVertex;
            minDists[columnIdx].minEdgeWeight = matrix->data[newMSTVertex][columnIdx];
        }
    }
}

TDistToVertex *findMinimumSpanningTree(const unsigned int numberOfVertices, workingResult *status, TMatrix *matrix) {
    bool *isVisited = malloc(sizeof(bool) * numberOfVertices);
    memset(isVisited, false, sizeof(bool) * numberOfVertices);
    TDistToVertex *minDists = initMinDistArray(numberOfVertices);
    unsigned int newVertexInMSTree = 0;
    // Начинаем с дерева из одной вершины.
    isVisited[0] = true;
    updateMinDestinationsToUnvisitedVertices(matrix, newVertexInMSTree, isVisited, minDists);
    bool isThereNextEdge = false;
    unsigned int nextVertexWithMinEdgeWeight = getNextVertexWithMinEdgeWeight(matrix->size, isVisited, minDists,
                                                                              &isThereNextEdge);
    for (size_t strIdx = 0; strIdx < matrix->size - 1; strIdx++) {
        if (!isThereNextEdge) {
            *status = noSpanningTree;
            break;
        }
        isVisited[nextVertexWithMinEdgeWeight] = true;
        newVertexInMSTree = nextVertexWithMinEdgeWeight;
        updateMinDestinationsToUnvisitedVertices(matrix, newVertexInMSTree, isVisited, minDists);
        isThereNextEdge = false;
        nextVertexWithMinEdgeWeight = getNextVertexWithMinEdgeWeight(matrix->size, isVisited, minDists,
                                                                     &isThereNextEdge);
    }
    free(isVisited);
    return minDists;
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

void printMinimumSpanningTree(const int numberOfVertices, TDistToVertex *minDists) {
    for (int i = 0; i < numberOfVertices; i++) {
        if (minDists[i].minEdgeWeight != INFINITY) {
            printf("%d %u\n", i + 1, minDists[i].vertexNum + 1);
        }
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
    TDistToVertex *minDists = NULL;
    if (status == success) {
        matrix = inputEdges(numberOfVertices, numberOfEdges, &status);
    }
    if (status == success) {
        if (numberOfVertices == 0 || (numberOfEdges == 0 && numberOfVertices != 1)) {
            status = noSpanningTree;
        }
    }
    if (status == success) {
        minDists = findMinimumSpanningTree(numberOfVertices, &status, matrix);
    }
    if (status == success) {
        printMinimumSpanningTree(numberOfVertices, minDists);
    } else {
        printMessageByStatus(status);
    }
    if (matrix != NULL) {
        freeMatrix(matrix);
    }
    if (minDists != NULL) {
        free(minDists);
    }
    return EXIT_SUCCESS;
}
