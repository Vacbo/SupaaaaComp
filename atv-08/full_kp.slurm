#!/bin/bash
#SBATCH --job-name=full_kp                                    # Nome do job
#SBATCH --output=full_kp_newEntrada03_%j.txt                     # Nome do arquivo de saída
#SBATCH --ntasks=8                                            # Número de tarefas (4 processos)
#SBATCH --cpus-per-task=4                                     # Número de threads por tarefa (2 threads por processo)
#SBATCH --mem=4096                                            # Memória total alocada por nó (1024 MB = 1 GB)
#SBATCH --time=00:10:00                                       # Tempo máximo de execução (10 minutos)
#SBATCH --partition=espec                                     # fila do cluster a ser utilizada

./kp_random_solution_vector < newEntrada03.txt
./kp_random_solution_vector < newEntrada03.txt
./kp_random_solution_vector < newEntrada03.txt
./kp_random_solution_vector < newEntrada03.txt
./kp_random_solution_vector < newEntrada03.txt
./kp_random_solution_vector < newEntrada03.txt
./kp_random_solution_vector < newEntrada03.txt
./kp_random_solution_vector < newEntrada03.txt
./kp_random_solution_vector < newEntrada03.txt
./kp_random_solution_vector < newEntrada03.txt
