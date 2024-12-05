#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <iomanip>
#include <omp.h>

using namespace std;
using namespace chrono;

// Function to read the graph from a file and create an adjacency matrix
vector<vector<int>> readGraph(const string& filename, int& numVertices) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open the file at " << filename << endl;
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

// Compute degrees for all vertices
vector<int> computeDegrees(const vector<vector<int>>& graph) {
    vector<int> degrees(graph.size(), 0);
    for (int i = 0; i < graph.size(); ++i) {
        degrees[i] = count(graph[i].begin(), graph[i].end(), 1);
    }
    return degrees;
}

// Greedy heuristic to find a clique starting from a given vertex
vector<int> greedyClique(const vector<vector<int>>& graph, const vector<int>& vertexOrder) {
    vector<int> clique;

    for (int v : vertexOrder) {
        bool canAdd = true;
        for (int c : clique) {
            if (graph[v][c] == 0) { // Not connected to all current clique members
                canAdd = false;
                break;
            }
        }
        if (canAdd) {
            clique.push_back(v);
        }
    }

    return clique;
}

int main(int argc, char** argv) {
    // Ensure a file path is provided
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <path_to_graph_file>" << endl;
        return 1;
    }

    string filename = argv[1]; // Read the file path from command-line arguments
    int numVertices;
    vector<vector<int>> graph = readGraph(filename, numVertices);

    // Compute vertex degrees
    vector<int> degrees = computeDegrees(graph);

    // Sort vertices by degree in descending order for greedy heuristic
    vector<int> vertexOrder(numVertices);
    iota(vertexOrder.begin(), vertexOrder.end(), 0); // Fill with 0, 1, ..., numVertices-1
    sort(vertexOrder.begin(), vertexOrder.end(), [&](int a, int b) {
        return degrees[a] > degrees[b]; // Descending order
    });

    vector<int> globalMaxClique;

    // Measure start time
    auto start = high_resolution_clock::now();

    // Parallel greedy clique computation
    #pragma omp parallel
    {
        vector<int> localMaxClique;
        #pragma omp for schedule(dynamic)
        for (int i = 0; i < numVertices; ++i) {
            // Rotate vertex order for each thread's starting vertex
            vector<int> threadVertexOrder = vertexOrder;
            rotate(threadVertexOrder.begin(), threadVertexOrder.begin() + i, threadVertexOrder.end());

            // Compute a greedy clique for this starting vertex
            vector<int> clique = greedyClique(graph, threadVertexOrder);

            // Update the global maximum clique
            #pragma omp critical
            {
                if (clique.size() > localMaxClique.size()) {
                    localMaxClique = clique;
                }
            }
        }

        // Compare with global maximum clique
        #pragma omp critical
        {
            if (localMaxClique.size() > globalMaxClique.size()) {
                globalMaxClique = localMaxClique;
            }
        }
    }

    // Measure end time
    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    // Output the maximum clique with 1-based indexing
    cout << "Maximum Clique Size: " << globalMaxClique.size() << endl;
    cout << "Maximum Clique Nodes: ";
    for (int v : globalMaxClique) {
        cout << (v + 1) << " "; // Convert to 1-based indexing for easier reading
    }
    cout << endl;

    // Output the time taken in seconds with customizable precision
    cout << fixed << setprecision(6);
    cout << "Time taken: " << duration.count() << " seconds" << endl;

    return 0;
}
