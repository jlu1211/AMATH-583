// @uw.edu
// AMATH 483-583
// strassen.cpp : starter code for Strassen implementation

#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>

using namespace std;

template <typename T>
vector<vector<T>> addMatrix(const vector<vector<T>> &A, const vector<vector<T>> &B)
{
    int n = A.size();
    int m = A[0].size();
    vector<vector<T>> C(n, vector<T>(m));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

template <typename T>
vector<vector<T>> subtractMatrix(const vector<vector<T>> &A, const vector<vector<T>> &B)
{
    int n = A.size();
    int m = A[0].size();
    vector<vector<T>> C(n, vector<T>(m));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

template <typename T>
vector<vector<T>> naiveMultiply(const vector<vector<T>> &A, const vector<vector<T>> &B)
{
    int n = A.size();
    int m = A[0].size();
    int p = B[0].size();
    if (m != B.size()) {
        throw invalid_argument("Matrix multiplication dimension mismatch: first matrix columns (" + 
            to_string(m) + ") must match second matrix rows (" + 
            to_string(B.size()) + ")");
    }
    
    vector<vector<T>> C(n, vector<T>(p, 0));
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < m; k++) {
            for (int j = 0; j < p; j++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

template <typename T>
vector<vector<T>> strassenMultiply(const vector<vector<T>> &A, const vector<vector<T>> &B)
{
    int n = A.size();
    int m = A[0].size();
    int p = B[0].size();
    
    if (m != B.size()) {
        throw invalid_argument("Matrix multiplication dimension mismatch: first matrix columns (" + 
            to_string(m) + ") must match second matrix rows (" + 
            to_string(B.size()) + ")");
    }
    
    if (n != m || n != p || n != B.size() || (n & (n - 1)) != 0) {
        throw invalid_argument("Strassen's algorithm requires square matrices with dimensions that are powers of 2");
    }
    
    if(n <= 64) {
        return naiveMultiply(A, B);
    }
    int k = n / 2;
    vector<vector<T>> A11(k, vector<T>(k)), A12(k, vector<T>(k)), A21(k, vector<T>(k)), A22(k, vector<T>(k));
    vector<vector<T>> B11(k, vector<T>(k)), B12(k, vector<T>(k)), B21(k, vector<T>(k)), B22(k, vector<T>(k));
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }
    vector<vector<T>> M1 = strassenMultiply(addMatrix(A11, A22), addMatrix(B11, B22));
    vector<vector<T>> M2 = strassenMultiply(addMatrix(A21, A22), B11);
    vector<vector<T>> M3 = strassenMultiply(A11, subtractMatrix(B12, B22));
    vector<vector<T>> M4 = strassenMultiply(A22, subtractMatrix(B21, B11));
    vector<vector<T>> M5 = strassenMultiply(addMatrix(A11, A12), B22);
    vector<vector<T>> M6 = strassenMultiply(subtractMatrix(A21, A11), addMatrix(B11, B12));
    vector<vector<T>> M7 = strassenMultiply(subtractMatrix(A12, A22), addMatrix(B21, B22));
    vector<vector<T>> C11 = addMatrix(subtractMatrix(addMatrix(M1, M4), M5), M7);
    vector<vector<T>> C12 = addMatrix(M3, M5);
    vector<vector<T>> C21 = addMatrix(M2, M4);
    vector<vector<T>> C22 = addMatrix(addMatrix(subtractMatrix(M1, M2), M3), M6);
    vector<vector<T>> C(n, vector<T>(n));
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            C[i][j] = C11[i][j];
            C[i][j + k] = C12[i][j];
            C[i + k][j] = C21[i][j];
            C[i + k][j + k] = C22[i][j];
        }
    }
    return C;
}

inline int nextPowerOf2(int n) {
    int power = 1;
    while (power < n) {
        power = power * 2;
    }
    return power;
}

template <typename T>
vector<vector<T>> padMatrix(const vector<vector<T>> &A, int p) {
    int n = A.size();
    vector<vector<T>> P(p, vector<T>(p, T(0)));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            P[i][j] = A[i][j];
        }
    }
    return P;
}

template <typename T>
vector<vector<T>> unpadMatrix(const vector<vector<T>> &P, int n) {
    vector<vector<T>> A(n, vector<T>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i][j] = P[i][j];
        }
    }
    return A;
}

template <typename T>
vector<vector<T>> strassenGeneral(const vector<vector<T>> &A, const vector<vector<T>> &B) {
    int n = A.size();
    if (A[0].size() != n || B.size() != n || B[0].size() != n) {
        throw invalid_argument("Matrix dimensions must be square and match");
    }
    int p = nextPowerOf2(n);
    if (p == n) {
        return strassenMultiply(A, B);
    }
    else {
        vector<vector<T>> AP = padMatrix(A, p);
        vector<vector<T>> BP = padMatrix(B, p);
        vector<vector<T>> CP = strassenMultiply(AP, BP);
        return unpadMatrix(CP, n);
    }
}

template <typename T>
void printMatrix(const vector<vector<T>> &matrix)
{
    int n = matrix.size();
    int m = matrix[0].size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

// int
template vector<vector<int>> addMatrix<int>(const vector<vector<int>> &A, const vector<vector<int>> &B);
template vector<vector<int>> subtractMatrix<int>(const vector<vector<int>> &A, const vector<vector<int>> &B);
template vector<vector<int>> strassenMultiply<int>(const vector<vector<int>> &A, const vector<vector<int>> &B);
template void printMatrix<int>(const vector<vector<int>> &matrix);
// double
template vector<vector<double>> addMatrix<double>(const vector<vector<double>> &A, const vector<vector<double>> &B);
template vector<vector<double>> subtractMatrix<double>(const vector<vector<double>> &A, const vector<vector<double>> &B);
template vector<vector<double>> strassenMultiply<double>(const vector<vector<double>> &A, const vector<vector<double>> &B);
template void printMatrix<double>(const vector<vector<double>> &matrix);

int main() {
    srand(0);
    const int ntrials = 10;
    cout << "n,avg_time_s,GFLOPS_s\n";
    for (int n = 2; n <= 512; ++n) {
        vector<vector<double>> A(n, vector<double>(n));
        vector<vector<double>> B(n, vector<double>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[i][j] = rand()/double(RAND_MAX);
                B[i][j] = rand()/double(RAND_MAX);
            }
        }
        double T = 0;
        for(int t=0;t<ntrials;t++){
          auto t0 = chrono::high_resolution_clock::now();
          auto C = strassenGeneral(A,B);
          auto t1 = chrono::high_resolution_clock::now();
          T += chrono::duration_cast<chrono::seconds>(t1-t0).count();
        }
        double avg = T/double(ntrials);
        double flops = pow(double(n), log2(7.0));
        double gflops = flops/ avg / 1e9;
        cout << n << "," 
             << setprecision(6) << fixed << avg << "," 
             << setprecision(3) << gflops << "\n";
    }
    return 0;
}