#include <iostream>
#include <fstream>
#include <string>
#include <mpi.h>
#include <omp.h>
#include <filesystem> 
#include <algorithm>  

namespace fs = std::filesystem;

void transcribe_dna_to_rna(std::string &sequence) {
    // Replace each base with its RNA complement
    #pragma omp parallel for
    for (size_t i = 0; i < sequence.size(); ++i) {
        switch (sequence[i]) {
            case 'A': sequence[i] = 'U'; break;
            case 'T': sequence[i] = 'A'; break;
            case 'C': sequence[i] = 'G'; break;
            case 'G': sequence[i] = 'C'; break;
            default: break;
        }
    }
    // Reverse the sequence to get the complementary RNA strand in 3' to 5' direction
    //std::reverse(sequence.begin(), sequence.end());
}

void read_fasta(const std::string &filename, std::string &sequence) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line[0] != '>') { // Skip header lines
            // Convert line to uppercase to ensure consistency
            std::transform(line.begin(), line.end(), line.begin(), ::toupper);
            sequence += line;
        }
    }
    file.close();
}

void write_rna_to_file(const std::string &filename, const std::string &sequence) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error writing to file: " << filename << std::endl;
        return;
    }
    file << sequence;
    file.close();
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Ensure the output directory exists (created by rank 0)
    if (world_rank == 0) {
        fs::create_directory("../ex2/output");
    }
    MPI_Barrier(MPI_COMM_WORLD); // Wait until output directory is created

    // Process each file according to the rank
    for (int i = world_rank + 1; i <= 22; i += world_size) {
        std::string input_filename = "../data/chr" + std::to_string(i) + ".subst.fa";
        std::string output_filename = "../ex2/output/chr" + std::to_string(i) + ".rna_5_to_3.fa";
        std::string sequence;

        // Read DNA sequence from the FASTA file
        read_fasta(input_filename, sequence);

        // Transcribe DNA to RNA with complement and inversion
        transcribe_dna_to_rna(sequence);

        // Write the RNA sequence to the output file
        write_rna_to_file(output_filename, sequence);
    }

    MPI_Finalize();

    // Print a message to indicate the completion of the program
    if (world_rank == 0) {
        std::cout << "DNA to RNA conversion completed successfully! RNA files written in output dir." << std::endl;
    }

    return 0;
}