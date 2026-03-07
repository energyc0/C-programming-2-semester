#include "AVLTreeInternal.h"
#include <stdlib.h>

static bool avlNodeIsBalanced(AVLNode* node);
static void avlInorderInternal(AVLNode* node, void (*printVal)(void*));
static int countNodes(AVLNode* node);
static int calcHeight(AVLNode* node);

void avlInorder(AVLTree* tree, void (*printVal)(void*))
{
    if (tree != NULL && tree->root != NULL)
        avlInorderInternal(tree->root, printVal);
}

bool avlIsBalanced(AVLTree* tree)
{
    if (tree == NULL)
        return true;
    return avlNodeIsBalanced(tree->root);
}

bool avlIsMetadataCorrect(AVLTree* tree)
{
    if (tree == NULL)
        return true;
    int children = countNodes(tree->root);
    return tree->nodes == children;
}

static void avlInorderInternal(AVLNode* node, void (*printVal)(void*))
{
    if (node->left != NULL)
        avlInorderInternal(node->left, printVal);

    printVal(node->value);
    printf(", balance = %d, height = %d\n", node->balance, calcHeight(node));
    if (node->right != NULL)
        avlInorderInternal(node->right, printVal);
}

static int countNodes(AVLNode* node)
{
    if (node == NULL)
        return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}

static bool avlNodeIsBalanced(AVLNode* node)
{
    if (node == NULL)
        return true;

    int leftChildren = calcHeight(node->left);
    int rightChildren = calcHeight(node->right);
    int diff = abs(rightChildren - leftChildren);
    return diff < 2 && avlNodeIsBalanced(node->left) && avlNodeIsBalanced(node->right);
}

static int calcHeight(AVLNode* node)
{
    if (node == NULL)
        return 0;
    int left = calcHeight(node->left);
    int right = calcHeight(node->right);
    return 1 + (left > right ? left : right);
}
