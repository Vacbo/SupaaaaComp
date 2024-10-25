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

    // Ensure there are more than 2 processes
    if (size < 3) {
        std::cerr << "This program requires at least 3 processes." << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Create a message buffer
    char message[20];
    
    // Initialize the message for process 0
    if (rank == 0) {
        strcpy(message, "Hello from 0");
        // Send message to the next process (rank 1)
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD);
        std::cout << "Process 0 sent: " << message << " to process 1" << std::endl;

        // Receive the final message from the last process (rank size - 1)
        MPI_Recv(message, 20, MPI_CHAR, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process 0 received: " << message << " from process " << size - 1 << std::endl;
    } else {
        // All other processes first receive the message from the previous rank
        MPI_Recv(message, 20, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process " << rank << " received: " << message << " from process " << rank - 1 << std::endl;

        // Modify the message
        sprintf(message, "Hello from %d", rank);

        // Send the message to the next process (or wrap around to 0 if at the last process)
        int next_rank = (rank + 1) % size;
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, next_rank, 0, MPI_COMM_WORLD);
        std::cout << "Process " << rank << " sent: " << message << " to process " << next_rank << std::endl;
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}