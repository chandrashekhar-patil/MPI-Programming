#include<mpi.h>
#include<stdio.h>

int main(int argc,char *argv[]){
    int rank;
    int size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    //MPI_Comm_size(MPI_COMM_WORLD,&size);

    printf("No Of Processor : %d And Runing\n",rank);

    MPI_Finalize();
    return 0;
}