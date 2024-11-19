#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int global_sum = 0, local_sum = 0;
    int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int N = sizeof(A) / sizeof(A[0]); 
    int chunk_size, start_idx, end_idx;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    chunk_size = N / size;
    start_idx = rank * chunk_size;
    end_idx = start_idx + chunk_size;

    for (int i = start_idx; i < end_idx; i++) {
        local_sum += A[i];
    }
    printf("Processor %d: Local Sum = %d\n", rank, local_sum);

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Global Sum = %d\n", global_sum);
    }

    MPI_Finalize();
    return 0;
}
