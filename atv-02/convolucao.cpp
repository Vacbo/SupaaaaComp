#include <iostream>   // Inclui a biblioteca padrão de entrada e saída.
#include <mpi.h>      // Inclui a biblioteca MPI para paralelismo entre múltiplos processos.
#include <omp.h>      // Inclui a biblioteca OpenMP para paralelismo com threads.
#include <chrono>     // Inclui a biblioteca para medição de tempo.

#define N 1000 // Define o tamanho da matriz NxN.
#define FILTER_SIZE 5 // Define o tamanho do filtro 5x5.
#define ITERATIONS 100 // Define o número de iterações de convolução.


// Função para realizar a convolução em um elemento da matriz.
int apply_filter(int x, int y, int matrix[N][N], int filter[FILTER_SIZE][FILTER_SIZE]) {
    int result = 0;
    int filter_offset = FILTER_SIZE / 2;  // Calcula o deslocamento do filtro para centralizar.

    // Aplica o filtro 5x5 ao elemento (x, y) da matriz.
    for (int i = -filter_offset; i <= filter_offset; i++) {
        for (int j = -filter_offset; j <= filter_offset; j++) {
            int xi = x + i;
            int yj = y + j;
            // Verifica se o índice está dentro dos limites da matriz.
            if (xi >= 0 && xi < N && yj >= 0 && yj < N) {
                result += matrix[xi][yj] * filter[i + filter_offset][j + filter_offset];
            }
        }
    }
    return result;  // Retorna o valor convoluído.
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);  // Inicializa o ambiente MPI.

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtém o rank (identificador) do processo atual.
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtém o número total de processos.

    int matrix[N][N]; // Declara a matriz original.
    int filter[FILTER_SIZE][FILTER_SIZE] = {{1, 1, 1, 1, 1}, 
                                            {1, 1, 1, 1, 1}, 
                                            {1, 1, 1, 1, 1}, 
                                            {1, 1, 1, 1, 1}, 
                                            {1, 1, 1, 1, 1}}; // Define o filtro 5x5 (exemplo de suavização).
    int result[N][N]; // Declara a matriz resultante após a convolução.

    // Inicialização da matriz pelo processo mestre (rank 0).
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i][j] = i + j; // Exemplo de inicialização: a soma dos índices.
            }
        }
    }

    // Sincroniza todos os processos antes de começar a medir o tempo.
    MPI_Barrier(MPI_COMM_WORLD);
    auto start = std::chrono::high_resolution_clock::now(); // Inicia a medição de tempo.

    // Broadcast da matriz original para todos os processos.
    MPI_Bcast(matrix, N*N, MPI_INT, 0, MPI_COMM_WORLD);

    // Divide a matriz entre os processos.
    int rows_per_process = N / size; // Calcula o número de linhas que cada processo irá processar.
    int start_row = rank * rows_per_process; // Determina a linha inicial que cada processo irá processar.
    int end_row = (rank == size - 1) ? N : start_row + rows_per_process; // Determina a linha final (último processo pode pegar as linhas restantes).

    // Loop de múltiplas convoluções.
    for (int iter = 0; iter < ITERATIONS; iter++) {
        #pragma omp parallel for collapse(2) // Paraleliza a convolução usando OpenMP.
        for (int i = start_row; i < end_row; i++) {
            for (int j = 0; j < N; j++) {
                result[i][j] = apply_filter(i, j, matrix, filter); // Aplica o filtro na matriz local.
            }
        }
        // Sincroniza os processos MPI para garantir que a matriz esteja atualizada para a próxima iteração.
        MPI_Allgather(MPI_IN_PLACE, 0, MPI_DATATYPE_NULL, result[start_row], rows_per_process*N, MPI_INT, MPI_COMM_WORLD);

        // Copia a matriz resultante para a matriz original para a próxima iteração.
        #pragma omp parallel for collapse(2) // Paraleliza a cópia usando OpenMP.
        for (int i = start_row; i < end_row; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i][j] = result[i][j];
            }
        }
    }

    // Sincroniza todos os processos antes de finalizar a medição de tempo.
    MPI_Barrier(MPI_COMM_WORLD);
    auto end = std::chrono::high_resolution_clock::now(); // Finaliza a medição de tempo.

    // Calcula o tempo de execução.
    std::chrono::duration<double> duration = end - start;
    double exec_time = duration.count(); // Tempo de execução em segundos.

    // Coleta os tempos de execução de todos os processos e exibe no processo mestre.
    double max_exec_time;
    MPI_Reduce(&exec_time, &max_exec_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) { // Apenas o processo mestre imprime o tempo de execução.
        std::cout << "A tarefa levou " << max_exec_time << " segundos para ser executada" << std::endl;
    }

    MPI_Finalize();  // Finaliza o ambiente MPI.
    return 0;  // Retorna 0, indicando que o programa terminou com sucesso.
}