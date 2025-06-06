#include <iostream>
#include <vector>
#include <complex>
#include <chrono>
#include <cmath>
#include <cstring>
#include <cuda_runtime.h>
#include <cufft.h>

#define CUDA_CHECK(call)                                                   \
  do {                                                                     \
    cudaError_t err = call;                                                \
    if(err != cudaSuccess) {                                               \
      std::cerr << "CUDA error at " << __FILE__ << ":" << __LINE__         \
                << " code=" << err << " \""                                 \
                << cudaGetErrorString(err) << "\"\n";                      \
      std::exit(EXIT_FAILURE);                                             \
    }                                                                      \
  } while(0)

#define CUFFT_CHECK(call)                                                  \
  do {                                                                     \
    cufftResult cres = call;                                               \
    if(cres != CUFFT_SUCCESS) {                                            \
      std::cerr << "cuFFT error at " << __FILE__ << ":" << __LINE__        \
                << " code=" << cres << "\n";                               \
      std::exit(EXIT_FAILURE);                                             \
    }                                                                      \
  } while(0)

void initializePlaneWave(std::complex<double>* phi, int n) {
    const double TWO_PI = 2.0 * M_PI;
    int kx = 1, ky = 2, kz = 3;

    for(int iz = 0; iz < n; iz++) {
        for(int iy = 0; iy < n; iy++) {
            for(int ix = 0; ix < n; ix++) {
                size_t idx = (size_t)iz * n * n + (size_t)iy * n + (size_t)ix;
                double x = double(ix) * (TWO_PI / double(n));
                double y = double(iy) * (TWO_PI / double(n));
                double z = double(iz) * (TWO_PI / double(n));
                double phase = kx*x + ky*y + kz*z;
                phi[idx] = std::complex<double>(cos(phase), sin(phase));
            }
        }
    }
}

inline double flopEstimate(int n) {
    return 360.0 * double(n)*double(n)*double(n) * std::log2(double(n));
}

__global__
void multiplyByIKx(cufftDoubleComplex* devPhi,
                   cufftDoubleComplex* devGx,
                   int n, double scale)
{
    size_t idx  = blockIdx.x * blockDim.x + threadIdx.x;
    size_t N    = size_t(n) * n * n;
    if(idx >= N) return;

    size_t tmp = idx;
    int ix = tmp % n;  tmp /= n;
    int iy = tmp % n;  tmp /= n;
    int iz = tmp;

    int kx = (ix <= n/2) ? ix : ix - n;

    cufftDoubleComplex ph = devPhi[idx];
    devGx[idx].x = scale * (-double(kx) * ph.y);
    devGx[idx].y = scale * ( double(kx) * ph.x);
}

__global__
void multiplyByIKy(cufftDoubleComplex* devPhi,
                   cufftDoubleComplex* devGy,
                   int n, double scale)
{
    size_t idx  = blockIdx.x * blockDim.x + threadIdx.x;
    size_t N    = size_t(n) * n * n;
    if(idx >= N) return;

    size_t tmp = idx;
    int ix = tmp % n;  tmp /= n;
    int iy = tmp % n;  tmp /= n;
    int iz = tmp;

    int ky = (iy <= n/2) ? iy : iy - n;
    cufftDoubleComplex ph = devPhi[idx];
    devGy[idx].x = scale * (-double(ky) * ph.y);
    devGy[idx].y = scale * ( double(ky) * ph.x);
}

__global__
void multiplyByIKz(cufftDoubleComplex* devPhi,
                   cufftDoubleComplex* devGz,
                   int n, double scale)
{
    size_t idx  = blockIdx.x * blockDim.x + threadIdx.x;
    size_t N    = size_t(n) * n * n;
    if(idx >= N) return;

    size_t tmp = idx;
    int ix = tmp % n;  tmp /= n;
    int iy = tmp % n;  tmp /= n;
    int iz = tmp;

    int kz = (iz <= n/2) ? iz : iz - n;
    cufftDoubleComplex ph = devPhi[idx];
    devGz[idx].x = scale * (-double(kz) * ph.y);
    devGz[idx].y = scale * ( double(kz) * ph.x);
}

double runCuFFTGradient(int n, int ntrial) {
    size_t N = size_t(n) * n * n;
    size_t bytes = sizeof(cufftDoubleComplex) * N;

    std::complex<double>* HostCopy =
        reinterpret_cast<std::complex<double>*>(malloc(sizeof(std::complex<double>) * N));
    initializePlaneWave(HostCopy, n);

    cufftDoubleComplex *devPhi, *devGx, *devGy, *devGz;
    CUDA_CHECK(cudaMalloc(&devPhi, bytes));
    CUDA_CHECK(cudaMalloc(&devGx,  bytes));
    CUDA_CHECK(cudaMalloc(&devGy,  bytes));
    CUDA_CHECK(cudaMalloc(&devGz,  bytes));

    cufftHandle plan;
    CUFFT_CHECK(cufftPlan3d(&plan, n, n, n, CUFFT_Z2Z));

    cudaEvent_t startEvent, stopEvent;
    CUDA_CHECK(cudaEventCreate(&startEvent));
    CUDA_CHECK(cudaEventCreate(&stopEvent));

    int blockSize = 256;
    int gridSize  = int((N + blockSize - 1) / blockSize);
    double scale  = 1.0 / double(N);

    double total_time_ms = 0.0;
    for(int trial = 0; trial < ntrial; trial++) {
        CUDA_CHECK(cudaMemcpy(devPhi, HostCopy, bytes, cudaMemcpyHostToDevice));

        CUDA_CHECK(cudaEventRecord(startEvent, 0));

        CUFFT_CHECK(cufftExecZ2Z(plan, devPhi, devPhi, CUFFT_FORWARD));

        multiplyByIKx<<<gridSize, blockSize>>>(devPhi, devGx, n, scale);
        multiplyByIKy<<<gridSize, blockSize>>>(devPhi, devGy, n, scale);
        multiplyByIKz<<<gridSize, blockSize>>>(devPhi, devGz, n, scale);

        CUFFT_CHECK(cufftExecZ2Z(plan, devGx, devGx, CUFFT_INVERSE));
        CUFFT_CHECK(cufftExecZ2Z(plan, devGy, devGy, CUFFT_INVERSE));
        CUFFT_CHECK(cufftExecZ2Z(plan, devGz, devGz, CUFFT_INVERSE));

        CUDA_CHECK(cudaEventRecord(stopEvent, 0));
        CUDA_CHECK(cudaEventSynchronize(stopEvent));
        float elapsed_ms = 0.0f;
        CUDA_CHECK(cudaEventElapsedTime(&elapsed_ms, startEvent, stopEvent));
        total_time_ms += double(elapsed_ms);
    }

    double avg_time_s = (total_time_ms / double(ntrial)) / 1000.0;

    cufftDestroy(plan);
    cudaFree(devPhi);
    cudaFree(devGx);
    cudaFree(devGy);
    cudaFree(devGz);
    free(HostCopy);
    cudaEventDestroy(startEvent);
    cudaEventDestroy(stopEvent);

    return avg_time_s;
}

int main() {
    std::cout << "n,avg_time_cufft_s,gflops_cufft\n";

    std::vector<int> ns = {16, 32, 64, 128, 256};
    int ntrial = 3;

    for(int n : ns) {
        double t_cufft = runCuFFTGradient(n, ntrial);
        double flops   = flopEstimate(n);
        double ggpu    = flops / (t_cufft * 1e9);

        std::printf("%d,%.6f,%.2f\n", n, t_cufft, ggpu);
    }

    return 0;
}