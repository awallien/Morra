How to Execute:

	The first step is to compile the main source file ( morra.c ) into an executable program using 
the mpicc compiler. The command is already written in the Makefile, so the user should enter:

		make morra

	Next, the user can specify whether to run the executable program on the local machine with its 
local processors or to run the executable program on a cluster of machines. The commands are written 
in the Makefile. The user needs to specify a number “X” that represents the number of rounds for each 
Morra game, and a number “Y” to assign the number of players for the Morra game. In addition, if the 
user wants to run the program on a cluster, then he needs to specify a hostfile “Z” that contains the 
hostnames of the other local machines.
The user may enter the commands as follows:
	If the program is running on a local machine:
		
		make run_local ROUNDS=X PLAYERS=Y
		
		Example: make run_local ROUNDS=5 PLAYERS=10
	
	If the program is running on a cluster of machines:
	
		make run_cluster ROUNDS=X PLAYERS=Y HOSTFILE=Z
	
		Example: make run_cluster PLAYERS=100 ROUNDS=20 HOSTFILE=hostfile 		
		where ‘hostfile’ is a valid file that contains all the hosts of the machines in a cluster
	
	If the program is running on a cluster of machines, and execution is divided among the 
	processors “round-robin” style:

		make run_round_robin ROUNDS=X PLAYERS=Y HOSTFILE=Z

		Example: make run_round_robin PLAYERS=500 ROUNDS=15 HOSTFILE=hostfile
		where ‘hostfile’ is a valid file that contains all the other hosts of the machines in cluster
