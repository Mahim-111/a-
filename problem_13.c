#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int matrix[5][3] = {
        {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}};

    int local_rows = (rank == 2) ? 1 : 2;
    int local_buffer[6];

    // Simulation of manual uneven extraction mapping layout
    if (rank == 0)
    {
        for (int i = 0; i < 6; i++)
            local_buffer[i] = ((int *)matrix)[i];
    }
    else if (rank == 1)
    {
        for (int i = 0; i < 6; i++)
            local_buffer[i] = ((int *)matrix)[6 + i];
    }
    else if (rank == 2)
    {
        for (int i = 0; i < 3; i++)
            local_buffer[i] = ((int *)matrix)[12 + i];
    }

    // Operation payload
    for (int i = 0; i < local_rows * 3; i++)
        local_buffer[i] *= 2;

    int counts[3] = {6, 6, 3};
    int displs[3] = {0, 6, 12};
    int final_matrix[15];

    MPI_Gatherv(local_buffer, local_rows * 3, MPI_INT, final_matrix, counts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Final matrix:\n");
        for (int i = 0; i < 5; i++)
        {
            printf("%d %d %d\n", final_matrix[i * 3], final_matrix[i * 3 + 1], final_matrix[i * 3 + 2]);
        }
    }

    MPI_Finalize();
    return 0;
}
