#include <vector>
#include <cstdio>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <random>

// Structure for an item
struct item {
    int id;
    int weight;
    int value;
};

// Function to generate a valid random solution without exceeding the weight limit
std::vector<bool> generateValidRandomSolution(const std::vector<item> &items, int maxWeight, int &totalWeight, int &totalValue) {
    std::vector<bool> solution(items.size(), false);
    totalWeight = 0;
    totalValue = 0;

    // Create an index vector for shuffling
    std::vector<int> indices(items.size());
    for (int i = 0; i < items.size(); ++i) {
        indices[i] = i;
    }

    // Shuffle the item indices to simulate randomness
    std::shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device()()));

    // Try to add items to the knapsack until the weight limit is reached
    for (int i : indices) {
        if (totalWeight + items[i].weight <= maxWeight) {
            solution[i] = true;  // Select the item
            totalWeight += items[i].weight;
            totalValue += items[i].value;
        }
    }

    return solution;  // Return the valid solution
}

// Function to evaluate and potentially add more items if possible (Mochila Cheia approach)
int tryAddMoreItems(const std::vector<item> &items, std::vector<bool> &solution, int maxWeight, int &currentWeight, int currentValue) {
    for (int i = 0; i < items.size(); ++i) {
        if (!solution[i] && currentWeight + items[i].weight <= maxWeight) {
            solution[i] = true;
            currentWeight += items[i].weight;
            currentValue += items[i].value;
        }
    }
    return currentValue;
}

// Function to display the selected items
void findSelectedItemsRandom(const std::vector<item> &items, const std::vector<bool> &solution) {
    std::cout << "Selected items: ";
    for (int i = 0; i < solution.size(); ++i) {
        if (solution[i]) {
            std::cout << items[i].id << " ";  // Print the selected item IDs
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
    for (int i = 0; i < n; ++i) {
        items[i].id = i;
        std::scanf("%d %d", &items[i].weight, &items[i].value);
    }

    // Variables to store the random solution
    std::vector<bool> randomSolution;
    int totalWeight = 0, totalValue = 0;

    // Generate a valid random solution
    randomSolution = generateValidRandomSolution(items, W, totalWeight, totalValue);

    // Try to add more items if there's still capacity
    totalValue = tryAddMoreItems(items, randomSolution, W, totalWeight, totalValue);

    // Output the valid and optimized random solution
    std::printf("Optimized random solution value: %d\n", totalValue);
    findSelectedItemsRandom(items, randomSolution);

    // Stop measuring time
    auto end = std::chrono::high_resolution_clock::now();

    // Compute and print the elapsed time
    std::chrono::duration<double> elapsed = end - start;
    std::printf("Elapsed time: %f seconds\n", elapsed.count());

    return 0;
}