#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <mpi.h>

using namespace std;
using namespace chrono;

// Function to read the graph from a file and create an adjacency matrix
vector<vector<int>> readGraph(const string& filename, int& numVertices) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open the file!" << endl;
        exit(1);
    }

    int numEdges;
    file >> numVertices >> numEdges;

    // Initialize the adjacency matrix
    vector<vector<int>> graph(numVertices, vector<int>(numVertices, 0));

    // Populate the adjacency matrix with edges
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        file >> u >> v;
        if (u <= 0 || u > numVertices || v <= 0 || v > numVertices) {
            cerr << "Error: Edge indices out of bounds!" << endl;
            exit(1);
        }
        u -= 1; // Convert to 0-based indexing
        v -= 1; // Convert to 0-based indexing
        graph[u][v] = 1;
        graph[v][u] = 1; // Undirected graph
    }

    file.close();
    return graph;
}

// Recursive function to find all cliques starting from a vertex
void findCliques(const vector<vector<int>>& graph, vector<int>& currentClique,
                 vector<int>& maxClique, int start) {
    if (currentClique.size() > maxClique.size()) {
        maxClique = currentClique;
    }

    for (int i = start; i < graph.size(); ++i) {
        bool canAdd = true;
        for (int v : currentClique) {
            if (graph[i][v] == 0) { // If vertex i is not connected to all vertices in the clique
                canAdd = false;
                break;
            }
        }

        if (canAdd) {
            currentClique.push_back(i);
            findCliques(graph, currentClique, maxClique, i + 1);
            currentClique.pop_back(); // Backtrack
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    string filename = "grafo.txt";
    int numVertices;
    vector<vector<int>> graph;

    // Rank 0 reads the graph
    if (rank == 0) {
        graph = readGraph(filename, numVertices);
    }

    // Broadcast the number of vertices to all processes
    MPI_Bcast(&numVertices, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Resize graph on non-root processes and broadcast the graph data
    if (rank != 0) {
        graph.resize(numVertices, vector<int>(numVertices, 0));
    }

    for (int i = 0; i < numVertices; ++i) {
        MPI_Bcast(graph[i].data(), numVertices, MPI_INT, 0, MPI_COMM_WORLD);
    }

    vector<int> localMaxClique;
    vector<int> currentClique;

    // Start timing
    auto startTime = high_resolution_clock::now();

    // Each process works on a subset of vertices
    for (int i = rank; i < numVertices; i += size) {
        currentClique.push_back(i);
        findCliques(graph, currentClique, localMaxClique, i + 1);
        currentClique.pop_back();
    }

    // Gather the maximum clique size and the rank from all processes
    int localData[2], globalData[2];
    localData[0] = localMaxClique.size(); // Maximum clique size found by this process
    localData[1] = rank;                  // Rank of this process

    // Use MPI_2INT and MPI_MAXLOC to find the maximum clique size and its corresponding rank
    MPI_Reduce(localData, globalData, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

    // Only the process with the maximum clique size sends its clique to rank 0
    if (rank == 0) {
        vector<int> globalMaxClique(globalData[0]);
        if (globalData[1] == 0) {
            globalMaxClique = localMaxClique;
        } else {
            MPI_Recv(globalMaxClique.data(), globalData[0], MPI_INT, globalData[1], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Stop timing
        auto endTime = high_resolution_clock::now();
        duration<double> duration = endTime - startTime;

        // Print the result
        cout << "Maximum Clique Size: " << globalMaxClique.size() << endl;
        cout << "Maximum Clique Nodes: ";
        for (int v : globalMaxClique) {
            cout << (v + 1) << " "; // Convert to 1-based indexing
        }
        cout << endl;
        cout << "Time taken: " << fixed << setprecision(6) << duration.count() << " seconds" << endl;
    } else if (rank == globalData[1]) {
        // Send the clique to rank 0
        MPI_Send(localMaxClique.data(), localData[0], MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
