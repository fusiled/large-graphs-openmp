
#include "Graph.h"
#include "graph_common.h"

#include <stdlib.h>
#include <stdio.h>

Graph * newGraph(int n_vert, int n_edge, int with_weight)


void destroyGraph(Graph * gr)


void build_adj_list_pointers(Graph * gr, int * n_neighbor)



void insertEdge(Graph * gr, int head, int tail, int weight)

void printGraph(Graph * gr)


int get_n_neighbor(Graph * gr, int index)
