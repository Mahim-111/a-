#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
int main(int argc, char **argv ){
    MPI_Init(&argc,&argv);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    int config[3];
    if(rank == 0){
        FILE *file = fopen("input.txt", "r");
        fscanf(file, "%d %d %d", &config[0], &config[1], &config[2]);
        fclose(file);
    }
    MPI_Bcast(config, 3, MPI_INT, 0, MPI_COMM_WORLD);
    printf("P%d received: Iterations: %d, Threshold: %d, Block Size: %d\n", rank, config[0], config[1], config[2]);
    MPI_Finalize();
    return 0;
}