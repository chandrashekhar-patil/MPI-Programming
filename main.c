#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

//rite  a parallel code to sort a list of numbers in descending order.
//use MPI to parallelize the code.

int compare(const void *a, const void *b)
{
    return (*(int *)b - *(int *)a); // Sort in descending order
}

int main(int argc, char *argv[])
{
    int rank, size, n, *arr = NULL, *local_arr = NULL;
    int local_n, remainder;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        // Root process takes user input
        printf("Enter the number of elements: ");
        fflush(stdout);
        scanf("%d", &n);

        // Allocate memory for the array and get user input
        arr = (int *)malloc(n * sizeof(int));
        printf("Enter %d elements: ", n);
        fflush(stdout);
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &arr[i]); // User inputs the numbers
        }

        printf("Array initialized by root process. Number of elements: %d\n", n);
        printf("Initial array: ");
        for (int i = 0; i < n; i++)
        {
            printf("%d ", arr[i]);
        }
        printf("\n");
        fflush(stdout);
    }

    // Broadcast the total number of elements to all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the number of elements for each process
    local_n = n / size;
    remainder = n % size;

    // Adjust the number of elements each process receives
    if (rank < remainder)
    {
        local_n += 1;
    }

    // Allocate memory for the local array in each process
    local_arr = (int *)malloc(local_n * sizeof(int));

    // Prepare sendcounts and displs arrays for Scatterv
    int *sendcounts = (int *)malloc(size * sizeof(int));
    int *displs = (int *)malloc(size * sizeof(int));
    int offset = 0;
    for (int i = 0; i < size; i++)
    {
        sendcounts[i] = n / size + (i < remainder ? 1 : 0);
        displs[i] = offset;
        offset += sendcounts[i];
    }

    // Scatter the data from the root process to all processes
    MPI_Scatterv(arr, sendcounts, displs, MPI_INT, local_arr, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Print local array in each process
    // printf("Process %d received: ", rank);
    // for (int i = 0; i < local_n; i++)
    // {
    //     printf("%d ", local_arr[i]);
    // }
    // printf("\n");
    fflush(stdout);

    // Sort the local arrays in descending order
    qsort(local_arr, local_n, sizeof(int), compare);

    // Gather the sorted arrays from all processes back to the root
    MPI_Gatherv(local_arr, local_n, MPI_INT, arr, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    // Only the root process will print the final sorted array
    if (rank == 0)
    {
        printf("Final sorted array: ");
        for (int i = 0; i < n; i++)
        {
            printf("%d ", arr[i]);
        }
        printf("\n");
        fflush(stdout);
    }

    free(local_arr);
    free(sendcounts);
    free(displs);
    if (rank == 0) free(arr); // Free arr only in root process

    MPI_Finalize();
    return 0;
}
