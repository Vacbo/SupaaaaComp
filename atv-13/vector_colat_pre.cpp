#include <vector>
#include <iostream>
#include <unistd.h>
#include <omp.h>
#include <iomanip>

double conta_complexa(int i) {
    return 2 * i;
}

int main() {
    int N = 10000;
    std::vector<double> vec(N);  // Pre-allocate the vector with size N

    // Measure start time
    double start_time = omp_get_wtime();

    // Parallelize the vector population
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        vec[i] = conta_complexa(i);  // Direct assignment, thread-safe because no dynamic resizing occurs
    }

    // Measure end time for vector population
    double end_time = omp_get_wtime();
    double population_time = end_time - start_time;

    // Print the vector
    for (int i = 0; i < N; i++) {
        std::cout << vec[i] << " ";
    }

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nTime taken to populate the vector (parallel with pre-allocation): " << population_time << " seconds\n";
    return 0;
}
