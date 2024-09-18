#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/transform.h>
#include <thrust/count.h>
#include <thrust/replace.h>
#include <thrust/functional.h>
#include <iostream>

// Functor to calculate the difference between consecutive elements
struct difference_op {
    __host__ __device__
    double operator()(const double& x, const double& y) const {
        return y - x;
    }
};

// Functor to identify non-positive differences (for replacement)
struct is_non_positive {
    __host__ __device__
    bool operator()(const double& x) const {
        return x <= 0.0;
    }
};

int main() {
    // Get 3112 stock prices
    const int n = 3112;
    double value = 0.0;

    thrust::host_vector<double> google_host(n);
    for (int i = 0; i < n; i++) {
        std::cin >> value;                     // Read a value from standard input
        google_host[i] = value;                // Store the value in the host vector
    }

    // Transfer data to the device
    thrust::device_vector<double> google_device = google_host;

    // Create a device vector to store the daily differences
    thrust::device_vector<double> daily_diff(n - 1);

    // Calculate the daily differences (y_i - x_i) for each pair of consecutive elements
    thrust::transform(google_device.begin(), google_device.end() - 1,
                      google_device.begin() + 1,
                      daily_diff.begin(),
                      difference_op());

    // Replace all non-positive differences with 0.0
    thrust::replace_if(daily_diff.begin(), daily_diff.end(), is_non_positive(), 0.0);

    // Count the number of positive changes (which are now non-zero values)
    int positive_count = thrust::count_if(daily_diff.begin(), daily_diff.end(), thrust::placeholders::_1 > 0.0);

    // Sum up all the positive changes (which are now the non-zero values in daily_diff)
    double positive_sum = thrust::reduce(daily_diff.begin(), daily_diff.end(), 0.0, thrust::plus<double>());

    // Calculate the average positive change (if there are positive changes)
    double average_positive_change = (positive_count > 0) ? (positive_sum / positive_count) : 0.0;

    // Print the number of positive changes and the average positive change
    std::cout << "Number of positive changes: " << positive_count << std::endl;
    std::cout << "Average positive change: " << average_positive_change << std::endl;

    return 0;
}
