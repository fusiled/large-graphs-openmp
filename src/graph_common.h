

#ifndef GRAPH_COMMON_H
#define GRAPH_COMMON_H

#include <limits.h>

#define GRAPH_WITH_WEIGHT 1
#define GRAPH_WITHOUT_WEIGHT 0

#define GRAPH_TYPE_ADJ_LIST 0
#define GRAPH_TYPE_MATRIX 1

#define INF INT_MAX

#ifdef TEST
	#include <stdio.h>
	#define test_result_name "t_r"
	FILE * test_fp;
#endif

#endif
