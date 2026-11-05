#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

int main(int argc, char **argv){
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int matrix[4][4];
    int local_row[4];
    int local_sum = 0;

    if(rank == 0){
        FILE *file = fopen("input.txt", "r");
        if(file == NULL){
            printf("Error opening input.txt\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                fscanf(file, "%d", &matrix[i][j]);
            }
        }
        fclose(file);
        for(int i = 1; i < size; i++){
            MPI_Send(matrix[i], 4, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        for(int j = 0; j < 4; j++){
            local_row[j] = matrix[0][j];
        }
    }else{
        MPI_Recv(local_row, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    for(int j = 0; j < 4; j++){
        local_sum += local_row[j];
    }
    if(rank == 0){
        int total_sum   = local_sum;
        printf("Process %d: local: %d %d %d %d  Total local sum = %d\n", rank,local_row[0], local_row[1], local_row[2], local_row[3], total_sum);
        for(int i = 1; i < size; i++){
            int recv_sum;
            int received_row[4];
            MPI_Recv(&recv_sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(received_row, 4, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Process %d: local: %d %d %d %d  Total local sum = %d\n", i, received_row[0], received_row[1], received_row[2], received_row[3], recv_sum);
            total_sum += recv_sum;
        }
        printf("Total sum = %d\n", total_sum);
    }
    else{
        MPI_Send(&local_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(local_row, 4, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}