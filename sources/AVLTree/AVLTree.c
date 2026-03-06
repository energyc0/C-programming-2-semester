#include "AVLTree.h"
#include <stdlib.h>
#include "AVLTreeInternal.h"

static AVLNode* avlNodeAlloc(void* key, void* value);
static void avlNodesFree(AVLNode* node);

static bool avlInsertInternal(AVLNode* node, void* key, void* value, Comparator comp, bool* hasNew);
static AVLNode* avlFindInternal(AVLNode* node, void* key, Comparator comp);

static AVLNode* avlNodeBalance(AVLNode* node);
static AVLNode* avlNodeRotateLeft(AVLNode* node);
static AVLNode* avlNodeRotateRight(AVLNode* node);
static AVLNode* avlNodeRotateRightLeft(AVLNode* node);
static AVLNode* avlNodeRotateLeftRight(AVLNode* node);

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
    bool res = false;
    if (compRes == 0) {
        node->value = value;
        *hasNew = false;
        res = true;
    } else if (compRes > 0) {
        if (node->left == NULL) {
            node->left = avlNodeAlloc(key, value);
            *hasNew = node->left != NULL;
            node->balance = *hasNew ? node->balance - 1 : node->balance;
            res = *hasNew;
        } else {
            res = avlInsertInternal(node->left, key, value, comp, hasNew);
            if (*hasNew) {
                node->balance--;
                node->left = avlNodeBalance(node->left);
            }
        }
    } else {
        if (node->right == NULL) {
            node->right = avlNodeAlloc(key, value);
            *hasNew = node->right != NULL;
            node->balance = *hasNew ? node->balance + 1 : node->balance;
            res = *hasNew;
        } else {
            res = avlInsertInternal(node->right, key, value, comp, hasNew);
            if (*hasNew) {
                node->balance++;
                node->right = avlNodeBalance(node->right);
            }
        }
    }

    return res;
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

static AVLNode* avlNodeBalance(AVLNode* node)
{
    if (node->balance == 2) {
        if (node->right->balance >= 0)
            return avlNodeRotateLeft(node);
        return avlNodeRotateRightLeft(node);
    }
    if (node->balance == -2) {
        if (node->left->balance <= 0)
            return avlNodeRotateRight(node);
        return avlNodeRotateLeftRight(node);
    }
    return node;
}

static AVLNode* avlNodeRotateLeft(AVLNode* node)
{
    AVLNode* center = node->right->left;
    AVLNode* newRoot = node->right;
    node->right = center;
    newRoot->left = node;

    if (newRoot->balance == 0) {
        newRoot->balance = -1;
        node->balance = 1;
    } else {
        newRoot->balance = 0;
        node->balance = 0;
    }

    return newRoot;
}
static AVLNode* avlNodeRotateRight(AVLNode* node)
{
    AVLNode* center = node->left->right;
    AVLNode* newRoot = node->left;
    node->left = center;
    newRoot->right = node;

    if (newRoot->balance == 0) {
        newRoot->balance = 1;
        node->balance = -1;
    } else {
        newRoot->balance = 0;
        node->balance = 0;
    }

    return newRoot;
}
static AVLNode* avlNodeRotateRightLeft(AVLNode* node)
{
    node = avlNodeRotateRight(node);
    return avlNodeRotateLeft(node);
}
static AVLNode* avlNodeRotateLeftRight(AVLNode* node)
{
    node = avlNodeRotateLeft(node);
    return avlNodeRotateRight(node);
}
