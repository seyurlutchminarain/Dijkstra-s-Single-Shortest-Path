#include <iostream>
#include <fstream>
#include <random>

using namespace std;

int main(int argc, char *argv[])
{
    ofstream Graph;
    Graph.open("graphs.txt"); // open the file withour appending

    //setting no of vertices in graph
    int numVertices;
    int Percent;
    if (argc != 2)
    {
        //error checking
        cout << "Invalid number of arguments given: expected 1" << endl;
        Graph.close(); // close the file
        return 0;
    }
    else
    {
        numVertices = atoi(argv[1]); // convert numVertices to int

        if (numVertices < 1)
        {
            cout << "Invalid No. of Vertices" << endl; // Validation (Cant have an empty graph)
            Graph.close();
            return 0;
        }
    }
    Percent = numVertices * ((30 + 0.0) / 100); // for density checking

    Graph << numVertices << endl;

    random_device rd; // for generating random numbers
    mt19937 generator(rd());

    //random weight between 1 and 20
    uniform_int_distribution<> dist(1, 20);
    vector<vector<int>> WMatrix(numVertices, vector<int>(numVertices, 0));

    //generating random weights
    for (int i = 0; i < numVertices; i++)
    {

        for (int j = 0; j < numVertices; j++)
        {

            if (i == j)
            {

                WMatrix[i][j] = 0;
            }
            else
            {

                if (WMatrix[i][j] == 0)
                {

                    WMatrix[i][j] = dist(generator);
                    WMatrix[j][i] = WMatrix[i][j];
                }
            }
        }
    }

    //setting random numVertices to be max distance (Based on percentage)
    auto position = uniform_int_distribution<>(0, numVertices - 1);

    for (int i = 0; i < Percent; i++)
    {
        int row = position(generator);
        int col = position(generator);

        if (row != col)
        {
            WMatrix[row][col] = 999999;
            WMatrix[col][row] = 999999;
        }
    }

    //writing the matrix in row major order to the Graph text file
    for (int i = 0; i < numVertices; i++)
    {
        for (int j = 0; j < numVertices; j++)
        {
            if (j == 0)
            {
                Graph << WMatrix[i][j];
            }
            else
            {
                Graph << ' ' << WMatrix[i][j];
            }
        }
        Graph << endl;
    }

    Graph.close();
    return 0;
}