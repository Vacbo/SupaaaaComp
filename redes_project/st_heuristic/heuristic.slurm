#!/bin/bash
#SBATCH --job-name=heuristic
#SBATCH --output=heuristic_clique_250_%j.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=1024
#SBATCH --time=00:15:00
#SBATCH --partition=espec

./max_clique_heuristic ../graphs/grafo_250.txt
