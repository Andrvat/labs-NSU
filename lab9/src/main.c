#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_VERTICES_SIZE 5000

/**
 * @brief Перечисляемый тип данных для контроля состояния выполненя программы
 * Каждое значение соответствует ровному своему названию
 */

typedef enum workingResult {
    badWeight,
    badVertex,
    success,
    noPath,
    overflow,
    pathCorrect,
    badNumOfLines,
    badNumOfVertices,
    badNumOfEdges
} workingResult;

/**
 * @struct Матрица смежности
 */

typedef struct adjacencyMatrix {
    size_t size;
    unsigned int **data;
} TMatrix;

/**
 * @struct Массив кратчайших расстояний до вершин
 */

typedef struct minDist {
    size_t size;
    long long int *data;
} TMinDist;

/**
 * @struct Простой аналог вектора из С++
 */

typedef struct intVector {
    size_t capacity;
    size_t size;
    int *vector;
} TVector;

/**
 * @brief Вывод сообщения об ошибке при сбое в чтении/записи файла
 * Добавлю, что нигде в обычных условиях не будет вызвана
 * Нужна для гитлаба - занять проверки fread(), scanf() и т.д.
 */

void printIOException() {
    printf("There is no input. Check your file to data");
}

/**
 * @brief Проверка на корректность числа вершин
 * См. условие
 */

int checkNumberOfVertices(const int numberOfVertices) {
    return numberOfVertices >= 0 && numberOfVertices <= MAX_VERTICES_SIZE;
}

/**
 * @brief Проверка на корректность числа ребер
 * См. условие
 */

int checkNumberOfEdges(const int numberOfEdges, const int numberOfVertices) {
    return numberOfEdges >= 0 && numberOfEdges <= numberOfVertices * (numberOfVertices + 1) / 2;
}

/**
 * @brief Создания и инициализация матрицы смежности
 * Изначально заполняем нулями с помощью memset()
 */

TMatrix *initMatrix(const int userSize) {
    TMatrix *matrix = malloc(sizeof(TMatrix));
    matrix->size = userSize;
    matrix->data = malloc(userSize * sizeof(int *));
    for (size_t i = 0; i < matrix->size; i++) {
        matrix->data[i] = malloc(sizeof(int) * userSize);
        memset(matrix->data[i], 0, userSize * sizeof(int));
    }
    return matrix;
}

int isBetweenBoundaries(const long long int checkingNumber, const int leftBoundary, const int rightBoundary) {
    return checkingNumber >= leftBoundary && checkingNumber <= rightBoundary;
}

void resizeVector(TVector *vector, size_t newSize) {
    if (newSize <= vector->capacity) {
        vector->size = newSize;
    } else {
        assert("Increasing vector size is not supported for now");
    }
}

/**
 * @brief Инициализация массива расстояний до каждой из вершин
 * Изначально заполняем значем long_long_max из limits.h
 * В начальной вершине ставим 0
 * См. реализацию алгоритма
 */

TMinDist *initMinDistancesArray(const int start, const int userSize) {
    TMinDist *minDist = malloc(sizeof(TMinDist));
    minDist->data = malloc(sizeof(long long int) * userSize);
    minDist->size = userSize;
    for (int i = 0; i < userSize; i++) {
        minDist->data[i] = __LONG_LONG_MAX__;
    }
    minDist->data[start - 1] = 0;
    return minDist;
}

void freeMatrix(TMatrix *matrix) {
    for (size_t i = 0; i < matrix->size; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

/**
 * @brief Освобождение используемой программой памяти
 */

void freeMemory(TMinDist *minDistances, TVector *path, TMatrix *matrix) {
    if (matrix != NULL) {
        freeMatrix(matrix);
    }
    if (minDistances != NULL) {
        free(minDistances->data);
        free(minDistances);
    }
    if (path != NULL) {
        free(path->vector);
        free(path);
    }

}

int inputInitialValues(int *numberOfVertices, int *start, int *finish, int *numberOfEdges) {
    int haveNextInput = scanf("%d%d%d%d", numberOfVertices, start, finish, numberOfEdges);
    return haveNextInput || *start < 1 || *numberOfVertices < *start;
}

/**
 * @brief Ввод данных вида вершина_1 - вершина_2 - вес
 * Параллельно проводится проверка на допустимые границы
 * См. условие
 */

TMatrix *inputEdges(const int numberOfEdges, const int numberOfVertices, workingResult *controlForCorrectInput) {
    TMatrix *matrix = initMatrix(numberOfVertices);
    if (numberOfVertices == 0) {
        *controlForCorrectInput = badVertex;
    }
    for (long long int i = 0; i < numberOfEdges; i++) {
        int begin, end;
        long long int weight;
        if (!scanf("%d%d%lld", &begin, &end, &weight)) {
            printIOException();
        }
        if (!isBetweenBoundaries(begin, 1, numberOfVertices)
            || !isBetweenBoundaries(end, 1, numberOfVertices)) {
            *controlForCorrectInput = badVertex;
            continue;
        }
        if (!isBetweenBoundaries(weight, 0, INT_MAX)) {
            *controlForCorrectInput = badWeight;
            continue;
        }
        // индекс = номер вершины - 1
        begin--;
        end--;
        matrix->data[begin][end] = (unsigned int) weight;
        matrix->data[end][begin] = (unsigned int) weight;
    }
    return matrix;
}

/**
 * @brief Нахождение кратчайших путей до каждой из вершин
 * Как это делается - см. реализацию алгоритма Дейкстры
 */

TMinDist *findAllShortestPath(const int start, TMatrix *matrix) {
    TMinDist *minDistances = initMinDistancesArray(start, matrix->size);
    // массив контроля посещенных вершин
    bool *verticesToVisit = malloc(sizeof(bool) * minDistances->size);
    // заполняем его
    memset(verticesToVisit, true, minDistances->size * sizeof(bool));
    int minimumIdx;
    do {
        minimumIdx = MAX_VERTICES_SIZE;
        long long int minValueOfPath = __LONG_LONG_MAX__;
        for (size_t i = 0; i < minDistances->size; i++) {
            if ((verticesToVisit[i] == true) && (minDistances->data[i] < minValueOfPath)) {
                minValueOfPath = minDistances->data[i];
                minimumIdx = i;
            }
        }
        if (minimumIdx != MAX_VERTICES_SIZE) {
            for (size_t i = 0; i < minDistances->size; i++) {
                long long int weightOfCurrentEdge = matrix->data[minimumIdx][i];
                if (weightOfCurrentEdge > 0) {
                    long long int temp = minValueOfPath + weightOfCurrentEdge;
                    if (temp < minDistances->data[i]) {
                        minDistances->data[i] = temp;
                    }
                }
            }
            verticesToVisit[minimumIdx] = false;
        }
    } while (minimumIdx < MAX_VERTICES_SIZE);
    free(verticesToVisit);

    return minDistances;
}

void printPathsToAllVertices(TMinDist *minDistances) {
    for (size_t i = 0; i < minDistances->size; i++) {
        if (minDistances->data[i] == __LONG_LONG_MAX__) {
            printf("oo ");
        } else if (minDistances->data[i] > INT_MAX) {
            printf("INT_MAX+ ");
        } else {
            printf("%lld ", minDistances->data[i]);
        }
    }
    printf("\n");
}

TVector *createVector(const int userSize) {
    TVector *initPath = malloc(sizeof(TVector));
    initPath->vector = malloc(sizeof(int) * userSize);
    initPath->size = userSize;
    initPath->capacity = userSize;
    return initPath;
}

/**
 * @brief Восстановление пути из заданной вершины до начальной
 * Как это делается - см. реализацию алгоритма Дейкстры
 */

TVector *getShortestPath(const int start, const int finish, TMinDist *minDistances, TMatrix *matrix,
                         workingResult *controlValueOfShortestPath) {
    TVector *path = createVector(matrix->size);
    int endOfPath = finish - 1;
    int pathIndex = 1;
    path->vector[0] = endOfPath + 1;
    long long int weight = minDistances->data[endOfPath];
    bool isThereNextPath = 0;
    bool isMoreThanOneBigPath = 0;
    while (endOfPath != start - 1) {
        for (size_t i = 0; i < path->size; i++) {
            if (matrix->data[endOfPath][i] != 0) {
                isThereNextPath = 1;
                long long int workingElement = matrix->data[endOfPath][i];
                long long int currentValueOfWeight = weight - workingElement;
                if (currentValueOfWeight == minDistances->data[i]) {
                    if (currentValueOfWeight == INT_MAX) {
                        for (size_t j = 0; j < path->size; j++) {
                            if (minDistances->data[endOfPath] - minDistances->data[j] == currentValueOfWeight &&
                                j != i) {
                                isMoreThanOneBigPath = 1;
                            }
                        }
                    }
                    weight = currentValueOfWeight;
                    endOfPath = i;
                    path->vector[pathIndex] = (int) i + 1;
                    pathIndex += 1;
                    break;
                }
            }
            if (i + 1 == path->size && !isThereNextPath) {
                // пути нет
                *controlValueOfShortestPath = noPath;
                return path;
            }
        }
    }
    if (isMoreThanOneBigPath) {
        *controlValueOfShortestPath = overflow;
    } else {
        *controlValueOfShortestPath = pathCorrect;
    }
    resizeVector(path, pathIndex);
    return path;
}

void printVector(TVector *path) {
    for (size_t i = 0; i < path->size; i++) {
        printf("%d ", path->vector[i]);
    }
}

bool isOverflow(const workingResult controlValueOfShortestPath, const TMinDist *minDistances,
                const int finish) {
    return controlValueOfShortestPath == overflow && minDistances->data[finish - 1] > INT_MAX;
}

void printMessageByStatus(const workingResult status) {
    switch (status) {
        case badNumOfLines:
            printf("bad number of lines");
            break;
        case badNumOfEdges:
            printf("bad number of edges");
            break;
        case badVertex:
            printf("bad vertex");
            break;
        case noPath:
            printf("no path");
            break;
        case overflow:
            printf("overflow");
            break;
        case badNumOfVertices:
            printf("bad number of vertices");
            break;
        case badWeight:
            printf("bad length");
            break;
        default:
            break;
    }
}


int main(void) {
    int numberOfVertices, numberOfEdges;
    int start, finish;
    workingResult status = success;
    if (!inputInitialValues(&numberOfVertices, &start, &finish, &numberOfEdges)) {
        status = badNumOfLines;
    } else if (!checkNumberOfVertices(numberOfVertices)) {
        status = badNumOfVertices;
    } else if (!checkNumberOfEdges(numberOfEdges, numberOfVertices)) {
        status = badNumOfEdges;
    }
    TMatrix *matrix = NULL;
    TMinDist *minDistances = NULL;
    TVector *path = NULL;
    if (status == success) {
        matrix = inputEdges(numberOfEdges, numberOfVertices, &status);
    }
    if (status == success) {
        minDistances = findAllShortestPath(start, matrix);
        printPathsToAllVertices(minDistances);
    }
    if (status == success) {
        path = getShortestPath(start, finish, minDistances, matrix, &status);
    }
    if (isOverflow(status, minDistances, finish)) {
        status = overflow;
    }
    if (status == pathCorrect) {
        printVector(path);
    } else {
        printMessageByStatus(status);
    }
    freeMemory(minDistances, path, matrix);
    return EXIT_SUCCESS;
}
