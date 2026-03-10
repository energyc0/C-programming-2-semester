#include "AVLTree.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    AVLTree* tree = avlAlloc(NULL, NULL, NULL);
    avlFree(&tree);
    printf("Hello!\n");
    return 0;
}