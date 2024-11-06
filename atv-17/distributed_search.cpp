#include <iostream>
#include <vector>
#include <mpi.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 1000; // Size of the vector
    int target = 69; // Value to search for
    std::vector<int> data(N);
    int chunk_size = N / size;
    int remainder = N % size;

    // Calculate start index and size of local chunk for each process
    int start = rank * chunk_size + std::min(rank, remainder);
    int local_chunk_size = chunk_size + (rank < remainder ? 1 : 0);

    // Initialize the vector in rank=0 and distribute it
    if (rank == 0) {
        // Fill the vector with example values
        for (int i = 0; i < N; i++) {
            data[i] = i % 100; // Example values: 0, 1, 2, ..., 99, repeated
        }
    }

    // Distribute the data
    std::vector<int> local_data(local_chunk_size);
    if (rank == 0) {
        for (int p = 1; p < size; ++p) {
            int p_start = p * chunk_size + std::min(p, remainder);
            int p_chunk_size = chunk_size + (p < remainder ? 1 : 0);
            MPI_Send(&data[p_start], p_chunk_size, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
        // Copy the local data for rank 0
        std::copy(data.begin(), data.begin() + local_chunk_size, local_data.begin());
    } else {
        MPI_Recv(local_data.data(), local_chunk_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Perform parallel search with OpenMP
    std::vector<int> local_positions;
    #pragma omp parallel for
    for (int i = 0; i < local_chunk_size; i++) {
        if (local_data[i] == target) {
            #pragma omp critical
            local_positions.push_back(start + i); // Store global index
        }
    }

    // Gather all positions from each process to rank 0
    if (rank == 0) {
        std::vector<int> global_positions = local_positions;
        for (int p = 1; p < size; ++p) {
            int p_chunk_size = (chunk_size + (p < remainder ? 1 : 0));
            int num_positions;
            
            // First, receive the count of found positions from each process
            MPI_Recv(&num_positions, 1, MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Receive positions if there are any
            if (num_positions > 0) {
                std::vector<int> p_positions(num_positions);
                MPI_Recv(p_positions.data(), num_positions, MPI_INT, p, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                global_positions.insert(global_positions.end(), p_positions.begin(), p_positions.end());
            }
        }

        // Print all found positions
        std::cout << "Positions of target value " << target << ": ";
        for (int pos : global_positions) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
    } else {
        // Send the number of found positions and the positions themselves to rank 0
        int local_count = local_positions.size();
        MPI_Send(&local_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        if (local_count > 0) {
            MPI_Send(local_positions.data(), local_count, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}