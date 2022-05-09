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
    int startPos = 0;
    int finalPos = 0;
    int buffer[N];
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
        
        finalPos = send;
        for (int i=1; i < size; i ++) {
            int pos, fin;
            pos = send * i;
            fin = pos + send;
            //For each process, sending full array, starting position to analyze and final position. 
            MPI_Send(randArray, N, MPI_INT, i, 0, MPI_COMM_WORLD);
        	MPI_Send(&pos, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        	MPI_Send(&fin, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

        }
    } else {
    	MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(randArray, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    	MPI_Recv(&startPos, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    	MPI_Recv(&finalPos, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int i=startPos; i<finalPos; i++) {
        // look for X in the determined interval for this process. 
        if (randArray[i] == x) {

            printf(" %d Encontrado no processo %d, na posição %d\n",x, rank, i);
        }
    }
    if(rank == 0){
        printf("Time elapsed: %f", time + MPI_Wtime());
    }

    MPI_Finalize();

    return 0;
}