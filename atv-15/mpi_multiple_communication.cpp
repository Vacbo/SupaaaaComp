#include <mpi.h>
#include <iostream>
#include <cstring>

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

    // Buffer for the messages
    char message[50];

    if (rank == 0) {
        // Process 0 sends a unique message to each other process
        for (int i = 1; i < size; ++i) {
            sprintf(message, "Message for process %d", i);
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            std::cout << "Process 0 sent: " << message << " to process " << i << std::endl;
        }
    } else {
        // All other processes receive their message from process 0
        MPI_Recv(message, 50, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process " << rank << " received: " << message << " from process 0" << std::endl;
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}