#!/bin/bash
#SBATCH --job-name=mpi_alternating_messages       
#SBATCH --output=mpi_alternating_messages_%j.txt      
#SBATCH --ntasks=4                                            
#SBATCH --cpus-per-task=1                                     
#SBATCH --mem=1024                                            
#SBATCH --time=00:03:00                                       
#SBATCH --partition=espec                                    

mpirun -np 4 ./mpi_alternating_messages
