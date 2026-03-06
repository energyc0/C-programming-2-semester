#pragma once
#include "AVLTree.h"
#include <stdio.h>

/*
 * This header is for internal purposes of AVL-tree library.
 * It cannot be included from the user side.
 * This header is used for testing purposes.
 */

typedef struct AVLNode {
    void* key;
    void* value;
    struct AVLNode* left;
    struct AVLNode* right;
    int balance; /* balance = rightChildren - leftChildren */
} AVLNode;

typedef struct AVLTree {
    Comparator comp;
    struct AVLNode* root;
    int nodes;
} AVLTree;

/*
 * Print tree values in-order
 */
void avlInorder(AVLTree* tree, void (*printVal) (void*));

/*
 * Check 'balance' value validity.
 * Very slow function used for debug.
 */
bool avlIsBalanced(AVLTree* tree);
/*
 * Very slow function used for debug.
 */
bool avlIsMetadataCorrect(AVLTree* tree);

