#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;

int vertices;               // number of vertices -> first line in textFile
const int maximum = 999999; // setting the maximum distance between vertices

void Dijkstra(vector<vector<int>> myGraph, int root)
{

    int distance[vertices]; // initialise distance vector
    bool Visited[vertices]; // initialise the visited vector -> keeps track of nodes already visited

    for (int i = 0; i < vertices; i++) // loop through each vertex node
    {
        distance[i] = myGraph[root][i]; // set the appropriate distance for the vertex i in the vector
        Visited[i] = false;             // mark it as unvisited
    }

    Visited[root] = true; // mark the root as visited
    int NextNode;
    int count = 1;

    while (count < vertices) // while we havent visited each node
    {
        int minidistance = maximum;
        for (int i = 0; i < vertices; i++)
        {

            if (distance[i] < minidistance && Visited[i] == false) // if the current distance is < current min and the node hasnt been visited
            {
                minidistance = distance[i]; // update the min distance
                NextNode = i;               // set the next node to be the node with the min distance
            }
        }

        Visited[NextNode] = true; // mark next node as visited
        count++;

        for (int i = 0; i < vertices; i++) // loop through each vertex
        {

            if (Visited[i] == false && ((minidistance + myGraph[NextNode][i]) < distance[i])) // if we havent visited this node and the accumulated distance
            {                                                                                 // is less than the current min distance
                distance[i] = minidistance + myGraph[NextNode][i];                            // update the distance in the vector
            }
        }
    }
}

int main(int argc, char *argv[])
{
    //get start node
    int root;

    if (argc != 2)
    {
        cout << "Invalid Amount of Arguments" << endl; // too few args -> should only pass which is the root
        return 0;
    }
    else
    {
        root = atoi(argv[1]); // convert the root to an int
    }

    //open text file
    fstream myGraph;
    myGraph.open("graphs.txt");

    myGraph >> vertices; // setting the number of vertices in eah myGraph
    vector<vector<int>> Board(vertices, vector<int>(vertices, 0));
    for (int i = 0; i < vertices; i++)
    {

        for (int j = 0; j < vertices; j++)
        {

            myGraph >> Board[i][j];
        }
    }

    auto start = chrono::high_resolution_clock::now();
    Dijkstra(Board, root);
    auto finish = chrono::high_resolution_clock::now();
    long long timeTaken = chrono::duration_cast<chrono::microseconds>(finish - start).count();

    cout << "Time taken for execution: " << timeTaken << " microseconds" << endl;

    myGraph.close();
    return 0;
}
