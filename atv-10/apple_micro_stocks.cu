#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/transform.h>
#include <thrust/reduce.h>
#include <iostream>
#include <sstream>
#include <string>

struct diff_functor {
    __host__ __device__
    double operator()(const double& a, const double& b) const {
        return a - b;
    }
};

int main() {
    const int n = 3520;  // Approximately 10 years of data
    thrust::host_vector<double> apple_host(n);
    thrust::host_vector<double> microsoft_host(n);

    std::string line;
    int i = 0;

    // Read from standard input (via redirection)
    while (std::getline(std::cin, line) && i < n) {
        std::stringstream ss(line);
        std::string apple_price, microsoft_price;
        std::getline(ss, apple_price, ',');
        std::getline(ss, microsoft_price, ',');

        apple_host[i] = std::stod(apple_price);
        microsoft_host[i] = std::stod(microsoft_price);
        i++;
    }

    // Transfer data to the device
    thrust::device_vector<double> apple_dev = apple_host;
    thrust::device_vector<double> microsoft_dev = microsoft_host;
    thrust::device_vector<double> diff_dev(n);

    // Calculate the point-by-point differences
    thrust::transform(apple_dev.begin(), apple_dev.end(), microsoft_dev.begin(), diff_dev.begin(), diff_functor());

    // Calculate the average difference
    double total_diff = thrust::reduce(diff_dev.begin(), diff_dev.end(), 0.0);
    double avg_diff = total_diff / n;

    std::cout << "The average price difference between Apple and Microsoft is: " << avg_diff << std::endl;

    return 0;
}
