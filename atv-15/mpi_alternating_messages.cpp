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
        for (int i = 1; i < size; ++i) {
            // Send a message to process i
            sprintf(message, "Message from process 0 to process %d", i);
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            std::cout << "Process 0 sent: " << message << " to process " << i << std::endl;

            // Receive a response from process i
            MPI_Recv(message, 50, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Process 0 received: " << message << " from process " << i << std::endl;
        }
    } else {
        // Each process (except process 0) receives a message from process 0
        MPI_Recv(message, 50, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process " << rank << " received: " << message << " from process 0" << std::endl;

        // Modify the message to indicate a response
        sprintf(message, "Response from process %d to process 0", rank);

        // Send the response back to process 0
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        std::cout << "Process " << rank << " sent: " << message << " to process 0" << std::endl;
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}