

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



Graph * analyze_build_node(FILE *, int);
void analyze_build_edge(FILE *, Graph *);

Graph * parseFile(char * path, int graph_type)
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
	gr = analyze_build_node(fp, graph_type);
	if(gr==NULL)
	{
		printf("Something is wrong with the node building\n");
		exit(1);
	}
	analyze_build_edge(fp, gr);
	fclose(fp);
	return gr;
}

Graph * analyze_build_node(FILE * fp, int graph_type)
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
				gr = newGraph(n_vert_buf, n_edge_buf, GRAPH_WITH_WEIGHT, graph_type);
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
				//get tail
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
	//loop = read all the lines of the file
	while( read = getline(&line, &line_size, fp )!= -1 )
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
	finalizeEdges(gr);
}


FILE * genGraphFile(unsigned int n_vert, unsigned int n_edge, char * path)
{
	FILE * fp = fopen(path, "w");
	if(fp==NULL)
	{
		printf("genGraphFile cannot create file!");
		return NULL;
	}
	fprintf(fp, "c genGraphFile utility\n");
	fprintf(fp, "p sp %d %d\n", n_vert, n_edge);
	for(int i=1; i<=n_edge; i++)
	{
		int head, tail, weight;
		fprintf(fp, "a %d %d %d\n", head, tail, weight);
	}

	rewind(fp);
	return fp;
}


