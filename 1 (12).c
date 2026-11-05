/*
Problem_12: Use MPI_Scatter to distribute a large vector among processes. Each process calculates the square or cube of its elements, and MPI_Gather collects the results at the root process. 
Sample Input
Number of processes: 4
Vector: 1 2 3 4 5 6 7 8

After MPI_Scatter:
P0: 1 2
P1: 3 4
P2: 5 6
P3: 7 8
Operation: y[i] = x[i]^2

Sample Output
P0: 1 4
P1: 9 16
P2: 25 36
P3: 49 64

Gathered result: 1 4 9 16 25 36 49 64
*/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, local[2], result[8];
    int data[8] = {1,2,3,4,5,6,7,8};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Scatter(data,2,MPI_INT,
                local,2,MPI_INT,
                0,MPI_COMM_WORLD);

    /* Local computation */
    for(int i=0;i<2;i++)
        local[i] = local[i] * local[i];

    MPI_Gather(local,2,MPI_INT,
               result,2,MPI_INT,
               0,MPI_COMM_WORLD);

    if(rank == 0) {
        printf("Squared vector: ");
        for(int i=0;i<8;i++)
            printf("%d ",result[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}