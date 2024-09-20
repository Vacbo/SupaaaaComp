#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/transform_reduce.h>
#include <thrust/functional.h>
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

    auto start = std::chrono::high_resolution_clock::now();

    // Calculate the sum of elements
    double sum = thrust::reduce(google_device.begin(), google_device.end(), 0.0, thrust::plus<double>());

    // Compute the mean
    double mean = sum / n;

    // Use transform_reduce to calculate the sum of squared differences in a single step
    double sum_squared_diff = thrust::transform_reduce(google_device.begin(), google_device.end(),
                                                       square_diff_from_mean(mean), 0.0, thrust::plus<double>());

    // Calculate variance
    double variance = sum_squared_diff / n;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;

    std::cout << "Variance (using transform_reduce): " << variance << std::endl;
    std::cout << "Elapsed time: " << elapsed_time.count() << " seconds" << std::endl;

    return 0;
}
