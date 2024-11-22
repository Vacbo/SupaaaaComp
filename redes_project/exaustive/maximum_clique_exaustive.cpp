#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>

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

// Recursive function to find the maximum clique
void findMaxClique(const vector<vector<int>>& graph, vector<int>& currentClique,
                   vector<int>& maxClique, int start) {
    // If the current clique is larger, update the maximum clique found
    if (currentClique.size() > maxClique.size()) {
        maxClique = currentClique;
    }

    // Check each vertex starting from 'start' to potentially add to the clique
    for (int i = start; i < graph.size(); ++i) {
        bool canAdd = true;
        for (int v : currentClique) {
            if (graph[i][v] == 0) { // If vertex i is not connected to all vertices in the current clique
                canAdd = false;
                break;
            }
        }

        if (canAdd) {
            currentClique.push_back(i);
            findMaxClique(graph, currentClique, maxClique, i + 1);
            currentClique.pop_back(); // Backtrack
        }
    }
}

int main() {
    string filename = "grafo.txt"; // Change this to your graph file name
    int numVertices;
    vector<vector<int>> graph = readGraph(filename, numVertices);

    vector<int> maxClique;
    vector<int> currentClique;

    // Measure start time
    auto start = high_resolution_clock::now();

    // Perform exhaustive search for the maximum clique
    findMaxClique(graph, currentClique, maxClique, 0);

    // Measure end time
    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    // Output the maximum clique with 1-based indexing
    cout << "Maximum Clique Size: " << maxClique.size() << endl;
    cout << "Maximum Clique Nodes: ";
    for (int v : maxClique) {
        cout << (v + 1) << " "; // Convert to 1-based indexing for easier reading
    }
    cout << endl;

    cout << fixed << setprecision(6);
    cout << "\nTime taken: " << duration.count() << " seconds" << endl;

    return 0;
}
