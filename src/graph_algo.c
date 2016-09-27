
#include "graph_algo.h"

#include "Graph.h"

#include "GraphMatrix.h"

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>

#define INF -1

void bfs_kernel(int, Graph *, char *, char *, int *);

void sssp_kernel_1(int, Graph *, char *, int *, int *);
void sssp_kernel_2(int, Graph *, char *, int *, int *);


int min(int x, int y)
{
	return x < y ? x : y; 
}

int ** apsp_fw_matrix(GraphMatrix *);

char isEmpty(char * F, int size)
{
	char empty_flag = 1;
	#pragma omp parallel for
	for(int i=0; i < empty_flag*size; i++)
	{
		if(F[i]!=0)
		{
			empty_flag = 0;
		}
	}
	if (empty_flag==0)
	{
		return 0;
	}
	return 1;
}

void bfs(Graph * gr, int S)
{
	//instance and init value
	char * F = malloc(sizeof(char)*getVertexNumber(gr));
	char * X = malloc(sizeof(char)*getVertexNumber(gr));
	int * C = malloc(sizeof(int)*getVertexNumber(gr));
	#pragma omp parallel for
	for(int i=0; i<getVertexNumber(gr); i++)
	{
		F[i]=0;
		X[i]=0;
		C[i]=INF;
	}
	F[S]=1;
	C[S]=0;
	while(isEmpty(F, getVertexNumber(gr))!=1)
	{
		for(int node_id=0; node_id<getVertexNumber(gr); node_id++)
		{
			if(F[node_id])
			{
				#pragma omp task
				bfs_kernel(node_id, gr, F, X, C);
				#pragma omp nowait
			}
		}
	}
	free(F);
	free(X);
	free(C);

}

void bfs_kernel(int node_id, Graph * gr, char * F, char * X, int * C)
{
	F[node_id]=0;
	X[node_id]=1;
	int n_neighbor;
	int * neighbors = getNeighbors(gr, node_id, &n_neighbor);
	for(int i=0; i < n_neighbor; i++ )
	{
		int nid = neighbors[i];
		if(X[nid]==0)
		{
			C[nid]=C[node_id]+1;
			F[nid]=1;
		}
	}
}


void sssp(Graph * gr, int S)
{
	char * M = malloc(sizeof(char)*getVertexNumber(gr));
	int * C = malloc(sizeof(int)*getVertexNumber(gr));
	int * U = malloc(sizeof(int)*getVertexNumber(gr));
	#pragma omp parallel for
	for(int i=0; i<getVertexNumber(gr); i++)
	{
		M[i]=0;
		C[i]=INF;
		U[i]=INF;
	}
	M[S]=1;
	C[S]=0;
	U[S]=0;
	while(isEmpty(M, getVertexNumber(gr))!=1)
	{
		for(int i=0; i<getVertexNumber(gr); i++)
		{
			sssp_kernel_1(i, gr, M, C, U);
			sssp_kernel_2(i, gr, M, C, U);
		}
	}
	free(M);
	free(C);
	free(U);
}


void sssp_kernel_1(int node_id, Graph * gr, char * M, int * C, int * U)
{
	if(M[node_id]==1)
	{
		M[node_id]=0;
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
	}
}

void sssp_kernel_2(int node_id, Graph * gr, char * M, int * C, int * U)
{
	if( C[node_id] > U[node_id])
	{
		C[node_id] = U[node_id];
		M[node_id] = 1;
	}
	U[node_id] = C[node_id];
}


void apsp_sssp(Graph * gr)
{
	char * M = malloc(sizeof(char)*getVertexNumber(gr));
	int * C = malloc(sizeof(int)*getVertexNumber(gr));
	int * U = malloc(sizeof(int)*getVertexNumber(gr));
	for(int S=0; S < getVertexNumber(gr); S++)
	{
		#pragma omp parallel for
		for(int i=0; i<getVertexNumber(gr); i++)
		{
			M[i]=0;
			C[i]=INF;
			U[i]=INF;
		}
		M[S]=1;
		C[S]=0;
		U[S]=0;
		while(isEmpty(M, getVertexNumber(gr))!=1)
		{
			for(int i=0; i<getVertexNumber(gr); i++)
			{
				sssp_kernel_1(i, gr, M, C, U);
				sssp_kernel_2(i, gr, M, C, U);
			}
		}
	}
	free(M);
	free(C);
	free(U);
}


int ** apsp_fw(Graph * gr)
{
	switch(gr->graph_type)
	{
		case GRAPH_TYPE_ADJ_LIST: 
			printf("Avoid to use asps_fw with adj list.. skipping");
		break;
		case GRAPH_TYPE_MATRIX:
			return apsp_fw_matrix(gr->graph);
		break;
		default:
			printf("Unknow GRAPH_TYPE parameter. Exiting\n");
			exit(-1);
	}
}


int ** apsp_fw_matrix(GraphMatrix * gr)
{
	//make a trick. Access to the matrix in case we're using GRAPH_TYPE_MATRIX
	//copy the adjacecny matrix
	int ** result = malloc(sizeof(int *)*getVertexNumberMatrix(gr));
	for(int i=0; i < getVertexNumberMatrix(gr); i++)
	{
		result[i] = malloc(sizeof(int)*getVertexNumberMatrix(gr));
		memcpy(result[i], gr->W_ma[i], sizeof(int)*getVertexNumberMatrix(gr));
	}
	for(int k=0; k<getVertexNumberMatrix(gr); k++)
	{
		#pragma omp parallel for collapse(2)
		for(int i=0; i<getVertexNumberMatrix(gr); i++)
		{
			for(int j=0; j<getVertexNumberMatrix(gr); j++)
			{
				result[i][j]= min(result[i][j], result[i][k]+result[k][j]);
			}
		}
	}
	return result;
}