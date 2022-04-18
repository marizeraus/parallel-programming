#include <stdio.h>
#include "mpi.h"
int main( argc, argv )
int argc;
char **argv;
{ 
	int rank, value, size;
	MPI_Status status;
	value = 1;
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );

	//se valor for par, executa
	if(size % 2 == 0){
		//calcula a tarefa destino e verifica se existe.
		if(rank == 0) {
			printf("tarefa mestre");
		}
		int dest = rank + size/2;
		if(dest < size) {
			//envia a mensagem para a tarefa destino
			MPI_Send(&rank, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
		}
		else {
			MPI_Status status;
			//recebe a mensagem
			MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			printf ("Tarefa %d enviou seu id para %d\n", value, rank);		
		}
	}
	MPI_Finalize();

	return 0;
}