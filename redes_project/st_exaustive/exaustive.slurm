#!/bin/bash
#SBATCH --job-name=exaustive_clique
#SBATCH --output=exaustive_clique_200_%j.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=1024
#SBATCH --time=00:15:00
#SBATCH --partition=espec

./maximum_clique_exaustive ../graphs/grafo_200.txt
