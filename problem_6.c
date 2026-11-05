#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int raw_array[8] = {10, 20, 30, 40, 50, 60, 70, 80};
    int local[2] = {raw_array[rank * 2], raw_array[rank * 2 + 1]};
    int left_halo = 0, right_halo = 0;
    if (rank > 0)
    {
        MPI_Send(&local[0], 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&left_halo, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    if (rank < size - 1)
    {
        MPI_Recv(&right_halo, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&local[1], 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }
    int out0 = left_halo + local[0] + local[1], out1 = local[0] + local[1] + right_halo;
    int final_out[8], local_out[2] = {out0, out1};
    MPI_Gather(local_out, 2, MPI_INT, final_out, 2, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("Stencil result:\n");
        for (int i = 0; i < 8; i++)
            printf("%d ", final_out[i]);
        printf("\n");
    }
    MPI_Finalize();
    return 0;
}
