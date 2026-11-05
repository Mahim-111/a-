#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv){
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int num_processes = 0, iterations = 0, message_size = 0;
    if(rank == 0){
        FILE *file = fopen("input.txt", "r");
        if(file == NULL){
            printf("Error opening input.txt\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fscanf(file, "%d", &num_processes);
        fscanf(file, "%d", &iterations);
        fscanf(file, "%d", &message_size);
        fclose(file);

    }
    MPI_Bcast(&num_processes, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&iterations, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&message_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if(num_processes != size || iterations <= 0 || message_size <= 0){
        if(rank == 0){
            printf("Error: Number of processes must be %d\n", num_processes);
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    char *message = (char *)malloc(message_size * sizeof(char));

    if(message == NULL){
        printf("Error allocating memory for message\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (rank == 0){
        double start_time = MPI_Wtime();
        for(int i = 0; i < iterations; i++){
            MPI_Send(message, message_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(message, message_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        double avg_round_trip_time = (MPI_Wtime() - start_time) / iterations;
        printf("Number of processes: %d\n", num_processes);
        printf("Number of iterations: %d\n", iterations);
        printf("Message size: %d bytes\n", message_size);
        printf("Average round-trip time: %f seconds\n", avg_round_trip_time);
        printf("Average one-way time: %f seconds\n", avg_round_trip_time / 2);
    }
    else if(rank == 1){
        for(int i = 0; i < iterations; i++){
            MPI_Recv(message, message_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(message, message_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    }
    free(message);
    MPI_Finalize();
    return 0;
}