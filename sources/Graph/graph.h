#pragma once

#include <stdio.h>
#include <stdbool.h>

typedef struct Graph Graph;

typedef struct VertexList {
    unsigned* vertices;
    unsigned count;
} VertexList;

Graph* graphCreate();

Graph* graphRead(const char* filename);

void graphFree(Graph** graph);

unsigned graphSize(Graph* graph); 

/* Connect 'a' and 'b' in graph */
bool graphConnect(Graph* graph, unsigned a, unsigned b);

bool graphAdd(Graph* graph);

bool graphHasConnection(Graph* graph, unsigned a, unsigned b);

/* Get neighbours of the given vertex,
 * *err = true if error ocurred,
 * *err = false otherwise
 */
VertexList graphGetNeighbours(Graph* graph, unsigned vertex, bool* err);

