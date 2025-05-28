#include <iostream>
#include <cuda_runtime.h>
#include <iomanip>

#define CHECK_CUDA(call)                                                   \
  do {                                                                     \
    cudaError_t err = (call);                                              \
    if (err != cudaSuccess) {                                              \
      std::cerr << "CUDA error at " << __FILE__ << ":" << __LINE__        \
                << " code=" << err << " (" << cudaGetErrorString(err)     \
                << ")\n";                                                  \
      std::exit(EXIT_FAILURE);                                             \
    }                                                                      \
  } while (0)

int main() {
    std::cout << "Bytes,H2D_GBps,D2H_GBps\n";
    std::cout << std::fixed << std::setprecision(6);

    for (size_t size = 1; size <= (1UL << 31); size <<= 1) {
        // Allocate pinned host buffer
        void* h_ptr = nullptr;
        CHECK_CUDA(cudaMallocHost(&h_ptr, size));

        // Allocate device buffer
        void* d_ptr = nullptr;
        CHECK_CUDA(cudaMalloc(&d_ptr, size));

        // Create events for timing
        cudaEvent_t start, stop;
        CHECK_CUDA(cudaEventCreate(&start));
        CHECK_CUDA(cudaEventCreate(&stop));

        // -------------------------
        // Host → Device (H2D)
        // -------------------------
        CHECK_CUDA(cudaEventRecord(start));
        CHECK_CUDA(cudaMemcpy(d_ptr, h_ptr, size, cudaMemcpyHostToDevice));
        CHECK_CUDA(cudaEventRecord(stop));
        CHECK_CUDA(cudaEventSynchronize(stop));

        float time_ms_h2d = 0.0f;
        CHECK_CUDA(cudaEventElapsedTime(&time_ms_h2d, start, stop));

        // -------------------------
        // Device → Host (D2H)
        // -------------------------
        CHECK_CUDA(cudaEventRecord(start));
        CHECK_CUDA(cudaMemcpy(h_ptr, d_ptr, size, cudaMemcpyDeviceToHost));
        CHECK_CUDA(cudaEventRecord(stop));
        CHECK_CUDA(cudaEventSynchronize(stop));

        float time_ms_d2h = 0.0f;
        CHECK_CUDA(cudaEventElapsedTime(&time_ms_d2h, start, stop));

        // Compute bandwidth in GiB/s
        double h2d_bw = (double)size / (time_ms_h2d * 1e-3) / (1UL << 30);
        double d2h_bw = (double)size / (time_ms_d2h * 1e-3) / (1UL << 30);

        // Output as CSV
        std::cout << size << "," << h2d_bw << "," << d2h_bw << "\n";

        // Clean up
        CHECK_CUDA(cudaFree(d_ptr));
        CHECK_CUDA(cudaFreeHost(h_ptr));
        CHECK_CUDA(cudaEventDestroy(start));
        CHECK_CUDA(cudaEventDestroy(stop));
    }

    return 0;
}
