///
/// file: morra.c
///
/// description:
///	the main program to play a game of morra
///
/// author:	Alexander Wall (asw8675)
///
/// date: 11/9/18
///


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define DELAY usleep( 1 )
#define NUM_FINGERS 5
#define ROOT 0

/// main function to get the number of rounds, initialize MPI, play morra,
/// and terminate MPI
/// @param argc: number of arguments, expected 2
/// @param argv: the command line argument contains the number of rounds
/// @return: SUCCESS or FAILURE
int main( int argc, char* argv[] ){

	// get number of rounds from command line argument
	if( argc != 2 ){

		fprintf( stderr, "Usage: ./morra number_of_rounds\n" );
		exit( EXIT_FAILURE );

	}

	int num_rounds;

	double start, finish;

	// validate number of rounds
	if( ( num_rounds = atoi( argv[1] ) ) < 0 ){

		fprintf( stderr, "Error: Not a valid number of rounds: %d\n", num_rounds );
		exit( EXIT_FAILURE );

	}

	start = MPI_Wtime();

	// Initialize MPI environment
	if( MPI_Init( &argc, &argv ) != MPI_SUCCESS ){

		fprintf( stderr, "MPI Initialization failed. \n" );
		exit( EXIT_FAILURE );

	}

	int ntasks, prank;
	int err_s = MPI_Comm_size( MPI_COMM_WORLD, &ntasks );
	int err_r = MPI_Comm_rank( MPI_COMM_WORLD, &prank );

	// validate number of "players" from the number of processors
	if ( ntasks < 2 ){

		fprintf( stderr, "Error: Need two or more players\n" );
		MPI_Abort( MPI_COMM_WORLD, EXIT_SUCCESS );
	}


	// check for problems with MPI_Comm just to shut up the
	// compiler for unused variables
	if( err_s + err_r != MPI_SUCCESS ){

		fprintf( stderr, "Error: problems with MPI_Comm\n" );
		MPI_Finalize();
		exit( EXIT_FAILURE );


	}

	
	// for """pure""" randomization in numbers
	srand( time( NULL )+prank );


	// for one player variables
	int fingers=0, guess=0, win_of_round=0, total_wins=0;
	// for all players variables
	int total_fingers=0, num_of_winners=0;
	int tally[ntasks];

	// play morra k number of rounds	
	for( int k=0; k<num_rounds; k++ ){

		// player's guess and fingers to extend
		// player's guess based on the number of its own fingers plus the others
		fingers = rand( ) % ( NUM_FINGERS+1 );
		guess = fingers + rand( ) % ( ( ntasks-1 ) * ( NUM_FINGERS+1 ) );

		// a delay for the print statements
		DELAY;
		printf( "I am %d. For run %d, I extend %d and my guess is %d.\n", 
					prank, k+1, fingers, guess );
		fflush( stdout );

		MPI_Allreduce( &fingers, &total_fingers, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD );
		
		// sync up the process before
		// getting total number of fingers
		MPI_Barrier( MPI_COMM_WORLD );


		// we have a ( potential ) winner!
		if( total_fingers == guess )
			win_of_round = 1;
			
		// gather all the winner(s) and send it to all the players
		MPI_Allreduce( &win_of_round, &num_of_winners, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD );

		// have one player announce the result of nobody winning
		if( prank == ROOT && !num_of_winners ){
		
			DELAY;
			printf( "Nobody won the run %d.\n", k+1 );
			fflush ( stdout );
		
		}
		// one winner
		else if( num_of_winners == 1 && win_of_round == 1 ){

			DELAY;
			printf( "I am %d, and I won the run %d.\n", prank, k+1 );
			fflush( stdout );
		
			total_wins++;

		}
		else if( num_of_winners > 1 && win_of_round == 1 ) {

			DELAY;
			printf( "I am %d, and I almost won the run %d.\n", prank, k+1 );
			fflush( stdout );

		}
	
		DELAY;	
		// reset win count for the next round
		win_of_round = 0;

		MPI_Barrier( MPI_COMM_WORLD );	
	

	}


	// tally up the results, the process finds its placement
	MPI_Allgather( &total_wins, 1, MPI_INT, tally, 1, MPI_INT, MPI_COMM_WORLD );

	int placement = 0;
	for( int j=0; j<ntasks; j++ )
		if( total_wins > tally[j] )
			placement++;


	MPI_Barrier( MPI_COMM_WORLD );

	printf( "Hey! I am %d, and I won %d time(s). Overall, I played better than %d player(s).\n",
			prank, total_wins, placement ); 


	finish = MPI_Wtime();

	// final barrier for printing the time
	MPI_Barrier( MPI_COMM_WORLD );

	// have one process print the trial run
	if( prank == 0 )
		printf( "For %d players and %d rounds, the time for this game is %.4lf.\n", ntasks,
				num_rounds, finish-start );

	// Terminate MPI and free stuff
	MPI_Finalize();


	return EXIT_SUCCESS;

}
