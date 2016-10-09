
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
	struct timeval tic, toc, time_result;
	gettimeofday(&tic,NULL);
	apsp_fw(gr);
	gettimeofday(&toc,NULL);
	timersub(&toc,&tic, &time_result);
	printf("%ld.%lu", time_result.tv_sec, time_result.tv_usec  );
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
