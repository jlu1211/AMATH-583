// test_strassen_recursive.cpp
#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>

using namespace std;

// Forward declarations (must match your signatures in strassen.cpp)
template<typename T>
vector<vector<T>> naiveMultiply(const vector<vector<T>>& A,
                                const vector<vector<T>>& B);
template<typename T>
vector<vector<T>> strassenMultiply(const vector<vector<T>>& A,
                                   const vector<vector<T>>& B);

int main(){
    const int n = 128;
    srand(0);

    // Build two random n×n matrices
    vector<vector<int>> A(n, vector<int>(n)), 
                        B(n, vector<int>(n));
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }

    // Compute both ways
    auto C_naive  = naiveMultiply(A, B);
    auto C_strass = strassenMultiply(A, B);

    // Verify they match exactly
    assert(C_naive == C_strass);
    cout << "✅ Strassen recursion test passed for n = " << n << "\n";

    return 0;
}
