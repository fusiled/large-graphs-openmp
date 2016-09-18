
typedef struct GraphMatrixStruct
{
	int n_vert;
	int n_edge;
	Weight ** W_ma;
	char with_weight;
} GraphMatrix;



GraphMatrix * newGraphMatrix(int, int, int);


void destroyGraphMatrix(GraphMatrix *);

void insertEdge(GraphMatrix *, int, int, int);