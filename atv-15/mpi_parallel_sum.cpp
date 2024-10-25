#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the rank of the current process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the total number of processes
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ensure there are more than 1 processes
    if (size < 2) {
        std::cerr << "This program requires at least 2 processes." << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Each process has a unique value (for simplicity, we'll set it as the rank)
    int my_value = rank;
    int sum = 0;

    if (rank == 0) {
        // Process 0 initializes the sum with its own value
        sum = my_value;

        // Sends initial sum to process 1
        MPI_Send(&sum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        std::cout << "Process 0 sent sum: " << sum << " to process 1" << std::endl;
    } else {
        // Each other process receives the sum from the previous process
        MPI_Recv(&sum, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process " << rank << " received sum: " << sum << " from process " << rank - 1 << std::endl;

        // Add the current process's value to the received sum
        sum += my_value;

        // If not the last process, send the updated sum to the next process
        if (rank < size - 1) {
            MPI_Send(&sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            std::cout << "Process " << rank << " sent sum: " << sum << " to process " << rank + 1 << std::endl;
        } else {
            // The last process displays the total sum
            std::cout << "Process " << rank << " (last process) has the total sum: " << sum << std::endl;
        }
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}