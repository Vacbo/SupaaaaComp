#include <mpi.h>
#include <omp.h>
#include <vector>
#include <iostream>
#include <iomanip>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 100000;

    std::vector<double> vec(N);
    int chunk_size = N / size;
    int remainder = N % size;

    // Calculate the start and end index for each process
    int start = rank * chunk_size + std::min(rank, remainder);
    int local_chunk_size = chunk_size + (rank < remainder ? 1 : 0);

    std::vector<double> local_data(local_chunk_size);

    //Iniatialize Vector with random values
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            vec[i] = i + 1;
        }
    }

    //Distribution of data
    if (rank == 0) {
        for (int p = 1; p < size; p++) {
            int p_start = p * chunk_size + std::min(p, remainder);
            int p_chunk_size = chunk_size + (p < remainder ? 1 : 0);
            MPI_Send(&vec[p_start], p_chunk_size, MPI_DOUBLE, p, 0, MPI_COMM_WORLD);
        }
        // copy the data for the root process
        std::copy(vec.begin(), vec.begin() + local_chunk_size, local_data.begin());
    } else {
        MPI_Recv(local_data.data(), local_chunk_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Calculate partial sum with OpenMP
    double local_sum = 0.0;
    #pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < local_chunk_size; i++) {
        local_sum += local_data[i] * local_data[i];
    }

    // Gather all partial sums to rank 0
    double total_sum = 0.0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Print the result
    if (rank == 0) {
        std::cout << "Sum of the elements squared: " << std::fixed << std::setprecision(6) << total_sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}
