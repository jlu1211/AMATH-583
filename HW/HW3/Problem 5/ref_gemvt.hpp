#ifndef REF_GEMVT_HPP
#define REF_GEMVT_HPP
#include<vector>
#include<stdexcept>

template <typename T>
void gemv(T a,
          const std::vector<std::vector<T>> &A,
          const std::vector<T> &x,
          T b,
          std::vector<T> &y) {
    size_t m = A.size();
    size_t n = A[0].size();
    if (x.size() != n || y.size() != m) {
        throw std::invalid_argument("dgemv: Dimension Mismatch");
    }
    std::vector<T> y_copy = y;
    for (size_t t = 0; t < m; t++) {
        T row_sum = T();
        for (size_t j = 0; j < n; j++) {
            row_sum += A[t][j] * x[j];
        }
        y[t] = a * row_sum + b * y_copy[t];
    }
}
#endif