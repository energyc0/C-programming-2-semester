#include "graph.h"
#include <stdio.h>
int main()
{
    Graph* graph = graphCreate();
    graphFree(&graph);
    printf("Hello!\n");
    return 0;
}