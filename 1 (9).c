/*
Problem_9: Divide an array among MPI processes. Each process 
sorts its local portion and exchanges data with neighboring 
processes to produce a globally sorted array. 
Sample Input
Number of processes: 4
Array: 45 12 78 3 56 23 89 10
Distribution:
P0: 45 12
P1: 78 3
P2: 56 23
P3: 89 10
Sample Output
Local sorting:
P0: 12 45
P1: 3 78
P2: 23 56
P3: 10 89
After data exchange and merge/exchange steps:
Sorted array: 3 10 12 23 45 56 78 89
*/

#include <mpi.h>
#include <stdio.h>

#define N 8

void sort(int a[], int n)
{
    int i,j,t;
    for(i=0;i<n-1;i++)
        for(j=i+1;j<n;j++)
            if(a[i]>a[j]) {
                t=a[i]; a[i]=a[j]; a[j]=t;
            }
}

int main(int argc, char *argv[])
{
    int rank, size, local[2], all[N];

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(size != 4) {
        if(rank==0) printf("Run with 4 processes\n");
        MPI_Finalize();
        return 0;
    }

    int data[N] = {45,12,78,3,56,23,89,10};

    MPI_Scatter(data,2,MPI_INT,local,2,MPI_INT,
                0,MPI_COMM_WORLD);

    sort(local,2);

    MPI_Gather(local,2,MPI_INT,all,2,MPI_INT,
               0,MPI_COMM_WORLD);

    if(rank==0) {
        sort(all,N);

        printf("Sorted array: ");
        for(int i=0;i<N;i++)
            printf("%d ",all[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}

