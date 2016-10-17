#!/bin/bash

executables="main_bfs_run main_sssp_run"
#executables="main_bfs_run"
output_file="test_result/test_output_free_scale_"`date -Iseconds`
gr_path1="graphs/gr1.gr"
gr_path2="graphs/gr2.gr"
graph_path="graphs/test_graph.gr"

graph_type="0"
CORE_CAP="24"
n_vert_pow="3"
normal_degree="6"
high_degree="1000"

merge_executable="./utils/mergeGraphs"

#make csv header
for exec in $executables
	do
		printf "n_vert," >>"$output_file"_"$exec"
		printf "n_threads," >>"$output_file"_"$exec"
		printf "t\n">> "$output_file"_"$exec"
done
#generate a graph with increasing number of vertices
while  [ $n_vert_pow -lt 8 ]
do
	n_vert=$(awk "BEGIN{print 10 ** $n_vert_pow}")
	low_vert=$(($n_vert/1000 ))
	./Randgraph/rg $gr_path1 $low_vert $graph_type 11 $high_degree
	./Randgraph/rg $gr_path2 $n_vert $graph_type 11 $normal_degree
	$merge_executable $gr_path1 $gr_path2 $graph_path
	rm $gr_path1
	rm $gr_path2
	echo "generated graph with $n_vert vertices, small graph was $low_vert"
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
