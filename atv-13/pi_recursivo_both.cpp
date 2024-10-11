#include <omp.h>
#include <iostream>
#include <iomanip>

static long num_steps = 1024l * 1024 * 1024 * 2;
#define MIN_BLK 1024 * 1024 * 1024

// Recursive function using both tasks and reduction
double pi_r(long Nstart, long Nfinish, double step) {
    long i;
    double sum = 0.0;

    // If the block size is small enough, perform the parallel reduction
    if (Nfinish - Nstart < MIN_BLK) {
        #pragma omp parallel for reduction(+:sum)
        for (i = Nstart; i < Nfinish; i++) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
    } else {
        // Otherwise, split the problem into tasks
        long mid = (Nstart + Nfinish) / 2;
        
        // Create two tasks, one for each half of the problem
        double sum1 = 0.0, sum2 = 0.0;
        
        #pragma omp task shared(sum1)
        sum1 = pi_r(Nstart, mid, step);
        
        #pragma omp task shared(sum2)
        sum2 = pi_r(mid, Nfinish, step);

        // Wait for the tasks to finish
        #pragma omp taskwait
        
        // Combine the results from the two halves
        sum = sum1 + sum2;
    }

    return sum;
}

int main() {
    double step = 1.0 / (double)num_steps;
    double init_time, final_time;
    
    init_time = omp_get_wtime();  // Start time

    double sum = 0.0;

    // Start the parallel region and single thread to initiate the recursive tasks
    #pragma omp parallel
    {
        #pragma omp single
        {
            sum = pi_r(0, num_steps, step);
        }
    }

    double pi = step * sum;
    final_time = omp_get_wtime() - init_time;  // End time

    std::cout << "Solution with both tasks and reduction: For " << num_steps << " steps, pi = "
              << std::setprecision(15) << pi << " in " << final_time << " secs\n";

    return 0;
}