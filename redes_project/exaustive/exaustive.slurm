#!/bin/bash
#SBATCH --job-name=exaustive_clique
#SBATCH --output=exaustive_clique_%j.txt
#SBATCH --ntasks=8
#SBATCH --cpus-per-task=4
#SBATCH --mem=1024
#SBATCH --time=00:15:00
#SBATCH --partition=espec

./maximum_clique_exaustive
