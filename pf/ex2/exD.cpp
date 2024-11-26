#include <iostream>
#include <vector>
#include <omp.h>

// Função que calcula a soma dos elementos de um vetor
double vectorSum(const std::vector<double>& vec) {
    double sum = 0.0; // Esta será a variável private
    #pragma omp parallel for reduction(+:sum)
    for (double val : vec) {
        sum += val;
    }
    return sum;
}

int main() {
    std::vector<std::vector<double>> listOfVectors = {
    {1.0, 2.0, 3.0},
    {4.0, 5.0, 6.0},
    {7.0, 8.0, 9.0}
    };

    #pragma omp parallel for
    for (const auto& vec : listOfVectors) {
        double sum = vectorSum(vec);
        #pragma omp critical
        {
            std::cout << "Sum of vector elements: " << sum << std::endl;
        }
    }
    return 0;
}
