#pragma once

#include <stdbool.h>

typedef struct Heap Heap;

/*
 * Comparator for structuring heap values.
 * The functon must return value > 0 if left operand is needed to pop up.
 */
typedef int (*HeapComparator) (int, int);

/*
 * Create heap with given numbers and comparator.
 * Create empty heap if nums == NULL and count == 0,
 * otherwise return NULL or if errors occured
 */
Heap* heapCreate(HeapComparator comp, unsigned count, int* nums);

/*
 * Push number into the heap.
 * Return false if error occured.
 */
bool heapPush(Heap* heap, int num);

/*
 * Remove and return value from the top of the heap.
 * If heap is NULL or empty return 0 always.
 * User must check the size of the heap manually via heapSize() or heapEmpty().
 */
int heapPop(Heap* heap);

/*
 * Return value from the top of the heap.
 * If heap is NULL or empty return 0 always.
 * User must check the size of the heap manually via heapSize() or heapEmpty().
 */
int heapTop(Heap* heap);

/*
 * Return true if heap is empty or NULL pointer.
 */
bool heapEmpty(const Heap* heap);

/*
 * Return the size of the heap or 0 is NULL pointer.
 */
unsigned heapSize(const Heap* heap); 

/*
 * Free data allocated for the heap and sets heap pointer to NULL
 */
void heapFree(Heap** heap);