#!/bin/bash

executables="main_bfs_run main_sssp_run main_apsp_sssp_run"
output_file="test_result/test_output_linked"
graph_path="graphs/test_graph.gr"

graph_type="0"
CORE_CAP="4"
n_vert_pow="3"

rm -f "$output_file"
#generate a graph with increasing number of vertices
while  [ $n_vert_pow -lt 6 ]
do
	n_vert=$(awk "BEGIN{print 10 ** $n_vert_pow}")
	./Randgraph/rg $graph_path $n_vert $graph_type 11
	echo "generated graph with $n_vert vertices"
	echo "n_vert=$n_vert">>$output_file
	#iterate over num_cores
	core_power="0"
	while [ $core_power -lt 5 ]
	do
		n_core=$(awk "BEGIN{print 2 ** $core_power}")
		echo "n_thread=$n_core">>$output_file
		#check the limit. We don't have more than 24 cores
		if [ "$n_core" -gt $CORE_CAP ]; then
			core_power="24"
		fi
		#exec all the algos
		for exec in $executables
		do
		#	echo "executing $exec"
			"bin/$exec" "$n_core" "$graph_path" 1 0 >> "$output_file"
		done
		core_power=$[$core_power+1]
	done
	n_vert_pow=$[$n_vert_pow+1]
done
