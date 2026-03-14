#include <stdlib.h>
#include <assert.h>
#include "graph.h"

/*
 * Graph contains of adjacency matrix.
 * vertices[i] is the i'th vertex which is adjacent to 
 * vertices containing in AdjacentList.
 */
typedef struct Graph {
    AdjacentList* vertices;
    unsigned size;  /* Count of vertices in the graph */
    unsigned capacity; /* Capacity of 'vertices' vector */
} Graph;

/*
 * AdjacentList is the list that contains
 * vertices adjacent to some other vertex
 */
typedef struct AdjacentList {
    unsigned vertex;    /* Main vertex */
    unsigned* vertices; /* Vertices that are adjacent to main vertex */
    unsigned* count; /* Pointer to the 'size' of Graph */
    //unsigned capacity; /* Current capacity of the 'vertices' list */
} AdjacentList;

/* 
 * Realloc list of AdjacentVertices in graph.
 * Return true and change to reallocated list if the operation was successful.
 * Return false and do not change the graph list if error occured.
 */
static bool reallocAdjacentLists(Graph* graph, unsigned newCap);

/*
 * Realloc list of adjacent vertices.
 * Return true if successful and change the metadata of AdjacentList.
 * Return false if error ocurred and do not change the metadata.
 */
static bool reallocNeighbours(AdjacentList* list, unsigned newCap);

/*
 * Free the list of AdjacentVertices in the graph.
 */
static void listFree(Graph* graph);

Graph* graphCreate()
{
    return calloc(1, sizeof(Graph));
}
/*
Graph* graphRead(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
        return NULL;

    Graph* graph = graphCreate();
    if (graph == NULL)
        return NULL;
    if (fscanf(file, "%u", &graph->size) != 1) {
        graphFree(&graph);
        return NULL;
    }

    AdjacentList* newVertices = reallocList(graph->vertices, graph->size);
    if (newVertices == NULL) {
        graphFree(&graph);
        return NULL;
    }
    memset(newVertices, 0, graph->size * sizeof(newVertices[0]));
    graph->vertices = newVertices;

    unsigned idx = 0;
    while (!feof(file) && idx < graph->size) {
        AdjacentList* list = &graph->vertices[idx++];
        if (!reallocNeighbours(list, graph->size)) {
            graphFree(&graph);
            return NULL;
        }

        for (unsigned j = 0; j < graph->size; j++) {
            fscanf(file, "%u", &list->vertices[j]);
        }

    }

    return graph;
}
*/
static bool reallocAdjacentLists(Graph* graph, unsigned newCap)
{
    AdjacentList* newList = realloc(graph->vertices, newCap * sizeof(AdjacentList));
    if (newList == NULL)
        return false;
    graph->vertices = newList;
    graph->capacity = newCap;
    return true;
}

static bool reallocNeighbours(AdjacentList* list, unsigned newCap)
{
    unsigned* vertices = realloc(list->vertices, sizeof(vertices[0]) * newCap);
    if (vertices == NULL) {
        return false;
    }

    list->vertices = vertices;
    //list->capacity = newCap;
    return true;
}

static void listFree(Graph* graph)
{
    if (graph == NULL || graph->vertices == NULL)
        return;

    for (unsigned i = 0; i < graph->size; i++)
        free(graph->vertices[i].vertices);
    free(graph->vertices);
    graph->vertices = NULL;
    graph->size = 0;
}

void graphFree(Graph** graph)
{
    if (graph == NULL || *graph == NULL)
        return;

    listFree(*graph);
    free(*graph);
    *graph = NULL;
}

unsigned graphSize(Graph* graph) 
{
    return graph->size;
}

AdjacentList* graphGetAdjacent(Graph* graph, unsigned vertex, bool* err)
{
    if (graph == NULL || vertex >= graph->size) {
        *err = true;
        return NULL;
    }

    *err = false;
    return &graph->vertices[vertex];
}

void graphFreeAdjacent(AdjacentList** list)
{
    if (list == NULL || *list == NULL)
        return;
    free(*list);
    *list = NULL;
}

bool graphAdd(Graph* graph, unsigned count)
{
    if (graph == NULL)
        return false;
    if (graph->capacity <= graph->size + count) {
        unsigned newCap = graph->capacity > 0 ? graph->capacity << 1 : 1;
        while (newCap < graph->size + count)
            newCap <<= 1;
        reallocAdjacentLists(graph, newCap);
    }
    int i = graph->size;
    graph->size += count;

    for(; i < graph->size; i++) {
        if (!reallocNeighbours(&graph->vertices[i], graph->capacity))
            return false;
        graph->vertices[i].count = &graph->size;
        graph->vertices[i].vertex = i;
    }

    return true;
}

bool graphHasConnection(Graph* graph, unsigned a, unsigned b)
{
    return graph != NULL &&
     graph->size > a &&
      graph->size > b &&
       graph->vertices[a].vertices[b] > 0;
}

unsigned graphConnection(Graph* graph, unsigned a, unsigned b, bool* err)
{
    if (graph == NULL || graph->size <= a || graph->size <= b) {
        if (err) *err = true;
        return 0;
    }

    return graph->vertices[a].vertices[b];
}

bool graphConnect(Graph* graph, unsigned a, unsigned b, unsigned weight)
{
    if (graph->size <= a || graph->size <= b || weight == 0)
        return false;
    graph->vertices[a].vertices[b] = weight;
    graph->vertices[b].vertices[a] = weight;
    return true;
}

unsigned adjacentGetSize(AdjacentList* list)
{
    if (list == NULL)
        return 0;
    assert(list->count != NULL);
    return *list->count;
}

unsigned adjacentGetVertex(AdjacentList* list)
{
    if (list == 0)
        return (unsigned)-1;
    return list->vertex;
}

bool adjacentHasConnection(AdjacentList* list, unsigned vertex)
{
    if (list == NULL)
        return false;
    assert(list->count != NULL);
    if (*list->count <= vertex)
        return false;

    
    return list->vertices[vertex] == 1;
}
