#include <iostream>
#include <vector>
#include <complex>
#include <chrono>
#include <cmath>
#include <cstring>

#include <fftw3.h>

inline std::complex<double>* allocHostArray(int n) {
    size_t N = size_t(n) * n * n;
    return reinterpret_cast<std::complex<double>*>(
        fftw_malloc(sizeof(std::complex<double>) * N)
    );
}

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

double runFFTWGradient(int n, int ntrial) {
    size_t N = size_t(n) * n * n;

    std::complex<double>* H   = allocHostArray(n);
    std::complex<double>* Gx  = allocHostArray(n);
    std::complex<double>* Gy  = allocHostArray(n);
    std::complex<double>* Gz  = allocHostArray(n);
    if(!H || !Gx || !Gy || !Gz) {
        std::cerr << "FFTW allocation failed for n=" << n << "\n";
        std::exit(EXIT_FAILURE);
    }

    for(size_t i = 0; i < N; i++) {
        H[i]  = {0.0, 0.0};
        Gx[i] = {0.0, 0.0};
        Gy[i] = {0.0, 0.0};
        Gz[i] = {0.0, 0.0};
    }

    std::complex<double>* HostCopy =
        reinterpret_cast<std::complex<double>*>(fftw_malloc(sizeof(std::complex<double>) * N));
    initializePlaneWave(HostCopy, n);

    fftw_plan plan_fwd = fftw_plan_dft_3d(n, n, n,
        reinterpret_cast<fftw_complex*>(H),
        reinterpret_cast<fftw_complex*>(H),
        FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_plan plan_inv = fftw_plan_dft_3d(n, n, n,
        reinterpret_cast<fftw_complex*>(H),
        reinterpret_cast<fftw_complex*>(H),
        FFTW_BACKWARD, FFTW_ESTIMATE);

    auto idxToK = [&](size_t idx, int &kx, int &ky, int &kz) {
        size_t tmp = idx;
        size_t ix  = tmp % n;  tmp /= n;
        size_t iy  = tmp % n;  tmp /= n;
        size_t iz  = tmp;

        kx = (ix <= n/2) ? int(ix) : int(ix) - n;
        ky = (iy <= n/2) ? int(iy) : int(iy) - n;
        kz = (iz <= n/2) ? int(iz) : int(iz) - n;
    };

    double total_time = 0.0;
    for(int trial = 0; trial < ntrial; trial++) {
        std::memcpy(H, HostCopy, sizeof(std::complex<double>) * N);

        auto t0 = std::chrono::high_resolution_clock::now();

        fftw_execute(plan_fwd);

        for(size_t idx = 0; idx < N; idx++) {
            int kx, ky, kz;
            idxToK(idx, kx, ky, kz);
            auto Hval = H[idx];
            // i*kx * Hval = (−kx * Im(Hval)) + i*(kx * Re(Hval))
            Gx[idx] = { -double(kx) * Hval.imag(),  double(kx) * Hval.real() };
            Gy[idx] = { -double(ky) * Hval.imag(),  double(ky) * Hval.real() };
            Gz[idx] = { -double(kz) * Hval.imag(),  double(kz) * Hval.real() };
        }

        fftw_execute_dft(plan_inv,
            reinterpret_cast<fftw_complex*>(Gx),
            reinterpret_cast<fftw_complex*>(Gx));
        fftw_execute_dft(plan_inv,
            reinterpret_cast<fftw_complex*>(Gy),
            reinterpret_cast<fftw_complex*>(Gy));
        fftw_execute_dft(plan_inv,
            reinterpret_cast<fftw_complex*>(Gz),
            reinterpret_cast<fftw_complex*>(Gz));

        auto t1 = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(t1 - t0).count();
        total_time += elapsed;
    }

    double avg_time = total_time / double(ntrial);

    fftw_destroy_plan(plan_fwd);
    fftw_destroy_plan(plan_inv);
    fftw_free(H);
    fftw_free(Gx);
    fftw_free(Gy);
    fftw_free(Gz);
    fftw_free(HostCopy);

    return avg_time;
}



int main() {
    std::cout << "n,avg_time_fftw_s,gflops_fftw\n";

    std::vector<int> ns = {16, 32, 64, 128, 256};
    int ntrial = 3;

    for(int n : ns) {
        double t_fftw = runFFTWGradient(n, ntrial);
        double flops  = flopEstimate(n);
        double gcpu   = flops / (t_fftw * 1e9);

        std::printf("%d,%.6f,%.2f\n", n, t_fftw, gcpu);
    }

    return 0;
}