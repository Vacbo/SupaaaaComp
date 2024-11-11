#include <iostream>
#include <fstream>
#include <string>
#include <mpi.h>
#include <omp.h>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

int count_valid_orfs(const std::string &sequence) {
    int valid_orf_count = 0;

    #pragma omp parallel for reduction(+:valid_orf_count)
    for (size_t i = 0; i <= sequence.size() - 3; i += 3) {
        std::string codon = sequence.substr(i, 3);

        // Check for the start codon
        if (codon == "AUG") {
            // Look for the next stop codon after "AUG" in the same reading frame
            bool found_stop = false;
            for (size_t j = i + 3; j <= sequence.size() - 3; j += 3) {
                std::string next_codon = sequence.substr(j, 3);
                if (next_codon == "UAA" || next_codon == "UAG" || next_codon == "UGA") {
                    // Valid ORF found from start to stop codon
                    valid_orf_count++;
                    found_stop = true;
                    i = j; // Move `i` to the position of the stop codon so the loop increment will advance it
                    break;
                }
            }
        }
    }

    return valid_orf_count;
}

void read_rna(const std::string &filename, std::string &sequence) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        // Convert the line to uppercase
        std::transform(line.begin(), line.end(), line.begin(), ::toupper);
        sequence += line;
    }
    file.close();
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int total_orf_count = 0;
    int local_orf_count = 0;

    // Process each RNA file according to the rank
    for (int i = world_rank + 1; i <= 22; i += world_size) {
        std::string input_filename = "../ex2/output/chr" + std::to_string(i) + ".rna_5_to_3.fa";
        std::string sequence;

        // Read RNA sequence from file
        read_rna(input_filename, sequence);

        // Count valid ORFs (start-to-stop sequences) in the RNA sequence
        local_orf_count += count_valid_orfs(sequence);
    }

    // Aggregate counts from all processes
    MPI_Reduce(&local_orf_count, &total_orf_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Output the result
    if (world_rank == 0) {
        std::cout << "Total count of valid ORFs across all files: " << total_orf_count << std::endl;
    }

    MPI_Finalize();
    return 0;
}