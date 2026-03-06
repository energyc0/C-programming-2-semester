#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "AVLTree.h"
#include "AVLTreeInternal.h"

int intCompare(void* a, void* b)
{
    int* aVal = a;
    int* bVal = b;
    if (*aVal == *bVal)
        return 0;
    if (*aVal > *bVal)
        return 1;
    return -1;
}

void printStr(void* p)
{
    printf("%s", (char*)p);
}

int main()
{
    int treeKeys[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    char* treeValues[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};
    AVLTree* tree = avlAlloc(intCompare);
    assert(tree != NULL);
    for (int i = 0; i < sizeof(treeKeys) / sizeof(treeKeys[0]); i++) {
        assert(avlInsert(tree, &treeKeys[i], treeValues[i]));
        printf("###\nInserted (%d, %s)\n", treeKeys[i], treeValues[i]);
        avlInorder(tree, printStr);
    }

    for (int i = 0; i < sizeof(treeKeys) / sizeof(treeKeys[0]); i++) {
        assert(avlContains(tree, &treeKeys[i]));
        bool hasFound = false;
        char* data = avlFind(tree, &treeKeys[i], &hasFound);
        assert(data != NULL);
        assert(hasFound);   
        assert(strcmp(data, treeValues[i]) == 0);

    }
    assert(avlIsMetadataCorrect(tree));
    assert(avlIsBalanced(tree));
    avlFree(&tree);
    assert(tree == NULL);
    printf("Good!\n");
    return 0;
}