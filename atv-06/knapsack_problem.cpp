#include <vector>
#include <cstdio>
#include <algorithm>
#include <chrono>

struct item {
    int id;
    int weight;
    int value;
};

int search(int W, int n, std::vector<item> items, std::vector<std::vector<int> > table){
    if (n == 0 || W == 0) {
        return 0;
    }

    if (table[n][W] != -1) {
        return table[n][W];
    }

    if (items[n-1].weight > W) {
        table[n][W] = search(W, n-1, items, table);
    } else {
        table[n][W] = std::max(search(W, n-1, items, table), items[n-1].value + search(W - items[n-1].weight, n-1, items, table));
    }

    return table[n][W];

    return 0;
}

int main(){
    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    //Read input for item quantity and knapsack capacity
    int n, W;
    scanf("%d %d", &n, &W);

    // Read input for items
    std::vector<item> items(n);
    for(int i = 0; i < n; i++) {
        items[i].id = i;
        std::scanf("%d %d", &items[i].weight, &items[i].value);
    }

    // Create a table to store the maximum value that can be obtained with a given weight
    std::vector<std::vector<int> > table(n+1, std::vector<int>(W+1, -1));

    // Compute the result using the search function
    int result = search(W, n, items, table);
    std::printf("%d\n", result);

    // Stop measuring time
    auto end = std::chrono::high_resolution_clock::now();

    // Compute and print the elapsed time
    std::chrono::duration<double> elapsed = end - start;
    std::printf("Elapsed time: %f seconds\n", elapsed.count());

    return 0;
}
