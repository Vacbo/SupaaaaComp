#!/bin/bash
#SBATCH --job-name=heuristic_conv
#SBATCH --output=heuristic_conv_%j.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=1024
#SBATCH --time=00:15:00
#SBATCH --partition=espec

./cross_over
