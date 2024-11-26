#include <climits>
#include <vector>
#include <iostream>
#include <omp.h>

int vec_mean(std::vector<int>& vec) {
    const int size = vec.size();

    int sum = 0;
    # pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < size; i++) {
        sum += vec[i];
    }
    return sum / size;
}

int vec_greater(std::vector<int>& vec) {
    int greater = INT_MIN;
    #pragma omp parallel for reduction(max:greater)
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] > greater) {
            greater = vec[i];
        }
    }
    return greater;
}

int vec_product(std::vector<int>& vec) {
    int product = 1;
    #pragma omp parallel for reduction(*:product)
    for (int i = 0; i < vec.size(); i++) {
        product *= vec[i];
    }
    return product;
}

int main(int argc, char *argv[]) {
    std::vector<std::vector<int>> listOfVectors = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
    };

    for (auto& vec : listOfVectors) {
        int mean, product, greater;
        #pragma omp task shared(mean, product, greater)
        {
            mean = vec_mean(vec);
            product = vec_product(vec);
            greater = vec_greater(vec);
        }

        #pragma omp taskwait
        std::cout << "Mean: " << mean << " Greater: " << greater << " Product: " << product << std::endl;
    }

    return 0;
}
