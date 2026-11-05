#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int values[4] = {10, 20, 30, 40};
    int local_val = values[rank];
    int prefix_sum = 0;

    MPI_Scan(&local_val, &prefix_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("P%d: Local value = %d, Prefix sum = %d\n", rank, local_val, prefix_sum);

    MPI_Finalize();
    return 0;
}
