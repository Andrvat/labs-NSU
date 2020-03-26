#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
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

typedef struct edge {
    short begin;
    short end;
    int weight;
    bool isInSpanningTree;
} TEdge;

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

int compareWeight(const void *a, const void *b) {
    return ((TEdge *) a)->weight - ((TEdge *) b)->weight;
}

TEdge *inputEdges(const int numberOfVertices, const int numberOfEdges,
                  workingResult *controlValueOfInputVertexes) {
    TEdge *listOfEdges = malloc(sizeof(TEdge) * numberOfEdges);
    workingResult checkForCorrectData = success;
    for (int idx = 0; idx < numberOfEdges; ++idx) {
        TEdge currentEdge;
        int haveNextInput = 0;
        if (!scanf("%hd%hd", &currentEdge.begin, &currentEdge.end)) {
            printIOException();
        }
        long long int currentWeight;
        haveNextInput = scanf("%lld", &currentWeight);
        // если вводится число - то haveNextInput обязан равняться единице
        if (haveNextInput != 1) {
            *controlValueOfInputVertexes = noInput;
            return listOfEdges;
        }
        if (!isBetweenBoundaries(currentWeight, 0, INT_MAX)) {
            checkForCorrectData = badLength;
            continue;
        }
        if (!(isBetweenBoundaries(currentEdge.begin, 1, numberOfVertices) &&
              isBetweenBoundaries(currentEdge.end, 1, numberOfVertices))) {
            checkForCorrectData = badVertex;
            continue;
        }
        currentEdge.weight = (int) currentWeight;
        currentEdge.isInSpanningTree = false;
        listOfEdges[idx] = currentEdge;
    }
    *controlValueOfInputVertexes = checkForCorrectData;
    return listOfEdges;
}

void printSpanningTree(const int numberOfEdges, TEdge *listOfEdges) {
    for (int i = 0; i < numberOfEdges; ++i) {
        if (listOfEdges[i].isInSpanningTree == true) {
            printf("%d %d\n", listOfEdges[i].begin, listOfEdges[i].end);
        }
    }
}

void makeSet(const int numberOfVertices, short *parents, short *rank) {
    // Каждая вершина в начале - дерево, где родитель - она сама
    // Ранг у дерево из одной вершины положим нуль
    for (int i = 0; i < numberOfVertices; i++) {
        parents[i] = (short) i;
        rank[i] = 0;
    }
}

short findSet(const short el, short *parents) {
    if (parents[el] == el) {
        return el;
    }
    // Эвристика: сжатие пути
    return parents[el] = findSet(parents[el], parents);
}

void setUnion(short begin, short end, short *parents, short *rank) {
    // Находим представителя множества - корень дерева
    begin = findSet(begin, parents);
    end = findSet(end, parents);
    // Дерево с меньшим рангом подвешиваем к дереву с большим рангом
    // Эвристика: объединение по рангу
    if (rank[begin] < rank[end]) {
        parents[begin] = end;
    } else {
        parents[end] = begin;
        if (rank[begin] == rank[end]) {
            rank[begin]++;
        }
    }
}

TEdge *findMinimumSpanningTree(const int numberOfEdges, const int numberOfVertices,
                               workingResult *status, TEdge *listOfEdges) {
    if (numberOfVertices == 0 || (numberOfEdges == 0 && numberOfVertices != 1)) {
        *status = noSpanningTree;
        return listOfEdges;
    }
    short *parents = malloc(sizeof(short) * numberOfVertices);
    short *rank = malloc(sizeof(short) * numberOfVertices);
    makeSet(numberOfVertices, parents, rank);
    int countOfVisitedVertices = 1;
    for (int idx = 0; idx < numberOfEdges; ++idx) {
        short begin = (short) (listOfEdges[idx].begin - 1);
        short end = (short) (listOfEdges[idx].end - 1);
        if (findSet(begin, parents) != findSet(end, parents)) {
            listOfEdges[idx].isInSpanningTree = true;
            countOfVisitedVertices++;
            setUnion(begin, end, parents, rank);
            if (countOfVisitedVertices == numberOfVertices) {
                break;
            }
        }
    }
    if (countOfVisitedVertices != numberOfVertices) {
        *status = noSpanningTree;
    }
    free(parents);
    free(rank);
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
    TEdge *listOfEdges = NULL;
    if (status == success) {
        listOfEdges = inputEdges(numberOfVertices, numberOfEdges, &status);
        qsort(listOfEdges, numberOfEdges, sizeof(TEdge), compareWeight);
    }
    if (status == success) {
        listOfEdges = findMinimumSpanningTree(numberOfEdges, numberOfVertices, &status, listOfEdges);
    }
    if (status == success) {
        printSpanningTree(numberOfEdges, listOfEdges);
    } else {
        printMessageByStatus(status);
    }
    if (listOfEdges != NULL) {
        free(listOfEdges);
    }
    return EXIT_SUCCESS;
}
