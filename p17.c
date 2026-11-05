#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int *values = NULL;
    int *send_data = malloc(size * sizeof(int));
    int *recv_data = malloc(size * sizeof(int));
    if (rank == 0)
    {
        values = malloc(size * size * sizeof(int));
        FILE *fp = fopen("input.txt", "r");
        for (int i = 0; i < size * size; i++)
        {
            fscanf(fp, "%d", &values[i]);
        }
        fclose(fp);
    }
    MPI_Scatter(values, size, MPI_INT,
                send_data, size, MPI_INT,
                0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();
    int repeat = 1000;
    for (int i = 0; i < repeat; i++)
    {
        MPI_Alltoall(send_data, 1, MPI_INT,
                     recv_data, 1, MPI_INT,
                     MPI_COMM_WORLD);
    }
    double end_time = MPI_Wtime();
    double local_time = (end_time - start_time) / repeat;
    double total_time;
    MPI_Reduce(&local_time, &total_time, 1,
               MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    printf("p%d received:", rank);
    for (int i = 0; i < size; i++)
    {
        printf(" %d", recv_data[i]);
    }
    printf("\n");
    if(rank == 0)
    {
        printf("\nAverage MPI_Alltoall communication time: %.9f seconds\n",
               total_time);
    }
    free(send_data);
    free(recv_data);
    if (rank == 0)
    {
        free(values);
    }
   
    MPI_Finalize();
    return 0;
}

