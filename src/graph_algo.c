
#include "graph_algo.h"

#include "Graph.h"
#include "BoolArray.h"

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>

#define INF -1

void bfs_kernel(int, Graph *, BoolArray *, char *, int *);

void sssp_kernel_1(int, Graph *, BoolArray *, int *, int *);
void sssp_kernel_2(int, Graph *, BoolArray *, int *, int *);

void bfs_common(Graph * gr, int S)
{
	//instance and init value
	BoolArray * F = newBoolArray(getVertexNumber(gr));
	char * X = malloc(sizeof(char)*getVertexNumber(gr));
	int * C = malloc(sizeof(int)*getVertexNumber(gr));
	#pragma omp parallel for
	for(int i=0; i<getVertexNumber(gr); i++)
	{
		setValue(F,i, UNS_FALSE);
		X[i]=0;
		C[i]=INF;
	}
	setValue(F,S, UNS_TRUE);
	C[S]=0;
	while(isEmpty(F)!=UNS_TRUE)
	{
		for(int node_id=0; node_id < getVertexNumber(gr); node_id++)
		{
			#pragma omp task
			bfs_kernel(node_id, gr, F, X, C);
			#pragma omp nowait
		}
	}
	destroyBoolArray(F);
	free(X);
	free(C);

}

void bfs_kernel(int node_id, Graph * gr, BoolArray * F, char * X, int * C)
{
	setValue(F,node_id, UNS_FALSE);
	if(getValue(F,node_id)==UNS_TRUE)
	{
		X[node_id]=1;
		int n_neighbor;
		int * neighbors = getNeighbors(gr, node_id, &n_neighbor);
		for(int i=0; i < n_neighbor; i++ )
		{
			int nid = neighbors[i];
			if(X[nid]==0)
			{
				C[nid]=C[node_id]+1;
				setValue(F,nid,UNS_TRUE);
			}
		}
		free(neighbors);
	}
}


void sssp_common(Graph * gr, int S)
{
	BoolArray * M = newBoolArray(getVertexNumber(gr));
	int * C = malloc(sizeof(int)*getVertexNumber(gr));
	int * U = malloc(sizeof(int)*getVertexNumber(gr));
	#pragma omp parallel for
	for(int i=0; i<getVertexNumber(gr); i++)
	{
		setValue(M,i,UNS_FALSE);
		C[i]=INF;
		U[i]=INF;
	}
	setValue(M,S,UNS_TRUE);
	C[S]=0;
	U[S]=0;
	while(isEmpty(M)!=UNS_TRUE)
	{
		for(int i=0; i<getVertexNumber(gr); i++)
		{
			sssp_kernel_1(i, gr, M, C, U);
			sssp_kernel_2(i, gr, M, C, U);
		}
	}
	destroyBoolArray(M);
	free(C);
	free(U);
}


void sssp_kernel_1(int node_id, Graph * gr, BoolArray * M, int * C, int * U)
{
	if(getValue(M,node_id)==UNS_TRUE)
	{
		setValue(M,node_id,UNS_FALSE);
		//for all neighbors of node_id
		int n_neighbor;
		int * neighbors = getNeighbors(gr, node_id, &n_neighbor);
		for(int i=0; i< n_neighbor; i++)
		{
			int neighbor = neighbors[i];
			if(U[neighbor] > C[node_id] + getWeight(gr,node_id,neighbor) )
			{
				U[i] = C[node_id] + getWeight(gr,node_id,neighbor);
			}
		}
		free(neighbors);
	}
}

void sssp_kernel_2(int node_id, Graph * gr, BoolArray * M, int * C, int * U)
{
	if( C[node_id] > U[node_id])
	{
		C[node_id] = U[node_id];
		setValue(M,node_id,UNS_TRUE);
	}
	U[node_id] = C[node_id];
}


void apsp_sssp_common(Graph * gr)
{
	BoolArray * M = newBoolArray(getVertexNumber(gr));
	int * C = malloc(sizeof(int)*getVertexNumber(gr));
	int * U = malloc(sizeof(int)*getVertexNumber(gr));
	for(int S=0; S < getVertexNumber(gr); S++)
	{
		#pragma omp parallel for
		for(int i=0; i<getVertexNumber(gr); i++)
		{
			setValue(M,i,UNS_FALSE);
			C[i]=INF;
			U[i]=INF;
		}
		setValue(M,S,UNS_TRUE);
		C[S]=0;
		U[S]=0;
		while(isEmpty(M)!=UNS_TRUE)
		{
			for(int i=0; i<getVertexNumber(gr); i++)
			{
				sssp_kernel_1(i, gr, M, C, U);
				sssp_kernel_2(i, gr, M, C, U);
			}
		}
	}
	destroyBoolArray(M);
	free(C);
	free(U);
}