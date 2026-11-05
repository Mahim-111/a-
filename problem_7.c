#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int arr0[8] = {10, 20, 30, 40, 50, 60, 70, 80}, arr1[8] = {1, 2, 3, 4, 5, 6, 7, 8}, my_arr[8], recv_arr[8];
    for (int i = 0; i < 8; i++)
        my_arr[i] = (rank == 0) ? arr0[i] : arr1[i];
    double t0 = MPI_Wtime();
    MPI_Request reqs[2];
    MPI_Isend(my_arr, 8, MPI_INT, 1 - rank, 0, MPI_COMM_WORLD, &reqs[0]);
    MPI_Irecv(recv_arr, 8, MPI_INT, 1 - rank, 0, MPI_COMM_WORLD, &reqs[1]);
    MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE);
    double t1 = MPI_Wtime();
    if (rank == 0)
        printf("Nonblocking communication completed.\nNonblocking time = %f s\n", t1 - t0);
    MPI_Finalize();
    return 0;
}
