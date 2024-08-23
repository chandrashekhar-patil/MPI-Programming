#include <stdio.h>
#include <math.h>
#include <mpi.h>

double f(double x){
    return sin(x);
}

void partition(double a, double b, int n, double *A){
    double h = (b - a) / n;
    for (int i = 0; i < n; i++) {
        A[i] = a + i * h;
    }
}

double BooleMethod(double a, double b, int n){
    int k = n / 4;
    double sum = 0.0;
    double h = (b - a) / n;
    double A[n + 1];

    partition(a, b, n, A);

    for (int i = 0; i < k; i++) {
        sum += 2 * h / 45.0 * (7 * f(A[4 * i]) +
                              32 * f(A[4 * i + 1]) +
                              12 * f(A[4 * i + 2]) +
                              32 * f(A[4 * i + 3]) +
                              7 * f(A[4 * i + 4]));
    }
    return sum;
}

int main(int argc, char *argv[]){
    int rank, size;
    double a = 0.0, b = 1.0;
    int n = 16;
    double total_integral;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_n = n / size;
    double local_a = a + rank * local_n * (b - a) / n;
    double local_b = local_a + local_n * (b - a) / n;

    double local_integral = BooleMethod(local_a, local_b, local_n);

    MPI_Reduce(&local_integral, &total_integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("The integral from %f to %f is approximately : %f\n", a, b, total_integral);
    }

    MPI_Finalize();
    return 0;
}
