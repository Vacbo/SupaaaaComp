#!/bin/bash
#SBATCH --job-name=ex3
#SBATCH --output=ex3_%j.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem=1024
#SBATCH --time=00:05:00
#SBATCH --partition=espec

./ex3
