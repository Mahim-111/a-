#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>

double f(double x){
    return x*x;
}

int main(int argc, char **argv ){

    MPI_Init(&argc,&argv);

    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    double a, b, h;
    int n;

    double local_sum = 0, total_sum = 0;

    if(rank == 0){

        FILE *file = fopen("input.txt", "r");
        fscanf(file, "%lf %lf %d", &a, &b, &n);
        fclose(file);
    }

    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    h = (b-a)/n;

    int start = rank * n/size;
    int end = (rank+1) * n/size;

    for(int i=start; i<end; i++){

        double x = a + i*h;
        double x1 = a + (i+1)*h;

        local_sum += (f(x) + f(x1)) * h / 2.0;
    }

    printf("Process %d Partial sum: %lf\n", rank, local_sum);

    
    MPI_Reduce(&local_sum, &total_sum, 1,
               MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
               
    if(rank == 0){
       printf("\nNumerical integral: %.6lf\n", total_sum);
    }

    MPI_Finalize();

    return 0;
}