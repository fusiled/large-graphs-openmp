

#ifndef GRAPH_COMMON_H
#define GRAPH_COMMON_H


#define GRAPH_WITH_WEIGHT 1
#define GRAPH_WITHOUT_WEIGHT 0

#define GRAPH_TYPE_ADJ_LIST 0
#define GRAPH_TYPE_MATRIX 1

#ifdef TEST
	#include <stdio.h>
	#define test_result_name "test_result"
	FILE * test_fp;
#endif

#endif
