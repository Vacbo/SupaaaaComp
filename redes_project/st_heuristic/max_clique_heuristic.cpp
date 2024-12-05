#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <iomanip>

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

// Function to compute degrees of all nodes
vector<int> computeDegrees(const vector<vector<int>>& graph) {
    vector<int> degrees(graph.size(), 0);
    for (int i = 0; i < graph.size(); ++i) {
        degrees[i] = count(graph[i].begin(), graph[i].end(), 1);
    }
    return degrees;
}

// Function to check if a node is connected to all nodes in the current clique
bool isConnectedToClique(const vector<vector<int>>& graph, const vector<int>& clique, int node) {
    for (int vertex : clique) {
        if (graph[node][vertex] == 0) { // Not connected to all nodes in the clique
            return false;
        }
    }
    return true;
}

// Recursive function to explore cliques
void exploreClique(const vector<vector<int>>& graph, vector<int>& currentClique, vector<int>& maxClique,
                   int start, const vector<int>& vertexOrder) {
    // Prune if remaining nodes + current clique cannot exceed the max clique size
    if (currentClique.size() + (vertexOrder.size() - start) <= maxClique.size()) {
        return;
    }

    for (int i = start; i < vertexOrder.size(); ++i) {
        int node = vertexOrder[i];
        if (isConnectedToClique(graph, currentClique, node)) {
            currentClique.push_back(node);
            exploreClique(graph, currentClique, maxClique, i + 1, vertexOrder);
            currentClique.pop_back(); // Backtrack
        }
    }

    // Update the global max clique if a larger one is found
    if (currentClique.size() > maxClique.size()) {
        maxClique = currentClique;
    }
}

// Function to find the maximum clique
vector<int> findMaxClique(const vector<vector<int>>& graph) {
    int numVertices = graph.size();
    vector<int> degrees = computeDegrees(graph);

    // Sort nodes by degree in descending order
    vector<int> vertexOrder(numVertices);
    iota(vertexOrder.begin(), vertexOrder.end(), 0); // Fill with 0, 1, ..., numVertices - 1
    sort(vertexOrder.begin(), vertexOrder.end(), [&](int a, int b) {
        return degrees[a] > degrees[b];
    });

    vector<int> maxClique;
    vector<int> currentClique;

    for (int i = 0; i < vertexOrder.size(); ++i) {
        currentClique.push_back(vertexOrder[i]);
        exploreClique(graph, currentClique, maxClique, i + 1, vertexOrder);
        currentClique.pop_back();
    }

    return maxClique;
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

    auto start = high_resolution_clock::now();

    vector<int> maxClique = findMaxClique(graph);

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    // Output the maximum clique
    cout << "Maximum Clique Size: " << maxClique.size() << endl;
    cout << "Maximum Clique Nodes: ";
    for (int node : maxClique) {
        cout << (node + 1) << " "; // Convert to 1-based indexing for easier interpretation
    }
    cout << endl;

    // Output the time taken
    cout << fixed << setprecision(6);
    cout << "Time taken: " << duration.count() << " seconds" << endl;

    return 0;
}
