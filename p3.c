#include<stdio.h>
#include<mpi.h>

int main(int argc, char **argv){
    
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int required_size = 4;
    int token;
    if(rank == 0)
    {
        FILE *file = fopen("input.txt", "r");
        if(file == NULL){
            printf("Error opening input.txt\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fscanf(file, "%d", &required_size);
        fscanf(file, "%d", &token);
        fclose(file);
    }
    MPI_Bcast(&required_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(size != required_size){
        if(rank == 0){
            printf("Error: Number of processes must be %d\n", required_size);
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;
    double start_time = 0.0, end_time = 0.0;
    if(rank == 0){
        start_time = MPI_Wtime();
        printf("Process %d sending token %d to process %d\n", rank, token, next);
        MPI_Send(&token, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        MPI_Recv(&token, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received token %d from process %d\n", rank, token, prev);
        end_time = MPI_Wtime();
        printf("Total time taken for token passing: %f seconds\n", end_time - start_time);
    
    }
    else{
        MPI_Recv(&token, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received token %d from process %d\n", rank, token, prev);
        printf("Process %d sending token %d to process %d\n", rank, token, next);
        MPI_Send(&token, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}