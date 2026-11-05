#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int matrix[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    int local_row[4], local_sum = 0;
    if (rank == 0)
    {
        for (int i = 1; i < size; i++)
            MPI_Send(matrix[i], 4, MPI_INT, i, 0, MPI_COMM_WORLD);
        for (int j = 0; j < 4; j++)
            local_row[j] = matrix[0][j];
    }
    else
    {
        MPI_Recv(local_row, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    for (int i = 0; i < 4; i++)
        local_sum += local_row[i];
    if (rank == 0)
    {
        int total_sum = local_sum;
        printf("P0: Row = %d %d %d %d Sum = %d\n", local_row[0], local_row[1], local_row[2], local_row[3], local_sum);
        for (int i = 1; i < size; i++)
        {
            int remote_sum, remote_row[4];
            MPI_Recv(&remote_sum, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(remote_row, 4, MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("P%d: Row = %d %d %d %d Sum = %d\n", i, remote_row[0], remote_row[1], remote_row[2], remote_row[3], remote_sum);
            total_sum += remote_sum;
        }
        printf("Total matrix sum = %d\n", total_sum);
    }
    else
    {
        MPI_Send(&local_sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(local_row, 4, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
