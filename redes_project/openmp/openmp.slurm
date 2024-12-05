#!/bin/bash
#SBATCH --job-name=omp_greedy_clique
#SBATCH --output=omp_greedy_clique_1000_%j.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=1024
#SBATCH --time=00:15:00
#SBATCH --partition=espec

./maximum_clique_greedy_openmp ../graphs/grafo_1000.txt
