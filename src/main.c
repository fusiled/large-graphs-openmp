
#include <stdio.h>

#include "Graph.h"
#include "parse_utils.h"
#include "graph_common.h"

#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define CHAL9 0
#define RANDGRAPH 1

void printHelp();

int main(int argc, char const *argv[])
{
	if(argc == 5)
	{
		int n_threads = atoi(argv[1]);
		#ifdef _OPENMP
			omp_set_num_threads(n_threads);
			#ifdef DEBUG
				printf("Starting with %d threads\n", omp_get_max_threads() );
			#endif
		#else
			printf("OpenMp is not enabled. n_threads will be ignored. Try to recompile if you need it\n");
		#endif
	}
	else
	{
		printf("WRONG PARAMETERS D:\n");
		printHelp();
		return -1;
	}
	Graph * gr;
	char * path = strdup(argv[2]);
	int graph_type = atoi(argv[4]);
	#ifdef TEST
		test_fp = fopen(test_result_name, "w");
		fclose(test_fp);
	#endif
	switch( atoi(argv[3]) )
	{
		case CHAL9: 
			gr = parseFileChallenge9(path, graph_type, GRAPH_WITH_WEIGHT ); 
		break;
		case RANDGRAPH:
			gr = parseFile(path, graph_type, GRAPH_WITH_WEIGHT );
		break;
		default:
			printf("Unknown parseType value. See help. Exiting");
		return -1;
	}
	free(path);
	//printGraph(gr);
	printf("starting bfs\n");
	struct timeval tic, toc, time_result;
	/*
	double * time_bfs = malloc(sizeof(double)*gr->n_vert);
	for(int node_id=0; node_id<gr->n_vert; node_id++)
	{
		gettimeofday(&tic,NULL);
		bfs(gr, node_id);
		gettimeofday(&toc,,NULL);
		time_bfs[node_id] = ((double) (toc-tic) /CLOCKS_PER_SEC);
	}
	double time_avg_bfs = 0;
	for(int i=0; i< gr->n_vert; i++)
	{
		time_avg_bfs += time_bfs[i];
	}
	time_avg_bfs = time_avg_bfs / gr->n_vert;
	printf("avg_bfs = %.2f", time_avg_bfs);
	free(time_bfs);
	*/
	gettimeofday(&tic,NULL);
	bfs(gr, 0);
	gettimeofday(&toc,NULL);
	timersub(&toc,&tic, &time_result);
	printf("bfs: %ld,%lu sec\n", time_result.tv_sec, time_result.tv_usec  );
	printf("bfs ended\n");
	printf("sssp begin\n");
	gettimeofday(&tic,NULL);
	sssp(gr, 0);
	gettimeofday(&toc,NULL);
	timersub(&toc,&tic, &time_result);
	printf("sssp: %ld,%lu sec\n", time_result.tv_sec, time_result.tv_usec  );
	printf("sssp ended\n");
	/*printf("apsp_sssp begin\n");
	gettimeofday(&tic,NULL);
	apsp_sssp(gr);
	gettimeofday(&toc,NULL);
	timersub(&toc,&tic, &time_result);
	printf("apsp_sssp: %ld,%lu sec\n", time_result.tv_sec, time_result.tv_usec  );
	printf("apsp_sssp ended\n");*/
	printf("apsp_fw begin\n");
	gettimeofday(&tic,NULL);
	apsp_fw(gr);
	gettimeofday(&toc,NULL);
	timersub(&toc,&tic, &time_result);
	printf("apsp_fw: %ld,%lu sec\n", time_result.tv_sec, time_result.tv_usec  );
	printf("apsp_fw ended\n");
	destroyGraph(gr);
	return 0;
}


void printHelp()
{
	printf("Usage: EXEC_FILE n_threads path_to_graph parser_type graph_type\n");
	printf("-n_threads: the number of threads to use\n-path_to_graph: the path to the graph\n");
	printf("-parser_type: 0 use challenge9 parser, 1 the randgraph parser\n");
	printf("graph_type: 0 compact adjacency list, 1 matrix\n");
}
