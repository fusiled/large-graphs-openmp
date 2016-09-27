
#ifndef GRAPH_LINKED_H
#define GRAPH_LINKED_H

#include "graph_common.h"

//A vertex knows its neigbors. It knows the number of neighbors and the addresses
//of the adjacency lists (one for neighbors number and one for the weight)
typedef struct Vertex
{
	int * adj_list;
	int * w_adj_list;
} Vertex;

//A graph is composed by an array of vertices V_ar and and array of edges E_ar.
//It also knows the number of vertices and edges. E_ar is composed by adjacency
//lists of the nodes concatenated, so you must insert new edges with care.
typedef struct GraphLinked
{
	Vertex * V_ar;
	int * E_ar; 
	int * W_ar;
	int n_vert;
	int n_edge;
} GraphLinked;

//create a new graph with given vertices and edges. Pass a flag to also set weights.
GraphLinked * newGraphLinked(int, int, int);

//just free the allocated emmory associated to the graph structure.
void destroyGraphLinked(GraphLinked * gr);

//insert an edge and return its reference.
void insertEdgeLinked(GraphLinked *, int, int, int, int);

//This is important. After you're sure to have set n_neighbor of each vertex properly
//(sum of n_neighbor of each vertex = n_edge) this function builds references of the adjacency
//list of each node
void buildAdjListPointers(GraphLinked *, int *);


int * getNeighborsLinked(GraphLinked *, int, int *);


//print every node and its neighbors
void printGraphLinked(GraphLinked *);


int getVertexNumberLinked(GraphLinked *);


int getWeightLinked(GraphLinked *, int, int);


GraphLinked * duplicateGraphLinked(GraphLinked *);

#endif