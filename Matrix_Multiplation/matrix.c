#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void read_matrix(const char *filename, int *matrix, int rows, int cols) {
    FILE *file = fopen(filename, "r");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(file, "%d", &matrix[i * cols + j]);
        }
    }
    fclose(file);
}

void write_matrix(const char *filename, int *matrix, int rows, int cols) {
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", matrix[i * cols + j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    int rank, size;
    int *A, *B, *C;
    int A_rows = 8, A_cols = 8, B_rows = 8, B_cols = 8;
    int local_rows, *local_C;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        A = (int *)malloc(A_rows * A_cols * sizeof(int));
        B = (int *)malloc(B_rows * B_cols * sizeof(int));
        C = (int *)malloc(A_rows * B_cols * sizeof(int));

        read_matrix("A.txt", A, A_rows, A_cols);
        read_matrix("B.txt", B, B_rows, B_cols);
    }

    MPI_Bcast(&A_rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&A_cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&B_cols, 1, MPI_INT, 0, MPI_COMM_WORLD);

    local_rows = A_rows / size;

    int *local_A = (int *)malloc(local_rows * A_cols * sizeof(int));
    local_C = (int *)malloc(local_rows * B_cols * sizeof(int));

    MPI_Scatter(A, local_rows * A_cols, MPI_INT, local_A, local_rows * A_cols, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, B_rows * B_cols, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_rows; i++) {
        for (int j = 0; j < B_cols; j++) {
            local_C[i * B_cols + j] = 0;
            for (int k = 0; k < A_cols; k++) {
                local_C[i * B_cols + j] += local_A[i * A_cols + k] * B[k * B_cols + j];
            }
        }
    }

    if (rank == 0) {
        C = (int *)malloc(A_rows * B_cols * sizeof(int));
    }
    MPI_Gather(local_C, local_rows * B_cols, MPI_INT, C, local_rows * B_cols, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        write_matrix("result.txt", C, A_rows, B_cols);
        free(A);
        free(B);
        free(C);
    }

    free(local_A);
    free(local_C);

    MPI_Finalize();
    return 0;
}
