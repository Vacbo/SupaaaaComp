#include <iostream>
#include <vector>
#include <ctime>
#include <omp.h>
#include <random>

const int N = 2000;  // Dimension of the matrix

void matrixMultiply(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B, std::vector<std::vector<double>>& C) {
    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0;
            # pragma omp parallel for reduction(+:sum) schedule(static)
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

int main() {
    // Initialize matrices
    std::vector<std::vector<double>> A(N, std::vector<double>(N));
    std::vector<std::vector<double>> B(N, std::vector<double>(N));
    std::vector<std::vector<double>> C(N, std::vector<double>(N));

    #pragma omp parallel
    {
        // Each thread gets its own random number generator
        std::random_device rd;
        std::mt19937 gen(rd() ^ omp_get_thread_num()); // Seed with unique thread number
        std::uniform_real_distribution<> dis(0, 100);  // Random numbers in the range [0, 100)

        #pragma omp parallel for collapse(2) schedule(static)
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = dis(gen);
                B[i][j] = dis(gen);
            }
        }
    }

    // Perform matrix multiplication
    // Timing the multiplication
    double start_time = omp_get_wtime();
    matrixMultiply(A, B, C);
    double time_taken = omp_get_wtime() - start_time;
    std::cout << "Time taken for matrix multiplication: " << time_taken << " seconds." << std::endl;

    // Optionally print matrix C
    // for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < N; j++) {
    //         std::cout << C[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    return 0;
}
