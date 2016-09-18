
#ifndef PARSE_UTILS_H
#define PARSE_UTILS_H

#include "Graph.h"

#include <stdio.h>


//parse the .gr file at the given path. It returns a Graph structure
Graph * parseFile(char *, int);

//generate a random graph with the given number of nodes and edges
FILE * genGraphFile(unsigned int, unsigned int, char *);


#endif