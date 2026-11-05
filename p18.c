#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int local_n = 4;
    int *data = NULL;
    int local[4] = {0};
    int hist[10] = {0};
    int global[10] = {0};

    if (rank == 0) {
        data = (int *)malloc(local_n * size * sizeof(int));
        FILE *fp = fopen("input.txt", "r");
        for (int i = 0; i < local_n * size; i++) {
            fscanf(fp, "%d", &data[i]);
        }
        fclose(fp);
    }
    MPI_Scatter(data, local_n, MPI_INT, local, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < local_n; i++) {
        hist[local[i]]++;
    }
    MPI_Allreduce(hist, global, 10, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Global histogram:\n");
        for (int i = 0; i < 10; i++) {
            printf("%d: %d\n", i, global[i]);
        }
        free(data);
    }

    MPI_Finalize();
    return 0;
}