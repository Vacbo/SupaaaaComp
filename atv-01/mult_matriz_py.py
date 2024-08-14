import time  # Importa o módulo time, que fornece funções para medir o tempo de execução do código.

def multiply_matrices(A, B):
    # Define a função multiply_matrices, que realiza a multiplicação de duas matrizes A e B.
    # O resultado é armazenado na matriz result.
    result = [[0 for _ in range(len(B[0]))] for _ in range(len(A))]
    # Cria uma matriz de zeros com o mesmo número de linhas que A e o mesmo número de colunas que B.
    # Esta matriz armazenará os resultados da multiplicação.

    for i in range(len(A)):
        # Itera sobre as linhas da matriz A.
        for j in range(len(B[0])):
            # Itera sobre as colunas da matriz B.
            for k in range(len(B)):
                # Itera sobre as colunas de A e as linhas de B para calcular o produto escalar da linha i de A com a coluna j de B.
                result[i][j] += A[i][k] * B[k][j]
                # Realiza a multiplicação dos elementos correspondentes de A e B e soma o resultado ao elemento result[i][j].

    return result
    # Retorna a matriz result, que contém o produto das matrizes A e B.

# Gerar duas matrizes de tamanho grande para o teste
N = 1300  # Define o tamanho N das matrizes quadradas (200x200).

A = [[i + j for j in range(N)] for i in range(N)]
# Gera a matriz A de tamanho N x N, onde cada elemento A[i][j] é a soma dos índices i e j.

B = [[i * j for j in range(N)] for i in range(N)]
# Gera a matriz B de tamanho N x N, onde cada elemento B[i][j] é o produto dos índices i e j.

start_time = time.time()
# Marca o tempo de início da multiplicação das matrizes usando a função time.time().

result = multiply_matrices(A, B)
# Chama a função multiply_matrices para multiplicar as matrizes A e B, armazenando o resultado em 'result'.

end_time = time.time()
# Marca o tempo de término da multiplicação usando a função time.time().

print(f"Tempo de execução para a multiplicação de matrizes: {end_time - start_time:.2f} segundos")
# Calcula e exibe o tempo de execução da multiplicação de matrizes, subtraindo start_time de end_time.
# O tempo é formatado para mostrar duas casas decimais.
