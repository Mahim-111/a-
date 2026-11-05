/*
Problem_1: Divide a large integer array among MPI processes. Each process exchanges its boundary elements with neighboring processes using point-to-point communication and computes a local result. 
Sample Input
Number of processes: 4
N = 12
Array: 10 20 30 40 50 60 70 80 90 100 110 120
Distribution:
P0: 10 20 30; 	P1: 40 50 60; 	P2: 70 80 90; 	P3: 100 110 120
Sample Output
P0: Right boundary sent = 30
P1: Left boundary received = 30
P1: Right boundary sent = 60
P2: Left boundary received = 60
P2: Right boundary sent = 90
P3: Left boundary received = 90
Boundary exchange completed successfully.
*/


#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int N = 12;
    int array[12] = {
        10, 20, 30, 40, 50, 60,
        70, 80, 90, 100, 110, 120
    };

    int local[3];
    int left_boundary = -1;
    int right_boundary = -1;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0)
            printf("Run with exactly 4 processes.\n");

        MPI_Finalize();
        return 0;
    }

    /* Distribute 3 elements to each process */
    MPI_Scatter(array, 3, MPI_INT,
                local, 3, MPI_INT,
                0, MPI_COMM_WORLD);

    /* Send right boundary to next process */
    if (rank < size - 1) {
        MPI_Send(&local[2], 1, MPI_INT,
                 rank + 1, 0, MPI_COMM_WORLD);
    }

    /* Receive left boundary from previous process */
    if (rank > 0) {
        MPI_Recv(&left_boundary, 1, MPI_INT,
                 rank - 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }

    /* Right boundary value */
    if (rank < size - 1)
        right_boundary = local[2];

    /* Print results */
    if (rank > 0)
        printf("P%d: Left boundary received = %d\n",
               rank, left_boundary);

    if (rank < size - 1)
        printf("P%d: Right boundary sent = %d\n",
               rank, right_boundary);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
        printf("Boundary exchange completed successfully.\n");

    MPI_Finalize();

    return 0;
}