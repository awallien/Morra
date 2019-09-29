#
# description: contains the commands to make the executable program and
#	run the program with the local machine or across a cluster
#
# author: Alexander Wall
# 
# date: 11/9/18
#

SOURCE_FILE=morra.c
OUTPUT_FILE=morra

# default values
PLAYERS=2
HOSTFILE=hostfile
ROUNDS=1

# compiling the morra source file
morra: ${SOURCE_FILE}
	mpicc -ggdb -Wall -Wextra -pedantic ${SOURCE_FILE} -o ${OUTPUT_FILE}

# clean up
clean:
	rm ${OUTPUT_FILE}

# for distributing among process on local machines
run_local:
	mpiexec -n ${PLAYERS} ./${OUTPUT_FILE} ${ROUNDS}

# for distributing among the other machines
run_cluster:
	mpiexec --mca btl_tcp_if_include eno1 --hostfile ${HOSTFILE} -n ${PLAYERS} ./${OUTPUT_FILE} ${ROUNDS}

# for distributing among cluster round-robin style
run_round_robin:
	mpiexec --map-by node:SPAN --hostfile ${HOSTFILE} -n ${PLAYERS} ./${OUTPUT_FILE} ${ROUNDS}
