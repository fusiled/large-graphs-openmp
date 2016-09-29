
#include <stdio.h>

#include "Graph.h"
#include "parse_utils.h"

#include <time.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char const *argv[])
{
	if(argc == 3)
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
		printf("Wrong parameters. Pass FIRST the number of threads, then the path to the graph file!\n");
		return -1;
	}
	Graph * gr =parseFileChallenge9(argv[2], GRAPH_TYPE_ADJ_LIST, GRAPH_WITH_WEIGHT );
	//printGraph(gr);
	printf("starting bfs\n");
	clock_t tic, toc;
	/*
	double * time_bfs = malloc(sizeof(double)*gr->n_vert);
	for(int node_id=0; node_id<gr->n_vert; node_id++)
	{
		tic = clock();
		bfs(gr, node_id);
		toc = clock();
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
	tic = clock();
	bfs(gr, 1);
	toc = clock();
	double bfs_time = ( ((double)(toc-tic)) /CLOCKS_PER_SEC);
	printf("bfs: %f seconds\n", bfs_time );
	printf("bfs ended\n");
	printf("sssp begin\n");
	tic = clock();
	sssp(gr, 1);
	toc = clock();
	double sssp_time = ( ((double)(toc-tic)) /CLOCKS_PER_SEC);
	printf("sssp: %f seconds\n", sssp_time );
	printf("sssp ended\n");
	printf("apsp_sssp begin\n");
	tic = clock();
	apsp_sssp(gr);
	toc = clock();
	double apsp_sssp_time = ( ((double)(toc-tic)) /CLOCKS_PER_SEC);
	printf("apsp_sssp: %f seconds\n", apsp_sssp_time );
	printf("apsp_sssp ended\n");
	/*printf("apsp_fw begin\n");
	tic = clock();
	apsp_fw(gr);
	toc = clock();
	double apsp_fw_time = ( ((double)(toc-tic)) /CLOCKS_PER_SEC);
	printf("apsp_fw: %f seconds\n", apsp_fw_time );
	printf("apsp_fw ended\n");*/
	destroyGraph(gr);
	return 0;
}