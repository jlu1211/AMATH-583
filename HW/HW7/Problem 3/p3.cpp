#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <cmath>

#include <cblas.h>
#include <cuda_runtime.h>
#include <cublas_v2.h>

#define CHECK_CUDA(call) \
    do { \
        cudaError_t err = (call); \
        if (err != cudaSuccess) { \
            std::cerr << "CUDA error at " << __FILE__ << ":" << __LINE__ \
                      << " code=" << err << " \"" \
                      << cudaGetErrorString(err) << "\"" << std::endl; \
            std::exit(EXIT_FAILURE); \
        } \
    } while (0)

#define CHECK_CUBLAS(call) \
    do { \
        cublasStatus_t status = (call); \
        if (status != CUBLAS_STATUS_SUCCESS) { \
            std::cerr << "cuBLAS error at " << __FILE__ << ":" \
                      << __LINE__ << " code=" << status << std::endl; \
            std::exit(EXIT_FAILURE); \
        } \
    } while (0)

int main() {
    const int ntrial = 3;
    const double alpha = 1.0, beta = 0.0;

    cublasHandle_t handle;
    CHECK_CUBLAS(cublasCreate(&handle));

    std::cout << "n,OpenBLAS_GFLOPS,cuBLAS_GFLOPS" << std::endl;

    for (int n = 2; n <= 16384; n *= 2) {
        size_t size = size_t(n) * n;
        size_t bytes = size * sizeof(double);

        std::vector<double> A(size, 1.0), B(size, 1.0), C(size, 0.0);
        for (size_t i = 0; i < size; ++i) {
            A[i] = drand48();
            B[i] = drand48();
        }

        // OpenBLAS: warm-up
        cblas_dgemm(
            CblasColMajor, CblasNoTrans, CblasNoTrans,
            n, n, n,
            alpha,
            A.data(), n,
            B.data(), n,
            beta,
            C.data(), n
        );

        // OpenBLAS timing
        auto t0 = std::chrono::high_resolution_clock::now();
        for (int rep = 0; rep < ntrial; ++rep) {
            cblas_dgemm(
                CblasColMajor, CblasNoTrans, CblasNoTrans,
                n, n, n,
                alpha,
                A.data(), n,
                B.data(), n,
                beta,
                C.data(), n
            );
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        double elapsed_cpu = std::chrono::duration<double>(t1 - t0).count() / ntrial;
        double gflops_cpu = (2.0 * double(n)*n*n + 2.0 * double(n)*n) / (elapsed_cpu * 1e9);

        // Allocate GPU memory
        double *d_A, *d_B, *d_C;
        CHECK_CUDA(cudaMalloc(&d_A, bytes));
        CHECK_CUDA(cudaMalloc(&d_B, bytes));
        CHECK_CUDA(cudaMalloc(&d_C, bytes));

        CHECK_CUDA(cudaMemcpy(d_A, A.data(), bytes, cudaMemcpyHostToDevice));
        CHECK_CUDA(cudaMemcpy(d_B, B.data(), bytes, cudaMemcpyHostToDevice));
        CHECK_CUDA(cudaMemcpy(d_C, C.data(), bytes, cudaMemcpyHostToDevice));

        cudaEvent_t start, stop;
        CHECK_CUDA(cudaEventCreate(&start));
        CHECK_CUDA(cudaEventCreate(&stop));

        // Warm-up run
        CHECK_CUBLAS(cublasDgemm(
            handle,
            CUBLAS_OP_N, CUBLAS_OP_N,
            n, n, n,
            &alpha,
            d_A, n,
            d_B, n,
            &beta,
            d_C, n
        ));

        // Timed loop
        for (int rep = 0; rep < ntrial; ++rep) {
            CHECK_CUDA(cudaMemcpy(d_C, C.data(), bytes, cudaMemcpyHostToDevice));
            CHECK_CUDA(cudaEventRecord(start, 0));
            CHECK_CUBLAS(cublasDgemm(
                handle,
                CUBLAS_OP_N, CUBLAS_OP_N,
                n, n, n,
                &alpha,
                d_A, n,
                d_B, n,
                &beta,
                d_C, n
            ));
            CHECK_CUDA(cudaEventRecord(stop, 0));
            CHECK_CUDA(cudaEventSynchronize(stop));
        }

        float ms = 0.0f;
        CHECK_CUDA(cudaEventElapsedTime(&ms, start, stop));
        double elapsed_gpu = (ms / 1e3) / ntrial;
        double gflops_gpu = (2.0 * double(n)*n*n + 2.0 * double(n)*n) / (elapsed_gpu * 1e9);

        cudaEventDestroy(start);
        cudaEventDestroy(stop);
        cudaFree(d_A);
        cudaFree(d_B);
        cudaFree(d_C);

        std::cout
            << n << ","
            << gflops_cpu << ","
            << gflops_gpu << std::endl;
    }

    cublasDestroy(handle);
    return 0;
}
