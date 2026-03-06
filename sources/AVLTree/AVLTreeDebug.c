#include "AVLTreeInternal.h"

static bool avlNodeIsBalanced(AVLNode* node);
static void avlInorderInternal(AVLNode* node, void (*printVal)(void*));
static int countNodes(AVLNode* node);

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

    int leftChildren = countNodes(node->left);
    int rightChildren = countNodes(node->right);
    int diff = rightChildren - leftChildren;
    return diff < 2 && avlNodeIsBalanced(node->left) && avlNodeIsBalanced(node->right);
}