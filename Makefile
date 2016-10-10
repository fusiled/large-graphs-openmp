#NOTES: 
# - $^ is the left side of the rule (the preconditions)
# - $@ is the right side of the rule (the name of the rule)
#
# - the function $(addprefix prefix, string_list) adds the prefix to all the 
#   elements in string_list
# - $(string_list:old_suffix=new_suffix) is very handy for going from .c files
#   to .o files (or similar).
# - $(patsubst src_regex,dest_regex, string_list) is a string substitution utility.
#   it substitute exploiting the 2 regexes all the elements of string_list


#source file EXCEPT MAIN FILES here
SRC = BoolArray.c GraphLinked.c GraphMatrix.c Graph.c parse_utils.c graph_algo.c
#put MAINs down here
MAIN = main_bfs.c main_sssp.c main_apsp_fw.c main_apsp_sssp.c
#your compiler
CC = cc

TEST_BIN=test_real.bash test_matrix.bash test_linked.bash
#TEST_BIN=easy.bash easy.bash

#your flags
#CFLAGS = -g -D DEBUG=1
CFLAGS = -O3
#CFLAGS += -D TEST=1
CFLAGS+= -fopenmp

#put source here
SRC_DIR=src

#executables will be saved here
BIN_DIR=bin
#object files will be saved here (for the reuse)
OBJ_DIR=obj
TEST_BIN_DIR=test

OBJECTS =$(SRC:.c=.o)
MAIN_OBJ = $(MAIN:.c=.o)
MAIN_NAME = $(MAIN:.c=_run)
MAIN_RULE = $(addprefix $(BIN_DIR)/, $(MAIN_NAME))
#all the object files excepts main object files
OBJECTS_PATH =$(addprefix $(OBJ_DIR)/,$(OBJECTS)) 
#all the object files
MAIN_OBJ_PATH=$(addprefix $(OBJ_DIR)/,$(MAIN_OBJ))
TEST_EXEC=$(addsuffix && ,$(addprefix $(TEST_BIN_DIR)/, $(TEST_BIN)) )
TEST_EXEC+= echo "finished"

#if nothing is passed to the make command do this. It the starting point of the
# compilation
all: $(MAIN_RULE)

#build executables in bin_dir. Creates bin_dir and obj_dir if needed. 
# also compile objects files using prerequisites.
#   NB: it compiles also main object files that are not needed for the current
#       rule. A future fix will be to compile only the correct main object
$(MAIN_RULE): $(BIN_DIR) $(OBJ_DIR) $(OBJECTS_PATH) $(MAIN_OBJ_PATH) 
	$(CC) $(OBJECTS_PATH) $(patsubst $(BIN_DIR)/%_run,$(OBJ_DIR)/%.o,$@) -o $@ $(CFLAGS)

#build objects taking source from src dir
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $^ -o $@ $(CFLAGS)

#create dirs
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

#remove all the object files
clean:
	rm $(OBJ_DIR)/*.o

#clean and build
full: clean all


test: $(MAIN_RULE)
	$(TEST_EXEC)
