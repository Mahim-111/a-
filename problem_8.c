#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int my_msg = (rank == 0) ? 100 : 200, recv_msg;
    if (rank == 0)
    {
        MPI_Send(&my_msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_msg, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P0 sent 100\nP0 received %d\nCommunication completed without deadlock.\n", recv_msg);
    }
    else if (rank == 1)
    {
        MPI_Recv(&recv_msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&my_msg, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
