#include <omp.h>
#include <iostream>
#include <iomanip>

static long num_steps = 1024l * 1024 * 1024 * 2;
#define MIN_BLK 1024 * 1024 * 1024

double pi_r(long Nstart, long Nfinish, double step) {
    long i, iblk;
    double sum = 0.0;

    if (Nfinish - Nstart < MIN_BLK) {
        #pragma omp parallel for reduction(+:sum)
        for (i = Nstart; i < Nfinish; i++) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
    } else {
        iblk = Nfinish - Nstart;
        sum += pi_r(Nstart, Nfinish - iblk / 2, step);
        sum += pi_r(Nfinish - iblk / 2, Nfinish, step);
    }

    return sum;
}

int main() {
    long i;
    double step, pi;
    double init_time, final_time;
    step = 1.0 / (double)num_steps;

    init_time = omp_get_wtime();

    // Call the recursive function and store the final result
    double sum = pi_r(0, num_steps, step);

    pi = step * sum;
    final_time = omp_get_wtime() - init_time;

    std::cout << "Recursive reduction-based solution: For " << num_steps << " steps, pi = " 
              << std::setprecision(15) << pi << " in " << final_time << " secs\n";

    return 0;
}