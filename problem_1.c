#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int N = 12;
    int full_array[12] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120};
    int local_n = N / size;
    int local_arr[3];
    for (int i = 0; i < local_n; i++)
        local_arr[i] = full_array[rank * local_n + i];
    int left_neighbor = rank - 1, right_neighbor = rank + 1, left_received = -1;
    for (int p = 0; p < size; p++)
    {
        if (rank == p && rank < size - 1)
        {
            int right_boundary = local_arr[local_n - 1];
            MPI_Send(&right_boundary, 1, MPI_INT, right_neighbor, 0, MPI_COMM_WORLD);
            printf("P%d: Right boundary sent = %d\n", rank, right_boundary);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == p && rank > 0)
        {
            MPI_Recv(&left_received, 1, MPI_INT, left_neighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("P%d: Left boundary received = %d\n", rank, left_received);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    if (rank == 0)
        printf("Boundary exchange completed successfully.\n");
    MPI_Finalize();
    return 0;
}
