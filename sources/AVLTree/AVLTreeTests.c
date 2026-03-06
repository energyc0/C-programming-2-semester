#include <assert.h>
#include <stdlib.h>
#include "AVLTree.h"

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

int main()
{
    int treeKeys[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    AVLTree* tree = avlAlloc(intCompare);
    assert(tree != NULL);
    for (int i = 0; i < sizeof(treeKeys) / sizeof(treeKeys[0]); i++)
        assert(avlInsert(tree, &treeKeys[i], &treeKeys[i]));

    for (int i = 0; i < sizeof(treeKeys) / sizeof(treeKeys[0]); i++) {
        assert(avlContains(tree, &treeKeys[i]));
        bool hasFound = false;
        int* data = data = avlFind(tree, &treeKeys[i], &hasFound);
        assert(data != NULL);
        assert(hasFound);
        assert(*data == treeKeys[i]);
    }
    return 0;
}