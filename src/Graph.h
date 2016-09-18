
#ifndef GRAPH_H
#define GRAPH_H

#include "graph_common.h"

//A graph is composed by an array of vertices V_ar and and array of edges E_ar.
//It also knows the number of vertices and edges. E_ar is composed by adjacency
//lists of the nodes concatenated, so you must insert new edges with care.
typedef struct Graph
{
	void * graph;
} Graph;

//create a new graph with given vertices and edges. Pass a flag to also set weights.
Graph * newGraph(int, int, int, int);

//just free the allocated emmory associated to the graph structure.
void destroyGraph(Graph * gr);

//insert an edge and return its reference.
void insertEdge(Graph *, int, int, int);

//This is important. After you're sure to have set n_neighbor of each vertex properly
//(sum of n_neighbor of each vertex = n_edge) this function builds references of the adjacency
//list of each node
void finalize(Graph *);



//print every node and its neighbors
void printGraph(Graph *);

#endif