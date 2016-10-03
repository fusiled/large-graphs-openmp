

#include "parse_utils.h"

#include "Graph.h"
#include "graph_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_GRAPH_HEADER 'p'
#define FILE_EDGE_IDENTIFIER 'a'
#define FILE_COMMENT 'c'
#define SPACE_STRING " "



Graph * analyze_build_node_challenge_9(FILE *, int, int);
void analyze_build_edge_challenge_9(FILE *, Graph *);

Graph * analyze_build_node(FILE *, int, int);
void analyze_build_edge(FILE *, Graph *);

Graph * parseFileChallenge9(char * path, int graph_type, int with_weight)
{
	//open the file
	FILE * fp = fopen(path, "r");
	if(fp==NULL)
	{
		printf("Error on opening the file\n");
		exit(1);
	}
	//build the graph using analyzeFile
	Graph * gr=NULL;
	gr = analyze_build_node_challenge_9(fp, graph_type, with_weight);
	if(gr==NULL)
	{
		printf("Something is wrong with the node building\n");
		exit(1);
	}
	analyze_build_edge_challenge_9(fp, gr);
	fclose(fp);
	return gr;
}

Graph * analyze_build_node_challenge_9(FILE * fp, int graph_type, int with_weight)
{	
	Graph * gr;
	//start from the beginning of the file
	rewind(fp);
	//init file vars
	size_t line_size = 0;
	ssize_t read;
	char * line = NULL;
	//loop = read all the lines of the file
	while( read = getline(&line, &line_size, fp )!= -1 )
	{
		char * line_copy=NULL;
		char * buf=NULL;
		//decide what to do with a line of file
		switch(line[0])
		{
			case FILE_GRAPH_HEADER:
				//otherwise create the new graph
				line_copy = strdup(line);
				//get p
				buf = strtok(line_copy, SPACE_STRING);
				//get sp
				buf = strtok(NULL, SPACE_STRING);
				buf = strtok(NULL, SPACE_STRING);
				int n_vert_buf = atoi(buf);
				buf = strtok(NULL, SPACE_STRING);
				int n_edge_buf = atoi(buf);
				gr = newGraph(n_vert_buf, n_edge_buf, with_weight, graph_type);
				if(gr==NULL)
				{
					free(buf);
					free(line_copy);
					printf("Error on graph malloc\n");
					exit(1);
				}
			break;
			case FILE_EDGE_IDENTIFIER:
				if(gr == NULL)
				{
					printf("Wrong syntax in graph file. Must specify the header first\n");
				}
				line_copy = strdup(line);
				//get a
				buf = strtok(line_copy, SPACE_STRING);
				buf = strtok(NULL, SPACE_STRING);
				int head = atoi(buf);
				head -= 1;
				if(graph_type==GRAPH_TYPE_ADJ_LIST)
				{
					increaseNeighborNumber(gr, head);
				}
			break;
			case FILE_COMMENT: //nothing to do. Ingore this line
			break;
			default:
				printf("Unknown rule ' %s '. Ignoring it...\n", line );
		}
		free(line_copy);
	}
	finalizeVertices(gr);
	return gr;
}


void analyze_build_edge_challenge_9(FILE * fp, Graph * gr)
{
	if(gr == NULL)
	{
		printf("You must call the function analyzeFile with PARSE_TYPE_BUILD_NODE before calling it with PARSE_TYPE_BUILD_EDGES");
		exit(1);
	}
	//start from the beginning of the file
	rewind(fp);
	//init file vars
	size_t line_size = 0;
	ssize_t read;
	char * line = NULL;
	//loop = read all the lines of the file
	while( read = getline(&line, &line_size, fp )!= -1 )
	{
		#pragma omp task
		{
		char * line_copy=NULL;
		char * buf=NULL;
		//decide what to do with a line of file
		switch(line[0])
		{
			case FILE_COMMENT: //nothing to do in these cases
			case FILE_GRAPH_HEADER:
			break;
			case FILE_EDGE_IDENTIFIER:
				if(gr == NULL)
				{
					free(line);
					printf("Wrong syntax in the opened file. Header must come before the arc list\n");
					exit(1);
				}
				line_copy = strdup(line);
				//get a
				buf = strtok(line_copy, SPACE_STRING);
				//get tail
				buf = strtok(NULL, SPACE_STRING);
				int head = atoi(buf);
				//build edge
				buf = strtok(NULL, SPACE_STRING);
				int tail = atoi(buf);
				buf = strtok(NULL, SPACE_STRING);
				int weight = atoi(buf);
				head -= 1;
				tail -= 1;
				insertEdge(gr, head, tail, weight);
			break;
			default:
				printf("Unknown rule ' %s '. Ignoring it..\n", line );
		}
		free(line_copy);
		}
		#pragma omp nowait
	}
	finalizeEdges(gr);
}


Graph * parseFile(char * path, int graph_type, int with_weight)
{
	//open the file
	FILE * fp = fopen(path, "r");
	if(fp==NULL)
	{
		printf("Error on opening the file\n");
		exit(1);
	}
	//build the graph using analyzeFile
	Graph * gr=NULL;
	gr = analyze_build_node(fp, graph_type, with_weight);
	if(gr==NULL)
	{
		printf("Something is wrong with the node building\n");
		exit(1);
	}
	analyze_build_edge(fp, gr);
	fclose(fp);
	return gr;
}

Graph * analyze_build_node(FILE * fp, int graph_type, int with_weight)
{	
	Graph * gr;
	//start from the beginning of the file
	rewind(fp);
	//init file vars
	size_t line_size = 0;
	ssize_t read;
	char * line = NULL;
	getline(&line, &line_size, fp);
	int n_vert = atoi(line);
	int n_edge = 0;
	//loop = read all the lines of the file
	while( read = getline(&line, &line_size, fp )!= -1 )
	{
		if(atoi(line)==-1)
		{
			break;
		}
		n_edge++;
	}
	gr = newGraph(n_vert, n_edge, with_weight, graph_type);
	rewind(fp);
	//skip first line
	getline(&line, &line_size, fp);
	//loop = read all the lines of the file
	while( read = getline(&line, &line_size, fp )!= -1 )
	{
		if(atoi(line)==-1)
		{
			break;
		}
		char * line_copy =strdup(line);
		char * buf =strtok(line_copy, SPACE_STRING);
		int head = atoi(buf);
		if(graph_type==GRAPH_TYPE_ADJ_LIST)
			{
				increaseNeighborNumber(gr, head);
			}
		free(line_copy);
	}
	finalizeVertices(gr);
	return gr;
}


void analyze_build_edge(FILE * fp, Graph * gr)
{
	if(gr == NULL)
	{
		printf("You must call the function analyzeFile with PARSE_TYPE_BUILD_NODE before calling it with PARSE_TYPE_BUILD_EDGES");
		exit(1);
	}
	//start from the beginning of the file
	rewind(fp);
	//init file vars
	size_t line_size = 0;
	ssize_t read;
	char * line = NULL;
	//skip first line
	getline(&line, &line_size, fp );
	//loop = read all the lines of the file
	char out_flag = 0;
	while( read = getline(&line, &line_size, fp )!= -1 && out_flag==0)
	{
		#pragma omp task shared(out_flag)
		{
			if(atoi(line)==-1)
			{
				out_flag=1;
			}
			else
			{
				char * line_copy =strdup(line);
				char * buf = strtok(line_copy, SPACE_STRING);
				int head = atoi(buf);
				buf = strtok(line_copy, SPACE_STRING);
				int tail = atoi(buf);
				buf = strtok(line_copy, SPACE_STRING);
				int weight = atoi(buf);
				insertEdge(gr, head, tail, weight);
				free(line_copy);
			}
		}
		#pragma omp nowait
	}
	finalizeEdges(gr);
}
