#!/bin/bash
#SBATCH --job-name=mpi_hello         
#SBATCH --output=mpi_hello_%j.txt      
#SBATCH --ntasks=2                                            
#SBATCH --cpus-per-task=1                                     
#SBATCH --mem=1024                                            
#SBATCH --time=00:03:00                                       
#SBATCH --partition=espec                                    

mpirun -np 2 ./mpi_hello
