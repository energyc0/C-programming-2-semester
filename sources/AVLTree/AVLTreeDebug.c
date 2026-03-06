#include "AVLTreeInternal.h"

static bool avlNodeIsBalanced(AVLNode* node);
static void avlInorderInternal(AVLNode* node, void (*printVal)(void*));
static int countChildren(AVLNode* node);
static bool isMetadataCorrect(AVLNode* node);

void avlInorder(AVLTree* tree, void (*printVal) (void*))
{
    if(tree != NULL && tree->root != NULL)
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
    return tree == NULL || isMetadataCorrect(tree->root);
}

static void avlInorderInternal(AVLNode* node, void (*printVal)(void*))
{
    if (node->left != NULL)
        avlInorderInternal(node->left, printVal);

    printVal(node->value);

    if (node->right != NULL)
        avlInorderInternal(node->right, printVal);
}

static int countChildren(AVLNode* node)
{
    if (node == NULL)
        return 0;
    return 1 + countChildren(node->left) + countChildren(node->right);
}

static bool isMetadataCorrect(AVLNode* node)
{
    if (node == NULL)
        return true;

    int leftChildren = countChildren(node->left);
    int rightChildren = countChildren(node->right);

    return node->chilren == leftChildren + rightChildren && node->balance == rightChildren - leftChildren;
}

static bool avlNodeIsBalanced(AVLNode* node)
{
    if (node == NULL)
        return true;

    int leftChildren = countChildren(node->left);
    int rightChildren = countChildren(node->right);
    int diff = rightChildren - leftChildren;
    return diff < 2 && avlNodeIsBalanced(node->left) && avlNodeIsBalanced(node->right);
}