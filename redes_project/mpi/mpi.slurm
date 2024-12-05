#!/bin/bash
#SBATCH --job-name=mpi_exaustive_clique
#SBATCH --output=mpi_exaustive_clique_100_%j.txt
#SBATCH --ntasks=8
#SBATCH --nodes=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=1024
#SBATCH --time=00:15:00
#SBATCH --partition=espec

mpirun -np 4 ./maximum_clique_exaustive_mpi ../graphs/grafo_100.txt
