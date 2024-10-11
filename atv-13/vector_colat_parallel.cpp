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
    std::vector<double> vec;  // No pre-allocation

    // Measure start time
    double start_time = omp_get_wtime();

    // Parallelize the vector population
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        double result = conta_complexa(i);

        // Use a critical section to ensure thread-safe push_back
        #pragma omp critical
        vec.push_back(result);
    }

    // Measure end time for vector population
    double end_time = omp_get_wtime();
    double population_time = end_time - start_time;

    // Measure time for printing (optional)
    start_time = omp_get_wtime();

    // Parallelize the printing of indices
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        std::cout << i << " ";
    }

    // Measure end time for printing
    end_time = omp_get_wtime();
    double printing_time = end_time - start_time;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nTime taken for population (without pre-allocation): " << population_time << " seconds\n";
    std::cout << "Time taken for printing: " << printing_time << " seconds\n";
    std::cout << "Total time taken: " << population_time + printing_time << " seconds\n";

    return 0;
}