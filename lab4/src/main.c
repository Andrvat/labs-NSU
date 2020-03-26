#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_SYMBOLS_SIZE 1001
#define EMPTY 0

typedef enum checkResult {
    error,
    success
} resultOfWorking;

enum arithmeticSymbol {
    number,
    arithmeticSign
};

struct TCalcToken {
    int value;
    enum arithmeticSymbol typeOfToken;
    char operationName;
};

struct TExpressionTokensForm {
    struct TCalcToken postfixTokensForm[MAX_SYMBOLS_SIZE];
    size_t sizeOfPostfixTokensForm;
};

struct TTokenStack {
    struct TCalcToken data;
    struct TTokenStack *previous;
};

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

void freeTokenStack(struct TTokenStack **pointer, size_t *stackSize) {
    while ((*pointer) != NULL) {
        popFromTokenStack(pointer, stackSize);
    }
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

int checkForForeignCharacters(const char symbol) {
    return strchr("()*/+-", symbol) != NULL;
}

resultOfWorking makeTokensFormString(const char *infixForm, struct TExpressionTokensForm *postfixForm) {
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
            currentToken.typeOfToken = number;
            currentToken.operationName = EMPTY;
            currentToken.value = currentNumber;
            postfixForm->postfixTokensForm[postfixFormIndex] = currentToken;
            postfixFormIndex++;
            // Считаем количество чисел
            counterOfNumbers++;
        } else {
            // Если не встречен доступный знак операции, то выдаем ERROR
            if (!checkForForeignCharacters(infixForm[idx])) {
                return error;
            }
            // если это символ операции, то добавляем его в блок
            currentToken.typeOfToken = arithmeticSign;
            currentToken.value = EMPTY;
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
        return error;
    }
    // Выражение некорректно, если на вход не была получена ни одна операция, но чисел было получено больше чем одно
    if (counterOfOperations == 0 && counterOfNumbers != 1) {
        return error;
    }
    return success;
}

// Проверка на наличие следующих конструкций: () и )(

int checkForEmptyBrackets(const struct TExpressionTokensForm *expressionTokensForm, const unsigned int currentIdx) {
    return (expressionTokensForm->postfixTokensForm[currentIdx].operationName == ')'
            && expressionTokensForm->postfixTokensForm[currentIdx - 1].operationName != '(') ||
           (expressionTokensForm->postfixTokensForm[currentIdx].operationName == '('
            && expressionTokensForm->postfixTokensForm[currentIdx - 1].operationName != ')');
}


void addTokenToArray(struct TExpressionTokensForm **polishNotation, const struct TCalcToken currentToken) {
    (*polishNotation)->postfixTokensForm[(*polishNotation)->sizeOfPostfixTokensForm] = currentToken;
    (*polishNotation)->sizeOfPostfixTokensForm += 1;
}

resultOfWorking makePostfixForm(const struct TExpressionTokensForm *expressionTokensForm,
                                struct TExpressionTokensForm *polishNotation) {
    polishNotation->sizeOfPostfixTokensForm = 0;
    struct TTokenStack *operationsStack = NULL;
    size_t sizeOfOperationsStack = 0;
    for (unsigned int idx = 0; idx < expressionTokensForm->sizeOfPostfixTokensForm; ++idx) {
        struct TCalcToken currentToken = expressionTokensForm->postfixTokensForm[idx];
        if (currentToken.typeOfToken == number) {
            addTokenToArray(&polishNotation, currentToken);
        } else if (currentToken.operationName == '(') {
            pushToTokenStack(&operationsStack, currentToken, &sizeOfOperationsStack);
        } else if (currentToken.operationName == ')') {
            // Если встретили закрывающую скобку и при этом ранее не было символов операций или '(' - выдаем ERROR
            if (!checkForEmptyBrackets(expressionTokensForm, idx)) {
                free(operationsStack);
                return error;
            }
            while (operationsStack->data.operationName != '(') {
                addTokenToArray(&polishNotation, operationsStack->data);
                popFromTokenStack(&operationsStack, &sizeOfOperationsStack);
                // Если в стеке не встретили открывающую скобку - выдаем ERROR
                if (checkForEmptyStack(sizeOfOperationsStack)) {
                    freeTokenStack(&operationsStack, &sizeOfOperationsStack);
                    return error;
                }
            }
            popFromTokenStack(&operationsStack, &sizeOfOperationsStack);
        } else {
            int priorityCurrentOperation = calcOperationPriority(currentToken.operationName);
            while (!checkForEmptyStack(sizeOfOperationsStack) &&
                   calcOperationPriority(operationsStack->data.operationName) >= priorityCurrentOperation) {
                addTokenToArray(&polishNotation, operationsStack->data);
                popFromTokenStack(&operationsStack, &sizeOfOperationsStack);
            }
            pushToTokenStack(&operationsStack, currentToken, &sizeOfOperationsStack);
        }
    }
    while (!checkForEmptyStack(sizeOfOperationsStack)) {
        // Выражение некорректно, если после обработки строки осталишь лишние открывающиеся скобки
        if (operationsStack->data.operationName == '(') {
            free(operationsStack);
            return error;
        }
        addTokenToArray(&polishNotation, operationsStack->data);
        popFromTokenStack(&operationsStack, &sizeOfOperationsStack);
    }
    freeTokenStack(&operationsStack, &sizeOfOperationsStack);
    return success;
}

int calculateValue(const struct TExpressionTokensForm *polishNotation, resultOfWorking *checkForRightExpression) {
    struct TTokenStack *numStack = NULL;
    size_t sizeOfNumStack = 0;
    for (unsigned int idx = 0; idx < polishNotation->sizeOfPostfixTokensForm; ++idx) {
        struct TCalcToken currentToken = polishNotation->postfixTokensForm[idx];
        if (currentToken.typeOfToken == number) {
            pushToTokenStack(&numStack, currentToken, &sizeOfNumStack);
        } else {
            int firstNum = numStack->data.value;
            popFromTokenStack(&numStack, &sizeOfNumStack);
            int secondNum = numStack->data.value;
            popFromTokenStack(&numStack, &sizeOfNumStack);
            struct TCalcToken resultsToken;
            resultsToken.typeOfToken = number;
            resultsToken.operationName = EMPTY;
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
                        freeTokenStack(&numStack, &sizeOfNumStack);
                        *checkForRightExpression = error;
                        return 1;
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
    *checkForRightExpression = success;
    return result;
}

int main(void) {
    char infixForm[MAX_SYMBOLS_SIZE];
    if (!scanf("%1000[^\r\n]", infixForm)) {
        printSyntaxError();
        return EXIT_SUCCESS;
    };
    struct TExpressionTokensForm expressionInTokensForm;
    resultOfWorking checkForRightExpression = makeTokensFormString(infixForm, &expressionInTokensForm);
    if (checkForRightExpression == error) {
        printSyntaxError();
        return EXIT_SUCCESS;
    }
    struct TExpressionTokensForm polishNotation;
    checkForRightExpression = makePostfixForm(&expressionInTokensForm, &polishNotation);
    if (checkForRightExpression == error) {
        printSyntaxError();
        return EXIT_SUCCESS;
    }
    int resultOfExpression = calculateValue(&polishNotation, &checkForRightExpression);
    if (checkForRightExpression == error) {
        printDivisionByZero();
    } else {
        printf("%d", resultOfExpression);
    }
    return EXIT_SUCCESS;
}
