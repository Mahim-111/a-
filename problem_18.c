#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int local_data[4][4] = {
        {1, 2, 2, 3},
        {2, 3, 4, 4},
        {1, 4, 5, 5},
        {0, 2, 5, 9}};

    int local_hist[10] = {0};
    int global_hist[10] = {0};

    // Calculate structural localized frequencies
    for (int i = 0; i < 4; i++)
    {
        local_hist[local_data[rank][i]]++;
    }

    MPI_Allreduce(local_hist, global_hist, 10, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Final histogram after MPI_Allreduce:\nValue\tFrequency\n");
        for (int i = 0; i < 10; i++)
        {
            printf("%d\t%d\n", i, global_hist[i]);
        }
    }

    MPI_Finalize();
    return 0;
}
