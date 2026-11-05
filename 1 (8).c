/*
Problem_8: Create a two-process communication program that can cause deadlock when blocking sends are used. Modify the program to avoid deadlock using appropriate MPI communication techniques. 
Sample Input
Number of processes: 2
P0 message = 100
P1 message = 200
Sample Output
Unsafe version:
P0: Waiting to send 100
P1: Waiting to send 200
Potential deadlock detected.
Corrected version:
P0 sent 100
P1 received 100
P1 sent 200
P0 received 200
Communication completed without deadlock.
*/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, send_data, recv_data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    send_data = (rank == 0) ? 100 : 200;

    if(rank == 0) {
        MPI_Send(&send_data,1,MPI_INT,1,0,MPI_COMM_WORLD);
        MPI_Recv(&recv_data,1,MPI_INT,1,0,MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }
    else {
        MPI_Recv(&recv_data,1,MPI_INT,0,0,MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        MPI_Send(&send_data,1,MPI_INT,0,0,MPI_COMM_WORLD);
    }

    printf("P%d received %d\n",rank,recv_data);

    MPI_Finalize();
    return 0;
}