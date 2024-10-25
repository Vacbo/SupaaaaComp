#!/bin/bash
#SBATCH --job-name=mpi_distributed_average      
#SBATCH --output=mpi_distributed_average_%j.txt      
#SBATCH --ntasks=8                                            
#SBATCH --cpus-per-task=1                                     
#SBATCH --mem=1024                                            
#SBATCH --time=00:03:00                                       
#SBATCH --partition=espec                                    

mpirun -np 8 ./mpi_distributed_average
