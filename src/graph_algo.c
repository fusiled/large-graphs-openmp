
#include "graph_algo.h"

#include "Graph.h"
#include "BoolArray.h"
#include "graph_common.h"

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>

void print_int_array(int *, int);

void bfs_kernel(int, Graph *, BoolArray *, BoolArray *, int *);

void sssp_base(Graph *, int, char);
void sssp_kernel_1(int, Graph *, BoolArray *, int *, int *);
void sssp_kernel_2(int, Graph *, BoolArray *, int *, int *);



void print_int_array(int * ar, int size)
{
	printf("int array @%d: ",ar);
	for(int i=0; i<size; i++)
	{
		printf("%d ",ar[i] );
	}
	printf("\n");
}

void bfs_common(Graph * gr, int S)
{
	//instance and init value
	BoolArray * F = newBoolArray(getVertexNumber(gr));
	BoolArray * X = newBoolArray(getVertexNumber(gr));
	int * C = malloc(sizeof(int)*getVertexNumber(gr));
	#pragma omp parallel for shared(F,X,C)
	for(int i=0; i<getVertexNumber(gr); i++)
	{
		setValue(F,i, UNS_FALSE);
		setValue(X,i, UNS_FALSE);
		C[i]=INF;
	}
	setValue(F,S, UNS_TRUE);
	C[S]=0;
	while(isEmpty(F)!=UNS_TRUE)
	{
		#pragma omp parallel shared(gr,F,X,C)
		#pragma omp for nowait
		for(int node_id=0; node_id < getVertexNumber(gr); node_id++)
		{
			//#pragma omp parallel
			//#pragma omp single nowait
			//{
			//#pragma omp task
				bfs_kernel(node_id, gr, F, X, C);
			//#pragma omp nowait
			//}
		}
	}
	#ifdef TEST
		test_fp = fopen(test_result_name, "a");
		fprintf(test_fp, "---bfs_common---RESULT---source:%d---\n", S );
		fprintf(test_fp, "node: cost (number of arcs, weight is ignored)\n");
		for(int i=0; i<getVertexNumber(gr); i++)
		{
			fprintf(test_fp, "%d: ",i );
			if(C[i]==INF)
			{
				fprintf(test_fp, "INF\n");
			}
			else
			{
				fprintf(test_fp, "%d\n", C[i] );
			}
		}
		fprintf(test_fp, "----------------------------------------\n");
		fclose(test_fp);
	#endif
	destroyBoolArray(F);
	destroyBoolArray(X);
	free(C);

}

void bfs_kernel(int node_id, Graph * gr, BoolArray * F, BoolArray * X, int * C)
{
	if(getValue(F,node_id)==UNS_TRUE)
	{
		setValue(F,node_id, UNS_FALSE);
		setValue(X,node_id,UNS_TRUE);
		int n_neighbor;
		int * neighbors = getNeighbors(gr, node_id, &n_neighbor);
		for(int i=0; i < n_neighbor; i++ )
		{
			int nid = neighbors[i];
			if(getValue(X,nid)==UNS_FALSE)
			{
				C[nid]=C[node_id]+1;
				setValue(F,nid,UNS_TRUE);
			}
		}
		free(neighbors);
	}
}

/**public interface. The default is to enable parallelism*/
void sssp_common(Graph * gr, int S)
{
	sssp_base(gr, S, 1);
}


void sssp_base(Graph * gr, int S, char enable_parallelism)
{
	BoolArray * M = newBoolArray(getVertexNumber(gr));
	int * C = malloc(sizeof(int)*getVertexNumber(gr));
	int * U = malloc(sizeof(int)*getVertexNumber(gr));
	#pragma omp parallel for shared(M,U,C) if(enable_parallelism)
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
			#pragma omp parallel if(enable_parallelism)
			#pragma omp single nowait
			{
				#pragma omp task if(enable_parallelism)
				{
					sssp_kernel_1(i, gr, M, C, U);
				}
				#pragma omp nowait
			} 
		}
		for(int node_id=0; node_id<getVertexNumber(gr); node_id++)
		{
			#pragma omp parallel if(enable_parallelism)
			#pragma omp single nowait
			{
				#pragma omp task if(enable_parallelism)
				{
					if(C[node_id] > U[node_id] )
					{
						C[node_id] = U[node_id];
						setValue(M,node_id,UNS_TRUE);
					}
				}
				#pragma omp nowait
			} 
		}
		//do copy in 1 step
		memcpy(U,C, sizeof(int)*getVertexNumber(gr));
	}
	#ifdef TEST
	if(enable_parallelism==1)
	{
		test_fp = fopen(test_result_name, "a");
		fprintf(test_fp, "---sssp_common---RESULT---source:%d---\n", S );
		fprintf(test_fp, "node: cost\n");
		for(int i=0; i<getVertexNumber(gr); i++)
		{
			fprintf(test_fp, "%d: ",i );
			if(C[i]==INF)
			{
				fprintf(test_fp, "INF\n");
			}
			else
			{
				fprintf(test_fp, "%d\n", C[i] );
			}
		}
		fprintf(test_fp, "----------------------------------------\n");
		fclose(test_fp);
	}
	#endif
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
			int updated_cost = (C[node_id]==INF || getWeight(gr,node_id, neighbor)==INF) ? INF : C[node_id] + getWeight(gr,node_id,neighbor);
			if( U[neighbor] > updated_cost )
			{
				U[neighbor] = updated_cost;
			}
		}
		free(neighbors);
	}
}

void sssp_kernel_2(int node_id, Graph * gr, BoolArray * M, int * C, int * U)
{
	if(C[node_id] > U[node_id] )
	{
		C[node_id] = U[node_id];
		setValue(M,node_id,UNS_TRUE);
	}
	U[node_id] = C[node_id];
}


void apsp_sssp_common(Graph * gr)
{
	#ifdef TEST
		test_fp = fopen(test_result_name, "a");
		fprintf(test_fp, "---apsp_sssp---RESULTS------\n");
		fclose(test_fp);
	#endif
	#pragma omp parallel for shared(gr)
	for(int S=0; S < getVertexNumber(gr); S++)
	{
		sssp_base(gr, S, 0);
	}
	#ifdef TEST
		test_fp = fopen(test_result_name, "a");
		fprintf(test_fp, "----------------------------------------\n");
		fclose(test_fp);
	#endif
}
