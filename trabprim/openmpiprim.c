#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>

// definindo o número de vértices
#define V 10

// A function to print the constructed MST stored in from[]
int printMST(int from[], int n, int **graph)
{
    int i;
    printf("Edge   Weight\n");
    for (i = 1; i < V; i++)
        printf("%d - %d    %d \n", from[i], i, graph[i][from[i]]);
}

int findMin(int visited[], int key[], int rank, int size)
{
    MPI_Barrier(MPI_COMM_WORLD);
    int receiveVisited[V];
    int receiveKey[V];
    int finalSize = V / size;

    MPI_Scatter(visited, finalSize, MPI_INT, receiveVisited, finalSize, MPI_INT,
                0, MPI_COMM_WORLD);

    MPI_Scatter(key, finalSize, MPI_INT, receiveKey, finalSize, MPI_INT,
                0, MPI_COMM_WORLD);

    int min = INT_MAX, u;
    int vertex;
    for (vertex = 0; vertex < finalSize; vertex++)
    {
        if (receiveVisited[vertex] == 0 && receiveKey[vertex] <= min)
        {
            min = receiveKey[vertex], u = vertex;
        }
    }
    int minIndexGlobal = rank * finalSize + u;
    int finalVal[2];
    finalVal[0] = min;
    finalVal[1] = minIndexGlobal;
    int finalMin[2];
    MPI_Reduce(&finalVal, &finalMin, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("%d, %d\n", finalMin[0], finalMin[1]);
        return finalMin[1];
    }
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int size;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int i, j;
    int mst[V];
    int key[V];
    int visited[V];
    int count;

    if (rank == 0)
    {
        int **graph = malloc(V * sizeof(int *));
        for (int i = 0; i < V; i++)
            graph[i] = malloc(V * sizeof(int));

        // Gera uma matriz de adjacência aleatória
        srand(time(NULL));
        for (i = 0; i < V; i++)
            for (j = 0; j < V; j++)
                graph[i][j] = rand() % 10;

        for (i = 0; i < V; i++)
        {
            graph[i][i] = 0;
        }

        for (i = 0; i < V; i++)
            for (j = 0; j < V; j++)
                graph[j][i] = graph[i][j];
        // time = -MPI_Wtime();
    }
    clock_t begin = clock();
    if (rank == 0)
    {
        for (i = 0; i < V; i++)
            key[i] = INT_MAX, visited[i] = 0;

        key[0] = 0;  // seleciona o vértice inicial como a raiz da árvore
        mst[0] = -1; // a origem dele é -1, pois é a raiz
    }
    for (count = 0; count < V - 1; count++)
    {
        if (rank == 0)
        {
            int min = INT_MAX, u;
            int vertex;
            MPI_Barrier(MPI_COMM_WORLD);
            u = findMin(visited, key, rank, size);
            visited[u] = 1;
        }
        else
        {
            MPI_Barrier(MPI_COMM_WORLD);
            findMin(visited, key, rank, size);
        }

        // seleciona o vizinho mais próximo que ainda não esta na arvore
        /*             for (vertex = 0; vertex < V; vertex++)
                    {
                        // se não foi visitado, e a distancia for menor que o minimo atual, substitui
                        if (visited[vertex] == 0 && key[vertex] < min)
                        {
                            min = key[vertex], u = vertex;
                        }
                    }
         */
        // define como visitado
        /*             visited[u] = 1;
                    int v;

                    // pega todos os vértices e verifica se existe uma aresta entre ele e u
                    // se existir, e for menor que o valor atual para ele na árvore, atualiza
                    for (v = 0; v < V; v++)
                        if (graph[u][v] && visited[v] == 0 && graph[u][v] < key[v])
                            mst[v] = u, key[v] = graph[u][v]; */
    }

    // inicia a execução do algoritmo
    // Inicializa todas as distâncias como infinito

    /*     clock_t end = clock();
        printMST(mst, V, graph);
        for (int i = 0; i < V; i++)
            free(graph[i]);
        free(graph);

        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("%f\n", time_spent);
     */
    MPI_Finalize();
    return 0;
}
