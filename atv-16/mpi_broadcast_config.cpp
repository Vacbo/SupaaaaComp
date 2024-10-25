#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the rank of the current process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Define the configuration variable for number of iterations
    int iterations = 0;

    // Root process (rank 0) sets the configuration
    if (rank == 0) {
        iterations = 1000; // Define the number of iterations
        std::cout << "Root process set iterations to " << iterations << std::endl;
    }

    // Broadcast the configuration to all processes
    MPI_Bcast(&iterations, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process performs a computation based on the number of iterations
    int local_sum = 0;
    for (int i = 0; i < iterations; ++i) {
        local_sum += rank + i; // Simple computation for demonstration
    }

    // Each process prints its local result
    std::cout << "Process " << rank << " completed calculation with local sum: " << local_sum << std::endl;

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}