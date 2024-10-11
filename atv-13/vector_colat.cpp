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
    std::vector<double> vec;

    // Measure start time for vector population
    double start_time = omp_get_wtime();

    // Populate the vector
    for (int i = 0; i < N; i++) {
        vec.push_back(conta_complexa(i));
    }

    // Measure end time for vector population
    double end_time = omp_get_wtime();
    double population_time = end_time - start_time;

    // Measure start time for printing
    start_time = omp_get_wtime();

    // Print the indices
    for (int i = 0; i < N; i++) {
        std::cout << i << " ";
    }

    // Measure end time for printing
    end_time = omp_get_wtime();
    double printing_time = end_time - start_time;

    // Output the times
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nTime taken for population: " << population_time << " seconds\n";
    std::cout << "Time taken for printing: " << printing_time << " seconds\n";
    std::cout << "Total time taken: " << population_time + printing_time << " seconds\n";

    return 0;
}