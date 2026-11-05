#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int config[3]; // [iterations, threshold, block_size]

    if (rank == 0)
    {
        config[0] = 1000;
        config[1] = 50;
        config[2] = 256;
    }

    MPI_Bcast(config, 3, MPI_INT, 0, MPI_COMM_WORLD);

    printf("P%d received: Iterations = %d, Threshold = %d, Block size = %d\n",
           rank, config[0], config[1], config[2]);

    MPI_Finalize();
    return 0;
}
