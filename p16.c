#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

int main(int argc, char **argv ){
    MPI_Init(&argc,&argv);
    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    int *values = NULL;
    int local_val;
    int prefix_sum = 0;
    if(rank == 0){
        FILE *fp = fopen("input.txt","r");
        values = malloc(size*sizeof(int));
        for(int i=0;i<size;i++){
            fscanf(fp,"%d",&values[i]);
        }
        fclose(fp);
    }
    MPI_Scatter(values,1,MPI_INT,&local_val,1,MPI_INT,0,MPI_COMM_WORLD);
    printf("Rank %d received value %d\n",rank,local_val);
    MPI_Scan(&local_val,&prefix_sum,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    printf("Rank %d has prefix sum %d\n",rank,prefix_sum);
    if (rank == 0) {
        free(values);
    }
    MPI_Finalize();
    return 0;
}