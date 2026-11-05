#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int vector[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int local[2];
    int local_res[2];
    int global_res[8];

    MPI_Scatter(vector, 2, MPI_INT, local, 2, MPI_INT, 0, MPI_COMM_WORLD);

    local_res[0] = local[0] * local[0];
    local_res[1] = local[1] * local[1];

    printf("P%d: %d %d\n", rank, local_res[0], local_res[1]);

    MPI_Gather(local_res, 2, MPI_INT, global_res, 2, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Gathered result: ");
        for (int i = 0; i < 8; i++)
            printf("%d ", global_res[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
