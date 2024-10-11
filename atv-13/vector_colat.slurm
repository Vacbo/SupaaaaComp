#!/bin/bash
#SBATCH --job-name=vec_colat              
#SBATCH --output=vector_colat_%j.txt      
#SBATCH --ntasks=8                                            
#SBATCH --cpus-per-task=4                                     
#SBATCH --mem=1024                                            
#SBATCH --time=00:15:00                                       
#SBATCH --partition=espec                                    

./vector_colat
