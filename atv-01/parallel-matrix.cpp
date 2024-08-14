#include <iostream>   // Inclui a biblioteca padrão de entrada e saída, usada para funções como std::cout.
#include <vector>     // Inclui a biblioteca de vetores da STL (Standard Template Library) do C++.
#include <chrono>     // Inclui a biblioteca para medição de tempo, utilizada para calcular a duração de execução.
#include <omp.h>      // Inclui a biblioteca OpenMP, usada para paralelismo em C++.

void multiply_matrices(const std::vector<std::vector<int> >& A, const std::vector<std::vector<int> >& B, std::vector<std::vector<int> >& result) {
    // Define a função que realiza a multiplicação de duas matrizes A e B, armazenando o resultado na matriz 'result'.
    #pragma omp parallel for
    // Diretiva OpenMP que paraleliza o loop 'for' que segue. Cada iteração do loop externo será executada em paralelo.
    for (size_t i = 0; i < A.size(); ++i) {
        // Itera sobre as linhas da matriz A. 'size_t' é um tipo de dado usado para representar tamanhos e índices.
        for (size_t j = 0; j < B[0].size(); ++j) {
            // Itera sobre as colunas da matriz B.
            result[i][j] = 0;
            // Inicializa o elemento [i][j] da matriz result com 0 antes de somar os produtos.
            for (size_t k = 0; k < B.size(); ++k) {
                // Itera sobre as colunas de A e as linhas de B para calcular o produto escalar da linha i de A com a coluna j de B.
                result[i][j] += A[i][k] * B[k][j];
                // Realiza a multiplicação dos elementos correspondentes de A e B, somando o resultado ao elemento result[i][j].
            }
        }
    }
}

int main() {
    size_t N = 1300;
    // Define o tamanho N das matrizes quadradas (200x200).
    std::vector<std::vector<int> > A(N, std::vector<int>(N));
    // Declara e inicializa a matriz A como uma matriz NxN preenchida com zeros.
    std::vector<std::vector<int> > B(N, std::vector<int>(N));
    // Declara e inicializa a matriz B como uma matriz NxN preenchida com zeros.
    std::vector<std::vector<int> > result(N, std::vector<int>(N));
    // Declara e inicializa a matriz result como uma matriz NxN preenchida com zeros, que armazenará o resultado da multiplicação.

    for (size_t i = 0; i < N; ++i) {
        // Itera sobre as linhas da matriz A e B.
        for (size_t j = 0; j < N; ++j) {
            // Itera sobre as colunas da matriz A e B.
            A[i][j] = i + j;
            // Preenche a matriz A com valores como a soma dos índices i e j.
            B[i][j] = i * j;
            // Preenche a matriz B com valores como o produto dos índices i e j.
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    // Marca o tempo de início da multiplicação das matrizes usando o relógio de alta resolução.
    multiply_matrices(A, B, result);
    // Chama a função multiply_matrices para multiplicar as matrizes A e B, armazenando o resultado em 'result'.
    auto end = std::chrono::high_resolution_clock::now();
    // Marca o tempo de término da multiplicação.

    std::chrono::duration<double> duration = end - start;
    // Calcula a duração da multiplicação subtraindo o tempo de início do tempo de término, armazenando o resultado em 'duration'.
    std::cout << "Tempo de execução para a multiplicação de matrizes em C++ com OpenMP: " << duration.count() << " segundos" << std::endl;
    // Exibe o tempo de execução da multiplicação de matrizes no console.

    return 0;
    // Retorna 0, indicando que o programa foi executado com sucesso.
}
