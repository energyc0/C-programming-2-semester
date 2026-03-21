#include "heap.h"
#include <stdlib.h>
#include <string.h>

typedef struct Heap {
    int* data;
    unsigned size;
    unsigned capacity;
    HeapComparator comp;
} Heap;

#define HEAP_TOP(heap) (heap->data[0])
#define HEAP_BACK(heap) (heap->data[heap->size - 1])

/* Search the least power of two that bigger than number */
static unsigned nearestPow2(unsigned num)
{
    unsigned res = 1;
    while (res < num)
        res <<=1;
    return res;
}

static void swapInt(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

bool heapIncreaseCapacity(Heap* heap)
{
    int* newData = realloc(heap->data, (heap->capacity << 1) * sizeof(heap->data[0]));
    if (newData == NULL)
        return false;

    heap->capacity <<= 1;
    heap->data = newData;
    return true;
}

Heap* heapCreate(HeapComparator comp, unsigned count, int* nums)
{
    if (comp == NULL || (count != 0 && nums == NULL) || (nums != NULL && count == 0))
        return NULL;

    Heap* heap = malloc(sizeof(*heap));
    if (heap == NULL)
        return NULL;

    heap->capacity = count > 8 ? nearestPow2(count) : 8;
    heap->data = malloc(sizeof(heap->data[0]) * heap->capacity);
    if (heap->data == NULL) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->comp = comp;

    for (unsigned i = 0; i < count; i++) {
        heapPush(heap, nums[i]);
    }
    return heap;
}

bool heapPush(Heap* heap, int num)
{
    if (heap == NULL)
        return false;

    /* Check if need to grow the data vector. */
    if (heap->size + 1 > heap->capacity && !heapIncreaseCapacity(heap))
        return false;

    /* Insert number at the end of the heap. */
    unsigned numPos = heap->size++;
    HEAP_BACK(heap) = num;

    /* Pop up the number. */
    while(numPos > 0 && heap->comp(heap->data[numPos], heap->data[(numPos - 1) / 2]) > 0) {
        swapInt(&heap->data[numPos], &heap->data[(numPos - 1) / 2]);
        numPos = (numPos - 1) / 2;
    }

    return true;
}

int heapPop(Heap* heap)
{
    if (heapEmpty(heap))
        return 0;

    int ret = HEAP_TOP(heap);
    swapInt(&HEAP_TOP(heap), &HEAP_BACK(heap));
    heap->size--;

    int numPos = 0;
    while(numPos < heap->size) {
        int topValue = numPos;

        if (heap->comp(heap->data[topValue], heap->data[numPos * 2 + 1]) < 0) {
            topValue = numPos * 2 + 1;
        }

        if (heap->comp(heap->data[topValue], heap->data[numPos * 2 + 2]) < 0) {
            topValue = numPos * 2 + 2;
        }
        if (topValue != numPos) {
            swapInt(&heap->data[numPos], &heap->data[topValue]);
            numPos = topValue;
        } else {
            break;
        }
    }

    return ret;
}

int heapTop(Heap* heap)
{
    return heapEmpty(heap) ? 0 : HEAP_TOP(heap);
}

void heapFree(Heap** heap)
{
    if (heap == NULL || *heap == NULL)
        return;

    free((*heap)->data);
    free(*heap);
    *heap = NULL;
}

bool heapEmpty(const Heap* heap)
{
    return heap == NULL || heap->size == 0;
}

unsigned heapSize(const Heap* heap)
{
    return heap == NULL ? 0 : heap->size;
}