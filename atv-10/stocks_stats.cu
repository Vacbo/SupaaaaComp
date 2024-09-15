#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/extrema.h>
#include <thrust/reduce.h>
#include <iostream>
using namespace std;

int main() {
    int n = 3650;  // Assume 10 years of data (10 * 365 = 3650)
    double value = 0.0;

    thrust::host_vector<double> host(n, 0);
    for (int i = 0; i < n; i++) {
        cin >> value;              // Lê um valor da entrada padrão
        host[i] = value;           // Armazena o valor lido no vetor host
    }

    thrust::device_vector<double> dev(host);  // Copia dados para o device

    // Total period statistics
    double total_avg = thrust::reduce(dev.begin(), dev.end(), 0.0) / n;
    auto total_minmax = thrust::minmax_element(dev.begin(), dev.end());

    // Last year statistics (last 365 days)
    int last_year_idx = n - 365;
    double last_year_avg = thrust::reduce(dev.begin() + last_year_idx, dev.end(), 0.0) / 365;
    auto last_year_minmax = thrust::minmax_element(dev.begin() + last_year_idx, dev.end());

    // Print results
    cout << "Total period avg price: " << total_avg << endl;
    cout << "Total period min price: " << *(total_minmax.first) << endl;
    cout << "Total period max price: " << *(total_minmax.second) << endl;

    cout << "Last year avg price: " << last_year_avg << endl;
    cout << "Last year min price: " << *(last_year_minmax.first) << endl;
    cout << "Last year max price: " << *(last_year_minmax.second) << endl;

    return 0;
}
