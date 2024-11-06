#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <iomanip>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 100; // Matrix dimension
    int data[N][N];
    int chunk_size = N / size;
    int local_data[chunk_size][N];

    // Start timing the execution
    double start_time = MPI_Wtime();

    // Initialize the matrix in process 0
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                data[i][j] = i + j; // Example initialization
            }
        }
    }

    // Distribute the matrix chunks to each process
    MPI_Scatter(data, chunk_size * N, MPI_INT, local_data, chunk_size * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Parallelization with OpenMP (optional)
    bool enable_openmp = false; // Set to false to disable OpenMP
    if (enable_openmp) {
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < chunk_size; i++) {
            for (int j = 0; j < N; j++) {
                local_data[i][j] *= local_data[i][j]; // Square the element
            }
        }
    } else {
        for (int i = 0; i < chunk_size; i++) {
            for (int j = 0; j < N; j++) {
                local_data[i][j] *= local_data[i][j]; // Square the element
            }
        }
    }

    // Gather the results back to process 0
    MPI_Gather(local_data, chunk_size * N, MPI_INT, data, chunk_size * N, MPI_INT, 0, MPI_COMM_WORLD);

    // End timing the execution
    double end_time = MPI_Wtime();
    double execution_time = end_time - start_time;

    // Process 0 prints the result
    if (rank == 0) {
        std::cout << "Execution Time: " << std::fixed << std::setprecision(6) << execution_time << " seconds\n" << std::endl;

        // Print the matrix
        std::cout << "Matrix Squared:" << std::endl;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                std::cout << data[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}