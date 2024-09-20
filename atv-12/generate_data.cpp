#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>

int main() {
    // Set the size of the vector
    const int N = 10000000;

    // Vector to store random values
    std::vector<double> random_values(N);

    // Seed the random number generator using the system clock
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());

    // Distribution for generating numbers between 0 and 1
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Fill the vector with random values
    for (int i = 0; i < N; ++i) {
        random_values[i] = dist(rng);
    }

    // Open the file to store the vector
    std::ofstream output_file("random_vector.txt");

    if (!output_file) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    // Write the values to the file, one per line
    for (int i = 0; i < N; ++i) {
        output_file << random_values[i] << "\n";
    }

    output_file.close();

    std::cout << "Random values written to random_vector.txt" << std::endl;

    return 0;
}
