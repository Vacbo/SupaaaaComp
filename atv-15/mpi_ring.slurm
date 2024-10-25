#!/bin/bash
#SBATCH --job-name=mpi_ring        
#SBATCH --output=mpi_ring_%j.txt      
#SBATCH --ntasks=3                                            
#SBATCH --cpus-per-task=1                                     
#SBATCH --mem=1024                                            
#SBATCH --time=00:03:00                                       
#SBATCH --partition=espec                                    

mpirun -np 3 ./mpi_ring
