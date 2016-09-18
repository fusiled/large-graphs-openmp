
#include "GraphMatrix.h"

#include <stdlib.h>
#include <stdio.h>

GraphMatrix * newGraphMatrix(int n_vert, int n_edge, int with_weight)
{
	GraphMatrix * gr = malloc(sizeof(GraphMatrix));
	if(gr==NULL)
	{
		printf("Cannot instantiate GraphMatrix... exiting\n");
		exit(-1);
	}
	gr->n_vert=n_vert;
	gr->n_edge=n_edge;
	gr->with_weight=with_weight;
	gr->W_ma=malloc(sizeof(Weight*)*n_vert);
	if(gr->W_ma==NULL)
	{
		printf("Cannot instantiate Weight Matrix... exiting\n");
		exit(-1);
	}
	for(int i=0; i < gr->n_vert; i++)
	{
		gr->W_ma[i]=malloc(sizeof(Weight)*n_vert);
		if(gr->W_ma[i]==NULL)
		{
			printf("Cannot instantiate Weight Matrix... exiting\n");
			exit(-1);
		}
	}
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


void insertEdge(GraphMatrix * gr, int head, int tail, int weight)
{
	if(gr->with_weight==GRAPH_WITH_WEIGHT)
	{
		gr->W_ma[head][tail]=weight;
	}
	gr->W_ma[head][tail]=1;
}