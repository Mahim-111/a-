/*
Problem_7: Implement a distributed vector computation using MPI_Isend 
and MPI_Irecv. Compare its execution time with the equivalent blocking 
MPI_Send/MPI_Recv implementation. 
Sample Input
Number of processes: 2
Array size = 8
P0: 10 20 30 40 50 60 70 80
P1: 1 2 3 4 5 6 7 8
Each process exchanges its array using MPI_Isend and MPI_Irecv.
Sample Output
P0 received: 1 2 3 4 5 6 7 8
P1 received: 10 20 30 40 50 60 70 80
Nonblocking communication completed.
Sample performance:
Blocking communication time    = 0.000021 s
Nonblocking communication time = 0.000015 s
*/

#include <mpi.h>
#include <stdio.h>

#define N 8

int main(int argc, char *argv[])
{
    int rank, i, a[N], recv[N];
    MPI_Request req[2];
    double t1, t2, blocking, nonblocking;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        for(i=0;i<N;i++) a[i]=(i+1)*10;
    else
        for(i=0;i<N;i++) a[i]=i+1;

    /* Blocking communication */
    MPI_Barrier(MPI_COMM_WORLD);
    t1 = MPI_Wtime();

    if(rank == 0) {
        MPI_Send(a,N,MPI_INT,1,0,MPI_COMM_WORLD);
        MPI_Recv(recv,N,MPI_INT,1,0,MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(recv,N,MPI_INT,0,0,MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        MPI_Send(a,N,MPI_INT,0,0,MPI_COMM_WORLD);
    }

    t2 = MPI_Wtime();
    blocking = t2-t1;

    /* Non-blocking communication */
    MPI_Barrier(MPI_COMM_WORLD);
    t1 = MPI_Wtime();

    MPI_Irecv(recv,N,MPI_INT,
              1-rank,1,MPI_COMM_WORLD,&req[0]);

    MPI_Isend(a,N,MPI_INT,
              1-rank,1,MPI_COMM_WORLD,&req[1]);

    MPI_Waitall(2,req,MPI_STATUSES_IGNORE);

    t2 = MPI_Wtime();
    nonblocking = t2-t1;

    printf("P%d received: ",rank);
    for(i=0;i<N;i++) printf("%d ",recv[i]);
    printf("\n");

    if(rank==0) {
        printf("Blocking time    = %f s\n",blocking);
        printf("Non-blocking time = %f s\n",nonblocking);
    }

    MPI_Finalize();
    return 0;
}