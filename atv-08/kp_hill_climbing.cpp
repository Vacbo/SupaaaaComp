#include <vector>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>

struct item {
    int id;
    int weight;
    int value;
};

// Function to generate a valid random solution
std::vector<bool> generateRandomSolution(const std::vector<item> &items, int maxWeight, int &currentWeight, int &currentValue) {
    std::vector<bool> solution(items.size(), false);
    currentWeight = 0;
    currentValue = 0;
    
    for (int i = 0; i < items.size(); ++i) {
        if ((rand() % 2 == 1) && (currentWeight + items[i].weight <= maxWeight)) {
            solution[i] = true;
            currentWeight += items[i].weight;
            currentValue += items[i].value;
        }
    }
    return solution;
}

// Optimized Hill Climbing algorithm
int hillClimbing(std::vector<item> &items, std::vector<bool> &solution, int maxWeight, int &currentWeight, int &currentValue) {
    bool improvement = true;

    // Keep improving until no better neighbor is found
    while (improvement) {
        improvement = false;

        // Iterate through each item, flipping one bit at a time
        for (int i = 0; i < items.size(); ++i) {
            // Try flipping the bit at position i (toggle inclusion of the item)
            bool wasSelected = solution[i];
            int newWeight = currentWeight;
            int newValue = currentValue;

            if (wasSelected) {
                // If the item was selected, we are removing it
                newWeight -= items[i].weight;
                newValue -= items[i].value;
            } else if (currentWeight + items[i].weight <= maxWeight) {
                // If the item wasn't selected, try adding it
                newWeight += items[i].weight;
                newValue += items[i].value;
            } else {
                // Skip if adding the item exceeds max weight
                continue;
            }

            // If the new solution is better, accept it
            if (newValue > currentValue) {
                solution[i] = !wasSelected;  // Flip the bit
                currentWeight = newWeight;
                currentValue = newValue;
                improvement = true;
                break;  // Restart with the new solution
            }
        }
    }

    return currentValue;
}

// Print the selected items in the solution
void printSelectedItems(const std::vector<item> &items, const std::vector<bool> &solution) {
    std::cout << "Selected items: ";
    for (int i = 0; i < solution.size(); ++i) {
        if (solution[i]) {
            std::cout << items[i].id << " ";
        }
    }
    std::cout << std::endl;
}

int main() {
    // Seed for random number generation
    srand(time(0) + getpid());

    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    // Read input for item quantity and knapsack capacity
    int n, W;
    std::scanf("%d %d", &n, &W);

    // Read input for items
    std::vector<item> items(n);
    for (int i = 0; i < n; i++) {
        items[i].id = i;
        std::scanf("%d %d", &items[i].weight, &items[i].value);
    }

    // Variables to store the current solution
    int currentWeight = 0;
    int currentValue = 0;
    
    // Step 1: Generate an initial random solution
    std::vector<bool> solution = generateRandomSolution(items, W, currentWeight, currentValue);

    // Step 2: Apply the Hill Climbing algorithm
    int bestValue = hillClimbing(items, solution, W, currentWeight, currentValue);

    // Output the final solution
    std::printf("Optimized random solution value: %d\n", bestValue);
    printSelectedItems(items, solution);

    // Stop measuring time
    auto end = std::chrono::high_resolution_clock::now();

    // Compute and print the elapsed time
    std::chrono::duration<double> elapsed = end - start;
    std::printf("Elapsed time: %f seconds\n", elapsed.count());

    return 0;
}