#include "refBLAS.hpp"
#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>

int main() {
    // ------------ Level-1 concrete (double) ------------
    {
        std::vector<double> vx{1.1, 2.2, 3.3}, vy{0.5, 1.5, 2.5};
        double alpha = 2.5;
        daxpy(alpha, vx, vy);
        std::cout << "daxpy<double> (" << alpha << " * x + y) actual: ";
        for (double v : vy)
            std::cout << std::setprecision(std::numeric_limits<double>::digits10)
                      << v << " ";
        std::cout << "| expected: 3.25 7.0 10.75\n";
    }

    // ------------ Level-1 template (int) ------------
    {
        std::vector<int> ix{10, 20, 30}, iy{5,  5,  5};
        int a = 4;
        axpy<int>(a, ix, iy);
        std::cout << "axpy<int> (" << a << " * x + y) actual: ";
        for (int v : iy) std::cout << v << " ";
        std::cout << "| expected: 45 85 125\n";
    }

    // ------------ Level-1 template (long) ------------
    {
        std::vector<long> lx{100L, 200L, 300L}, ly{ 50L,  60L,  70L};
        long a = 3L;
        axpy<long>(a, lx, ly);
        std::cout << "axpy<long> (" << a << " * x + y) actual: ";
        for (long v : ly) std::cout << v << " ";
        std::cout << "| expected: 350 660 970\n";
    }

    // ------------ Level-1 template (long double) ------------
    {
        std::vector<long double> ldx{1.25L, 2.75L, 3.50L}, ldy{0.25L, 1.25L, 2.25L};
        long double a = 1.5L;
        axpy<long double>(a, ldx, ldy);
        std::cout << "axpy<long double> (" << a << " * x + y) actual: ";
        for (auto v : ldy)
            std::cout << std::setprecision(std::numeric_limits<long double>::digits10)
                      << v << " ";
        std::cout << "| expected: 2.125 5.375 7.5\n";
    }

    // ------------ Level-2 concrete (double) ------------
    {
        std::vector<std::vector<double>> A{
            {1.5, -2.0, 0.5},
            {3.0,  0.0, 1.0}
        };
        std::vector<double> x{2.0, -1.0, 3.0}, y{0.0, 1.0};
        double alpha = 2.0, beta = 0.5;
        dgemv(alpha, A, x, beta, y);
        std::cout << "dgemv<double> (" << alpha << "*A*x + " << beta << "*y) actual: ";
        for (double v : y)
            std::cout << std::setprecision(std::numeric_limits<double>::digits10)
                      << v << " ";
        std::cout << "| expected: 13.0 18.5\n";
    }

    // ------------ Level-2 template (int) ------------
    {
        std::vector<std::vector<int>> B{
            {2, -1},
            {0,  3},
            {4,  5}
        };
        std::vector<int> xi{1, 2}, yi{1, 1, 1};
        int alpha = 1, beta = 2;
        gemv<int>(alpha, B, xi, beta, yi);
        std::cout << "gemv<int> (" << alpha << "*B*x + " << beta << "*y) actual: ";
        for (int v : yi) std::cout << v << " ";
        std::cout << "| expected: 2 8 16\n";
    }

    // ------------ Level-3 concrete (double) ------------
    // ------------ C <- a*A*B + b*C
    {
        std::vector<std::vector<double>> M1{
            {1.0, 2.0},
            {-1.0, 3.0}
        }, M2{
            {4.0, 0.5},
            {1.5, -2.5}
        }, M3(2, std::vector<double>(2, 1.0));
        double alpha = 0.5, beta = 2.0;
        dgemm(alpha, M1, M2, beta, M3);
        std::cout << "dgemm<double> (" << alpha << "*M1*M2 + " << beta << "*M3) actual:\n";
        for (auto &row : M3) {
            for (double v : row)
                std::cout << std::setw(7)
                          << std::setprecision(std::numeric_limits<double>::digits10)
                          << v << " ";
            std::cout << "\n";
        }
        std::cout << "| expected:\n"
                  << "   5.5  -0.25\n"
                  << "   2.25  -2\n";
    }

    // ------------ Level-3 template (int) ------------
    {
        std::vector<std::vector<int>> Q1{
            {1, 0, 2},
            {0, 3, 1}
        }, Q2{
            {2, 1},
            {1, 0},
            {0, 4}
        }, Q3(2, std::vector<int>(2, 10));
        int alpha = 3, beta = 1;
        gemm<int>(alpha, Q1, Q2, beta, Q3);
        std::cout << "gemm<int> (" << alpha << "*Q1*Q2 + " << beta << "*Q3) actual:\n";
        for (auto &row : Q3) {
            for (int v : row)
                std::cout << std::setw(4) << v << " ";
            std::cout << "\n";
        }
        std::cout << "| expected:\n"
                  << "  16 37\n"
                  << "  19 22\n";
    }

    // ------------ Level-3 template (long double) ------------
    {
        std::vector<std::vector<long double>> L1{
            {0.5L, 1.5L},
            {2.5L, 3.5L},
            {4.5L, 5.5L}
        }, L2{
            {1.0L, -1.0L, 2.0L},
            {0.5L,  2.5L, 0.0L}
        }, L3(3, std::vector<long double>(3, 0.5L));
        long double alpha = 1.25L, beta = 0.75L;
        gemm<long double>(alpha, L1, L2, beta, L3);
        std::cout << "gemm<long double> (" << alpha << "*L1*L2 + " << beta << "*L3) actual:\n";
        for (auto &row : L3) {
            for (auto v : row)
                std::cout << std::setw(8)
                          << std::setprecision(std::numeric_limits<long double>::digits10)
                          << v << " ";
            std::cout << "\n";
        }
        std::cout << "| expected:\n"
                  << "  1.9375  4.4375  1.625\n"
                  << "  5.6875  8.1875  6.625\n"
                  << "  9.4375  11.9375  11.625\n";
    }

    return 0;
}
