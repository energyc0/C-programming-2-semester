#include "AVLTree.h"
#include "AVLTreeInternal.h"
#include <stdlib.h>

struct InsertData {
    void* key;
    void* value;
    Comparator comp;
    bool hasNew;
    bool hasError;
    bool hasIncHeight;
};

static AVLNode* avlNodeAlloc(void* key, void* value);
static void avlNodesFree(AVLNode* node);

static AVLNode* avlInsertInternal(AVLNode* node, struct InsertData* data);
static AVLNode* avlFindInternal(AVLNode* node, void* key, Comparator comp);

static AVLNode* avlNodeBalance(AVLNode* node, struct InsertData* data);
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
    tree->nodes = 0;
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
    struct InsertData data = {
        .key = key,
        .value = value,
        .comp = tree->comp,
        .hasNew = false,
        .hasError = false,
        .hasIncHeight = false,
    };
    tree->root = avlInsertInternal(tree->root, &data);

    if (data.hasNew)
        tree->nodes++;

    return !data.hasError;
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

static AVLNode* avlInsertInternal(AVLNode* node, struct InsertData* data)
{
    if (node == NULL) {
        AVLNode* newNode = avlNodeAlloc(data->key, data->value);
        data->hasNew = newNode != NULL;
        data->hasError = !data->hasNew;
        data->hasIncHeight = data->hasNew;
        return newNode;
    }

    int compRes = data->comp(node->key, data->key);
    if (compRes == 0) {
        data->hasError = false;
        data->hasNew = false;
        data->hasIncHeight = false;
        return node;
    }
    if (compRes > 0) {
        node->left = avlInsertInternal(node->left, data);
        node->balance = data->hasIncHeight ? node->balance - 1 : node->balance;
    } else {
        node->right = avlInsertInternal(node->right, data);
        node->balance = data->hasIncHeight ? node->balance + 1 : node->balance;
    }

    return avlNodeBalance(node, data);
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

static AVLNode* avlNodeBalance(AVLNode* node, struct InsertData* data)
{
    /* When inserted node has a sibling. */
    if (node->balance == 0) {
        data->hasIncHeight = false;
    } else {
        if (node->balance == 2) {
            data->hasIncHeight = false;
            if (node->right->balance >= 0)
                return avlNodeRotateLeft(node);
            return avlNodeRotateRightLeft(node);
        }
        if (node->balance == -2) {
            data->hasIncHeight = false;
            if (node->left->balance <= 0)
                return avlNodeRotateRight(node);
            return avlNodeRotateLeftRight(node);
        }
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
    } else { /* newRoot->balance == 1 */
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
    } else { /* newRoot->balance == -1 */
        newRoot->balance = 0;
        node->balance = 0;
    }

    return newRoot;
}
static AVLNode* avlNodeRotateRightLeft(AVLNode* node)
{
    AVLNode* X = node;
    AVLNode* Z = node->right;
    AVLNode* Y = node->right->left;

    Z->left = Y->right;
    Y->right = Z;
    X->right = Y->left;
    Y->left = X;

    if (Y->balance == 0) {
        X->balance = 0;
        Z->balance = 0;
    } else if (Y->balance == 1) {
        X->balance = -1;
        Z->balance = 0;
    } else { /* Y->balance == -1 */
        X->balance = 0;
        Z->balance = 1;
    }

    return Y;
}
static AVLNode* avlNodeRotateLeftRight(AVLNode* node)
{
    AVLNode* X = node;
    AVLNode* Z = node->left;
    AVLNode* Y = node->left->right;

    Z->right = Y->left;
    Y->left = Z;
    X->left = Y->right;
    Y->right = X;

    if (Y->balance == 0) {
        X->balance = 0;
        Z->balance = 0;
    } else if (Y->balance == 1) {
        X->balance = 0;
        Z->balance = -1;
    } else { /* Y->balance == -1 */
        X->balance = 1;
        Z->balance = 0;
    }

    return Y;
}
