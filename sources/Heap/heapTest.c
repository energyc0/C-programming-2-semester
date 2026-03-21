#include "heap.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

int greaterInt(int a, int b)
{
    if (a > b)
        return 1;
    if (a < b)
        return -1;
    return 0;
}

int lessInt(int a, int b)
{
    if (a < b)
        return 1;
    if (a > b)
        return -1;
    return 0;
}

int main()
{
    int values[] = {1, 34, 13, 14, 15, -2, 17, 10, 5, -33};
    int sortedGreaterValues[] = {-33, -2, 1, 5, 10, 13, 14, 15, 17, 34};
    int sortedLessValues[] = {34, 17, 15, 14, 13, 10, 5, 1, -2, -33};

    Heap* maxHeap = heapCreate(greaterInt, ARRAY_SIZE(values), values);
    assert(maxHeap != NULL);
    assert(heapSize(maxHeap) == ARRAY_SIZE(values));

    Heap* minHeap = heapCreate(lessInt, ARRAY_SIZE(values), values);
    assert(minHeap != NULL);
    assert(heapSize(minHeap) == ARRAY_SIZE(values));

    for (int i = ARRAY_SIZE(values)-1; i >= 0; i--) { 
        assert(!heapEmpty(minHeap));
        assert(!heapEmpty(maxHeap));
        int min = heapTop(minHeap);
        int max = heapTop(maxHeap);

        assert(min == heapPop(minHeap));
        assert(max == heapPop(maxHeap));
        assert(min == sortedLessValues[i]);
        assert(max == sortedGreaterValues[i]);
        assert(heapSize(minHeap) == i);
        assert(heapSize(maxHeap) == i);
    }

    assert(heapEmpty(minHeap));
    assert(heapEmpty(maxHeap));
    heapFree(&maxHeap);
    heapFree(&minHeap);
    assert(maxHeap == NULL && minHeap == NULL);

    maxHeap = heapCreate(greaterInt, 0, NULL);
    minHeap = heapCreate(lessInt, 0, NULL);
    assert(heapSize(maxHeap) == 0 && heapEmpty(maxHeap));
    assert(heapSize(minHeap) == 0 && heapEmpty(minHeap));

    for (int i = 0; i < ARRAY_SIZE(values); i++) {
        assert(heapPush(minHeap, sortedLessValues[i]));
        assert(heapPush(maxHeap, sortedGreaterValues[i]));

        assert(heapTop(minHeap) == sortedLessValues[i]);
        assert(heapTop(maxHeap) == sortedGreaterValues[i]);

        assert(heapSize(minHeap) == i+1);
        assert(heapSize(maxHeap) == i+1);
    }

    heapFree(&maxHeap);
    heapFree(&minHeap);
    assert(maxHeap == NULL && minHeap == NULL);
    return 0;
}