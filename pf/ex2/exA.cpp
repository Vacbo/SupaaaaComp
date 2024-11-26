#include <iostream>
#include <omp.h>

long long factorial(int n) {
    long long result = 1;
    #pragma omp parallel for reduction(*:result)
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    std::cout << "Fatorial de 10 é " << factorial(10) << std::endl;
}
