#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define EMPTY_TREE_HEIGHT 0

typedef struct Tree {
    int vertexValue;
    unsigned int treeHeight;
    struct Tree *leftSubtree;
    struct Tree *rightSubtree;
} Tree;

bool isTreeEmpty(Tree *currentTree) {
    return currentTree == NULL;
}

unsigned int getMaxValue(const unsigned int a, const unsigned int b) {
    return a > b ? a : b;
}

unsigned int getTreeHeight(Tree *currentTree) {
    if (isTreeEmpty(currentTree)) {
        return EMPTY_TREE_HEIGHT;
    } else {
        return currentTree->treeHeight;
    }
}

void setNewHeight(Tree *currentTree) {
    unsigned int leftHeight = getTreeHeight(currentTree->leftSubtree);
    unsigned int rightHeight = getTreeHeight(currentTree->rightSubtree);
    currentTree->treeHeight = getMaxValue(leftHeight, rightHeight) + 1;
}

int getBalanceFactor(Tree *currentTree) {
    return (int) (getTreeHeight(currentTree->rightSubtree) - getTreeHeight(currentTree->leftSubtree));
}

Tree *doRightTreeRotation(Tree *currentTree) {
    Tree *auxiliaryTree = currentTree->leftSubtree;
    currentTree->leftSubtree = auxiliaryTree->rightSubtree;
    auxiliaryTree->rightSubtree = currentTree;
    setNewHeight(currentTree);
    setNewHeight(auxiliaryTree);
    return auxiliaryTree;
}

Tree *doLeftTreeRotation(Tree *currentTree) {
    Tree *auxiliaryTree = currentTree->rightSubtree;
    currentTree->rightSubtree = auxiliaryTree->leftSubtree;
    auxiliaryTree->leftSubtree = currentTree;
    setNewHeight(currentTree);
    setNewHeight(auxiliaryTree);
    return auxiliaryTree;
}

Tree *balanceAVLTree(Tree *currentTree) {
    setNewHeight(currentTree);
    if (getBalanceFactor(currentTree) == 2) {
        if (getBalanceFactor(currentTree->rightSubtree) < 0) {
            currentTree->rightSubtree = doRightTreeRotation(currentTree->rightSubtree);
        }
        currentTree = doLeftTreeRotation(currentTree);
    }
    if (getBalanceFactor(currentTree) == -2) {
        if (getBalanceFactor(currentTree->leftSubtree) > 0) {
            currentTree->leftSubtree = doLeftTreeRotation(currentTree->leftSubtree);
        }
        currentTree = doRightTreeRotation(currentTree);
    }
    return currentTree;
}

Tree *createNewLeaf(const int userValue) {
    Tree *newLeaf = malloc(sizeof(Tree));
    newLeaf->vertexValue = userValue;
    newLeaf->treeHeight = 1;
    newLeaf->leftSubtree = NULL;
    newLeaf->rightSubtree = NULL;
    return newLeaf;
}

Tree *addNewLeafToTree(const int userValue, Tree *currentTree) {
    if (isTreeEmpty(currentTree)) {
        currentTree = createNewLeaf(userValue);
    } else {
        if (userValue < currentTree->vertexValue) {
            currentTree->leftSubtree = addNewLeafToTree(userValue, currentTree->leftSubtree);
        } else {
            currentTree->rightSubtree = addNewLeafToTree(userValue, currentTree->rightSubtree);
        }
    }
    // Делаем балансировку дерева сразу после добавления нового листа
    return balanceAVLTree(currentTree);
}

// По аналогии с обходом дерева снизу вверх
void deleteTree(Tree *currentTree) {
    if (!isTreeEmpty(currentTree)) {
        deleteTree(currentTree->leftSubtree);
        deleteTree(currentTree->rightSubtree);
        free(currentTree);
    }
}

void printIOException() {
    printf("There is no input. Check your file to data");
}

int main(void) {
    FILE *inputStream = freopen("in.txt", "r", stdin);
    unsigned int quantityOfNumbers = 0;
    if (scanf("%u", &quantityOfNumbers) < 1) {
        printIOException();
        return EXIT_SUCCESS;
    }
    Tree *AVLTree = NULL;
    for (unsigned int i = 0; i < quantityOfNumbers; ++i) {
        int currentNumber;
        if (scanf("%d", &currentNumber) < 1) {
            printIOException();
            return EXIT_SUCCESS;
        }
        AVLTree = addNewLeafToTree(currentNumber, AVLTree);
    }
    printf("%u", getTreeHeight(AVLTree));
    deleteTree(AVLTree);
    fclose(inputStream);
    return EXIT_SUCCESS;
}
