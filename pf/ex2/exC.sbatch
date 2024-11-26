#!/bin/bash
#SBATCH --job-name=exC
#SBATCH --output=exC_%j.txt
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=4
#SBATCH --mem=1024
#SBATCH --time=00:05:00
#SBATCH --partition=espec

./exC
