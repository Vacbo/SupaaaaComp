#!/bin/bash
#SBATCH --job-name=convolucao_limitado    # Nome do job
#SBATCH --output=process_cpu_2_3_%j.txt   # Gera um novo arquivo de saída a cada execução
#SBATCH --ntasks=2                        # Número de tarefas
#SBATCH --cpus-per-task=3                 # Número de threads por tarefa
#SBATCH --mem=1024MB                      # Memória total alocada por nó
#SBATCH --time=00:10:00                   # Tempo máximo de execução
#SBATCH --partition=espec                 # Fila do cluster a ser utilizada

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
mpirun ./convolucao