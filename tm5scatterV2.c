#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 100000
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Input reading for process 0
    int x;
    int* buffer;
    int randArray[N];
    int send = 0;
    double time;
    if (rank == 0)
    {

        FILE *myFile;
        myFile = fopen("file.txt", "r");
        fscanf(myFile, "%d\n", &x);
        //read X and now starts sending it to each process. 
        for (int i=1; i < size; i ++) {
            MPI_Send(&x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        time = -MPI_Wtime();

        for(int i=0;i<N;i++)
            fscanf(myFile, "%d\n", &randArray[i]);
        send = N / size;

        if (send * N < size)
            send ++;
    } else {
        //all processess but master receive X. 
    	MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    //considering N is always divisible by size, may not be accurate. 
    int finalSize = N/size;
    buffer = (int *)malloc((finalSize) * sizeof(int));

    MPI_Scatter(randArray, finalSize, MPI_INT, buffer, finalSize, MPI_INT,
    0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    //received part from scatter and now starts looping through array to see if value is found. 
    for (int i=0; i<finalSize; i++) {
        if (buffer[i] == x) {
            printf(" %d Encontrado no processo %d, na posição %d\n",buffer[i],rank,  i + rank * finalSize);
        }
    }

    if(rank == 0){
        printf("Time elapsed: %f", time + MPI_Wtime());
    }
    MPI_Finalize();
    //free(buffer);

    return 0;
}