#include <iostream>
#include <iomanip> 
#include <random>   
#include <ctime>    
#include <omp.h>
#include <cmath>

double estimatePi(int totalPoints) {
    int pointsInsideCircle = 0;

    // Same seed for all threads)
    std::mt19937 rng(static_cast<unsigned int>(std::time(0)));
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    #pragma omp parallel
    {
        int localPointsInsideCircle = 0;

        #pragma omp for
        for (int i = 0; i < totalPoints; ++i) {
            // Generate random points between 0 and 1 for x and y
            double x =  dist(rng);
            double y =  dist(rng);

            // Check if the point is inside the circle
            if (x * x + y * y <= 1) {
                localPointsInsideCircle++;
            }
        }

        #pragma omp critical
        {
            pointsInsideCircle += localPointsInsideCircle;
        }
    }

    return 4.0 * pointsInsideCircle / totalPoints;
}

int main() {
    const int N = 10000000;

    // Start timing
    double startTime = omp_get_wtime();

    // Estimate pi in parallel
    double piEstimate = estimatePi(N);

    // End timing
    double endTime = omp_get_wtime();

    // Calculate Pi accuracy in percentage
    double piAccuracy = (1.0 - std::abs(piEstimate - M_PI) / M_PI) * 100.0;

    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Estimated value of Pi: " << piEstimate << std::endl;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Execution time: " << (endTime - startTime) << " seconds" << std::endl;

    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Pi Accuracy: " << piAccuracy << "%" << std::endl;

    return 0;
}