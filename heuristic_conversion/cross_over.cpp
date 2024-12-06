#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include <ctime>
#include <cmath>

struct KnapsackProblem {
    std::vector<int> weights;
    std::vector<int> values;
    int capacity;
};

struct GAParameters {
    int population_size;    // Number of solutions per population
    int num_generations;    // Number of generations to run
    double crossover_rate;  // Probability of performing crossover
    double mutation_rate;   // Probability of mutating a given offspring
};

std::mt19937 gen((unsigned)std::time(nullptr)); // random number generator

// Generate a random integer in [min, max]
int rand_int(int min_val, int max_val) {
    std::uniform_int_distribution<int> dist(min_val, max_val);
    return dist(gen);
}

// Generate a random double in [0, 1]
double rand_double() {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gen);
}

// Fitness function: sum of values if under capacity, else 0
std::vector<int> fitness_function(const KnapsackProblem &problem, const std::vector<std::vector<int>> &population) {
    std::vector<int> fitness(population.size(), 0);
    for (size_t i = 0; i < population.size(); i++) {
        int total_value = 0;
        int total_weight = 0;
        for (size_t j = 0; j < population[i].size(); j++) {
            if (population[i][j] == 1) {
                total_value += problem.values[j];
                total_weight += problem.weights[j];
            }
        }
        if (total_weight <= problem.capacity) {
            fitness[i] = total_value;
        } else {
            fitness[i] = 0; // Over capacity, fitness = 0
        }
    }
    return fitness;
}

// Selection: choose the best individuals to be parents
std::vector<std::vector<int>> selection(const std::vector<int> &fitness, int num_parents, const std::vector<std::vector<int>> &population) {
    // We pick the top fitness individuals
    // Make a vector of indices sorted by fitness descending
    std::vector<std::pair<int,int>> fit_idx;
    for (size_t i = 0; i < fitness.size(); i++) {
        fit_idx.push_back({fitness[i], (int)i});
    }
    std::sort(fit_idx.begin(), fit_idx.end(), [](auto &a, auto &b) {
        return a.first > b.first;
    });

    std::vector<std::vector<int>> parents(num_parents);
    for (int i = 0; i < num_parents; i++) {
        parents[i] = population[fit_idx[i].second];
    }

    return parents;
}

// Crossover: single-point crossover at the midpoint
std::vector<std::vector<int>> crossover(const std::vector<std::vector<int>> &parents, int num_offsprings, double crossover_rate) {
    // We'll perform crossover between pairs of parents chosen sequentially
    std::vector<std::vector<int>> offsprings;
    offsprings.reserve(num_offsprings);

    int p_size = (int)parents.size();
    int gene_length = (int)parents[0].size();
    int crossover_point = gene_length / 2;

    int i = 0;
    int count_off = 0;
    while (count_off < num_offsprings) {
        int parent1_index = i % p_size;
        int parent2_index = (i + 1) % p_size;
        double x = rand_double();
        if (x <= crossover_rate) {
            std::vector<int> child(gene_length);
            for (int g = 0; g < crossover_point; g++) {
                child[g] = parents[parent1_index][g];
            }
            for (int g = crossover_point; g < gene_length; g++) {
                child[g] = parents[parent2_index][g];
            }
            offsprings.push_back(child);
            count_off++;
        }
        i++;
    }

    return offsprings;
}

// Mutation: Flip a random gene with some probability
std::vector<std::vector<int>> mutation(const std::vector<std::vector<int>> &offsprings, double mutation_rate) {
    std::vector<std::vector<int>> mutants = offsprings;
    for (size_t i = 0; i < mutants.size(); i++) {
        double r = rand_double();
        if (r <= mutation_rate) {
            // pick a random gene to flip
            int gene_length = (int)mutants[i].size();
            int idx = rand_int(0, gene_length-1);
            mutants[i][idx] = 1 - mutants[i][idx]; // flip
        }
    }
    return mutants;
}

int main() {
    // Problem setup
    int num_items = 40;
    int knapsack_capacity = 100;

    // Generate random items (weights and values)
    std::vector<int> weights(num_items);
    std::vector<int> values(num_items);
    for (int i = 0; i < num_items; i++) {
        weights[i] = rand_int(1, 15);
        values[i] = rand_int(1, 100);
    }

    KnapsackProblem problem{weights, values, knapsack_capacity};

    // GA parameters
    GAParameters ga_params;
    ga_params.population_size = 16;
    ga_params.num_generations = 1000;
    ga_params.crossover_rate = 0.8;
    ga_params.mutation_rate = 0.15;

    // Initialize population randomly
    std::vector<std::vector<int>> population(ga_params.population_size, std::vector<int>(num_items,0));
    for (int i = 0; i < ga_params.population_size; i++) {
        for (int j = 0; j < num_items; j++) {
            population[i][j] = rand_int(0,1);
        }
    }

    int num_parents = ga_params.population_size / 2;
    int num_offsprings = ga_params.population_size - num_parents;

    // GA main loop
    std::vector<std::vector<int>> best_solutions;
    std::vector<std::vector<int>> fitness_history; // store fitness of each generation

    for (int generation = 0; generation < ga_params.num_generations; generation++) {
        std::vector<int> fitness = fitness_function(problem, population);
        // Keep track of fitness for visualization
        fitness_history.push_back(fitness);

        // Selection
        std::vector<std::vector<int>> parents = selection(fitness, num_parents, population);

        // Crossover
        std::vector<std::vector<int>> offsprings = crossover(parents, num_offsprings, ga_params.crossover_rate);

        // Mutation
        std::vector<std::vector<int>> mutants = mutation(offsprings, ga_params.mutation_rate);

        // New population
        for (int i = 0; i < num_parents; i++) {
            population[i] = parents[i];
        }
        for (int i = 0; i < num_offsprings; i++) {
            population[num_parents + i] = mutants[i];
        }
    }

    // After final generation
    std::vector<int> final_fitness = fitness_function(problem, population);
    auto max_iter = std::max_element(final_fitness.begin(), final_fitness.end());
    int max_index = (int)std::distance(final_fitness.begin(), max_iter);
    int max_fit = *max_iter;

    std::cout << "Best Individual from the last generation (Fitness = " << max_fit << "):" << std::endl;
    for (int g = 0; g < num_items; g++) {
        std::cout << population[max_index][g] << " ";
    }
    std::cout << std::endl;

    // Print chosen items
    std::cout << "Selected items:" << std::endl;
    int total_weight = 0;
    for (int g = 0; g < num_items; g++) {
        if (population[max_index][g] == 1) {
            std::cout << "Item " << (g+1) << " (W=" << weights[g] << ",V=" << values[g] << ") ";
            total_weight += weights[g];
        }
    }
    std::cout << std::endl;
    std::cout << "Total weight = " << total_weight << std::endl;

    // Print some statistics about fitness over generations
    // (Mean and Max fitness per generation)
    std::cout << "\nFitness over generations:" << std::endl;
    for (int gen = 0; gen < ga_params.num_generations; gen++) {
        double sum_f = 0.0;
        int max_f = 0;
        for (auto f : fitness_history[gen]) {
            sum_f += f;
            if (f > max_f) max_f = f;
        }
        double mean_f = sum_f / fitness_history[gen].size();
        if (gen % (ga_params.num_generations / 10) == 0) { // Print every 10% generations
            std::cout << "Gen " << gen << ": Mean Fitness = " << mean_f << ", Max Fitness = " << max_f << std::endl;
        }
    }

    return 0;
}