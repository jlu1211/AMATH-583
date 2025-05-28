#include <iostream>
#include <complex>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <chrono>
#include <limits>
#include <cblas.h>
#include <lapacke.h>

int main() {
    const double eps = std::numeric_limits<double>::epsilon();

    std::cout << "n,l2norm_residual,normalized_error" << std::endl;

    for (int n = 16; n <= 8192; n *= 2) {
        const int ma = n, na = n, nrhs = 1;
        lapack_complex_double *a   = (lapack_complex_double*) malloc(sizeof(lapack_complex_double) * ma * na);
        lapack_complex_double *A0  = (lapack_complex_double*) malloc(sizeof(lapack_complex_double) * ma * na);
        lapack_complex_double *b   = (lapack_complex_double*) malloc(sizeof(lapack_complex_double) * ma);
        lapack_complex_double *b0  = (lapack_complex_double*) malloc(sizeof(lapack_complex_double) * ma);
        lapack_complex_double *z   = (lapack_complex_double*) malloc(sizeof(lapack_complex_double) * na);
        lapack_complex_double *r   = (lapack_complex_double*) malloc(sizeof(lapack_complex_double) * na);
        lapack_int *ipiv = (lapack_int*) malloc(sizeof(lapack_int) * n);

        // Initialize A and save a copy in A0
        srand(0);
        int k = 0;
        for (int j = 0; j < na; j++) {
            for (int i = 0; i < ma; i++) {
                a[k] = 0.5 - (double)rand() / (double)RAND_MAX + lapack_make_complex_double(0, 1) * (0.5 - (double)rand() / (double)RAND_MAX);
                if (i == j) {
                    a[k] *= static_cast<double>(ma);
                }
                A0[k] = a[k];
                k++;
            }
        }
        // Initialize b and save b0
        srand(1);
        for (int i = 0; i < ma; i++) {
            b[i] = 0.5 - (double)rand() / (double)RAND_MAX + lapack_make_complex_double(0, 1) * (0.5 - (double)rand() / (double)RAND_MAX);
            b0[i] = b[i];
            r[i] = b[i];
        }

        // Solve A x = b in-place; solution ends up in b
        LAPACKE_zgesv(LAPACK_COL_MAJOR, n, nrhs, a, ma, ipiv, b, ma);

        // Copy solution into z
        for (int i = 0; i < na; i++) z[i] = b[i];

        // Compute norm of z
        double normz = cblas_dznrm2(na, z, 1);

        // Compute infinity norm of A (max row sum)
        double norma = 0.0;
        for (int i = 0; i < n; i++) {
            double row_sum = 0.0;
            for (int j = 0; j < n; j++) {
                std::complex<double> val = ((std::complex<double>*)A0)[j * n + i];
                row_sum += std::abs(val);
            }
            if (row_sum > norma) norma = row_sum;
        }
        
        // Compute r = B - A * z
        lapack_complex_double alpha = lapack_make_complex_double(-1.0, 0.0);
        lapack_complex_double beta = lapack_make_complex_double(1.0, 0.0);
        cblas_zgemv(CblasColMajor, CblasNoTrans, ma, na, &alpha, A0, ma, z, 1, &beta, r, 1);
        
        double l2norm_r = cblas_dznrm2(na, r, 1);
        double normalized_error = l2norm_r / (norma * normz * eps);

        std::cout << n << "," << l2norm_r << "," << normalized_error << std::endl;

        // clean up
        free(a);
        free(A0);
        free(b);
        free(b0);
        free(z);
        free(r);
        free(ipiv);
    }

    return 0;
}
