#include <mpi.h>
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the number of processes
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ensure there are at least two processes
    if (size < 2) {
        std::cerr << "This program requires at least two processes." << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Buffer to store messages
    char message[20];

    if (rank == 0) {
        // Process 0 sends "Hello" to process 1
        strcpy(message, "Hello");
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        std::cout << "Process 0 sent: " << message << " to process 1" << std::endl;
    } else if (rank == 1) {
        // Process 1 receives the message from process 0
        MPI_Recv(message, 20, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process 1 received: " << message << " from process 0" << std::endl;

        // Process 1 sends "Hi" back to process 0
        strcpy(message, "Hi");
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        std::cout << "Process 1 sent: " << message << " to process 0" << std::endl;
    } else if (rank == 0) {
        // Process 0 receives the "Hi" message from process 1
        MPI_Recv(message, 20, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Process 0 received: " << message << " from process 1" << std::endl;
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}