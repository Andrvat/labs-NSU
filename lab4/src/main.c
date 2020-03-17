#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NOT_NUMBER 0
#define ITS_NUMBER 'n'
#define MAX_SIZE 1001
#define ERROR "err"
#define SUCCESS "ok"


/**
 * @struct Токен, хранящий два поля - значение и имя операции
 * @brief Возможны два случая:
 * 1) Токен - число: value = userData, operationName = ITS_NUMBER
 * 2) Токен - символ операции: value = NOT_NUMBER, operationName = userData
 */

struct TCalcToken {
    int value;
    char operationName;
};

/**
 * @struct Массив токенов, описанных выше
 */

struct TExpressionTokensForm {
    struct TCalcToken postfixTokensForm[MAX_SIZE];
    size_t sizeOfPostfixTokensForm;
};

/**
 * @struct Стэк из токенов, описанных ранее.
 * @brief Необходим для подсчета значения по польской записи
 * Для понимания советую обратиться к видео на youtube
 */

struct TTokenStack {
    struct TCalcToken data;
    struct TTokenStack *previous;
};

/**
 * @brief Получение приоритета операции в зависимости от поданного на вход символа
 *
 * @param operation
 * @return приоритет от 1 до 4
 */

int calcOperationPriority(const char operation) {
    switch (operation) {
        case '(':
            return 1;
        case ')':
            return 2;
        case '+':
            return 3;
        case '-':
            return 3;
        case '*':
            return 4;
        case '/':
            return 4;
        default:
            return 0;
    }
}

int getNextIntDigit(const char symbol) {
    return symbol - '0';
}

int isSymbolDigit(const char symbol) {
    return symbol >= '0' && symbol <= '9';
}

struct TTokenStack *createTokenStack(const struct TCalcToken newData, size_t *stackSize) {
    struct TTokenStack *newBlock = malloc(sizeof(struct TTokenStack));
    newBlock->previous = NULL;
    newBlock->data = newData;
    *stackSize = *stackSize + 1;
    return newBlock;
}

void pushToTokenStack(struct TTokenStack **pointer, const struct TCalcToken newData, size_t *stackSize) {
    struct TTokenStack *newBlock = malloc(sizeof(struct TTokenStack));
    newBlock->data = newData;
    *stackSize = *stackSize + 1;
    newBlock->previous = *pointer;
    *pointer = newBlock;
}

void popFromTokenStack(struct TTokenStack **pointer, size_t *stackSize) {
    struct TTokenStack *currentBlock = *pointer;
    *pointer = (*pointer)->previous;
    if (currentBlock != NULL) {
        free(currentBlock);
    }
    *stackSize = *stackSize - 1;
}

int checkForEmptyStack(const size_t stackSize) {
    return stackSize == 0;
}

void printDivisionByZero() {
    printf("division by zero");
}

void printSyntaxError() {
    printf("syntax error");
}

/**
 * @brief Под foreign characters понимаем все символы, которые
 * не могут использоваться в аримфетической записи выражения
 *
 * @param symbol
 * @return 1 или 0 в зависимости от того, допустим данный символ или нет
 */

int checkForForeignCharacters(const char symbol) {
    return symbol == '('
           || symbol == ')'
           || symbol == '*'
           || symbol == '/'
           || symbol == '+'
           || symbol == '-';
}

/**
 *
 * @brief Работу функции смотри по оставленным комментариям
 *
 * @param infixForm
 * @param postfixForm
 * @return SUCCESS в случае если перевод в набор токенов прошел успешно,
 * ERROR в противном случае
 */

char *makeTokensFormString(const char *infixForm, struct TExpressionTokensForm *postfixForm) {
    unsigned int postfixFormIndex = 0;
    size_t infixFormSize = strlen(infixForm);
    unsigned int idx = 0;
    unsigned int counterOfNumbers = 0;
    unsigned int counterOfOperations = 0;
    while (idx < infixFormSize) {
        struct TCalcToken currentToken;
        // Если встречена цифра, то проверяем далее наличие числа; добавляем в блок
        if (isSymbolDigit(infixForm[idx])) {
            int currentNumber = getNextIntDigit(infixForm[idx]);
            idx++;
            while (isSymbolDigit(infixForm[idx])) {
                currentNumber *= 10;
                currentNumber += getNextIntDigit(infixForm[idx]);
                idx++;
            }
            currentToken.operationName = ITS_NUMBER;
            currentToken.value = currentNumber;
            postfixForm->postfixTokensForm[postfixFormIndex] = currentToken;
            postfixFormIndex++;
            // Считаем количество чисел
            counterOfNumbers++;
        } else {
            // Если не встречен доступный знак операции, то выдаем ERROR
            if (!checkForForeignCharacters(infixForm[idx])) {
                return ERROR;
            }
            // если это символ операции, то добавляем его в блок
            currentToken.value = NOT_NUMBER;
            currentToken.operationName = infixForm[idx];
            postfixForm->postfixTokensForm[postfixFormIndex] = currentToken;
            postfixFormIndex++;
            // Считаем количество знаков операций, исключая скобки
            if (currentToken.operationName != ')' && currentToken.operationName != '(') {
                counterOfOperations++;
            }
            idx++;
        }
    }
    // счетчик количества данных блоков
    postfixForm->sizeOfPostfixTokensForm = postfixFormIndex;
    // Выражение некорректно, когда количество чисел != количеству операций + 1
    if (counterOfNumbers != counterOfOperations + 1) {
        return ERROR;
    }
    // Выражение некорректно, если на вход не была получена ни одна операция, но чисел было получено больше чем одно
    if (counterOfOperations == 0 && counterOfNumbers != 1) {
        return ERROR;
    }
    return SUCCESS;
}

/**
 * @brief Проверка на наличие следующих конструкций: () и )(
 *
 * @param expressionTokensForm
 * @param currentIdx
 * @return 1 или 0 в зависимости от того, встретилась ли () или )(
 */

int checkForEmptyBrackets(const struct TExpressionTokensForm *expressionTokensForm, const unsigned int currentIdx) {
    return (expressionTokensForm->postfixTokensForm[currentIdx].operationName == ')'
            && expressionTokensForm->postfixTokensForm[currentIdx - 1].operationName != '(') ||
           (expressionTokensForm->postfixTokensForm[currentIdx].operationName == '('
            && expressionTokensForm->postfixTokensForm[currentIdx - 1].operationName != ')');
}


/**
 *
 * @param expressionTokensForm
 * @param polishNotation
 * @return Возвращает SUCCESS в случае успешного перевода выражения в обратную польскую запись
 * Иначе - возвращает ERROR
 */

char *makePostfixForm(const struct TExpressionTokensForm *expressionTokensForm,
                      struct TExpressionTokensForm *polishNotation) {
    polishNotation->sizeOfPostfixTokensForm = 0;
    struct TTokenStack *operationsStack = NULL;
    size_t sizeOfOperationsStack = 0;
    for (unsigned int idx = 0; idx < expressionTokensForm->sizeOfPostfixTokensForm; ++idx) {
        struct TCalcToken currentToken = expressionTokensForm->postfixTokensForm[idx];
        if (currentToken.operationName == ITS_NUMBER) {
            polishNotation->postfixTokensForm[polishNotation->sizeOfPostfixTokensForm] = currentToken;
            ++polishNotation->sizeOfPostfixTokensForm;
        } else if (currentToken.operationName == '(') {
            if (checkForEmptyStack(sizeOfOperationsStack)) {
                operationsStack = createTokenStack(currentToken, &sizeOfOperationsStack);
            } else {
                pushToTokenStack(&operationsStack, currentToken, &sizeOfOperationsStack);
            }
        } else if (currentToken.operationName == ')') {
            // Если встретили закрывающую скобку и при этом ранее не было символов операций или '(' - выдаем ERROR
            if (!checkForEmptyBrackets(expressionTokensForm, idx)) {
                free(operationsStack);
                return ERROR;
            }
            while (operationsStack->data.operationName != '(') {
                polishNotation->postfixTokensForm[polishNotation->sizeOfPostfixTokensForm] = operationsStack->data;
                ++polishNotation->sizeOfPostfixTokensForm;
                popFromTokenStack(&operationsStack, &sizeOfOperationsStack);
                // Если в стеке не встретили открывающую скобку - выдаем ERROR
                if (checkForEmptyStack(sizeOfOperationsStack)) {
                    free(operationsStack);
                    return ERROR;
                }
            }
            popFromTokenStack(&operationsStack, &sizeOfOperationsStack);
        } else {
            int priorityCurrentOperation = calcOperationPriority(currentToken.operationName);
            while (!checkForEmptyStack(sizeOfOperationsStack) &&
                   calcOperationPriority(operationsStack->data.operationName) >= priorityCurrentOperation) {
                polishNotation->postfixTokensForm[polishNotation->sizeOfPostfixTokensForm] = operationsStack->data;
                ++polishNotation->sizeOfPostfixTokensForm;
                popFromTokenStack(&operationsStack, &sizeOfOperationsStack);
            }
            if (checkForEmptyStack(sizeOfOperationsStack)) {
                operationsStack = createTokenStack(currentToken, &sizeOfOperationsStack);
            } else {
                pushToTokenStack(&operationsStack, currentToken, &sizeOfOperationsStack);
            }
        }
    }
    while (!checkForEmptyStack(sizeOfOperationsStack)) {
        // Выражение некорректно, если после обработки строки осталишь лишние открывающиеся скобки
        if (operationsStack->data.operationName == '(') {
            free(operationsStack);
            return ERROR;
        }
        polishNotation->postfixTokensForm[polishNotation->sizeOfPostfixTokensForm] = operationsStack->data;
        ++polishNotation->sizeOfPostfixTokensForm;
        popFromTokenStack(&operationsStack, &sizeOfOperationsStack);
    }
    free(operationsStack);
    return SUCCESS;
}

/**
 *
 * @param polishNotation
 * @param checkForRightExpression
 * @return Возвращает -1 в случае деления на ноль, иначе - значение выражения
 */

int calculateValue(const struct TExpressionTokensForm *polishNotation, char **checkForRightExpression) {
    struct TTokenStack *numStack = NULL;
    size_t sizeOfNumStack = 0;
    for (unsigned int idx = 0; idx < polishNotation->sizeOfPostfixTokensForm; ++idx) {
        struct TCalcToken currentToken = polishNotation->postfixTokensForm[idx];
        if (currentToken.operationName == ITS_NUMBER) {
            if (checkForEmptyStack(sizeOfNumStack)) {
                numStack = createTokenStack(currentToken, &sizeOfNumStack);
            } else {
                pushToTokenStack(&numStack, currentToken, &sizeOfNumStack);
            }
        } else {
            int firstNum = numStack->data.value;
            popFromTokenStack(&numStack, &sizeOfNumStack);
            int secondNum = numStack->data.value;
            popFromTokenStack(&numStack, &sizeOfNumStack);
            struct TCalcToken resultsToken;
            resultsToken.operationName = ITS_NUMBER;
            switch (currentToken.operationName) {
                case '+':
                    resultsToken.value = secondNum + firstNum;
                    break;
                case '-':
                    resultsToken.value = secondNum - firstNum;
                    break;
                case '*':
                    resultsToken.value = secondNum * firstNum;
                    break;
                case '/':
                    // провека деления на ноль
                    if (firstNum == 0) {
                        free(numStack);
                        *checkForRightExpression = ERROR;
                        return -1;
                    }
                    resultsToken.value = secondNum / firstNum;
                    break;
                default:
                    resultsToken.value = 0;
            }
            pushToTokenStack(&numStack, resultsToken, &sizeOfNumStack);
        }
    }
    int result = numStack->data.value;
    free(numStack);
    *checkForRightExpression = SUCCESS;
    return result;
}

int isExpressionWithErrors(char *checker) {
    if (strcmp(checker, ERROR)) {
        return 0;
    } else {
        return 1;
    }
}

int main(void) {
    FILE *file = freopen("in.txt", "r", stdin);
    char infixForm[MAX_SIZE];
    if (!scanf("%1000[^\r\n]", infixForm)) {
        printSyntaxError();
        return EXIT_SUCCESS;
    };
    struct TExpressionTokensForm expressionInTokensForm;
    char *checkForRightExpression = makeTokensFormString(infixForm, &expressionInTokensForm);
    if (isExpressionWithErrors(checkForRightExpression)) {
        printSyntaxError();
        return EXIT_SUCCESS;
    }
    struct TExpressionTokensForm polishNotation;
    checkForRightExpression = makePostfixForm(&expressionInTokensForm, &polishNotation);
    if (isExpressionWithErrors(checkForRightExpression)) {
        printSyntaxError();
        return EXIT_SUCCESS;
    }
    int resultOfExpression = calculateValue(&polishNotation, &checkForRightExpression);
    if (isExpressionWithErrors(checkForRightExpression)) {
        printDivisionByZero();
    } else {
        printf("%d", resultOfExpression);
    }
    fclose(file);
    return EXIT_SUCCESS;
}

/**
 * @mainpage Лабораторная работа #4, НГУ, ФИТ, 1 курс
 * @author Андрей Валитов
 * @date 17.03.2020
 * @version 2.3
 *
 * @brief Калькулятор аримфетических выражений (без возведения в степень, взятия корня и т.д.)
 *
 * @attention Удален include(common_lab) из файла CMakeLists.txt. Для лаб необходимо его вернуть в конец файла.
 */
