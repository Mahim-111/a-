/*
Problem_17: Each MPI process generates data intended for every other process. Implement the communication using MPI_Alltoall and analyze its communication cost as the number of processes increases. 
Sample Input
Number of processes: 4
P0: 0 1 2 3
P1: 10 11 12 13
P2: 20 21 22 23
P3: 30 31 32 33
Each process sends one value to every process.
Sample Output
After MPI_Alltoall:
P0 received: 0 10 20 30
P1 received: 1 11 21 31
P2 received: 2 12 22 32
P3 received: 3 13 23 33
Students should measure communication time for P = 2, 4, 8, 16 and different message sizes.
*/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, send[4], recv[4];
    double t1, t2;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size != 4) {
        if(rank == 0)
            printf("Run with 4 processes\n");
        MPI_Finalize();
        return 0;
    }

    for(int i=0; i<4; i++)
        send[i] = rank*10 + i;

    MPI_Barrier(MPI_COMM_WORLD);
    t1 = MPI_Wtime();

    MPI_Alltoall(send, 1, MPI_INT,
                 recv, 1, MPI_INT,
                 MPI_COMM_WORLD);

    t2 = MPI_Wtime();

    printf("P%d received: ", rank);
    for(int i=0; i<4; i++)
        printf("%d ", recv[i]);

    printf("\nP%d communication time = %f s\n",
           rank, t2-t1);

    MPI_Finalize();
    return 0;
}
