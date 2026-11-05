#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int data[4][3] = {
        {10, 20, 30}, {40, 50, 60}, {70, 80, 90}, {100, 110, 120}};

    int local_sum = data[rank][0] + data[rank][1] + data[rank][2];
    printf("Local sums:\nP%d = %d\n", rank, local_sum);

    int global_sum, global_max, global_min;
    int local_max = data[rank][2];
    int local_min = data[rank][0];

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Global Sum = %d\n", global_sum);
        printf("Global Maximum = %d\n", global_max);
        printf("Global Minimum = %d\n", global_min);
        printf("Average = %.2f\n", (float)global_sum / 12.0);
    }

    MPI_Finalize();
    return 0;
}
