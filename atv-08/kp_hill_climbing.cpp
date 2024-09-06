#include <vector>
#include <cstdio>
#include <iostream>
#include <cstdlib> // For random generation
#include <ctime>
#include <algorithm>

struct item {
    int id;
    int weight;
    int value;
};

// Function to generate a valid random solution
std::vector<bool> generateRandomSolution(const std::vector<item> &items, int maxWeight, int &currentWeight) {
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

// Function to evaluate the value of a solution and ensure it respects the weight limit
int evaluateSolution(const std::vector<item> &items, const std::vector<bool> &solution, int maxWeight, int &currentWeight) {
    int totalValue = 0;
    currentWeight = 0;
    for (int i = 0; i < items.size(); ++i) {
        if (solution[i]) {
            currentWeight += items[i].weight;
            totalValue += items[i].value;
        }
    }
    if (currentWeight > maxWeight) {
        return -1; // Invalid solution
    }
    return totalValue;
}

// Function to generate neighbors by flipping one bit at a time
std::vector<std::vector<bool>> generateNeighbors(const std::vector<bool> &solution) {
    std::vector<std::vector<bool>> neighbors;
    for (int i = 0; i < solution.size(); ++i) {
        std::vector<bool> neighbor = solution;
        neighbor[i] = !neighbor[i];  // Flip the bit
        neighbors.push_back(neighbor);
    }
    return neighbors;
}

// Hill Climbing algorithm
int hillClimbing(std::vector<item> &items, std::vector<bool> &solution, int maxWeight, int &currentWeight) {
    bool improvement = true;
    int currentValue = evaluateSolution(items, solution, maxWeight, currentWeight);

    // Keep improving until no better neighbor is found
    while (improvement) {
        improvement = false;
        std::vector<std::vector<bool>> neighbors = generateNeighbors(solution);

        // Iterate through each neighbor and find the best one
        for (const auto &neighbor : neighbors) {
            int neighborWeight;
            int neighborValue = evaluateSolution(items, neighbor, maxWeight, neighborWeight);

            if (neighborValue > currentValue) {
                // Move to the better neighbor
                solution = neighbor;
                currentWeight = neighborWeight;
                currentValue = neighborValue;
                improvement = true;
                break;  // Restart with the new solution
            }
        }
    }

    return currentValue;
}

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

    // Step 1: Generate an initial random solution
    int currentWeight;
    std::vector<bool> solution = generateRandomSolution(items, W, currentWeight);

    // Step 2: Apply the Hill Climbing algorithm
    int bestValue = hillClimbing(items, solution, W, currentWeight);

    // Output the final solution
    std::printf("Best solution value: %d\n", bestValue);
    printSelectedItems(items, solution);

    // Stop measuring time
    auto end = std::chrono::high_resolution_clock::now();

    // Compute and print the elapsed time
    std::chrono::duration<double> elapsed = end - start;
    std::printf("Elapsed time: %f seconds\n", elapsed.count());

    return 0;
}
