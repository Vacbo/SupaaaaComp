#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

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

    float max_value = 0;

    // Root process initializes the array with random values and finds the maximum
    if (rank == 0) {
        array.resize(array_size);
        srand(time(0));

        // Populate array with random values between 0 and 100
        for (int i = 0; i < array_size; ++i) {
            array[i] = static_cast<float>(rand() % 100);
        }

        // Find the maximum value in the array
        max_value = *std::max_element(array.begin(), array.end());
        std::cout << "Root process found max value: " << max_value << std::endl;
    }

    // Broadcast the maximum value to all processes
    MPI_Bcast(&max_value, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Scatter the array among all processes
    std::vector<float> local_array(portion_size);
    MPI_Scatter(array.data(), portion_size, MPI_FLOAT, local_array.data(), portion_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Normalize the local array segment
    for (int i = 0; i < portion_size; ++i) {
        local_array[i] /= max_value;
    }

    // Gather the normalized segments back to the root process
    MPI_Gather(local_array.data(), portion_size, MPI_FLOAT, array.data(), portion_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // The root process prints the normalized array
    if (rank == 0) {
        std::cout << "Normalized array: ";
        for (int i = 0; i < array_size; ++i) {
            std::cout << array[i] << " ";
        }
        std::cout << std::endl;
    }

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}