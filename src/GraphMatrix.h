
#ifndef GRAPH_MATRIX_H
#define GRAPH_MATRIX_H

typedef struct GraphMatrixStruct
{
	int n_vert;
	int ** W_ma;
	char with_weight;
} GraphMatrix;



GraphMatrix * newGraphMatrix(int, int);

void destroyGraphMatrix(GraphMatrix *);

void insertEdgeMatrix(GraphMatrix *, int, int, int);

int * getNeighborsMatrix(GraphMatrix *, int, int *);

void printGraphMatrix(GraphMatrix *);

int getVertexNumberMatrix(GraphMatrix *);


int getWeightMatrix(GraphMatrix *, int, int);

GraphMatrix * duplicateGraphMatrix(GraphMatrix *);


int ** apsp_fw_matrix(GraphMatrix *);

#endif
