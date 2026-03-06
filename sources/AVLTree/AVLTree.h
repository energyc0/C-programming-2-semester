#pragma once

#include <stdbool.h>

/*
 * AVL-tree structure. It stores key-value data.
 * Key and values can be arbitrary types, so it must take Comparator as initializer value.
 * If you use some one type, while comparator use the other, the behaviour is undefined.
 */
typedef struct AVLTree AVLTree;

/*
 * Compare function that is used in AVLTree for arbitrary types.
 * It compares keys.
 */
typedef int (*Comparator)(void*, void*);

/*
 * Initialize AVL-tree. After initializing it will
 * take types that defined in comparator.
 */
AVLTree* avlAlloc(Comparator comp);

/*
 * Free space used by AVL-tree.
 * Sets tree pointer to NULL
 */
void avlFree(AVLTree** tree);

/*
 * Return true, if has the given key in the tree.
 * Return false otherwise.
 */
bool avlContains(AVLTree* tree, void* key);

/*
 * Return true, if inserted successfully.
 * Return false, if error occurred.
 * If has already the key in the tree, replace the value.
 */
bool avlInsert(AVLTree* tree, void* key, void* value);

/*
 * Find the key in the tree and return its value, sets isFound = true.
 * If the key is not in the tree, return NULL and set isFound = false.
 */
void* avlFind(AVLTree* tree, void* key, bool* isFound);

/*
 * Find the key in the tree and delete its data in the tree.
 */
void avlDelete(AVLTree* tree, void* key);
