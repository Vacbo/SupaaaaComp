#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <mpi.h>
#include <filesystem>
#include <algorithm>
#include <omp.h>

namespace fs = std::filesystem;

// Define the codon table based on the provided codons
std::unordered_map<std::string, std::string> codon_table = {
    {"CCA", "Prolina"}, {"CCG", "Prolina"}, {"CCU", "Prolina"}, {"CCC", "Prolina"},
    {"UCU", "Serina"}, {"UCA", "Serina"}, {"UCG", "Serina"}, {"UCC", "Serina"},
    {"CAG", "Glutamina"}, {"CAA", "Glutamina"},
    {"ACA", "Treonina"}, {"ACC", "Treonina"}, {"ACU", "Treonina"}, {"ACG", "Treonina"},
    {"AUG", "Metionina (início)"},
    {"UGA", "STOP"}, {"UAA", "STOP"}, {"UAG", "STOP"},
    {"UGC", "Cisteína"}, {"UGU", "Cisteína"},
    {"GUG", "Valina"}, {"GUA", "Valina"}, {"GUC", "Valina"}, {"GUU", "Valina"}
};

// Function to read RNA sequence from a file
void read_rna(const std::string &filename, std::string &sequence) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::transform(line.begin(), line.end(), line.begin(), ::toupper); // Convert to uppercase
        sequence += line;
    }
    file.close();
}

// Function to translate only valid ORFs in the RNA sequence
std::vector<std::string> translate_to_amino_acids(const std::string &sequence, std::unordered_map<std::string, int> &amino_acid_counts) {
    std::vector<std::string> all_amino_acids;

    // Parallelized search for valid ORFs
    #pragma omp parallel
    {
        std::unordered_map<std::string, int> local_amino_acid_counts;
        std::vector<std::string> local_amino_acids;

        #pragma omp for
        for (size_t i = 0; i <= sequence.size() - 3; i += 3) {
            std::string codon = sequence.substr(i, 3);

            // Look for start codon
            if (codon == "AUG") {
                std::vector<std::string> orf_amino_acids;
                bool in_orf = true;

                // Translate until a stop codon is found
                for (size_t j = i; j <= sequence.size() - 3; j += 3) {
                    std::string amino_codon = sequence.substr(j, 3);

                    // Stop if a stop codon is found
                    if (amino_codon == "UGA" || amino_codon == "UAA" || amino_codon == "UAG") {
                        in_orf = false;
                        i = j; // Move `i` to the stop codon position to avoid recounting
                        break;
                    }

                    // Translate codon to amino acid and count it
                    if (codon_table.find(amino_codon) != codon_table.end()) {
                        std::string amino_acid = codon_table[amino_codon];
                        orf_amino_acids.push_back(amino_acid);
                        local_amino_acid_counts[amino_acid]++;
                    }
                }

                // If the ORF is valid, add it to the local list
                if (!orf_amino_acids.empty()) {
                    local_amino_acids.insert(local_amino_acids.end(), orf_amino_acids.begin(), orf_amino_acids.end());
                }
            }
        }

        // Combine local results into shared variables (requires critical section)
        #pragma omp critical
        {
            for (const auto &pair : local_amino_acid_counts) {
                amino_acid_counts[pair.first] += pair.second;
            }
            all_amino_acids.insert(all_amino_acids.end(), local_amino_acids.begin(), local_amino_acids.end());
        }
    }

    return all_amino_acids;
}

// Function to write amino acid sequence to a file
void write_amino_acids_to_file(const std::string &filename, const std::vector<std::string> &amino_acids) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error writing to file: " << filename << std::endl;
        return;
    }
    file << "Amino Acids Sequence:\n";
    for (const auto &amino_acid : amino_acids) {
        file << amino_acid << " ";
    }
    file.close();
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Ensure the output directory exists (created by rank 0)
    if (world_rank == 0) {
        fs::create_directory("../ex4/translated_output");
    }
    MPI_Barrier(MPI_COMM_WORLD); // Wait until output directory is created

    // Local amino acid counts for each process
    std::unordered_map<std::string, int> local_amino_acid_counts;

    // Process each RNA file according to the rank
    for (int i = world_rank + 1; i <= 22; i += world_size) {
        std::string input_filename = "../ex2/output/chr" + std::to_string(i) + ".rna_5_to_3.fa";
        std::string output_filename = "../ex4/translated_output/chr" + std::to_string(i) + ".amino_acids.txt";
        std::string sequence;

        // Read RNA sequence from file
        read_rna(input_filename, sequence);

        // Translate RNA sequence to amino acids and count them
        std::vector<std::string> amino_acids = translate_to_amino_acids(sequence, local_amino_acid_counts);

        // Write the amino acids sequence to the output file
        write_amino_acids_to_file(output_filename, amino_acids);
    }

    // Gather all local counts on the root process
    std::unordered_map<std::string, int> total_amino_acid_counts;
    if (world_rank == 0) {
        total_amino_acid_counts = local_amino_acid_counts;
        for (int i = 1; i < world_size; i++) {
            for (const auto &pair : local_amino_acid_counts) {
                int temp;
                MPI_Recv(&temp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                total_amino_acid_counts[pair.first] += temp;
            }
        }
    } else {
        for (const auto &pair : local_amino_acid_counts) {
            MPI_Send(&pair.second, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    // Print the total counts on the terminal (only on the root process)
    if (world_rank == 0) {
        std::cout << "Total Amino Acid Counts across all files:\n";
        for (const auto &entry : total_amino_acid_counts) {
            std::cout << entry.first << ": " << entry.second << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}