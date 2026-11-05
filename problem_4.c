#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int iterations = 5, message_size = 1024;
    char *buffer = (char *)malloc(message_size);
    double start_time = 0.0;
    if (rank == 0)
    {
        start_time = MPI_Wtime();
        for (int i = 0; i < iterations; i++)
        {
            MPI_Send(buffer, message_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(buffer, message_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        double avg_round_trip = (MPI_Wtime() - start_time) / iterations;
        printf("Message size : %d bytes\nIterations : %d\nTotal round trips : %d\n", message_size, iterations, iterations);
        printf("Average round-trip : %f seconds\nAverage one-way : %f seconds\n", avg_round_trip, avg_round_trip / 2.0);
    }
    else if (rank == 1)
    {
        for (int i = 0; i < iterations; i++)
        {
            MPI_Recv(buffer, message_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buffer, message_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
        }
    }
    free(buffer);
    MPI_Finalize();
    return 0;
}
