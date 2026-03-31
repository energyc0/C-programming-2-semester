#include "graph.h"
#include "heap.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* 1024x1024 */
#define CITY_COUNT_LIMIT (1048510)

typedef struct VertexRoad {
    unsigned vert;
    unsigned weight;
} VertexRoad;

/* Comparator for heap */
static int lessRoad(const void* a, const void* b)
{
    const VertexRoad* aRoad = a;
    const VertexRoad* bRoad = b;
    if (aRoad->weight < bRoad->weight)
        return 1;
    if (aRoad->weight > bRoad->weight)
        return -1;
    return 0;
}

static bool readStates(const char* filename, Graph** graph, int** states, int* stateCount)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open.");
        return false;
    }

    int roadCount = 0;
    int cityCount = 0;
    if (fscanf(file, "%d %d", &cityCount, &roadCount) != 2) {
        fprintf(stderr, "Failed to parse road count and city count.\n");
        fclose(file);
        return false;
    }
    if (cityCount > CITY_COUNT_LIMIT) {
        fprintf(stderr, "Invalid value of cities count.\n");
        fclose(file);
        return false;
    }

    Graph* newGraph = graphCreate();
    if (newGraph == NULL) {
        fprintf(stderr, "Failed to allocate memory for the graph.\n");
        fclose(file);
        return false;
    }
    if (!graphAdd(newGraph, cityCount)) {
        fprintf(stderr, "Failed to add vertices into the graph.\n");
        graphFree(&newGraph);
        fclose(file);
        return false;
    }

    for (int i = 0; i < roadCount; i++) {
        int a = 0;
        int b = 0;
        int weight = 0;
        if (fscanf(file, "%d %d %d", &a, &b, &weight) != 3) {
            fprintf(stderr, "Failed to parse data for an edge.\n");
            fclose(file);
            return false;
        }
        if (!graphConnect(newGraph, a - 1, b - 1, weight)) {
            fprintf(stderr, "Failed to connect vertices in the graph.\n");
            graphFree(&newGraph);
            fclose(file);
            return false;
        }
    }

    *stateCount = 0;
    if (fscanf(file, "%d", stateCount) != 1) {
        fprintf(stderr, "Failed to get states count from the file.\n");
        graphFree(&newGraph);
        fclose(file);
        return false;
    }

    if (*stateCount > cityCount || *stateCount <= 0) {
        fprintf(stderr, "Invalid state count in the file.\n");
        graphFree(&newGraph);
        fclose(file);
        return false;
    }

    /* I think it works right */
    // NOLINTNEXTLINE(clang-analyzer-optin.taint.TaintedAlloc)
    *states = calloc(1, sizeof((*states)[0]) * (*stateCount));
    if (*states == NULL) {
        fprintf(stderr, "Failed to allocate memory for states.\n");
        graphFree(&newGraph);
        fclose(file);
        return false;
    }
    for (int i = 0; i < *stateCount; i++) {
        if (fscanf(file, "%d", &(*states)[i]) != 1) {
            fprintf(stderr, "Failed to get state number from the file.\n");
            free(*states);
            graphFree(&newGraph);
            fclose(file);
            return false;
        }
        /* From 0 in graph */
        (*states)[i]--;
    }

    fclose(file);
    *graph = newGraph;
    return true;
}

static void heapsFree(Heap** heaps, int count)
{
    for (int i = 0; i < count; i++)
        heapFree(&heaps[i], free);
}

bool divideConquer(Graph* graph, int* cityStates, unsigned citiesCount, int* states, int stateCount)
{
    assert(citiesCount >= stateCount);
    /* Initialize heaps */
    Heap* stateHeaps[stateCount];

    /* Initialize with -1 means there is no state occupied the city */
    for (int i = 0; i < citiesCount; i++) {
        cityStates[i] = -1;
    }

    /* Initialize start cities and heaps for BFS */
    for (int i = 0; i < stateCount; i++) {
        VertexRoad* newRoad = malloc(sizeof(*newRoad));
        if (newRoad == NULL) {
            fprintf(stderr, "Failed to allocate memory.\n");
            heapsFree(stateHeaps, stateCount);
            return false;
        }
        newRoad->vert = states[i];
        newRoad->weight = 0;
        stateHeaps[i] = heapCreate(lessRoad, 1, (void**)&newRoad);
        if (stateHeaps[i] == NULL) {
            fprintf(stderr, "Failed to allocate memory for the heap.\n");
            heapsFree(stateHeaps, i);
            return false;
        }
    }

    /* BFS for every state */
    while (true) {
        bool hasNonEmpty = false;

        for (int i = 0; i < stateCount; i++) {
            if (!heapEmpty(stateHeaps[i])) {
                Heap* heap = stateHeaps[i];
                while (!heapEmpty(heap)) {
                    VertexRoad* vertRoad = heapPop(heap);
                    unsigned vert = vertRoad->vert;
                    free(vertRoad);
                    vertRoad = NULL;
                    /* City is occupied*/
                    if (cityStates[vert] != -1)
                        continue;
                    cityStates[vert] = states[i];
                    /* Get adjacent cities */
                    bool err = false;
                    AdjacentList* adjList = graphGetAdjacent(graph, vert, &err);
                    if (err) {
                        fprintf(stderr, "Failed to get adjacent cities.\n");
                        heapsFree(stateHeaps, stateCount);
                        return false;
                    }
                    /* Add adjacent cities */
                    for (unsigned adjVert = 0; adjVert < citiesCount; adjVert++) {
                        if (cityStates[adjVert] == -1 && adjacentHasConnection(adjList, adjVert)) {
                            VertexRoad* newRoad = malloc(sizeof(*newRoad));
                            if (newRoad == NULL) {
                                fprintf(stderr, "Failed to allocate memory.\n");
                                heapsFree(stateHeaps, stateCount);
                                return false;
                            }
                            newRoad->vert = adjVert;
                            newRoad->weight = adjacentGetConnection(adjList, adjVert);
                            if (!heapPush(heap, newRoad)) {
                                fprintf(stderr, "Failed to push value to the heap.\n");
                                heapsFree(stateHeaps, stateCount);
                                return false;
                            }
                        }
                    }
                    hasNonEmpty = true;
                    break;
                }
            }
        }

        if (!hasNonEmpty)
            break;
    }

    heapsFree(stateHeaps, stateCount);
    return true;
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Expected input file.\n");
        return 1;
    }

    Graph* graph = NULL;
    int* states = NULL;
    int stateCount = 0;

    if (!readStates(argv[1], &graph, &states, &stateCount)) {
        fprintf(stderr, "Failed to read \"%s\"\n", argv[1]);
        return 1;
    }

    /* Which city is correspond to a state */
    unsigned citiesCount = graphSize(graph);
    int cityStates[citiesCount];

    if (!divideConquer(graph, cityStates, citiesCount, states, stateCount)) {
        graphFree(&graph);
        free(states);
        fprintf(stderr, "Failed to divide cities between states.\n");
        return 1;
    }

    for (unsigned j = 0; j < stateCount; j++) {
        int state = states[j];
        printf("State number %d has cities:", state + 1);
        for (unsigned i = 0; i < citiesCount; i++) {
            if (cityStates[i] == state)
                printf(" %d", i + 1);
        }
        putchar('\n');
    }

    graphFree(&graph);
    free(states);
    return 0;
}