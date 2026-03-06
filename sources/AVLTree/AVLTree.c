#include <stdlib.h>
#include "AVLTree.h"

typedef struct AVLNode {
    void* key;
    void* value;
    struct AVLNode* left;
    struct AVLNode* right;
    int balance;
} AVLNode;

typedef struct AVLTree {
    Comparator comp;
    struct AVLNode* root;
} AVLTree;

static AVLNode* avlNodeAlloc(void* key, void* value);
static void avlNodesFree(AVLNode* node);

AVLTree* avlAlloc(Comparator comp)
{
    AVLTree* tree = malloc(sizeof(*tree));
    if (tree == NULL)
        return NULL;

    tree->comp = comp;
    tree->root = NULL;
    return tree;
}

void avlFree(AVLTree** tree)
{
    avlNodesFree((*tree)->root);
    free(*tree);
    *tree = NULL;
}

static AVLNode* avlNodeAlloc(void* key, void* value)
{
    AVLNode* node = malloc(sizeof(*node));
    if (node == NULL)
        return node;

    node->balance = 0;
    node->left = node->right = NULL;
    node->key = key;
    node->value = value;

    return node;
}

static void avlNodesFree(AVLNode* node)
{
    if (node == NULL)
        return;

    avlNodesFree(node->left);
    avlNodesFree(node->right);
    free(node);
}

bool avlInsert(AVLTree* tree, void* key, void* value)
{
    return false;
}

void* avlFind(AVLTree* tree, void* key, bool* isFound)
{
    return NULL;
}


void avlDelete(AVLTree* tree, void* key)
{

}

bool avlContains(AVLTree* tree, void* key)
{
    bool isFound = false;
    avlFind(tree, key, &isFound);
    return isFound;
}