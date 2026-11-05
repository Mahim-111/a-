/*
Problem_18: Divide a large dataset among MPI processes. Each process generates a local histogram, and MPI_Allreduce combines all local histograms into a global histogram. 
Sample Input
Number of processes: 4
Values range from 0 to 9.
P0: 1 2 2 3
P1: 2 3 4 4
P2: 1 4 5 5
P3: 0 2 5 9
Sample Output
Final histogram after MPI_Allreduce:
Value    Frequency
0        1
1        2
2        4
3        2
4        3
5        3
6        0
7        0
8        0
9        1

Total number of elements = 20
*/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, local[4], hist[10] = {0};
    int global[10];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int data[4];

    if(rank == 0) {
        int x[] = {1,2,2,3};
        for(int i=0;i<4;i++) data[i] = x[i];
    }
    else if(rank == 1) {
        int x[] = {2,3,4,4};
        for(int i=0;i<4;i++) data[i] = x[i];
    }
    else if(rank == 2) {
        int x[] = {1,4,5,5};
        for(int i=0;i<4;i++) data[i] = x[i];
    }
    else {
        int x[] = {0,2,5,9};
        for(int i=0;i<4;i++) data[i] = x[i];
    }

    for(int i=0;i<4;i++)
        hist[data[i]]++;

    MPI_Allreduce(hist, global, 10, MPI_INT,
                  MPI_SUM, MPI_COMM_WORLD);

    if(rank == 0) {
        printf("Global Histogram:\n");
        for(int i=0;i<10;i++)
            printf("%d : %d\n", i, global[i]);
    }

    MPI_Finalize();
    return 0;
}