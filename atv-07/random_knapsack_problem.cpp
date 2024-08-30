#include <vector>
#include <cstdio>
#include <algorithm>
#include <chrono>
#include <random>

struct item {
    int id;
    int weight;
    int value;
};

int random_knapsack_search(int W, int n, std::vector<item> items) {
    int max_value = 0;
    int capacity_used = 0;

    // Set up random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < n; i++) {
        // Randomly decide whether to include the item or not
        if (dis(gen) == 1 < 0.5 && capacity_used + items[i].weight <= W) {
            max_value += items[i].value;
            capacity_used += items[i].weight;
        }
    }
    
    return max_value;
}

int main() {
    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    // Read input for item quantity and knapsack capacity
    int n, W;
    std::scanf("%d %d", &n, &W);

    // Read input for items
    std::vector<item> items(n);
    for(int i = 0; i < n; i++) {
        items[i].id = i;
        std::scanf("%d %d", &items[i].weight, &items[i].value);
    }

    // Set the seed for the random number generator
    std::srand(std::time(nullptr));

    // Shuffle the items
    std::random_shuffle(items.begin(), items.end());

    // Compute the result using the dumb_random_search function
    int result = random_knapsack_search(W, n, items);
    std::printf("Maximum value: %d\n", result);

    // Stop measuring time
    auto end = std::chrono::high_resolution_clock::now();

    // Compute and print the elapsed time
    std::chrono::duration<double> elapsed = end - start;
    std::printf("Elapsed time: %f seconds\n", elapsed.count());

    return 0;
}