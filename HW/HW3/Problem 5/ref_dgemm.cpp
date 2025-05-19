#include "ref_dgemm.hpp"
#include<vector>
#include<stdexcept>
#include<random>
#include<iostream>

// C <- aAB + bC
void dgemm(double a, const std::vector<std::vector<double>> &A,
           const std::vector<std::vector<double>> &B, double b,
           std::vector<std::vector<double>> &C) {
    size_t m = A.size();
    size_t p = A[0].size();
    size_t n = B[0].size();
    if (B.size() != p || C.size() != m || C[0].size() != n) {
        throw std::invalid_argument("Dimension Mismatch");
    }
    auto C_copy = C;
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            double element_sum = 0.0;
            for (size_t k = 0; k < p; k++) {
                element_sum += A[i][k] * B[k][j];
            }
            C[i][j] = a * element_sum + b * C_copy[i][j];
        }
    }
}

// int main() {
//     const int ntrial = 3;
//     std::mt19937 gen(0);
//     std::uniform_real_distribution<double> dist(-1.0, 1.0);
//     std::cout << "n,flops_per_sec\n";
//     for (int n = 2; n <= 512; n++) {
//         long double elapsed_time = 0.L;
//         int m = n, p = n, q = n;
//         std::vector<std::vector<double>> A(m, std::vector<double>(p));
//         std::vector<std::vector<double>> B(p, std::vector<double>(q));
//         std::vector<std::vector<double>> C(m, std::vector<double>(q)), C_copy;
//         for (int i = 0; i < m; ++i) {
//             for (int j = 0; j < p; ++j) {
//                 A[i][j] = dist(gen);
//             }
//         }
//         for (int j = 0; j < m; ++j) {
//             for (int k = 0; k < p; ++k) {
//                 B[j][k] = dist(gen);
//             }
//         }
//         for (int i = 0; i < m; ++i) {
//             for (int k = 0; k < p; ++k) {
//                 C[i][k] = dist(gen);
//             }
//         }
//         for (int t = 0; t < ntrial; t++) {
//             C_copy = C;
//             auto start = std::chrono::high_resolution_clock::now();
//             dgemm(1.23, A, B, 2.34, C_copy);
//             auto stop = std::chrono::high_resolution_clock::now();
//             auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
//             elapsed_time += (duration.count() * 1.e-9);
//         }
//         long double avg_time = elapsed_time / static_cast<long double>(ntrial);
//         long double flops_count = 2.L * n * n * n + 2.L * n * n;
//         double perf = flops_count / avg_time;
//         std::cout << n << "," << perf << "\n";
//     }
//     return 0;
// }