#!/bin/bash
#SBATCH --job-name=pi_rec             
#SBATCH --output=pi_recursivo_%j.txt      
#SBATCH --ntasks=8                                            
#SBATCH --cpus-per-task=4                                     
#SBATCH --mem=1024                                            
#SBATCH --time=00:15:00                                       
#SBATCH --partition=espec                                    

./pi_recursivo
