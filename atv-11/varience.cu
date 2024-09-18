#include <thrust/iterator/constant_iterator.h>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/transform_reduce.h>
#include <thrust/functional.h>
#include <iostream>

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
    // Get average of 3112 numbers
    const int n = 3112;
    double value = 0.0;

    thrust::host_vector<double> google_host(n);
    for (int i = 0; i < n; i++) {
        std::cin >> value;                     // Read a value from standard input
        google_host[i] = value;                // Store the value in the host vector
    }

    // Transfer data to the device
    thrust::device_vector<double> google_device = google_host;

    // Compute the sum of the elements
    double sum = thrust::reduce(google_device.begin(), google_device.end(), (double) 0, thrust::plus<double>());

    // Compute the mean
    double mean = sum / n;

    // Compute the sum of squared differences from the mean
    double sum_squared_diff = thrust::transform_reduce(google_device.begin(), google_device.end(),
                                                       square_diff_from_mean(mean), 0.0, thrust::plus<double>());

    // Compute the variance
    double variance = sum_squared_diff / n;

    // Print the variance
    std::cout << "variance: " << variance << std::endl;

    return 0;
}
