#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <mpi.h>

using namespace std;
using namespace std::chrono;

// Function to read the graph from a file and create an adjacency matrix
vector<vector<int>> readGraph(const string& filename, int& numVertices) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open the file at " << filename << endl;
        exit(1);
    }

    int numEdges;
    file >> numVertices >> numEdges;

    vector<vector<int>> graph(numVertices, vector<int>(numVertices, 0));
    for (int i = 0; i < numEdges; ++i) {
        int u, v;
        file >> u >> v;
        if (u <= 0 || u > numVertices || v <= 0 || v > numVertices) {
            cerr << "Error: Edge indices out of bounds!" << endl;
            exit(1);
        }
        u -= 1; // 0-based indexing
        v -= 1;
        graph[u][v] = 1;
        graph[v][u] = 1; // undirected
    }

    file.close();
    return graph;
}

// Recursive function to find all cliques starting from a vertex, with pruning
void findCliques(const vector<vector<int>>& graph, vector<int>& currentClique,
                 vector<int>& maxClique, int start) {
    int n = (int)graph.size();

    // Pruning: if even using all remaining vertices we cannot exceed current max clique size, stop
    if ((int)currentClique.size() + (n - start) <= (int)maxClique.size()) {
        return;
    }

    if (currentClique.size() > maxClique.size()) {
        maxClique = currentClique;
    }

    for (int i = start; i < n; ++i) {
        bool canAdd = true;
        // Pruning here as well:
        // If adding this vertex doesn't lead to a bigger clique, no point adding:
        if ((int)currentClique.size() + (n - i) <= (int)maxClique.size()) {
            break; // no chance of improvement
        }

        for (int v : currentClique) {
            if (graph[i][v] == 0) {
                canAdd = false;
                break;
            }
        }

        if (canAdd) {
            currentClique.push_back(i);
            findCliques(graph, currentClique, maxClique, i + 1);
            currentClique.pop_back(); // backtrack
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0) {
            cerr << "Usage: " << argv[0] << " <path_to_graph_file>" << endl;
        }
        MPI_Finalize();
        return 1;
    }

    string filename = argv[1];
    int numVertices;
    vector<vector<int>> graph;

    if (rank == 0) {
        graph = readGraph(filename, numVertices);
    }

    // Broadcast the number of vertices
    MPI_Bcast(&numVertices, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // On non-root, resize graph
    if (rank != 0) {
        graph.resize(numVertices, vector<int>(numVertices, 0));
    }

    // Broadcast the entire adjacency matrix in one go for efficiency
    // Flatten the graph into a single buffer
    {
        int totalSize = numVertices * numVertices;
        if (rank == 0) {
            std::vector<int> flatGraph(numVertices * numVertices);
            for (int i = 0; i < numVertices; ++i) {
                std::copy(graph[i].begin(), graph[i].end(), flatGraph.begin() + i * numVertices);
            }
            // Broadcast flatGraph
            MPI_Bcast(flatGraph.data(), numVertices * numVertices, MPI_INT, 0, MPI_COMM_WORLD);

            // On all ranks (including root), reconstruct graph from flatGraph
            if (rank != 0) {
                for (int i = 0; i < numVertices; ++i) {
                    std::copy(flatGraph.begin() + i * numVertices,
                              flatGraph.begin() + (i+1) * numVertices,
                              graph[i].begin());
                }
            }
        } else {
            std::vector<int> flatGraph(numVertices * numVertices);
            // Receive flatGraph
            MPI_Bcast(flatGraph.data(), numVertices * numVertices, MPI_INT, 0, MPI_COMM_WORLD);
            // Reconstruct graph
            for (int i = 0; i < numVertices; ++i) {
                std::copy(flatGraph.begin() + i * numVertices,
                          flatGraph.begin() + (i+1)*numVertices,
                          graph[i].begin());
            }
        }
    }

    vector<int> localMaxClique;
    vector<int> currentClique;
    currentClique.reserve(numVertices);

    // Start timing
    auto startTime = high_resolution_clock::now();

    // Distribute initial vertices across ranks
    for (int i = rank; i < numVertices; i += size) {
        currentClique.clear();
        currentClique.push_back(i);
        findCliques(graph, currentClique, localMaxClique, i + 1);
    }

    // Gather the maximum clique size and the rank from all processes
    int localData[2], globalData[2];
    localData[0] = (int)localMaxClique.size(); // local max clique size
    localData[1] = rank;

    MPI_Reduce(localData, globalData, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

    // Process that found global max clique sends it to root
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

        // Print result
        cout << "Maximum Clique Size: " << globalMaxClique.size() << endl;
        cout << "Maximum Clique Nodes: ";
        for (int v : globalMaxClique) {
            cout << (v + 1) << " "; // convert to 1-based indexing
        }
        cout << endl;
        cout << "Time taken: " << fixed << setprecision(6) << duration.count() << " seconds" << endl;
    } else if (rank == globalData[1]) {
        MPI_Send(localMaxClique.data(), localData[0], MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
