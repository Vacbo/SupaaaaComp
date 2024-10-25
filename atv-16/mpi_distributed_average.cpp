#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the rank of the current process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the total number of processes
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Array size and portion size per process
    const int array_size = 10000;
    int portion_size = array_size / size;
    std::vector<int> array;

    // Only the root process (rank 0) initializes and fills the array
    if (rank == 0) {
        array.resize(array_size);
        srand(time(0)); // Seed for random number generation
        for (int i = 0; i < array_size; ++i) {
            array[i] = rand() % 100; // Random values between 0 and 99
        }
        std::cout << "Root process initialized array with random values." << std::endl;
    }

    // Each process gets a portion of the array
    std::vector<int> local_array(portion_size);
    MPI_Scatter(array.data(), portion_size, MPI_INT, local_array.data(), portion_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the local average
    int local_sum = std::accumulate(local_array.begin(), local_array.end(), 0);
    double local_avg = static_cast<double>(local_sum) / portion_size;

    // Gather all local averages at the root process
    std::vector<double> local_averages(size);
    MPI_Gather(&local_avg, 1, MPI_DOUBLE, local_averages.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // The root process calculates the global average
    if (rank == 0) {
        double global_avg = std::accumulate(local_averages.begin(), local_averages.end(), 0.0) / size;
        std::cout << "Global average calculated from local averages: " << global_avg << std::endl;
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}