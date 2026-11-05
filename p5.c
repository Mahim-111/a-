#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

int main(int argc, char **argv){
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int A[2][2], B[2][2];
    int local_row[2], local_res[2] = {0, 0};

    if(rank == 0){
        FILE *file = fopen("input.txt", "r");
        if(file == NULL){
            printf("Error opening input.txt\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                fscanf(file, "%d", &A[i][j]);
            }
        }
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                fscanf(file, "%d", &B[i][j]);
            }
        }
        fclose(file);
    }

    MPI_Bcast(B, 4, MPI_INT, 0, MPI_COMM_WORLD);
    if(rank == 0){
        MPI_Send(A[1], 2, MPI_INT, 1, 0, MPI_COMM_WORLD);
        for(int j = 0; j < 2; j++)
            local_row[j] = A[0][j];
    } else if(rank == 1){
        MPI_Recv(local_row, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    for(int j = 0; j < 2; j++){
        for(int k = 0; k < 2; k++)
            local_res[j] += local_row[k] * B[k][j];
    }
    if(rank == 0){
        int remote_res[2];
        MPI_Recv(remote_res, 2, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Matrix C = A x B\n%d %d\n%d %d\n", local_res[0], local_res[1], remote_res[0], remote_res[1]);
    } else if(rank == 1){
        MPI_Send(local_res, 2, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }   
    MPI_Finalize();
    return 0;
}