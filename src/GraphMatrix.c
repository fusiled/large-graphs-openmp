
#include "graph_common.h"
#include "GraphMatrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int min(int x, int y)
{

	return (x < y) ? x : y; 
}

GraphMatrix * newGraphMatrix(int n_vert, int with_weight)
{
	GraphMatrix * gr = malloc(sizeof(GraphMatrix));
	if(gr==NULL)
	{
		printf("Cannot instantiate GraphMatrix... exiting\n");
		exit(-1);
	}
	gr->n_vert=n_vert;
	gr->with_weight=with_weight;
	gr->W_ma=malloc(sizeof(int*)*n_vert);
	if(gr->W_ma==NULL)
	{
		printf("Cannot instantiate Weight Matrix... exiting\n");
		exit(-1);
	}
	for(int i=0; i < gr->n_vert; i++)
	{
		gr->W_ma[i]=malloc(sizeof(int)*n_vert);
		if(gr->W_ma[i]==NULL)
		{
			printf("Cannot instantiate Weight Matrix... exiting\n");
			exit(-1);
		}
		for(int j=0; j<gr->n_vert; j++ )
		{
			gr->W_ma[i][j]=INF;
		}
	}
	return gr;
}


void destroyGraphMatrix(GraphMatrix * gr)
{
	for(int i=0; i < gr->n_vert; i++)
	{
		free(gr->W_ma[i]);
	}
	free(gr->W_ma);
	free(gr);
}


void insertEdgeMatrix(GraphMatrix * gr, int head, int tail, int weight)
{
	if( gr->with_weight == GRAPH_WITH_WEIGHT)
	{
		gr->W_ma[head][tail]=weight;
		return;
	}
	gr->W_ma[head][tail]=1;
}

int * getNeighborsMatrix(GraphMatrix * gr, int index, int * n_neighbor_ref)
{
	int * line = gr->W_ma[index];
	int n_neighbor=0;
	for(int i=0; i < gr->n_vert; i++)
	{
		if(line[i]!=0)
		{
			n_neighbor++;
		}
	}
	int * ret_ref = malloc(sizeof(int)*n_neighbor);
	int ret_ref_iter=0;
	*n_neighbor_ref = n_neighbor;
	for(int i=0; i< gr->n_vert; i++)
	{
		if(line[i]!=0)
		{
			ret_ref[ret_ref_iter]=i;
			ret_ref_iter++;
		}
	}
	return ret_ref;
}

void printGraphMatrix(GraphMatrix * gr)
{
	for(int i=0; i< gr->n_vert; i++)
	{
		printf("Node %d:", i);
		for(int j=0; j < gr->n_vert; j++)
		{
			if(gr->W_ma[i][j]!=INF)
			{
				printf("->%d", j);
			}
		}
		printf("\n");
	}
}


int getVertexNumberMatrix(GraphMatrix * gr)
{
	return gr->n_vert;
}


int getWeightMatrix(GraphMatrix * gr, int head, int tail)
{
	return gr->W_ma[head][tail];
}


GraphMatrix * duplicateGraphMatrix(GraphMatrix * gr)
{
	GraphMatrix * copy = malloc(sizeof(GraphMatrix));
	copy->n_vert = gr->n_vert;
	copy-> with_weight = gr->with_weight;
	copy->W_ma = malloc(sizeof(int*)*getVertexNumberMatrix(gr));
	for(int i=0; i<getVertexNumberMatrix(gr); i++)
	{
		copy->W_ma[i]=malloc(sizeof(int)*getVertexNumberMatrix(gr));
		memcpy(copy->W_ma[i], gr->W_ma[i], sizeof(int)*getVertexNumberMatrix(gr));
	}
	return copy;
}

int ** apsp_fw_matrix(GraphMatrix * gr)
{
	int ** result = malloc(sizeof(int *)*getVertexNumberMatrix(gr));
	for(int i=0; i < getVertexNumberMatrix(gr); i++)
	{
		#pragma omp task
		{
			result[i] = malloc(sizeof(int)*getVertexNumberMatrix(gr));
			memcpy(result[i], gr->W_ma[i], sizeof(int)*getVertexNumberMatrix(gr));
		}
		#pragma omp nowait
	}
	for(int i=0; i<getVertexNumberMatrix(gr);i++)
	{
		for(int j=0; j<getVertexNumberMatrix(gr); j++)
		{
			printf("%d ", result[i][j]);
		}
		printf("\n");
	}
	for(int k=0; k<getVertexNumberMatrix(gr); k++)
	{
		#pragma omp parallel for collapse(2) shared(result, gr)
		for(int i=0; i<getVertexNumberMatrix(gr); i++)
		{
			for(int j=0; j<getVertexNumberMatrix(gr); j++)
			{
				int a = result[i][k];
				int b = result[k][j];
				int c = (a==INF || b==INF) ? INF : result[i][k]+result[k][j] ;
				result[i][j] = min(result[i][j], c);
			}
		}
	}
	return result;
}
