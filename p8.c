#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>


int main(int argc, char **argv){
    
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int msg0 = 0, msg1 = 0;
    int my_msg, recv_msg;
    if(rank == 0){
        FILE *fp = fopen("input.txt", "r");
        fscanf(fp, "%d", &msg0);
        fscanf(fp, "%d", &msg1);
        fclose(fp);
    }
    MPI_Bcast(&msg0, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&msg1, 1, MPI_INT, 0, MPI_COMM_WORLD);
    my_msg = (rank == 0) ? msg0 : msg1;
    if(rank == 0){
        MPI_Send(&my_msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&recv_msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P0 Sent: %d, Received: %d\n", my_msg, recv_msg);
    }else if(rank == 1){
        MPI_Recv(&recv_msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&my_msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("P1 Sent: %d, Received: %d\n", my_msg, recv_msg);
    }
    MPI_Finalize();
    return 0;
}