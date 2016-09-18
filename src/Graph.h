
#ifndef GRAPH_H
#define GRAPH_H

#include "graph_common.h"

//A graph is composed by an array of vertices V_ar and and array of edges E_ar.
//It also knows the number of vertices and edges. E_ar is composed by adjacency
//lists of the nodes concatenated, so you must insert new edges with care.
typedef struct Graph
{
	void * graph;
	char graph_type;
	char finalized;
	int * linked_buffer;
} Graph;

//create a new graph with given vertices and edges. Pass a flag to also set weights.
Graph * newGraph(int, int, int, int);

//just free the allocated emmory associated to the graph structure.
void destroyGraph(Graph * gr);

//insert an edge.
void insertEdge(Graph *, int, int, int);

//IT IS STRICTLY NEEDED ONLY FOR GRAPH_TYPE_ADJ_LIST. We must do some work before
//inserting edges. For GRAPH_TYPE_MATRIX it is not strictly needed.
void finalizeVertices(Graph *);

//It is not mandatory but STRONGLY SUGGESTED to call this function when you have
//finished to add edges in GRAPH_TYPE_ADJ_LIST graphs. It frees some memory and
//blocks alteration of the graph.
void finalizeEdges(Graph *);

//Get a vector that contains the node_id of the neighbors of the past node_id
//you can save the save of the array at the address pinted by the third param.
int * getNeighbors(Graph *, int, int *);

//print every node and its neighbors
void printGraph(Graph *);

//NEEDED ONLY BY GRAPH_TYPE_ADJ_LIST. Call it to set the number of neeighbors
//of a vertex BEFORE CALLING finalizeVertices. After the cited invokation you
//won't be able to change the number of neighbors of a node.
void increaseNeighborNumber(Graph *, int);


int getVertexNumber(Graph *);

int getWeight(Graph *, int, int);

#endif