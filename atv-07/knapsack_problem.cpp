#include <vector>
#include <cstdio>
#include <algorithm>
#include <chrono>
#include <iostream>

struct item {
    int id;
    int weight;
    int value;
};

int search(int W, int n, std::vector<item> items, std::vector<std::vector<int> > table, std::vector<std::vector<bool> > &itens_to_keep){
    if (n == 0 || W == 0) {
        return 0;
    }

    if (table[n][W] != -1) {
        return table[n][W];
    }

    if (items[n-1].weight > W) {
        table[n][W] = search(W, n-1, items, table, itens_to_keep);
    } else {
        int without_item = search(W, n-1, items, table, itens_to_keep);
        int with_item = items[n-1].value + search(W - items[n-1].weight, n-1, items, table, itens_to_keep);

        if (with_item > without_item) {
            table[n][W] = with_item;
            itens_to_keep[n][W] = true;
        } else {
            table[n][W] = without_item;
            itens_to_keep[n][W] = false;
        }
    }

    return table[n][W];
}

void findSelectedItems(int W, int n, const std::vector<item>& items, const std::vector<std::vector<bool> >& keep) {
    int w = W;
    std::vector<int> selectedItems;

    for (int i = n; i > 0; i--) {
        if (keep[i][w]) {
            selectedItems.push_back(items[i-1].id);
            w -= items[i-1].weight;
        }
    }

    std::reverse(selectedItems.begin(), selectedItems.end()); // Optional: reverse to get the items in the order of input
    std::cout << "Selected items: ";
    for (int id : selectedItems) {
        std::cout << id << " ";
    }
    std::cout << std::endl;
}

int main(){
    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    //Read input for item quantity and knapsack capacity
    int n, W;
    std::scanf("%d %d", &n, &W);

    // Read input for items
    std::vector<item> items(n);
    for(int i = 0; i < n; i++) {
        items[i].id = i;
        std::scanf("%d %d", &items[i].weight, &items[i].value);
    }

    // Create a table to store the maximum value that can be obtained with a given weight
    std::vector<std::vector<int> > table(n+1, std::vector<int>(W+1, -1));

    // Create a table to store the items that were selected
    std::vector<std::vector<bool> > itens_to_keep(n+1, std::vector<bool>(W+1, false));

    // Compute the result using the search function
    int result = search(W, n, items, table, itens_to_keep);
    std::printf("Maximum value: %d\n", result);

    // Find the selected items
    findSelectedItems(W, n, items, itens_to_keep);

    // Stop measuring time
    auto end = std::chrono::high_resolution_clock::now();

    // Compute and print the elapsed time
    std::chrono::duration<double> elapsed = end - start;
    std::printf("Elapsed time: %f seconds\n", elapsed.count());

    return 0;
}
