#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>
#include <chrono>

using namespace std;

int Vertices;
int MAXIMUM = 999999;

void Dijkstra(int nVertices, int source, int *Weights, int *Dis, MPI_Comm comm)
{

    int numV;
    int firstVertex;
    int lastVertex;
    int localMinPair[2], globalMinPair[2];

    int u, uDistance;
    int num_procs, myRank;

    MPI_Comm_size(comm, &num_procs);
    MPI_Comm_rank(comm, &myRank);

    numV = nVertices / num_procs;
    firstVertex = myRank * numV;
    lastVertex = firstVertex + numV - 1;

    for (int j = 0; j < numV; j++)
    {
        Dis[j] = Weights[source * numV + j];
    }

    int *visited = new int[numV];
    for (int j = 0; j < numV; j++)
    {
        visited[j] = 1;
    }
    if (source >= firstVertex && source <= lastVertex)
    {
        visited[source - firstVertex] = 0;
    }

    for (int i = 0; i < nVertices; i++)
    {

        localMinPair[0] = MAXIMUM;
        localMinPair[1] = -1;

        for (int j = 0; j < numV; j++)
        {

            if (visited[j] && Dis[j] < localMinPair[0])
            {
                localMinPair[0] = Dis[j];
                localMinPair[1] = firstVertex + j;
            }
        }

        MPI_Allreduce(localMinPair, globalMinPair, 1, MPI_2INT, MPI_MINLOC, comm);

        uDistance = globalMinPair[0];
        u = globalMinPair[1];

        if (u == localMinPair[1])
        {
            visited[u - firstVertex] = 0;
        }

        for (int j = 0; j < numV; j++)
        {

            if (visited[j] && ((uDistance + Weights[u * numV + j]) < Dis[j]))
            {
                Dis[j] = uDistance + Weights[u * numV + j];
            }
        }
    }
}

int main(int argc, char *argv[])
{

    //LocalV is number of vertices for each thread/proc
    int Rank, num_procs, LocalV, root;
    int *Dis;

    int *Buffer;
    vector<vector<int>> WMatrix;

    fstream Graph;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);

    if (Rank == 0)
    {

        if (argc != 2)
        {
            cout << "Not enough arguments: 2 expected" << endl;
            exit(0);
        }
        root = atoi(argv[1]);

        Graph.open("graphs.txt");
        Graph >> Vertices;
        WMatrix.resize(Vertices, vector<int>(Vertices, 0));

        for (int i = 0; i < Vertices; i++)
        {

            for (int j = 0; j < Vertices; j++)
            {

                Graph >> WMatrix[i][j];
            }
        }
    }

    auto start = chrono::high_resolution_clock::now(); // start the timing
    MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&Vertices, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int *distance = new int[Vertices];
    Dis = new int[Vertices];
    Buffer = new int[Vertices * Vertices];

    LocalV = Vertices / num_procs;

    int *LWeight = new int[LocalV * Vertices];
    int *LDis = new int[LocalV * Vertices];

    if (Rank == 0)
    {

        for (int i = 0; i < num_procs; i++)
        {

            for (int j = 0; j < Vertices; j++)
            {

                for (int k = 0; k < LocalV; k++)
                {
                    Buffer[i * Vertices * LocalV + j * LocalV + k] = WMatrix[j][i * LocalV + k];
                }
            }
        }
    }

    MPI_Scatter(Buffer, LocalV * Vertices, MPI_INT, LWeight, LocalV * Vertices, MPI_INT, 0, MPI_COMM_WORLD);
    Dijkstra(Vertices, root, LWeight, LDis, MPI_COMM_WORLD);
    MPI_Gather(LDis, LocalV, MPI_INT, distance, LocalV, MPI_INT, 0, MPI_COMM_WORLD);
    auto finish = chrono::high_resolution_clock::now(); // stop the timing

    if (Rank == 0)
    {

        long long timeTaken = chrono::duration_cast<chrono::microseconds>(finish - start).count();

        cout << "Time taken for execution: " << timeTaken << " microseconds" << endl;

        // for (int i = 0; i < Vertices; i++)
        // {
        //     cout << "Vertex : " << i << endl;
        //     cout << "Weight : " << distance[i] << endl;
        // }
    }

    MPI_Finalize();
    return 0;
}