#include <iostream>
#include <vector>
#include <ctime>
#include <omp.h>
#include <random>
#include <mpi.h>
#include <iomanip>

const int N = 1000;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<std::vector<double>> A(N, std::vector<double>(N));

    // Calculate the the line each process will start and end
    int lines_per_process = N / size;
    int remainder = N % size;

    int start_line = rank * lines_per_process + std::min(rank, remainder);
    int end_line = start_line + lines_per_process + (rank < remainder ? 1 : 0);

    std::vector<std::vector<double>> A_local((end_line - start_line) * N);

    //Initialize matrix A with random values
    if (rank == 0) {
        #pragma omp parallel
        {
            // Each thread gets its own random number generator
            std::random_device rd;
            std::mt19937 gen(rd() ^ omp_get_thread_num());
            std::uniform_real_distribution<> dis(0, 100);

            #pragma omp parallel for collapse(2) schedule(static)
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    A[i][j] = dis(gen);
                }
            }
        }
    }

    //Distribution of matrix A lines
    if (rank == 0) {
        for (int p = 1; p < size; p++) {
            int p_start_line = p * lines_per_process + std::min(p, remainder);
            int p_end_line = p_start_line + lines_per_process + (p < remainder ? 1 : 0);
            MPI_Send(&A[p_start_line][0], (p_end_line - p_start_line) * N, MPI_DOUBLE, p, 0, MPI_COMM_WORLD);
        }
        // Copy the lines that belong to the root process
        std::vector<std::vector<double>> A_local(end_line - start_line, std::vector<double>(N));
    } else {
        MPI_Recv(&A_local[0][0], (end_line - start_line) * N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Calculate the mean of each line of the matrix
    // Each process calculates the mean of the lines it has

    double local_mean = 0.0;
    for (auto& line : A_local) {
        double local_sum = 0.0;
        #pragma omp parallel for reduction(+:local_sum)
        for (int i = 0; i < N; i++) {
            local_sum += line[i];
        }
        local_mean = local_sum / N;
        MPI_Send(&local_mean, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    // Gather all partial means to rank 0
    double total_mean = 0.0;
    MPI_Reduce(&local_mean, &total_mean, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Print the result
    if (rank == 0) {
        total_mean /= N;
        std::cout << "Mean of matrix: " << std::fixed << std::setprecision(6) << total_mean << std::endl;
    }

    MPI_Finalize();
    return 0;

}
