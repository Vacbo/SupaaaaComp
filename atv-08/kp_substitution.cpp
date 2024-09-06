#include <vector>
#include <cstdio>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>  
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
    std::random_device rd;
    std::mt19937 gen(rd()); // Seed the random number generator
    std::shuffle(indices.begin(), indices.end(), gen); // Shuffle with mt19937

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

// Function to perform "Mochila Cheia" - Try adding more items to fill the knapsack if possible
int mochilaCheia(const std::vector<item> &items, std::vector<bool> &solution, int maxWeight, int &currentWeight, int currentValue) {
    for (int i = 0; i < items.size(); ++i) {
        if (!solution[i] && currentWeight + items[i].weight <= maxWeight) {
            solution[i] = true;
            currentWeight += items[i].weight;
            currentValue += items[i].value;
        }
    }
    return currentValue;
}

// Function to perform optimized item substitution
int performItemSubstitution(std::vector<item> &items, std::vector<bool> &solution, int maxWeight, int &currentWeight, int currentValue) {
    bool improvementMade = true;

    // Track selected and unselected items
    std::vector<int> selectedItems, unselectedItems;
    for (int i = 0; i < items.size(); ++i) {
        if (solution[i]) selectedItems.push_back(i);  // Track selected items
        else unselectedItems.push_back(i);  // Track unselected items
    }

    // Iterate until no more improvements can be made
    while (improvementMade) {
        improvementMade = false;  // Reset the flag

        // Try to substitute each selected item with an unselected one
        for (int sel : selectedItems) {
            for (int unsel : unselectedItems) {
                int newWeight = currentWeight - items[sel].weight + items[unsel].weight;
                if (newWeight <= maxWeight) {  // Check if the substitution fits the knapsack
                    int newValue = currentValue - items[sel].value + items[unsel].value;
                    if (newValue > currentValue) {  // If substitution improves the value
                        // Perform the substitution
                        solution[sel] = false;
                        solution[unsel] = true;
                        currentWeight = newWeight;
                        currentValue = newValue;
                        improvementMade = true;

                        // Reorganize the selected and unselected lists
                        selectedItems.push_back(unsel);
                        selectedItems.erase(std::remove(selectedItems.begin(), selectedItems.end(), sel), selectedItems.end());
                        unselectedItems.push_back(sel);
                        unselectedItems.erase(std::remove(unselectedItems.begin(), unselectedItems.end(), unsel), unselectedItems.end());

                        break;  // Break early since an improvement was made
                    }
                }
            }
            if (improvementMade) break;  // Restart if an improvement is found
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

    // Step 1: Generate a random solution
    randomSolution = generateValidRandomSolution(items, W, totalWeight, totalValue);

    // Step 2: Execute "Mochila Cheia" to fill the knapsack as much as possible
    totalValue = mochilaCheia(items, randomSolution, W, totalWeight, totalValue);

    // Step 3: Perform item substitution to improve the solution
    totalValue = performItemSubstitution(items, randomSolution, W, totalWeight, totalValue);

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