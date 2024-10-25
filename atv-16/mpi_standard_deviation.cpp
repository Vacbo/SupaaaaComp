#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the rank of the current process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the total number of processes
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Define array size and portion size per process
    const int array_size = 10000;
    int portion_size = array_size / size;
    std::vector<float> array;

    // Root process initializes the array with random values
    if (rank == 0) {
        array.resize(array_size);
        srand(time(0));  // Seed for random number generation
        for (int i = 0; i < array_size; ++i) {
            array[i] = static_cast<float>(rand() % 100); // Random values between 0 and 99
        }
        std::cout << "Root process initialized array with random values." << std::endl;
    }

    // Scatter the array among all processes
    std::vector<float> local_array(portion_size);
    MPI_Scatter(array.data(), portion_size, MPI_FLOAT, local_array.data(), portion_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Calculate local mean and variance
    float local_sum = std::accumulate(local_array.begin(), local_array.end(), 0.0);
    float local_mean = local_sum / portion_size;

    float local_variance_sum = 0.0;
    for (float value : local_array) {
        local_variance_sum += (value - local_mean) * (value - local_mean);
    }
    float local_variance = local_variance_sum / portion_size;

    // Gather local means and variances at the root process
    std::vector<float> all_means(size), all_variances(size);
    MPI_Gather(&local_mean, 1, MPI_FLOAT, all_means.data(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Gather(&local_variance, 1, MPI_FLOAT, all_variances.data(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Root process calculates the global standard deviation
    if (rank == 0) {
        // Calculate the global mean as the average of local means
        float global_mean = std::accumulate(all_means.begin(), all_means.end(), 0.0) / size;

        // Calculate global variance using all means and variances
        float global_variance_sum = 0.0;
        for (int i = 0; i < size; ++i) {
            global_variance_sum += all_variances[i] + (all_means[i] - global_mean) * (all_means[i] - global_mean);
        }
        float global_variance = global_variance_sum / size;
        float global_std_dev = std::sqrt(global_variance);

        std::cout << "Global standard deviation: " << global_std_dev << std::endl;
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}