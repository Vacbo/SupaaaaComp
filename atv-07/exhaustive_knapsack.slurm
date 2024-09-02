#!/bin/bash
#SBATCH --job-name=exhaustive_knapsack_problem                # Nome do job
#SBATCH --output=exhaustive_knapsack_newEntrada04_%j.txt      # Nome do arquivo de saída
#SBATCH --ntasks=8                                            # Número de tarefas (4 processos)
#SBATCH --cpus-per-task=4                                     # Número de threads por tarefa (2 threads por processo)
#SBATCH --mem=4096                                            # Memória total alocada por nó (1024 MB = 1 GB)
#SBATCH --time=03:00:00                                       # Tempo máximo de execução (10 minutos)
#SBATCH --partition=espec                                     # fila do cluster a ser utilizada

./knapsack_problem < newEntrada04.txt
