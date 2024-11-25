#!/bin/bash
#SBATCH --job-name=dist_vec_mpi_omp
#SBATCH --output=dist_vec_mpi_omp_%j.txt
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=5
#SBATCH --mem=1024
#SBATCH --time=00:15:00
#SBATCH --partition=espec

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

mpirun -np 4 ./dist_vec_mpi_omp
