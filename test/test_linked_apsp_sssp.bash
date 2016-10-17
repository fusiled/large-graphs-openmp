#!/bin/bash

#executables="main_bfs_run main_sssp_run main_apsp_sssp_run"
executables="main_apsp_sssp_run"
output_file="test_result/test_output_linked_"`date -Iseconds`
graph_path="graphs/test_graph.gr"

graph_type="0"
CORE_CAP="24"
n_vert_pow="3"

#make csv header
for exec in $executables
	do
		printf "n_vert," >>"$output_file"_"$exec"
		printf "n_threads," >>"$output_file"_"$exec"
		printf "t\n">> "$output_file"_"$exec"
done
#generate a graph with increasing number of vertices
while  [ $n_vert_pow -lt 16 ]
do
	n_vert=$(awk "BEGIN{print 2 ** $n_vert_pow}")
	./Randgraph/rg $graph_path $n_vert $graph_type 11 6
	echo "generated graph with $n_vert vertices"
	#iterate over num_cores
	core_power="0"
	while [ $core_power -lt 6 ]
	do
		n_core=$(awk "BEGIN{print 2 ** $core_power}")
		#check the limit. We don't have more than 24 cores
		if [ $n_core -gt $CORE_CAP ]; then
			n_core="24"
		fi
		#exec all the algos
		for exec in $executables
		do
			printf "%s," "$n_vert" >>"$output_file"_"$exec"
			printf "%s," "$n_core"  >>"$output_file"_"$exec"
			time_res=`bin/$exec $n_core $graph_path 1 0`
			printf "%s\n"  $time_res >> "$output_file"_"$exec"
		done
		core_power=$[$core_power+1]
	done
	n_vert_pow=$[$n_vert_pow+1]
done
