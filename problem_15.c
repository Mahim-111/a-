#include <mpi.h>
#include <stdio.h>

double f(double x)
{
    return x * x;
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 0.0, b = 1.0;
    int n = 1000;
    double h = (b - a) / n;

    int local_n = n / size;
    double local_a = a + rank * local_n * h;
    double local_b = local_a + local_n * h;

    double local_integral = (f(local_a) + f(local_b)) / 2.0;
    for (int i = 1; i < local_n; i++)
    {
        local_integral += f(local_a + i * h);
    }
    local_integral *= h;

    double total_integral;
    MPI_Reduce(&local_integral, &total_integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Numerical integral ~ %f\n", total_integral);
        printf("Exact integral = 0.333333\nAbsolute error ~ %f\n", total_integral - 0.333333);
    }

    MPI_Finalize();
    return 0;
}
