#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int token = 0, next = (rank + 1) % size, prev = (rank - 1 + size) % size;
    double start_time = 0.0, end_time = 0.0;
    if (rank == 0)
    {
        token = 100;
        start_time = MPI_Wtime();
        printf("P0 sends %d to P1\n", token);
        MPI_Send(&token, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        MPI_Recv(&token, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P0 receives %d from P%d\n", token, prev);
        end_time = MPI_Wtime();
        printf("Ring communication completed. Execution time: %f seconds\n", end_time - start_time);
    }
    else
    {
        MPI_Recv(&token, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P%d receives %d from P%d\n", rank, token, prev);
        printf("P%d sends %d to P%d\n", rank, token, next);
        MPI_Send(&token, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
