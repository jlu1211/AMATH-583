// main.cpp
#include <chrono>
#include <vector>
#include <random>
#include <iostream>
#include "gemm.hpp"

template<typename T>
double benchmark_gemm(void (*kernel)(T, const std::vector<T>&,
                                     const std::vector<T>&, T,
                                     std::vector<T>&, int,int,int),
                      int n, int ntrial)
{
    std::vector<T> A(n*n), B(n*n), C(n*n);
    std::mt19937 gen(0);
    std::uniform_real_distribution<T> dist(0,1);

    for (auto& x : A) x = dist(gen);
    for (auto& x : B) x = dist(gen);
    for (auto& x : C) x = dist(gen);

    T alpha = T(1), beta = T(1);
    double total = 0;
    for (int t = 0; t < ntrial; ++t) {
      auto Ccopy = C;
      auto t0 = std::chrono::high_resolution_clock::now();
      kernel(alpha, A, B, beta, Ccopy, n, n, n);
      auto t1 = std::chrono::high_resolution_clock::now();
      total += std::chrono::duration<double>(t1-t0).count();
    }
    return total / ntrial;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <O0|O3>" << std::endl;
    return 1;
  }
  std::string opt_level = argv[1];
  if (opt_level != "O0" && opt_level != "O3") {
    std::cerr << "Error: opt_level must be 'O0' or 'O3'" << std::endl;
    return 1;
  }

  const int ntrial = 3;
  std::cout << "n,variant,type,opt_level,flops_per_sec\n";
  for (int n = 2; n <= 512; ++n) {
      double t;
      t = benchmark_gemm<float>(mm_kij<float>, n, ntrial);
      std::cout << n << ",kij,float," << opt_level << "," << (2.0*n*n*n/t) << "\n";

      t = benchmark_gemm<float>(mm_jki<float>, n, ntrial);
      std::cout << n << ",jki,float," << opt_level << "," << (2.0*n*n*n/t) << "\n";

      t = benchmark_gemm<double>(mm_kij<double>, n, ntrial);
      std::cout << n << ",kij,double," << opt_level << "," << (2.0*n*n*n/t) << "\n";

      t = benchmark_gemm<double>(mm_jki<double>, n, ntrial);
      std::cout << n << ",jki,double," << opt_level << "," << (2.0*n*n*n/t) << "\n";
  }
  return 0;
}
