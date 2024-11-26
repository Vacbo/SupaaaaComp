#!/bin/bash
#SBATCH --job-name=programa
#SBATCH --output=programa_%j.txt
#SBATCH --nodes=5
#SBATCH --ntasks=5
#SBATCH --cpus-per-task=4
#SBATCH --mem=16G
#SBATCH --time=01:00:00
#SBATCH --partition=prova

export OMP_NUM_THREADS=15

mpirun -np 5 ./programa
