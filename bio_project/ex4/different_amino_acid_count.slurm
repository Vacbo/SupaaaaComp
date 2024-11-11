#!/bin/bash
#SBATCH --job-name=different_amino_acid_count
#SBATCH --output=different_amino_acid_count_%j.txt
#SBATCH --ntasks=4              
#SBATCH --cpus-per-task=4
#SBATCH --mem=1024        
#SBATCH --time=01:00:00          
#SBATCH --partition=espec      

# Run the MPI program
mpirun -np 4 ./different_amino_acid_count
