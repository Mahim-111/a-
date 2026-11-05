
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int *arr = NULL;
    int n = 0;

    if (rank == 0)
    {
        FILE *fp = fopen("input.txt", "r");

        if (fp == NULL)
        {
            printf("Error opening input.txt\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fscanf(fp, "%d", &n);

        arr = malloc(n * sizeof(int));

        if (arr == NULL)
        {
            printf("Memory allocation failed\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        for (int i = 0; i < n; i++)
            fscanf(fp, "%d", &arr[i]);

        fclose(fp);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (size != 3)
    {
        if (rank == 0)
            printf("Run with exactly 3 processes.\n");

        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0)
    {
        int consumer = 1;

        for (int i = 0; i < n; i++)
        {
            // Send data
            MPI_Send(&arr[i], 1, MPI_INT,
                     consumer, 0, MPI_COMM_WORLD);

            printf("P0 sent: %d to P%d\n",
                   arr[i], consumer);

            // Receive acknowledgment
            int ack;

            MPI_Recv(&ack, 1, MPI_INT,
                     consumer, 1, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            // Print confirmed reception
            printf("P%d received: %d\n",
                   consumer, arr[i]);

            fflush(stdout);

            consumer = (consumer == 1) ? 2 : 1;
        }

        // Stop both consumers
        int stop = -1;

        MPI_Send(&stop, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&stop, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);

        printf("All data sent successfully.\n");
    }
    else
    {
        while (1)
        {
            int recv_msg;

            MPI_Recv(&recv_msg, 1, MPI_INT,
                     0, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            if (recv_msg == -1)
                break;

            // Send acknowledgment after receiving
            int ack = 1;

            MPI_Send(&ack, 1, MPI_INT,
                     0, 1, MPI_COMM_WORLD);
        }
    }

    free(arr);

    MPI_Finalize();

    return 0;
}
