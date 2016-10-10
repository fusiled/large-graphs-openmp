#!/bin/bash

executables="main_bfs_run main_sssp_run "
output_file="test_result/test_output_real_"`date -Iseconds`
graph_path="/temp/fusi/"

CORE_CAP="24"
n_vert_pow="3"

#make csv header
for exec in $executables
	do
		printf "graph," >>"$output_file"_"$exec"
		printf "n_threads," >>"$output_file"_"$exec"
		printf "t\n">> "$output_file"_"$exec"
done
#iterate over graph_path
for graph in `ls $graph_path`
do
	echo "testing graph $graph"
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
			printf "%s," "$graph" >>"$output_file"_"$exec"
			printf "%s," "$n_core"  >>"$output_file"_"$exec"
			time_res=`bin/$exec $n_core $graph_path$graph 0 0`
			printf "%s\n" "$time_res" >> "$output_file"_"$exec"
		done
		core_power=$[$core_power+1]
	done
done
