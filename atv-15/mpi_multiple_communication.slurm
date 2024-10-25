#!/bin/bash
#SBATCH --job-name=mpi_multiple_communication        
#SBATCH --output=mpi_multiple_communication_%j.txt      
#SBATCH --ntasks=4                                            
#SBATCH --cpus-per-task=1                                     
#SBATCH --mem=1024                                            
#SBATCH --time=00:03:00                                       
#SBATCH --partition=espec                                    

mpirun -np 4 ./mpi_multiple_communication
