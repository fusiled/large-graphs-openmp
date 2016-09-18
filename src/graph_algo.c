
#include "graph_algo.h"

#include "Graph.h"

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define INF -1

void bfs_kernel(int, Graph *, char *, char *, int *);

void sssp_kernel_1(int, Graph *, char *, int *, int *);
void sssp_kernel_2(int, Graph *, char *, int *, int *);

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
	char * F = malloc(sizeof(char)*gr->n_vert);
	char * X = malloc(sizeof(char)*gr->n_vert);
	int * C = malloc(sizeof(int)*gr->n_vert);
	#pragma omp parallel for
	for(int i=0; i<gr->n_vert; i++)
	{
		F[i]=0;
		X[i]=0;
		C[i]=INF;
	}
	F[S]=1;
	C[S]=0;
	while(isEmpty(F, gr->n_vert)!=0)
	{
		for(int node_id=0; node_id<gr->n_vert; node_id++)
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
	for(int nid=0; nid < get_n_neighbor(gr,node_id); nid++ )
	{
		if(X[nid]==0)
		{
			C[nid]=C[node_id]+1;
			F[nid]=1;
		}
	}
}


void sssp(Graph * gr, int S)
{
	char * M = malloc(sizeof(char)*gr->n_vert);
	int * C = malloc(sizeof(int)*gr->n_vert);
	int * U = malloc(sizeof(int)*gr->n_vert);
	#pragma omp parallel for
	for(int i=0; i<gr->n_vert; i++)
	{
		M[i]=0;
		C[i]=INF;
		U[i]=INF;
	}
	M[S]=1;
	C[S]=0;
	U[S]=0;
	while(isEmpty(M, gr->n_vert)!=0)
	{
		for(int i=0; i<gr->n_vert; i++)
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
		for(int i=0; i< get_n_neighbor(gr, node_id); i++)
		{
			if(U[i] > C[node_id] + gr->V_ar[node_id].w_adj_list[i])
			{
				U[i] = C[node_id] + gr->V_ar[node_id].w_adj_list[i];
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
	char * M = malloc(sizeof(char)*gr->n_vert);
	int * C = malloc(sizeof(int)*gr->n_vert);
	int * U = malloc(sizeof(int)*gr->n_vert);
	for(int S=0; S < gr->n_vert; S++)
	{
		#pragma omp parallel for
		for(int i=0; i<gr->n_vert; i++)
		{
			M[i]=0;
			C[i]=INF;
			U[i]=INF;
		}
		M[S]=1;
		C[S]=0;
		U[S]=0;
		while(isEmpty(M, gr->n_vert)!=0)
		{
			for(int i=0; i<gr->n_vert; i++)
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