/*
Problem_6: Divide a one-dimensional array among MPI processes. 
Exchange halo/boundary values between neighboring processes 
using MPI_Send and MPI_Recv, then perform a stencil computation. 
Sample Input
Number of processes: 4
Array: 10 20 30 40 50 60 70 80
Radius = 1
Distribution:
P0: 10 20
P1: 30 40
P2: 50 60
P3: 70 80
Stencil: out[i] = left[i] + current[i] + right[i]
Sample Output
After halo exchange:
P0: [10 20] + right halo 30
P1: left halo 20, [30 40], right halo 50
P2: left halo 40, [50 60], right halo 70
P3: left halo 60, [70 80]
Stencil result (using the stated simple boundary convention):
30 60 90 120 150 180 210 230
*/

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int a[8] = {10,20,30,40,50,60,70,80};
    int local[2], result[2];
    int left = 0, right = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0) printf("Run with 4 processes.\n");
        MPI_Finalize();
        return 0;
    }

    MPI_Scatter(a, 2, MPI_INT, 
            local, 2, MPI_INT,
            0, MPI_COMM_WORLD);

    /* Exchange left and right halo values */
    if (rank > 0)
        MPI_Sendrecv(&local[0], 1, MPI_INT, rank-1, 0,
                     &left, 1, MPI_INT, rank-1, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    if (rank < size-1)
        MPI_Sendrecv(&local[1], 1, MPI_INT, rank+1, 1,
                     &right, 1, MPI_INT, rank+1, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    /* Stencil calculation */
    result[0] = left + local[0] + local[1];
    result[1] = local[0] + local[1] + right;

    printf("P%d: %d %d\n", rank, result[0], result[1]);

    MPI_Finalize();
    return 0;
}