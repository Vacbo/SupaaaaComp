#!/bin/bash
#SBATCH --job-name=executavel
#SBATCH --output=executavel_%j.txt
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=5
#SBATCH --mem=3G
#SBATCH --time=00:15:00
#SBATCH --partition=prova

export OMP_NUM_THREADS=10

mpirun -np 2 ./executavel
