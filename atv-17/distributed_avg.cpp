#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <vector>
#include <iomanip>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 1000; // Array size
    std::vector<double> data(N);
    int chunk_size = N / size;
    int remainder = N % size;

    // Define start and end indices for each process to handle non-uniform division
    int start = rank * chunk_size + std::min(rank, remainder);
    int local_chunk_size = chunk_size + (rank < remainder ? 1 : 0);

    std::vector<double> local_data(local_chunk_size);

    // Initialize the array in rank=0
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            data[i] = i + 1; // Example initialization: 1, 2, 3, ..., N
        }
    }

    // Distribute the data to all processes
    if (rank == 0) {
        for (int p = 1; p < size; ++p) {
            int p_start = p * chunk_size + std::min(p, remainder);
            int p_chunk_size = chunk_size + (p < remainder ? 1 : 0);
            MPI_Send(&data[p_start], p_chunk_size, MPI_DOUBLE, p, 0, MPI_COMM_WORLD);
        }
        // Copy local data for rank 0
        std::copy(data.begin(), data.begin() + local_chunk_size, local_data.begin());
    } else {
        // Receive the data chunk
        MPI_Recv(local_data.data(), local_chunk_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Calculate partial sum with OpenMP
    double local_sum = 0.0;
    #pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < local_chunk_size; i++) {
        local_sum += local_data[i];
    }

    // Gather all partial sums to rank 0
    double total_sum = 0.0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Calculate the mean in rank 0
    if (rank == 0) {
        double mean = total_sum / N;
        std::cout << "Mean of the array: " << std::fixed << std::setprecision(6) << mean << std::endl;
    }

    MPI_Finalize();
    return 0;
}