#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 3

int main(int argc, char *argv[]) {
    int rank, size;
    double A[N][N], x[N], y[N];
    double *tempA, *tempY;
    int rpp;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Initialize matrix A and vector x
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = i + j;
            }
            x[i] = i;
        }

        printf("Matrix A:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%lf ", A[i][j]);
            }
            printf("\n");
        }

        printf("Vector x:\n");
        for (int i = 0; i < N; i++) {
            printf("%lf ", x[i]);
        }
        printf("\n");
    }

    // Compute rows per process
    rpp = N / size;
    tempA = (double *)malloc(rpp * N * sizeof(double));
    tempY = (double *)malloc(rpp * sizeof(double));

    // Scatter the rows of matrix A
    MPI_Scatter(&A[0][0], rpp * N, MPI_DOUBLE, tempA, rpp * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Perform the matrix-vector multiplication for the rows assigned to this process
    for (int i = 0; i < rpp; i++) {
        tempY[i] = 0.0;
        for (int j = 0; j < N; j++) {
            tempY[i] += tempA[i * N + j] * x[j];
        }
    }

    // Gather the results from all processes
    MPI_Gather(tempY, rpp, MPI_DOUBLE, y, rpp, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Resulting vector y:\n");
        for (int i = 0; i < N; i++) {
            printf("%lf ", y[i]);
        }
        printf("\n");
    }

    // Clean up and finalize
    free(tempA);
    free(tempY);
    MPI_Finalize();
    return 0;
}
