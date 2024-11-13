#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <mpi.h>
#include <omp.h>
#include <algorithm> 

void count_bases(const std::string &sequence, int &countA, int &countT, int &countC, int &countG) {
    #pragma omp parallel for reduction(+:countA, countT, countC, countG)
    for (size_t i = 0; i < sequence.size(); ++i) {
        switch (sequence[i]) {
            case 'A': countA++; break;
            case 'T': countT++; break;
            case 'C': countC++; break;
            case 'G': countG++; break;
            default: break;
        }
    }
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
            // Convert line to uppercase before appending
            std::transform(line.begin(), line.end(), line.begin(), ::toupper);
            sequence += line;
        }
    }
    file.close();
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int countA = 0, countT = 0, countC = 0, countG = 0;

    // Determine files to process by each rank
    for (int i = world_rank + 1; i <= 22; i += world_size) {
        std::string filename = "../data/chr" + std::to_string(i) + ".subst.fa";
        std::string sequence;
        read_fasta(filename, sequence);

        // Count bases in the current file sequence
        count_bases(sequence, countA, countT, countC, countG);
    }

    // Aggregate counts from all processes
    int totalA = 0, totalT = 0, totalC = 0, totalG = 0;
    MPI_Reduce(&countA, &totalA, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&countT, &totalT, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&countC, &totalC, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&countG, &totalG, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Output the result
    if (world_rank == 0) {
        std::cout << "Base counts across all files:" << std::endl;
        std::cout << "A: " << totalA << std::endl;
        std::cout << "T: " << totalT << std::endl;
        std::cout << "C: " << totalC << std::endl;
        std::cout << "G: " << totalG << std::endl;
    }

    MPI_Finalize();
    return 0;
}