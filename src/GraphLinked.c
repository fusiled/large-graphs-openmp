
#include "GraphLinked.h"
#include "graph_common.h"

#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
	#include <stdio.h>
#endif


int getNeighborNumber(GraphLinked *, int);

GraphLinked * newGraphLinked(int n_vert, int n_edge, int with_weight)
{
	GraphLinked * gr = malloc(sizeof(GraphLinked));
	if (gr==NULL)
	{
		printf("Cannot malloc graph\n");
		exit(1);
	}
	gr -> n_vert = n_vert;
	gr -> V_ar = calloc(sizeof(Vertex),n_vert);
	if(gr -> V_ar == NULL)
	{
		printf("Error on Vertex Array allocation." );
		exit(1);
	}
	gr -> n_edge = n_edge;
	gr -> E_ar = malloc(sizeof(int)*n_edge);
	if(gr -> E_ar == NULL)
	{
		printf("Error on Edge Array allocation\n" );
		exit(1);
	}
	if(with_weight)
	{
		gr -> W_ar = malloc(sizeof(int)*n_edge);
	}
	else
	{
		gr -> W_ar = NULL;
	}
	#ifdef DEBUG
		printf("Allocating a graph of %d vertices and %d edges\n", gr->n_vert, gr->n_edge );
	#endif
	return gr;
}

void destroyGraphLinked(GraphLinked * gr)
{
	free(gr->V_ar);
	free(gr->E_ar);
	if(gr -> W_ar != NULL)
	{
		free(gr -> W_ar);
	}
	free(gr);
	#ifdef DEBUG
		printf("GraphLinked destroyed\n" );
	#endif
}

void buildAdjListPointers(GraphLinked * gr, int * n_neighbor)
{
	#ifdef DEBUG
		//check correctness of n_neighbor
		int correct_counter = 0;
		for (int i=0; i<gr->n_vert; i++)
		{
			correct_counter+=n_neighbor[i];
		}
		if (correct_counter != gr->n_edge)
		{
			printf("wrong n_neighbors(%d, but should be %d)! check code\n", correct_counter, gr->n_edge);
			exit(1);
		}
	#endif
	//build pointers
	int counter=0;
	for(int i=0; i<gr->n_vert; i++)
	{
		if(n_neighbor[i]==0)
		{
			gr->V_ar[i].adj_list = NULL;
			if(gr->W_ar!= NULL)
			{
				gr->V_ar[i].w_adj_list = NULL;
			}
		}
		else
		{
			gr->V_ar[i].adj_list = gr->E_ar+counter;
			if(gr->W_ar!= NULL)
			{
				gr->V_ar[i].w_adj_list = gr->W_ar+counter;
			}
		}
		counter += n_neighbor[i];
	}
}



void insertEdgeLinked(GraphLinked * gr, int head, int tail, int weight, int index)
{
	if(index >= getNeighborNumber(gr,head) )
	{
		#ifdef DEBUG
			printf("Cannot insert edge in %d vertex: index of insertEdge out of n_neighbor: %d >= %d \n",head, index, getNeighborNumber(gr,head)  );
		#endif
		return;
	}
	gr->V_ar[head].adj_list[index] = tail;
	if(gr->W_ar != NULL)
	{
		gr->V_ar[head].w_adj_list[index] = weight;
	}
}

void printGraphLinked(GraphLinked * gr)
{
	for(int i=0; i< gr->n_vert; i++)
	{
		printf("Node: %d:", i+1);
		for (int j=0; j < getNeighborNumber(gr,i); j++)
		{
			printf("-> %d", gr->V_ar[i].adj_list[j]+1 );
		}
		printf("\n");
	}
}

int getNeighborNumber(GraphLinked * gr, int index)
{
	//check bounds
	if(index >= gr->n_vert || index < 0)
	{
		#ifdef DEBUG
			printf("index out of bounds in getNeighborNumber\n");
		#endif
		return -1;
	}
	//there are no neighbors
	if(gr->V_ar[index].adj_list==NULL)
	{
		return 0;
	}
	//corner case: index is the last element
	if(index == gr->n_vert-1)
	{
		return( gr->E_ar+(gr->n_edge) - gr->V_ar[index].adj_list) ;
	}
	//common case
	int * adj_ref = gr->V_ar[index].adj_list;
	int * next_adj_ref = NULL;
	int buf_next_index = index +1;
	//search for the next pointer in the edge array remaining into V_ar bounds
	while(buf_next_index < gr->n_vert && gr->V_ar[buf_next_index].adj_list==NULL)
	{
		buf_next_index++;
	}
	//corner case: our buf_next_index is the last element and it has no neighbors -> take ref of the last element
	//of E_ar (that is the last element of the adj_list of index).
	next_adj_ref = (buf_next_index >= gr->n_vert && gr->V_ar[buf_next_index-1].adj_list==NULL ) ?
						gr->E_ar+(gr->n_edge) :
						gr->V_ar[buf_next_index].adj_list;
	//return n_neighbor
	return (next_adj_ref - adj_ref);
}


int * getNeighborsLinked(GraphLinked * gr, int index, int * n_neighbor_ref)
{
	int n_neighbor = getNeighborNumber(gr, index);
	int * ret_ref = malloc(sizeof(int)*n_neighbor);
	memcpy(ret_ref, gr->V_ar[index].adj_list, sizeof(int)*n_neighbor);
	*n_neighbor_ref = n_neighbor;
	return ret_ref;
}


int getVertexNumberLinked(GraphLinked * gr)
{
	return gr->n_vert;
}


int getWeightLinked(GraphLinked * gr, int head, int tail)
{
	int i;
	int n_neighbor =  getNeighborNumber(gr,head);
	for(i=0; i < n_neighbor; i++)
	{
		if(gr->V_ar[head].adj_list[i]==tail)
		{
			break;
		}
	}
	if( i >= n_neighbor )
	{
		#ifdef DEBUG
			printf("%d and %d are not neighbors! Returning 0\n", head,tail);
		#endif
			return 0;
	}
	return gr->V_ar[head].w_adj_list[i];
}