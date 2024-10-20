#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TOLERANCE 0.0001

void read_matrix(const char *filename, int *rows, int *cols, double **matrix) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open input file.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    fscanf(file, "%d %d", rows, cols); // Read the dimensions

    // Allocate memory for matrix
    *matrix = (double *)malloc((*rows) * (*cols) * sizeof(double));

    // Read matrix data
    for (int i = 0; i < (*rows); i++) {
        for (int j = 0; j < (*cols); j++) {
            fscanf(file, "%lf", (*matrix) + (i * (*cols)) + j);
        }
    }

    fclose(file);
}

void write_solution(const char *filename, double *solution, int rows) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Unable to open output file.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    fprintf(file, "Solution:\n");
    for (int i = 0; i < rows; i++) {
        fprintf(file, "x%d = %lf\n", i + 1, solution[i]);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows, cols;
    double *matrix = NULL;
    if (rank == 0) {
        // Root reads the input matrix
        read_matrix("input.txt", &rows, &cols, &matrix);
    }

    // Broadcast the number of rows and columns to all processes
    MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_rows = rows / size;
    int extra_rows = rows % size;
    if (rank == size - 1) {
        local_rows += extra_rows;
    }

    // Allocate memory for the local matrices in each process
    double *local_matrix = (double *)malloc(local_rows * cols * sizeof(double));
    double *prev = (double *)malloc((cols - 1) * sizeof(double));
    double *local_sol = (double *)malloc(local_rows * sizeof(double));

    // Scatter the matrix among all processes
    MPI_Scatter(matrix, local_rows * cols, MPI_DOUBLE, local_matrix, local_rows * cols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < cols - 1; i++) {
        prev[i] = 0.0; // Initialize previous solutions to 0
    }

    int done = 0;
    while (!done) {
        done = 1; // Assume we're done unless proven otherwise

        // Compute the local solution
        for (int i = 0; i < local_rows; i++) {
            double sum = 0.0;
            for (int j = 0; j < cols - 1; j++) {
                if (i != j) {
                    sum += local_matrix[i * cols + j] * prev[j];
                }
            }
            local_sol[i] = (local_matrix[i * cols + (cols - 1)] - sum) / local_matrix[i * cols + i];
            if (fabs(local_sol[i] - prev[i]) > TOLERANCE) {
                done = 0; // If not converged, mark as not done
            }
        }

        // Gather solutions from all processes
        MPI_Allgather(local_sol, local_rows, MPI_DOUBLE, prev, local_rows, MPI_DOUBLE, MPI_COMM_WORLD);

        // Check if all processes have converged
        int all_done;
        MPI_Allreduce(&done, &all_done, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);
        done = all_done;
    }

    // Gather final solution at the root process
    if (rank == 0) {
        double *solution = (double *)malloc(rows * sizeof(double));
        MPI_Gather(local_sol, local_rows, MPI_DOUBLE, solution, local_rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Write the final solution to output file
        write_solution("output.txt", solution, rows);

        free(solution);
    }

    // Free allocated memory
    free(local_matrix);
    free(prev);
    free(local_sol);

    if (rank == 0) {
        free(matrix);
    }

    MPI_Finalize();
    return 0;
}
