
SRC = BoolArray.c GraphLinked.c GraphMatrix.c Graph.c parse_utils.c graph_algo.c main.c
CC = cc
#CFLAGS = -g -D DEBUG=1
CFLAGS = -O3
CFLAGS += -D TEST=1
OPENMP_FLAG= -fopenmp
OUT = run

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)_seq
	$(CC) $(CFLAGS) $(OPENMP_FLAG) $(SRC) -o $(OUT)_par
