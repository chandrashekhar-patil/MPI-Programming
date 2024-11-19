#include <mpi.h>
#include <stdio.h>
#include <string.h>

void main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int N = 4;
    int dim = 2;
    int dimv[2] = {2, 2};
    int period[2] = {1, 1};
    int reorder = 1;

    MPI_Comm Cart_Topology;
    MPI_Cart_create(MPI_COMM_WORLD, dim, dimv, period, reorder, &Cart_Topology);

    int rank;
    MPI_Comm_rank(Cart_Topology, &rank);

    int A[4][4], B[4][4], C[4][4] = {0};
    int A_local[2][2], B_local[2][2], C_local[2][2] = {0};

    if (rank == 0)
    {
        int A_temp[4][4] = {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 16}};
        int B_temp[4][4] = {
            {16, 15, 14, 13},
            {12, 11, 10, 9},
            {8, 7, 6, 5},
            {4, 3, 2, 1}};
        memcpy(A, A_temp, sizeof(A_temp));
        memcpy(B, B_temp, sizeof(B_temp));
    }

    MPI_Scatter(&A, 4, MPI_INT, &A_local, 4, MPI_INT, 0, Cart_Topology);
    MPI_Scatter(&B, 4, MPI_INT, &B_local, 4, MPI_INT, 0, Cart_Topology);

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                C_local[i][j] += A_local[i][k] * B_local[k][j];
            }
        }
    }

    MPI_Gather(&C_local, 4, MPI_INT, &C, 4, MPI_INT, 0, Cart_Topology);

    if (rank == 0)
    {
        printf("Resulting Matrix C (A * B):\n");
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
}
