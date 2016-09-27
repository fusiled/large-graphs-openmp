
#include "Graph.h"
#include "graph_common.h"
#include "graph_algo.h"

#include "GraphLinked.h"
#include "GraphMatrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Graph * newGraph(int n_vert, int n_edge, int with_weight, int graph_type)
{
	Graph * gr = malloc(sizeof(Graph));
	if(gr==NULL)
	{
		printf("Cannot malloc graph wrapper. Exiting\n");
		exit(-1);
	}
	switch(graph_type)
	{
		case GRAPH_TYPE_ADJ_LIST: 
			gr->graph = newGraphLinked(n_vert,n_edge,with_weight);
			gr->linked_buffer = malloc(sizeof(int)*n_vert);
		break;
		case GRAPH_TYPE_MATRIX:
			gr->graph = newGraphMatrix(n_vert,with_weight);
			gr->linked_buffer=NULL;
		break;
		default:
			printf("Unknow GRAPH_TYPE parameter. Exiting\n");
			exit(-1);
	}
	gr->graph_type = graph_type;
	gr->finalized = 0;
	return gr;
}


void destroyGraph(Graph * gr)
{
	if(gr->linked_buffer!=NULL)
	{
		free(gr->linked_buffer);
	}
	switch(gr->graph_type)
	{
		case GRAPH_TYPE_ADJ_LIST: 
			destroyGraphLinked(gr->graph);
		break;
		case GRAPH_TYPE_MATRIX:
			destroyGraphMatrix(gr->graph);
		break;
		default:
			printf("Unknow GRAPH_TYPE parameter (Probably it has been corrupted). Exiting\n");
			exit(-1);
	}
}


void finalizeVertices(Graph * gr)
{
	if(gr->finalized==1)
	{
		#ifdef DEBUG
			printf("Vertices are still finalized. Skipping this invokation\n");
		#endif
		return;
	}
	switch(gr->graph_type)
	{
		case GRAPH_TYPE_ADJ_LIST: 
			buildAdjListPointers(gr->graph, gr->linked_buffer);
			free(gr->linked_buffer);
			gr->linked_buffer = calloc(sizeof(int),( ((GraphLinked *)gr->graph)->n_vert) ); 
		break;
		case GRAPH_TYPE_MATRIX:
			//nothing to do
		break;
		default:
			printf("Unknow GRAPH_TYPE parameter (Probably it has been corrupted). Exiting\n");
			exit(-1);
	}
	gr->finalized = 1;
}

void finalizeEdges(Graph * gr)
{
	if(gr->finalized==0)
	{
		#ifdef DEBUG
			printf("Finalize nodes before Edges. Skipping this invokation\n");
		#endif
		return;
	}
	if(gr->finalized==2)
	{
		#ifdef DEBUG
			printf("Edges are still finalized. Skipping this invokation\n");
		#endif
		return;
	}
	switch(gr->graph_type)
	{
		case GRAPH_TYPE_ADJ_LIST: 
			free(gr->linked_buffer);
			gr->linked_buffer=NULL;
		break;
		case GRAPH_TYPE_MATRIX:
			//nothing to do
		break;
		default:
			printf("Unknow GRAPH_TYPE parameter (Probably it has been corrupted). Exiting\n");
			exit(-1);
	}
}


void insertEdge(Graph * gr, int head, int tail, int weight)
{
	switch(gr->graph_type)
	{
		case GRAPH_TYPE_ADJ_LIST: 
			insertEdgeLinked(gr->graph, head, tail, weight, gr->linked_buffer[head]);
			gr->linked_buffer[head]++;
		break;
		case GRAPH_TYPE_MATRIX:
			insertEdgeMatrix(gr->graph, head,tail, weight);
		break;
		default:
			printf("Unknow GRAPH_TYPE parameter (Probably it has been corrupted). Exiting\n");
			exit(-1);
	}
}

void printGraph(Graph * gr)
{
	switch(gr->graph_type)
	{
		case GRAPH_TYPE_ADJ_LIST: 
			return printGraphLinked(gr->graph);
		break;
		case GRAPH_TYPE_MATRIX:
			return printGraphMatrix(gr->graph);
		break;
		default:
			printf("Unknow GRAPH_TYPE parameter (Probably it has been corrupted). Exiting\n");
			exit(-1);
	}
}


int * getNeighbors(Graph * gr, int index, int * n_neighbor)
{
	switch(gr->graph_type)
	{
		case GRAPH_TYPE_ADJ_LIST: 
			return getNeighborsLinked(gr->graph, index, n_neighbor);
		break;
		case GRAPH_TYPE_MATRIX:
			return getNeighborsMatrix(gr->graph, index, n_neighbor);
		break;
		default:
			printf("Unknow GRAPH_TYPE parameter (Probably it has been corrupted). Exiting\n");
			exit(-1);
	}
	return NULL;
}


void increaseNeighborNumber(Graph * gr, int index)
{
	if(gr->graph_type!=GRAPH_TYPE_ADJ_LIST)
	{
		#ifdef DEBUG
			printf("Cannot increase neighbor number on non GRAPH_TYPE_ADJ_LIST graphs. Skipping invokation\n");
		#endif
		return;
	}
	if(gr->finalized!=0)
	{
		#ifdef DEBUG
			printf("It is possible to increase neighbor number only when finalized is set to 0. Skipping invokation\n");
		#endif
		return;
	}
	gr->linked_buffer[index] += 1;
}


int getVertexNumber(Graph * gr)
{
	switch(gr->graph_type)
	{
		case GRAPH_TYPE_ADJ_LIST: 
			return getVertexNumberLinked(gr->graph);
		break;
		case GRAPH_TYPE_MATRIX:
			return getVertexNumberMatrix(gr->graph);
		break;
		default:
			printf("Unknow GRAPH_TYPE parameter (Probably it has been corrupted). Exiting\n");
			exit(-1);
	}
	return -1;	
}


int getWeight(Graph * gr, int head, int tail)
{
	switch(gr->graph_type)
	{
		case GRAPH_TYPE_ADJ_LIST: 
			return getWeightLinked(gr->graph, head, tail);
		break;
		case GRAPH_TYPE_MATRIX:
			return getWeightMatrix(gr->graph, head, tail);
		break;
		default:
			printf("Unknow GRAPH_TYPE parameter (Probably it has been corrupted). Exiting\n");
			exit(-1);
	}	
}


Graph * duplicateGraph(Graph * gr)
{
	Graph * copy = malloc(sizeof(Graph));
	switch(gr->graph_type)
	{
		case GRAPH_TYPE_ADJ_LIST: 
			//TODO NO IMPLEMENTED
			//copy->graph = duplicateGraphLinked(gr->graph);
			printf("GRAPH_TYPE_ADJ_LIST duplication is not implemented\n");
			exit(-1);
		break;
		case GRAPH_TYPE_MATRIX:
			copy->graph = duplicateGraphMatrix(gr->graph);
		break;
		default:
			printf("Unknow GRAPH_TYPE parameter (Probably it has been corrupted). Exiting\n");
			exit(-1);
	}
	copy->linked_buffer = malloc(sizeof(int)*getVertexNumber(gr));
	memcpy(copy->linked_buffer, gr->linked_buffer, sizeof(int)*getVertexNumber(gr));
	copy->graph_type = gr->graph_type;
	copy->finalized = gr->finalized;
	return copy;
}


void bfs(Graph * gr, int S)
{
	bfs_common(gr, S);
}


void sssp(Graph * gr, int S)
{
	sssp_common(gr, S);
}

void apsp_fw(Graph * gr)
{
	switch(gr->graph_type)
	{
		case GRAPH_TYPE_ADJ_LIST: 
			printf("fw with adj is not implemented yet. Skipping..\n");
		break;
		case GRAPH_TYPE_MATRIX:
			apsp_fw_matrix(gr->graph);
		break;
		default:
			printf("Unknow GRAPH_TYPE parameter (Probably it has been corrupted). Exiting\n");
			exit(-1);
	}
}

void apsp_sssp(Graph * gr)
{
	apsp_sssp_common(gr);
}
