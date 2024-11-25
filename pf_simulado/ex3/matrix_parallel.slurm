#!/bin/bash
#SBATCH --job-name=matrix_parallel
#SBATCH --output=matrix_parallel_%j.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=5
#SBATCH --mem=1024
#SBATCH --time=00:15:00
#SBATCH --partition=espec

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

./matrix_parallel
