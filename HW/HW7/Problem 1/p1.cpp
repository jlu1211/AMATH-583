#include <iostream>
#include <vector>
#include <chrono>
#include <cblas.h>
#include <iomanip>

int main() {
    const int ntrial = 25;
    const int N_min = 2;
    const int N_max = 4096;
    const int stride = 2;

    std::cout << "level,N,MLOPS\n";
    for (int level = 1; level <= 3; ++level) {
        for (int N = N_min; N <= N_max; N *= stride) {
            std::vector<double> X(N, 1.0), Y(N, 1.0);
            std::vector<double> A(N*N, 1.0), B(N*N, 1.0), C(N*N, 0.0);

            // warm up
            if (level == 1) {
                cblas_daxpy(N, 1.0, X.data(), 1, Y.data(), 1);
            } else if (level == 2) {
                cblas_dgemv(CblasColMajor, CblasNoTrans, N, N, 1.0, A.data(), N, B.data(), 1, 0.0, C.data(), 1);
            } else if (level == 3) {
                cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, A.data(), N, B.data(), N, 0.0, C.data(), N);
            }

            // measure time
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < ntrial; ++i) {
                if (level == 1) {
                    cblas_daxpy(N, 1.0, X.data(), 1, Y.data(), 1);
                } else if (level == 2) {
                    cblas_dgemv(CblasColMajor, CblasNoTrans, N, N, 1.0, A.data(), N, B.data(), 1, 0.0, C.data(), 1);
                } else if (level == 3) {
                    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, A.data(), N, B.data(), N, 0.0, C.data(), N);
                }
            }
            auto end = std::chrono::high_resolution_clock::now();
            double time_s = std::chrono::duration<double>(end - start).count();
            double avg_time_s = time_s / ntrial;
            double flops;
            if (level == 1) {
                // DAXPY: 2N flops (N additions and N multiplications)
                flops = 2.0 * N;
            } else if (level == 2) {
                // DGEMV: 2N² flops (N² multiplications and N² additions)
                flops = 2.0 * N * N;
            } else if (level == 3) {
                // DGEMM: 2N³ flops (N³ multiplications and N³ additions)
                flops = 2.0 * N * N * N;
            }
            double MFLOPS = (flops / avg_time_s) * 1e-6; // Convert to millions of operations per second
            std::cout << level << "," << N << "," << std::fixed << std::setprecision(5) << MFLOPS << "\n";
        }
    }
    return 0;
}