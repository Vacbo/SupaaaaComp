#!/bin/bash
#SBATCH --job-name=pi_mc            
#SBATCH --output=pi_monte_carlo_%j.txt      
#SBATCH --ntasks=8                                            
#SBATCH --cpus-per-task=4                                     
#SBATCH --mem=1024                                            
#SBATCH --time=00:15:00                                       
#SBATCH --partition=espec                                    

./monte_carlo_pi
