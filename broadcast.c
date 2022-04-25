#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Input reading for process 0
    int n_elements;
    double *buffer;
    if (rank == 0)
    {
        /********* reading the number of elements in n_elements  ********/
        scanf("%d", &n_elements);

        buffer = (double *)malloc(n_elements * sizeof(double));

        for (int i = 0; i < n_elements; ++i)
            /*** storing each element in buffer[i]   *****/
            scanf("%lf", &buffer[i]);
    }
    // 1- Broadcast the value n_elements to all processes
    // [...]

    MPI_Bcast(&n_elements, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    // Here we create the buffers for the non-root processes now that
    // we have n_elements
    if (rank != 0)
        buffer = (double *)malloc(n_elements * sizeof(double));

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(buffer, n_elements, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    // 2- Broadcast the buffer to every process
    //    NOTE : The type here should be MPI_DOUBLE not MPI_FLOAT !
    // [...]

    // 3- Compute the sum depending on the rank
    double sum = 0.0;
    // [...]
    if (rank == 1)
    {
        for (int i = 0; i < n_elements; ++i)
        {
            sum += buffer[i];
        }
    }
    else if (rank == 2)
    {
        for (int i = 0; i < n_elements; ++i)
        {
            if (buffer[i] > 0)
                sum += buffer[i];
        }
    }
    if (rank != 0)
        printf("soma rank %d: %lf\n", rank, sum);

    // Printing the result and terminating the program
    // Precision is set high for the validation process, please do not modify this.
    // [....] //

    MPI_Finalize();
    free(buffer);

    return 0;
}