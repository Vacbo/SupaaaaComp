#include <algorithm>
#include <vector>
#include <chrono>
#include <iostream>

struct item {
    int id;
    int weight;
    int value;
};

bool greatest_value(const item &a, const item &b) {
    return a.value > b.value;
}

int greedy_kp_solution(const std::vector<item> &items, std::vector<bool> &solution, int maxWeight) {
    int totalValue = 0;
    int totalWeight = 0;
    for (int i = 0; i < items.size(); i++) {
        if (totalWeight + items[i].weight <= maxWeight) {
            solution[items[i].id] = true;
            totalValue += items[i].value;
            totalWeight += items[i].weight;
        }
    }
    return totalValue;
}

void print_solution(const std::vector<bool> &solution) {
    std::printf("Items in the Knapsack:\n");
    for (int i = 0; i < solution.size(); i++) {
        if (solution[i]) {
            std::printf("%d ", i);
        }
    }
    std::printf("\n\n");
}

int main() {
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

    // Sort itens by greater value
    std::sort(items.begin(), items.end(), greatest_value);

    // Solve the problem
    std::vector<bool> solution(n, false);
    int totalValue = greedy_kp_solution(items, solution, W);

    std::printf("Total value: %d\n\n", totalValue);

    // Print solution
    print_solution(solution);

    // Stop measuring time
    auto end = std::chrono::high_resolution_clock::now();

    // Compute and print the elapsed time
    std::chrono::duration<double> elapsed = end - start;
    std::printf("Elapsed time: %f seconds\n", elapsed.count());
}
