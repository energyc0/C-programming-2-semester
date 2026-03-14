#include <stdio.h>
#include "graph.h"
#include <assert.h>

int main()
{
    #define COUNT (7)
    unsigned adjacency[COUNT][COUNT] = {
        {0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1},
        {0, 1, 0, 1, 0, 1, 0},
        {0, 0, 1, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 1, 0},
        {0, 0, 1, 0, 1, 0, 1},
        {1, 1, 0, 0, 0, 1, 0},
    };
    Graph* graph = graphCreate();
    assert(graph != NULL);
    assert(graphAdd(graph, COUNT));
    assert(graphConnect(graph, 0, 1));
    assert(graphConnect(graph, 0, 6));
    assert(graphConnect(graph, 1, 6));
    assert(graphConnect(graph, 1, 2));
    assert(graphConnect(graph, 2, 5));
    assert(graphConnect(graph, 2, 3));
    assert(graphConnect(graph, 3, 4));
    assert(graphConnect(graph, 4, 5));
    assert(graphConnect(graph, 5, 6));

    for (int i = 0; i < COUNT; i++) {
        bool err = false;
        AdjacentList* adjList = graphGetAdjacent(graph, i, &err);
        assert(adjList != NULL);
        assert(adjacentGetVertex(adjList) == i);
        
        assert(!err && adjList != NULL);
        for(int j = 0; j < COUNT; j++) {
            if (adjacency[i][j] == 1) {
                assert(graphHasConnection(graph, i, j));
                assert(adjacentHasConnection(adjList, j));
            }
        }
    }

    graphFree(&graph);
    assert(graph == NULL);

    printf("Tests passed.\n");
    return 0;
    #undef COUNT
}