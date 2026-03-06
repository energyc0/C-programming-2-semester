#include "AVLTree.h"
#include <stdlib.h>
#include "AVLTreeInternal.h"

static AVLNode* avlNodeAlloc(void* key, void* value);
static void avlNodesFree(AVLNode* node);

static bool avlInsertInternal(AVLNode* node, void* key, void* value, Comparator comp);
static AVLNode* avlFindInternal(AVLNode* node, void* key, Comparator comp);
static void avlInorderInternal(AVLNode* node, void (*printVal)(void*));

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
    if (tree->root == NULL) {
        tree->root = avlNodeAlloc(key, value);
        return true;
    }
    return avlInsertInternal(tree->root, key, value, tree->comp);
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

static bool avlInsertInternal(AVLNode* node, void* key, void* value, Comparator comp)
{
    int compRes = comp(node->key, key);
    if (compRes == 0) {
        node->value = value;
        return true;
    } else if (compRes > 0) {
        if (node->left == NULL) {
            node->left = avlNodeAlloc(key, value);
            return node->left != NULL;
        }
        return avlInsertInternal(node->left, key, value, comp);
    } else {
        if (node->right == NULL) {
            node->right = avlNodeAlloc(key, value);
            return node->right != NULL;
        }
        return avlInsertInternal(node->right, key, value, comp);
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

static void avlInorderInternal(AVLNode* node, void (*printVal)(void*))
{
    if (node->left != NULL)
        avlInorderInternal(node->left, printVal);

    printVal(node->value);

    if (node->right != NULL)
        avlInorderInternal(node->right, printVal);
}

void avlInorder(AVLTree* tree, void (*printVal) (void*))
{
    if(tree != NULL && tree->root != NULL)
        avlInorderInternal(tree->root, printVal);
}

