#include <stdio.h>

int main()
{
    int n;
    printf("Enter the order of matrix : ");
    scanf("%d", &n);

    int arr1[n][n];
    int arr2[n][n];
    int mul[n][n];

    printf("Enter the element : ");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            scanf("%d", &arr1[i][j]);
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf(" %d", arr1[i][j]);
        }
        printf("\n");
    }

    printf("Enter the element : ");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            scanf("%d", &arr2[i][j]);
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf(" %d", arr2[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            mul[i][j] = arr1[i][j] * arr2[i][j];
        }
        printf("\n");
    }
    printf("Result : ");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf(" %d", mul[i][j]);
        }
        printf("\n");
        }
    return 0;
}