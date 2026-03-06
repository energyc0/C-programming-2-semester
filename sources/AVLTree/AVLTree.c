#include "AVLTree.h"
#include <stdlib.h>
#include "AVLTreeInternal.h"

static AVLNode* avlNodeAlloc(void* key, void* value);
static void avlNodesFree(AVLNode* node);

static bool avlInsertInternal(AVLNode* node, void* key, void* value, Comparator comp, bool* hasNew);
static AVLNode* avlFindInternal(AVLNode* node, void* key, Comparator comp);

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

bool avlInsert(AVLTree* tree, void* key, void* value)
{
    bool hasNew = false;
    bool res = false;
    if (tree->root == NULL) {
        tree->root = avlNodeAlloc(key, value);
        hasNew = true;
        res = true;
    } else {
        res = avlInsertInternal(tree->root, key, value, tree->comp, &hasNew);
    }

    if (hasNew)
        tree->nodes++;
        
    return res;
}

void* avlFind(AVLTree* tree, void* key, bool* isFound)
{
    if (tree == NULL || tree->root == NULL) {
        *isFound = false;
        return NULL;
    }
    AVLNode* node = avlFindInternal(tree->root, key, tree->comp);
    if (node == NULL) {
        *isFound = false;
        return NULL;
    }
    *isFound = true;
    return node->value;
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

static bool avlInsertInternal(AVLNode* node, void* key, void* value, Comparator comp, bool* hasNew)
{
    int compRes = comp(node->key, key);
    if (compRes == 0) {
        node->value = value;
        *hasNew = false;
        return true;
    } else if (compRes > 0) {
        if (node->left == NULL) {
            node->left = avlNodeAlloc(key, value);
            *hasNew = node->left != NULL;
            return *hasNew;
        }
        return avlInsertInternal(node->left, key, value, comp, hasNew);
    } else {
        if (node->right == NULL) {
            node->right = avlNodeAlloc(key, value);
            *hasNew = node->right != NULL;
            return *hasNew;
        }
        return avlInsertInternal(node->right, key, value, comp, hasNew);
    }
}

static AVLNode* avlFindInternal(AVLNode* node, void* key, Comparator comp)
{
    int compRes = comp(node->key, key);
    if (compRes == 0) {
        return node;
    } else if (compRes > 0) {
        if (node->left == NULL)
            return NULL;
        return avlFindInternal(node->left, key, comp);
    } else {
        if (node->right == NULL)
            return NULL;
        return avlFindInternal(node->right, key, comp);
    }
}
