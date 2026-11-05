#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int items[8] = {10, 20, 30, 40, 50, 60, 70, 80};

    if (rank == 0)
    { // Producer
        int consumer = 1;
        for (int i = 0; i < 8; i++)
        {
            printf("P0 produced: %d\n", items[i]);
            MPI_Send(&items[i], 1, MPI_INT, consumer, 0, MPI_COMM_WORLD);
            consumer = (consumer == 1) ? 2 : 1; // Round-robin load distribution
        }
        int stop = -1;
        MPI_Send(&stop, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&stop, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
    }
    else
    { // Consumers (P1 and P2)
        while (1)
        {
            int item;
            MPI_Recv(&item, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (item == -1)
                break;
            printf("P%d consumed: %d\n", rank, item);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0)
        printf("All 8 items processed successfully.\n");

    MPI_Finalize();
    return 0;
}
