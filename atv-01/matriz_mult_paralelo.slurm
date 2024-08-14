#!/bin/bash

#SBATCH --job-name=matriz_mult_paralelo
# Define o nome do job como "matrix_mult_paralelo". Esse nome aparece nas listas de jobs e é útil para identificar o job.

#SBATCH --output=matriz_mult_paralelo.out
# Especifica o arquivo onde a saída padrão (stdout) do job será salva.

#SBATCH --ntasks=1
# Define o número de tarefas que o job executará. Neste caso, o job executa uma única tarefa.

#SBATCH --cpus-per-task=4
# Especifica o número de CPUs que devem ser alocadas para esta tarefa.

#SBATCH --time=00:10:00
# Define o tempo máximo de execução para o job. Neste caso, o job tem um tempo limite de 10 minutos. Se o job exceder esse tempo, ele será automaticamente encerrado.

#SBATCH --partition=normal
# Especifica a partição (ou fila) onde o job será submetido. Aqui, o job será submetido a fila "normal".


./mult_matriz_paralelo
# Executa o programa compilado "mult_matriz_paralelo" dentro do nó de computação.
