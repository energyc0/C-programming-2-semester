#pragma once

#include <stdbool.h>

typedef struct Heap Heap;

/*
 * Comparator for structuring heap values.
 * The functon must return value > 0 if left operand is greater,
 * return 0 if equals, return < 0 if less.
 */
typedef int (*HeapComparator)(const void*, const void*);

/*
 * Cleaner to clean data after destroying heap.
 */
typedef void (*HeapCleaner)(void*);

/*
 * Create heap with given values and comparator.
 * Create empty heap if nums == NULL and count == 0,
 * otherwise return NULL or if errors occured
 */
Heap* heapCreate(HeapComparator comp, unsigned count, void** data);

/*
 * Push value into the heap.
 * Return false if error occured.
 */
bool heapPush(Heap* heap, void* val);

/*
 * Remove and return value from the top of the heap.
 * If heap is NULL or empty return 0 always.
 * User must check the size of the heap manually via heapSize() or heapEmpty().
 */
void* heapPop(Heap* heap);

/*
 * Return value from the top of the heap.
 * If heap is NULL or empty return 0 always.
 * User must check the size of the heap manually via heapSize() or heapEmpty().
 */
void* heapTop(Heap* heap);

/*
 * Return true if heap is empty or NULL pointer.
 */
bool heapEmpty(const Heap* heap);

/*
 * Return the size of the heap or 0 is NULL pointer.
 */
unsigned heapSize(const Heap* heap);

/*
 * Free data allocated for the heap and sets heap pointer to NULL.
 * Pass free function if data is needed to clean otherwise NULL.
 */
void heapFree(Heap** heap, HeapCleaner cleaner);