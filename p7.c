#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

int main(int argc, char **argv){
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int n = 0;
    int *arr0 = NULL;
    int *arr1 = NULL;
    int *my_arr = NULL;
    int *recv_arr = NULL;

    if (rank == 0){
        FILE *file = fopen("input.txt", "r");
        fscanf(file, "%d", &n);
        arr0 = malloc(n * sizeof(int));
        arr1 = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++)
            fscanf(file, "%d", &arr0[i]);
        for (int i = 0; i < n; i++)
            fscanf(file, "%d", &arr1[i]);
        fclose(file);
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    my_arr = malloc(n * sizeof(int));
    recv_arr = malloc(n * sizeof(int));

    if(rank == 0){
        for (int i = 0; i < n; i++)
            my_arr[i] = arr0[i];
        MPI_Send(arr1, n, MPI_INT, 1, 1, MPI_COMM_WORLD);
    } else {
        MPI_Recv(my_arr, n, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    double blocking_start = MPI_Wtime();
    if (rank == 0){
        MPI_Send(my_arr, n, MPI_INT, 1, 2, MPI_COMM_WORLD);
        MPI_Recv(recv_arr, n, MPI_INT, 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(recv_arr, n, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(my_arr, n, MPI_INT, 0, 2, MPI_COMM_WORLD); 
    }
    double blocking_end = MPI_Wtime();
    double blocking_time = blocking_end - blocking_start;
        
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Process 0 received from Process 1: ");

        for (int i = 0; i < n; i++)
            printf("%d ", recv_arr[i]);

        printf("\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 1)
    {
        printf("Process 1 received from Process 0: ");

        for (int i = 0; i < n; i++)
            printf("%d ", recv_arr[i]);

        printf("\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    double nonblocking_start = MPI_Wtime();
    MPI_Request reqs[2];
    if(rank == 0){
        MPI_Isend(my_arr, n, MPI_INT, 1, 3, MPI_COMM_WORLD, &reqs[0]);
        MPI_Irecv(recv_arr, n, MPI_INT, 1, 3, MPI_COMM_WORLD, &reqs[1]);
    } else {
        MPI_Irecv(recv_arr, n, MPI_INT, 0, 3, MPI_COMM_WORLD, &reqs[1]);
        MPI_Isend(my_arr, n, MPI_INT, 0, 3, MPI_COMM_WORLD, &reqs[0]);
    }
    MPI_Waitall(2, reqs, MPI_STATUSES_IGNORE);
    double nonblocking_end = MPI_Wtime();
    double nonblocking_time = nonblocking_end - nonblocking_start;
    
    if (rank == 0) {
        printf("Blocking time: %f\n", blocking_time);
        printf("Non-blocking time: %f\n", nonblocking_time);
    }

    MPI_Finalize();
    return 0;
}