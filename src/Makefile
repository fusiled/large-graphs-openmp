
SRC = Graph.c parse_utils.c graph_algo.c main.c
CC = cc
CFLAGS = -g -D DEBUG=1
CFLAGS+= -fopenmp
OUT = run

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

