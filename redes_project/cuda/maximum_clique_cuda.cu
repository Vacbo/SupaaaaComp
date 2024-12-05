#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <cuda_runtime.h>

using namespace std;
using namespace std::chrono;

// CUDA error checking
#define CHECK_CUDA(call) do { \
    cudaError_t e = (call); \
    if(e != cudaSuccess) { \
        cerr << "CUDA Error: " << cudaGetErrorString(e) << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        exit(EXIT_FAILURE); \
    } \
} while(0)

// CUDA kernel to check if a vertex i is connected to all vertices in currentClique
__global__ void canAddVertexKernel(const int* d_graph, int numVertices,
                                   const int* d_currentClique, int cliqueSize, int newVertex,
                                   int* d_result) {
    // Each thread checks one vertex in the currentClique
    int idx = threadIdx.x;
    if (idx < cliqueSize) {
        int v = d_currentClique[idx];
        // d_graph[u * numVertices + v] indicates connectivity between u and v
        // Check if newVertex is connected to currentClique[idx]
        if (d_graph[newVertex * numVertices + v] == 0) {
            // If not connected, mark result as 0
            atomicExch(d_result, 0);
        }
    }
}

// Helper function that launches the kernel and returns boolean result
bool canAddVertex(const int* d_graph, int numVertices,
                  const vector<int>& currentClique, int newVertex) {
    int cliqueSize = (int)currentClique.size();
    if (cliqueSize == 0) return true; // If no vertices in clique, always can add

    // Copy currentClique to device
    int* d_currentClique;
    CHECK_CUDA(cudaMalloc((void**)&d_currentClique, cliqueSize * sizeof(int)));
    CHECK_CUDA(cudaMemcpy(d_currentClique, currentClique.data(), cliqueSize * sizeof(int), cudaMemcpyHostToDevice));

    // result on device
    int* d_result;
    CHECK_CUDA(cudaMalloc((void**)&d_result, sizeof(int)));
    int initial_result = 1;
    CHECK_CUDA(cudaMemcpy(d_result, &initial_result, sizeof(int), cudaMemcpyHostToDevice));

    // Launch kernel: one block, cliqueSize threads
    canAddVertexKernel<<<1, cliqueSize>>>(d_graph, numVertices, d_currentClique, cliqueSize, newVertex, d_result);
    CHECK_CUDA(cudaDeviceSynchronize());

    // Copy result back
    int host_result;
    CHECK_CUDA(cudaMemcpy(&host_result, d_result, sizeof(int), cudaMemcpyDeviceToHost));

    // Free device memory
    CHECK_CUDA(cudaFree(d_currentClique));
    CHECK_CUDA(cudaFree(d_result));

    return (host_result == 1);
}

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
        u -= 1;
        v -= 1;
        graph[u][v] = 1;
        graph[v][u] = 1;
    }

    return graph;
}

// Exhaustive clique search with pruning, using GPU to check connectivity.
void findCliquesGPU(const int* d_graph, int numVertices, vector<int>& currentClique,
                    vector<int>& maxClique, int start) {
    // Pruning
    if ((int)currentClique.size() > (int)maxClique.size()) {
        maxClique = currentClique;
    }

    // Another simple pruning:
    // If even adding all remaining vertices won't exceed the maxClique size, stop
    if ((int)currentClique.size() + (numVertices - start) <= (int)maxClique.size()) {
        return;
    }

    for (int i = start; i < numVertices; ++i) {
        if ((int)currentClique.size() + (numVertices - i) <= (int)maxClique.size()) {
            break; // no chance of improvement
        }

        // Use GPU to check if we can add vertex i
        if (canAddVertex(d_graph, numVertices, currentClique, i)) {
            currentClique.push_back(i);
            findCliquesGPU(d_graph, numVertices, currentClique, maxClique, i + 1);
            currentClique.pop_back();
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <path_to_graph_file>" << endl;
        return 1;
    }

    string filename = argv[1];
    int numVertices;
    vector<vector<int>> graph = readGraph(filename, numVertices);

    // Flatten the graph for GPU
    vector<int> flatGraph(numVertices * numVertices);
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            flatGraph[i * numVertices + j] = graph[i][j];
        }
    }

    int* d_graph;
    CHECK_CUDA(cudaMalloc((void**)&d_graph, numVertices * numVertices * sizeof(int)));
    CHECK_CUDA(cudaMemcpy(d_graph, flatGraph.data(), numVertices * numVertices * sizeof(int), cudaMemcpyHostToDevice));

    auto startTime = high_resolution_clock::now();

    vector<int> maxClique;
    vector<int> currentClique;

    // Start exhaustive search
    findCliquesGPU(d_graph, numVertices, currentClique, maxClique, 0);

    auto endTime = high_resolution_clock::now();
    duration<double> duration = endTime - startTime;

    cout << "Maximum Clique Size: " << maxClique.size() << endl;
    cout << "Maximum Clique Nodes: ";
    for (int v : maxClique) {
        cout << (v + 1) << " ";
    }
    cout << endl;
    cout << "Time taken: " << fixed << setprecision(6) << duration.count() << " seconds" << endl;

    CHECK_CUDA(cudaFree(d_graph));

    return 0;
}
