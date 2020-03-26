#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define VERTEX_WAS_CHECKED -1
#define MAX_VERTEX_NUM 1000

typedef enum checkData {
    badVertex,
    success,
    noInput,
    impossibleToSort,
    badNumOfLines,
    badNumOfVertices,
    badNumOfEdges
} resultOfWorking;

typedef struct edge {
    int begin;
    int end;
} edge;

int checkNumberOfEdges(const int numberOfEdges, const int numberOfVertices) {
    return (numberOfEdges <= numberOfVertices * (numberOfVertices + 1) / 2);
}

int *initColumnsArray(const int numberOfVertices, const int numberOfEdges, const edge *listOfEdges) {
    int *columns = malloc(sizeof(int) * numberOfVertices);
    memset(columns, 0, numberOfVertices * sizeof(int));
    for (int idx = 0; idx < numberOfEdges; ++idx) {
        columns[listOfEdges[idx].end - 1]++;
    }
    return columns;
}

int *topologicalSort(const edge *vertexes, const int numberOfVertices, const int numberOfEdges, const edge *listOfEdges,
                     resultOfWorking *status) {
    int *columns = initColumnsArray(numberOfVertices, numberOfEdges, listOfEdges);
    int *answerList = malloc(sizeof(int) * numberOfVertices);
    int answersIdx = 0;
    while (answersIdx < numberOfVertices) {
        int columnsIdx = 0;
        int workingIdx = -1;
        while (columnsIdx < numberOfVertices) {
            if (columns[columnsIdx] == 0) {
                workingIdx = columnsIdx;
                break;
            }
            columnsIdx++;
            if (columnsIdx == numberOfVertices) {
                *status = impossibleToSort;
                free(columns);
                return answerList;
            }
        }
        answerList[answersIdx] = workingIdx + 1;
        columns[columnsIdx] = VERTEX_WAS_CHECKED;
        for (int idx = 0; idx < numberOfEdges; ++idx) {
            if (vertexes[idx].begin == workingIdx + 1) {
                columns[vertexes[idx].end - 1]--;
            }
        }
        answersIdx++;
    }
    free(columns);
    return answerList;
}

void printAnswerList(const int *answerList, const int numberOfVertices) {
    for (int idx = 0; idx < numberOfVertices; ++idx) {
        printf("%d ", answerList[idx]);
    }
}

int inputInitialValues(int *numberOfVertices, int *numberOfEdges) {
    int haveNextInput = 0;
    if (!scanf("%d", numberOfVertices)) {
        fprintf(stderr, "No input!");
    }
    haveNextInput = scanf("%d", numberOfEdges);
    return haveNextInput == 1;
}

int isBetweenBoundaries(const int checkingVertex, const int leftBoundary, const int rightBoundary) {
    return (checkingVertex >= leftBoundary && checkingVertex <= rightBoundary);
}

void printIOException() {
    printf("There is no input. Check your file to data");
}

edge *inputEdges(const int numberOfVertices, const int numberOfEdges, resultOfWorking *controlValueOfInputVertexes) {
    edge *listOfEdges = malloc(sizeof(edge) * numberOfEdges);
    resultOfWorking checkForCorrectData = success;
    for (int idx = 0; idx < numberOfEdges; ++idx) {
        edge currentVertexes;
        int haveNextInput = 0;
        if (!scanf("%d", &currentVertexes.begin)) {
            printIOException();
        }
        haveNextInput = scanf("%d", &currentVertexes.end);
        // если вводится число - то haveNextInput обязан равняться единице
        if (haveNextInput != 1) {
            *controlValueOfInputVertexes = noInput;
            return listOfEdges;
        }
        if (!(isBetweenBoundaries(currentVertexes.begin, 1, numberOfVertices) &&
              isBetweenBoundaries(currentVertexes.end, 1, numberOfVertices))) {
            checkForCorrectData = badVertex;
            continue;
        }
        listOfEdges[idx] = currentVertexes;
    }
    *controlValueOfInputVertexes = checkForCorrectData;
    return listOfEdges;
}

void freeMemory(edge *listOfEdges, int *answerList) {
    free(listOfEdges);
    free(answerList);
}

void printMessageByStatus(const resultOfWorking status) {
    switch (status) {
        case badNumOfLines:
            printf("bad number of lines");
            break;
        case badNumOfEdges:
            printf("bad number of edges");
            break;
        case badNumOfVertices:
            printf("bad number of vertices");
            break;
        case noInput:
            printf("bad number of lines");
            break;
        case badVertex:
            printf("bad vertex");
            break;
        case impossibleToSort:
            printf("impossible to sort");
            break;
        default:
            break;
    }
}

int main(void) {
    int numberOfVertices;
    int numberOfEdges;
    resultOfWorking status = success;
    if (!inputInitialValues(&numberOfVertices, &numberOfEdges)) {
        status = badNumOfLines;
    } else if (!isBetweenBoundaries(numberOfVertices, 0, MAX_VERTEX_NUM)) {
        status = badNumOfVertices;
    } else if (!checkNumberOfEdges(numberOfEdges, numberOfVertices)) {
        status = badNumOfEdges;
    }
    edge *listOfEdges = NULL;
    int *answerList = NULL;
    if (status == success) {
        listOfEdges = inputEdges(numberOfVertices, numberOfEdges, &status);
    }
    if (status == success) {
        answerList = topologicalSort(listOfEdges, numberOfVertices, numberOfEdges, listOfEdges, &status);
    }
    if (status == success) {
        printAnswerList(answerList, numberOfVertices);
    } else {
        printMessageByStatus(status);
    }
    freeMemory(listOfEdges, answerList);
    return EXIT_SUCCESS;
}
