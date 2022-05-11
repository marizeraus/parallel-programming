
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mpi.h"
#define buffer_count 10000

void print_buffer(int* buffer)
{
  for (int i = 0; i< buffer_count; i ++){
    printf("%d, ", buffer[i]);
  }
  printf("\n");
}

int main( argc, argv )
int argc;
char **argv;
{   
  int rank,  size;
  int buffer[buffer_count];

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  MPI_Request request;
  MPI_Status  status;
  int request_finished = 0;

  // Initialising buffer :
  for (int i=0; i < buffer_count; ++i)
    buffer[i] = (rank == 0 ? i*2 : 0);

  MPI_Barrier(MPI_COMM_WORLD);
  // Starting the chronometer
  double time = -MPI_Wtime(); // This command helps us measure time. We will see more about it later on !

  if (rank == 0) {
    sleep(3);

    MPI_Isend(buffer, buffer_count, MPI_INT, 1, 1, MPI_COMM_WORLD, &request);
    double time_left = 6000.0;
    while (time_left > 0.0) {
      usleep(1000); // We work for 1ms

      // 2- Test if the request is finished (only if not already finished)
      MPI_Test(&request, &request_finished, &status);      // 1ms left to work
      time_left -= 1000.0;
    }

    if(request_finished == 0){
      MPI_Wait(&request, &status);
    }
    // Modifying the buffer for second step
    for (int i=0; i < buffer_count; ++i)
      buffer[i] = -i;

    MPI_Isend(buffer, buffer_count, MPI_INT, 1, 2, MPI_COMM_WORLD, &request);
    
    time_left = 3000.0;
    while (time_left > 0.0) {
      usleep(1000); // We work for 1ms

      MPI_Test(&request, &request_finished, &status);      // 1ms left to work

      // 1ms left to work
      time_left -= 1000.0;
    }
    if(request_finished == 0){
      MPI_Wait(&request, &status);
    }
  }
  else {
    // Work for 5 seconds
    sleep(5);

    MPI_Irecv(buffer, buffer_count, MPI_INT, 0, 1, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);

    print_buffer(buffer);
    
    // Work for 3 seconds
    sleep(3);

    MPI_Irecv(buffer, buffer_count, MPI_INT, 0, 2, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);

    print_buffer(buffer);
  }
  

  // Stopping the chronometer
  time += MPI_Wtime();

  // This line gives us the maximum time elapsed on each process.
  // We will see about reduction later on !
  double final_time;
  MPI_Reduce(&time, &final_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
  
  if (rank == 0)
    printf( "Total time for non-blocking scenario : %f seconds\n", final_time );


  MPI_Finalize( );
  return(0);


}