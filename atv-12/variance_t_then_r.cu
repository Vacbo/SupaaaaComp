#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/transform.h>
#include <thrust/reduce.h>
#include <iostream>
#include <chrono>

// Functor to compute squared difference from mean
struct square_diff_from_mean {
    double mean;
    square_diff_from_mean(double _mean) : mean(_mean) {}

    __host__ __device__
    double operator()(const double& x) const {
        double diff = x - mean;
        return diff * diff;
    }
};

int main() {
    const int n = 10000000;
    double value = 0.0;

    thrust::host_vector<double> google_host(n);
    for (int i = 0; i < n; i++) {
        std::cin >> value;
        google_host[i] = value;
    }

    thrust::device_vector<double> google_device = google_host;
    thrust::device_vector<double> squared_diff(n);

    auto start = std::chrono::high_resolution_clock::now();

    // Step 1: Calculate the sum of elements
    double sum = thrust::reduce(google_device.begin(), google_device.end(), 0.0, thrust::plus<double>());

    // Step 2: Compute the mean
    double mean = sum / n;

    // Step 3: Transform the elements by subtracting the mean and squaring the difference
    thrust::transform(google_device.begin(), google_device.end(), squared_diff.begin(),
                      square_diff_from_mean(mean));

    // Step 4: Sum the squared differences
    double sum_squared_diff = thrust::reduce(squared_diff.begin(), squared_diff.end(), 0.0, thrust::plus<double>());

    // Step 5: Calculate the variance
    double variance = sum_squared_diff / n;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;

    std::cout << "Variance (using separate operations): " << variance << std::endl;
    std::cout << "Elapsed time: " << elapsed_time.count() << " seconds" << std::endl;

    return 0;
}
