#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int compare(const void *a, const void *b) { return (*(int *)a - *(int *)b); }
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int data[8] = {45, 12, 78, 3, 56, 23, 89, 10};
    int local[2] = {data[rank * 2], data[rank * 2 + 1]};
    qsort(local, 2, sizeof(int), compare);
    int *global_array = NULL;
    if (rank == 0)
        global_array = (int *)malloc(8 * sizeof(int));
    MPI_Gather(local, 2, MPI_INT, global_array, 2, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        qsort(global_array, 8, sizeof(int), compare);
        printf("Sorted array: ");
        for (int i = 0; i < 8; i++)
            printf("%d ", global_array[i]);
        printf("\n");
        free(global_array);
    }
    MPI_Finalize();
    return 0;
}
