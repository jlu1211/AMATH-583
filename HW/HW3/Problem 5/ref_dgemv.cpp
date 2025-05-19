#include "ref_dgemv.hpp"
#include<vector>
#include<stdexcept>

void dgemv(double a,
           const std::vector<std::vector<double>> &A,
           const std::vector<double> &x,
           double b,
           std::vector<double> &y) {
    size_t m = A.size();
    size_t n = A[0].size();
    if (x.size() != n || y.size() != m) {
        throw std::invalid_argument("dgemv: Dimension Mismatch");
    }
    std::vector<double> y_copy = y;
    for (size_t t = 0; t < m; ++t) {
        double row_sum = 0;
        for (size_t j = 0; j < n; ++j) {
            row_sum += A[t][j] * x[j];
        }
        y[t] = a * row_sum + b * y_copy[t];
    }
}

// int main() {
//     const int ntrial = 3;
//     std::mt19937 gen(0);
//     std::uniform_real_distribution<double> dist(-1.0, 1.0);
//     std::cout << "n,flops_per_sec\n";
//     for (int n = 2; n <= 512; n++) {
//         long double elapsed_time = 0.L;
//         int m = n;
//         int p = n;
//         std::vector<std::vector<double>> A(m, std::vector<double>(p));
//         std::vector<double> x(p), y(m), y_copy(m);
//         for (int i = 0; i < m; ++i)
//             for (int j = 0; j < p; ++j)
//                 A[i][j] = dist(gen);
//         for (int i = 0; i < p; ++i) x[i] = dist(gen);
//         for (int i = 0; i < m; ++i) y[i] = dist(gen);
//         for (int t = 0; t < ntrial; t++) {
//             y_copy = y;
//             auto start = std::chrono::high_resolution_clock::now();
//             dgemv(1.23, A, x, 2.34, y_copy);
//             auto stop = std::chrono::high_resolution_clock::now();
//             auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
//             elapsed_time += (duration.count() * 1.e-9);
//         }
//         long double avg_time = elapsed_time / ntrial;
//         long double flops_count = 2.L * n * n + 2.L * n;
//         double perf = flops_count / avg_time;
//         std::cout << n << "," << perf << "\n";
//         elapsed_time = 0.L;
//     }
//     return 0;
// }