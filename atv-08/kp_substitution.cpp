#include <vector>
#include <cstdio>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <cstdlib> // For random generation
#include <ctime>

struct item {
    int id;
    int weight;
    int value;
};

// Function to generate a valid random solution
std::vector<bool> generateValidRandomSolution(const std::vector<item> &items, int maxWeight, int &currentWeight) {
    std::vector<bool> solution(items.size(), false);
    currentWeight = 0;
    for (int i = 0; i < items.size(); ++i) {
        if ((rand() % 2 == 1) && (currentWeight + items[i].weight <= maxWeight)) {
            solution[i] = true;
            currentWeight += items[i].weight;
        }
    }
    return solution;
}

// Function to evaluate the current value of the solution
int evaluateSolution(const std::vector<item> &items, const std::vector<bool> &solution) {
    int totalValue = 0;
    for (int i = 0; i < items.size(); ++i) {
        if (solution[i]) {
            totalValue += items[i].value;
        }
    }
    return totalValue;
}

// Function to perform "Mochila Cheia" - Try adding more items to fill the knapsack if possible
int mochilaCheia(std::vector<item> &items, std::vector<bool> &solution, int maxWeight, int &currentWeight, int currentValue) {
    for (int i = 0; i < items.size(); ++i) {
        if (!solution[i] && currentWeight + items[i].weight <= maxWeight) {
            solution[i] = true;
            currentWeight += items[i].weight;
            currentValue += items[i].value;
        }
    }
    return currentValue;
}

// Function to perform item substitution
int performItemSubstitution(std::vector<item> &items, std::vector<bool> &solution, int maxWeight, int &currentWeight, int currentValue) {
    bool improvementMade = true;

    // Iterate until no more improvements can be made
    while (improvementMade) {
        improvementMade = false;  // Reset the flag

        // Try to substitute each selected item with an unselected one
        for (int i = 0; i < items.size(); ++i) {
            if (solution[i]) {  // Only consider selected items for substitution
                for (int j = 0; j < items.size(); ++j) {
                    if (!solution[j]) {  // Try replacing with unselected items
                        int newWeight = currentWeight - items[i].weight + items[j].weight;
                        if (newWeight <= maxWeight) {  // Check if the new item fits within the capacity
                            int newValue = currentValue - items[i].value + items[j].value;
                            if (newValue > currentValue) {  // Check if the substitution improves the value
                                // Perform the substitution
                                solution[i] = false;
                                solution[j] = true;
                                currentWeight = newWeight;  // Update weight
                                currentValue = newValue;    // Update value
                                improvementMade = true;     // Mark that we made an improvement

                                // After substitution, repeat from "Mochila Cheia" step
                                currentValue = mochilaCheia(items, solution, maxWeight, currentWeight, currentValue);
                                break;  // Restart from the first item
                            }
                        }
                    }
                }
            }
            if (improvementMade) break;  // If an improvement was made, restart from step 2
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

    // Read input for item quantity and knapsack capacity
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
    int randomValue, totalWeight = 0;

    // Step 1: Generate a random solution
    randomSolution = generateValidRandomSolution(items, W, totalWeight);

    // Step 2: Execute "Mochila Cheia" to fill the knapsack as much as possible
    randomValue = mochilaCheia(items, randomSolution, W, totalWeight, evaluateSolution(items, randomSolution));

    // Steps 3 to 6: Perform item substitution to improve the solution
    randomValue = performItemSubstitution(items, randomSolution, W, totalWeight, randomValue);

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
