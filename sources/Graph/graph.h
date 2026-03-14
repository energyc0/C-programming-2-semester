#pragma once

#include <stdio.h>
#include <stdbool.h>

/*
 * Graph stores vertices
 */
typedef struct Graph Graph;

typedef struct AdjacentList AdjacentList;

/*
 * Create graph, needs to call graphFree()
 */
Graph* graphCreate();

/*
 * Read graph data from the file
 */
Graph* graphRead(const char* filename);

/*
 * Frees memory allocated for the graph
 */
void graphFree(Graph** graph);

/*
 * Return count of vertices in the graph
 */
unsigned graphSize(Graph* graph); 

/* 
 * Connect vertices 'a' and 'b' in graph.
 * Return true if successful, return false if error ocurred.
 */
bool graphConnect(Graph* graph, unsigned a, unsigned b);

/*
 * Add 'amount' new vertices in the graph.
 */
bool graphAdd(Graph* graph, unsigned amount);

/*
 * Return true if vertex 'a' is connected to vertex 'b'
 */
bool graphHasConnection(Graph* graph, unsigned a, unsigned b);

/* Get adjacent vertices of the given vertex.
 * *err = true if error ocurred,
 * *err = false otherwise
 */
AdjacentList* graphGetAdjacent(Graph* graph, unsigned vertex, bool* err);

/*
 * Return count of vertices in the graph of the vertex 
 */
unsigned adjacentGetSize(AdjacentList* list);

/*
 * Return true if the vertex of adjacent list is connected to the given vertex
 */
bool adjacentHasConnection(AdjacentList* list, unsigned vertex);

/*
 * Return the main vertex of adjacent list
 * If list is freed, return ((unsigned)-1)
 */
unsigned adjacentGetVertex(AdjacentList* list);

/*
 * Free the memory allocated for the AdjacentList
 * and set pointer to NULL.
 */
///void graphFreeAdjacent(AdjacentList** list);
