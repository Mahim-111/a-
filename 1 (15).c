/*
Problem_15: Implement the trapezoidal rule for numerical integration. Each MPI process calculates a portion of the integral, and MPI_Reduce combines the partial results. 
Sample Input
Number of processes: 4
Function: f(x) = x²
Lower limit = 0
Upper limit = 1
Number of intervals = 1000
Use the trapezoidal rule.
Sample Output
Numerical integral ≈ 0.333333
Exact integral     = 0.333333
Absolute error     ≈ 0.000000
Students should report partial integrals and repeat the experiment with different numbers of intervals.

*/

#include <mpi.h>
#include <stdio.h>

double f(double x)
{
    return x*x;
}

int main(int argc, char *argv[])
{
    int rank, size, n=1000, i;
    double a=0, b=1, h, local=0, total;
    double x1, x2;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    h = (b-a)/n;

    int start = rank * n/size;
    int end   = (rank+1) * n/size;

    for(i=start;i<end;i++) {
        x1 = a + i*h;
        x2 = a + (i+1)*h;

        local += (f(x1)+f(x2))*h/2;
    }

    MPI_Reduce(&local,&total,1,MPI_DOUBLE,
               MPI_SUM,0,MPI_COMM_WORLD);

    if(rank==0)
        printf("Integral = %.6f\n",total);

    MPI_Finalize();
    return 0;
}