#ifndef GEMM_HPP
#define GEMM_HPP

template<typename T>
void mm_kij(T a,
            const std::vector<T>& A,
            const std::vector<T>& B,
            T b,
            std::vector<T>& C,
            int m, int p, int n) {
    // C <- bC
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; j++) {
            C[i*n + j] *= b;
        }
    }
    // aAB
    for (int k = 0; k < p; ++k) {
        for (int i = 0; i < m; ++i) {
            T Aik = A[i*p + k];
            for (int j = 0; j < n; ++j) {
                C[i*n + j] += a * Aik * B[k*n + j];
            }
        }
    }
}

template<typename T>
void mm_jki(T a,
            const std::vector<T>& A,
            const std::vector<T>& B,
            T b,
            std::vector<T>& C,
            int m, int p, int n) {
    // C <- bC
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i*n + j] *= b;
        }
    }
    // aAB
    for (int j = 0; j < n; ++j) {
        for (int k = 0; k < p; ++k) {
            T Bkj = B[k*n + j];
            for (int i = 0; i < m; ++i) {
                C[i*n + j] += a * A[i*p + k] * Bkj;
            }
        }
    }
}
#endif