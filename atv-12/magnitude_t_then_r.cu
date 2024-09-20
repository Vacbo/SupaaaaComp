#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/transform.h>
#include <thrust/reduce.h>
#include <cmath>
#include <iostream>
#include <chrono>

// Functor to square the values
struct square {
    __host__ __device__
    double operator()(const double& x) const {
        return x * x;
    }
};

int main() {
    const int n = 10000000;
    double value = 0.0;

    // Host vector
    thrust::host_vector<double> google_host(n);
    for (int i = 0; i < n; i++) {
        std::cin >> value;
        google_host[i] = value;
    }

    // Transfer to device
    thrust::device_vector<double> google_device = google_host;
    thrust::device_vector<double> squared_elements(n);

    auto start = std::chrono::high_resolution_clock::now();

    // Step 1: Square each element
    thrust::transform(google_device.begin(), google_device.end(), squared_elements.begin(), square());

    // Step 2: Sum the squared elements
    double sum_of_squares = thrust::reduce(squared_elements.begin(), squared_elements.end(), 0.0, thrust::plus<double>());

    // Step 3: Compute the magnitude
    double magnitude = std::sqrt(sum_of_squares);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;

    std::cout << "Magnitude (using separate operations): " << magnitude << std::endl;
    std::cout << "Elapsed time: " << elapsed_time.count() << " seconds" << std::endl;

    return 0;
}
