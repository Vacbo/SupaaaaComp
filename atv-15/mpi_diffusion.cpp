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

    // Buffer for the message
    char message[20];

    if (rank == 0) {
        // Process 0 initializes the message and sends it to process 1
        strcpy(message, "Message from 0");
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        std::cout << "Process 0 sent: " << message << " to process 1" << std::endl;
    } else {
        // All other processes receive the message from the previous rank
        MPI_Recv(message, 20, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process " << rank << " received: " << message << " from process " << rank - 1 << std::endl;

        // If this is not the last process, send the message to the next process
        if (rank < size - 1) {
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD);
            std::cout << "Process " << rank << " sent: " << message << " to process " << rank + 1 << std::endl;
        }
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}