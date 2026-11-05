/*
Problem_11: The root process reads configuration parameters 
and broadcasts them to all MPI processes. Each process uses 
the received parameters to perform a computation. 
Sample Input
Number of processes: 4
Root process reads:
Number of iterations = 1000
Threshold = 50
Block size = 256
Sample Output
P0 received: Iterations = 1000, Threshold = 50, Block size = 256
P1 received: Iterations = 1000, Threshold = 50, Block size = 256
P2 received: Iterations = 1000, Threshold = 50, Block size = 256
P3 received: Iterations = 1000, Threshold = 50, Block size = 256
*/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank;
    int iterations, threshold, block_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0) {
        iterations = 1000;
        threshold  = 50;
        block_size = 256;
    }

    MPI_Bcast(&iterations,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&threshold,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&block_size,1,MPI_INT,0,MPI_COMM_WORLD);

    printf("P%d: Iterations=%d, Threshold=%d, Block=%d\n",
           rank,iterations,threshold,block_size);

    MPI_Finalize();
    return 0;
}

