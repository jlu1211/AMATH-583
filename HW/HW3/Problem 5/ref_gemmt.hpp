#ifndef REF_GEMMT_HPP
#define REF_GEMMT_HPP
#include<vector>
#include<stdexcept>

template <typename T>
void gemm(T a,
          const std::vector<std::vector<T>> &A,
          const std::vector<std::vector<T>> &B,
          T b,
          std::vector<std::vector<T>> &C) {
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
#endif