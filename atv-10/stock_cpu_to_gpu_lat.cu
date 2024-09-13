#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <iostream>
#include <chrono>
using namespace std;

int main() {
    int n = 389;
    double value = 0.0;
    std::chrono::duration<double> diff;

    auto leitura_i = std::chrono::steady_clock::now();

    thrust::host_vector<double> host(n, 0);
    for (int i = 0; i < n; i++) {
        cin >> value;              // Lê um valor da entrada padrão
        host[i] = value;           // Armazena o valor lido no vetor host
    }

    auto leitura_f = std::chrono::steady_clock::now();

    diff = leitura_f - leitura_i;

    cout << "Tempo de LEITURA (em segundos)  " << diff.count() << endl;

    auto copia_i = std::chrono::steady_clock::now();

    thrust::device_vector<double> dev(host);

    auto copia_f = std::chrono::steady_clock::now();

    diff = copia_f - copia_i;

    cout << "Tempo de CÓPIA (em segundos)  " << diff.count() << endl;

    return 0;
}
