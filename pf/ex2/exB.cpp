#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

std::vector<double> normalize(std::vector<double>& v) {
    double sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (double num : v) {
        sum += num * num;
    }

    double magnitude = sqrt(sum);
    #pragma omp parallel for
    for (double& num : v) {
        num /= magnitude;
    }
    return v;
}

int main() {
    std::vector<double> vec{1.0, 2.0, 3.0, 4.0};
    vec = normalize(vec);
    for (auto v : vec) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    return 0;
}
