
#ifndef PARSE_UTILS_H
#define PARSE_UTILS_H

#include "Graph.h"

#include <stdio.h>


//parse the .gr file at the given path. It returns a Graph structure
Graph * parseFileChallenge9(char *, int, int);

Graph * parseFile(char *, int, int);


#endif