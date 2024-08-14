#include <iostream>   // Inclui a biblioteca padrão de entrada e saída do C++ (necessária para usar std::cout).
#include <vector>     // Inclui a biblioteca de vetores da STL (Standard Template Library) do C++, usada para criar matrizes dinâmicas.
#include <chrono>     // Inclui a biblioteca de medição de tempo do C++ (necessária para medir o tempo de execução).

// Função para multiplicar duas matrizes A e B, armazenando o resultado na matriz 'result'.
void multiply_matrices(const std::vector<std::vector<int> >& A, const std::vector<std::vector<int> >& B, std::vector<std::vector<int> >& result) {
   // Loop para iterar sobre as linhas da matriz A.
   for (size_t i = 0; i < A.size(); ++i) {
      // Loop para iterar sobre as colunas da matriz B.
      for (size_t j = 0; j < B[0].size(); ++j) {
            result[i][j] = 0; // Inicializa o elemento result[i][j] com 0 antes de somar os produtos.
            // Loop para calcular o produto escalar da linha i de A com a coluna j de B.
            for (size_t k = 0; k < B.size(); ++k) {
               result[i][j] += A[i][k] * B[k][j]; // Realiza a multiplicação e soma dos elementos correspondentes de A e B.
            }
      }
   }
}

// Função principal do programa.
int main() {
   size_t N = 1300; // Define o tamanho N das matrizes quadradas (200x200).

   // Declara e inicializa a matriz A como uma matriz NxN preenchida com zeros.
   std::vector<std::vector<int> > A(N, std::vector<int>(N));

   // Declara e inicializa a matriz B como uma matriz NxN preenchida com zeros.
   std::vector<std::vector<int> > B(N, std::vector<int>(N));

   // Declara e inicializa a matriz result como uma matriz NxN preenchida com zeros, que armazenará o resultado da multiplicação.
   std::vector<std::vector<int> > result(N, std::vector<int>(N));

   // Loop para preencher as matrizes A e B com valores.
   for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
            A[i][j] = i + j; // Preenche a matriz A com a soma dos índices i e j.
            B[i][j] = i * j; // Preenche a matriz B com o produto dos índices i e j.
      }
   }

   // Marca o tempo de início da multiplicação das matrizes usando o relógio de alta resolução.
   auto start = std::chrono::high_resolution_clock::now();

   // Chama a função multiply_matrices para multiplicar as matrizes A e B, armazenando o resultado em 'result'.
   multiply_matrices(A, B, result);

   // Marca o tempo de término da multiplicação.
   auto end = std::chrono::high_resolution_clock::now();

   // Calcula a duração da multiplicação subtraindo o tempo de início do tempo de término, armazenando o resultado em 'duration'.
   std::chrono::duration<double> duration = end - start;

   // Exibe o tempo de execução da multiplicação de matrizes no console.
   std::cout << "Tempo de execução para a multiplicação de matrizes em C++: " << duration.count() << " segundos" << std::endl;

   return 0; // Retorna 0, indicando que o programa foi executado com sucesso.
}
