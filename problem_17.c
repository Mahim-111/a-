#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int send_data[4][4] = {
        {0, 1, 2, 3},
        {10, 11, 12, 13},
        {20, 21, 22, 23},
        {30, 31, 32, 33}};
    int recv_data[4];

    MPI_Alltoall(send_data[rank], 1, MPI_INT, recv_data, 1, MPI_INT, MPI_COMM_WORLD);

    printf("P%d received: %d %d %d %d\n", rank, recv_data[0], recv_data[1], recv_data[2], recv_data[3]);

    MPI_Finalize();
    return 0;
}
