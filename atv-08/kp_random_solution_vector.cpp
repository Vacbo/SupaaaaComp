#include <vector>
#include <cstdio>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <ctime>

struct item {
    int id;
    int weight;
    int value;
};

// Function to generate a random solution
std::vector<bool> generateRandomSolution(int n) {
    std::vector<bool> solution(n);
    for (int i = 0; i < n; ++i) {
        solution[i] = rand() % 2; // Randomly select items (0 or 1)
    }
    return solution;
}

// Function to evaluate the solution
int evaluateRandomSolution(const std::vector<item> &items, const std::vector<bool> &solution, int maxWeight) {
    int totalWeight = 0, totalValue = 0;
    for (int i = 0; i < items.size(); ++i) {
        if (solution[i]) { // If the item is selected (1 in solution vector)
            totalWeight += items[i].weight;
            totalValue += items[i].value;
        }
    }
    if (totalWeight > maxWeight) {
        return -1; // Invalid solution (overweight)
    }
    return totalValue;
}

// Function to add more items to the knapsack if possible
int tryAddMoreItems(std::vector<item> &items, std::vector<bool> &solution, int maxWeight, int currentWeight, int currentValue) {
    for (int i = 0; i < items.size(); ++i) {
        if (!solution[i] && currentWeight + items[i].weight <= maxWeight) {
            solution[i] = true;
            currentWeight += items[i].weight;
            currentValue += items[i].value;
        }
    }
    return currentValue;
}

void findSelectedItemsRandom(const std::vector<item> &items, const std::vector<bool> &solution) {
    std::cout << "Selected items: ";
    for (int i = 0; i < solution.size(); ++i) {
        if (solution[i]) {
            std::cout << items[i].id << " "; // Print item IDs that are included
        }
    }
    std::cout << std::endl;
}

int main() {
    // Seed for random number generation
    srand(time(0));

    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    //Read input for item quantity and knapsack capacity
    int n, W;
    std::scanf("%d %d", &n, &W);

    // Read input for items
    std::vector<item> items(n);
    for (int i = 0; i < n; i++) {
        items[i].id = i;
        std::scanf("%d %d", &items[i].weight, &items[i].value);
    }

    // Variables to store the random solution
    std::vector<bool> randomSolution;
    int randomValue, totalWeight;

    // Keep generating random solutions until a valid one is found
    do {
        randomSolution = generateRandomSolution(n);
        randomValue = evaluateRandomSolution(items, randomSolution, W);

        if (randomValue != -1) {
            // Calculate the current total weight
            totalWeight = 0;
            for (int i = 0; i < items.size(); ++i) {
                if (randomSolution[i]) {
                    totalWeight += items[i].weight;
                }
            }

            // Try to add more items if there's still capacity
            randomValue = tryAddMoreItems(items, randomSolution, W, totalWeight, randomValue);
            break; // Exit the loop once a valid solution is found and optimized
        }
    } while (randomValue == -1); // Continue until a valid solution is generated

    // Output the valid and optimized random solution
    std::printf("Optimized random solution value: %d\n", randomValue);
    findSelectedItemsRandom(items, randomSolution);

    // Stop measuring time
    auto end = std::chrono::high_resolution_clock::now();

    // Compute and print the elapsed time
    std::chrono::duration<double> elapsed = end - start;
    std::printf("Elapsed time: %f seconds\n", elapsed.count());

    return 0;
}
