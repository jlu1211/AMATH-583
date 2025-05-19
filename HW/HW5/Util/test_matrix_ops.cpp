// test_matrix_ops.cpp
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

// forward‐declare your functions from strassen.cpp
template<typename T>
vector<vector<T>> addMatrix(const vector<vector<T>>& A,
                            const vector<vector<T>>& B);
template<typename T>
vector<vector<T>> subtractMatrix(const vector<vector<T>>& A,
                                 const vector<vector<T>>& B);
template<typename T>
vector<vector<T>> naiveMultiply(const vector<vector<T>>& A,
                                const vector<vector<T>>& B);
template<typename T>
vector<vector<T>> strassenMultiply(const vector<vector<T>>& A,
                                   const vector<vector<T>>& B);

template<typename T>
void printMatrix(const vector<vector<T>>& M) {
    for (auto &row : M) {
        for (auto &x : row) cout << x << " ";
        cout << "\n";
    }
}

int main() {
    // two test matrices A and B
    vector<vector<int>> A = {{1, 2},
                             {3, 4}};
    vector<vector<int>> B = {{5, 6},
                             {7, 8}};
    // compute
    auto C_add    = addMatrix(A,B);
    auto C_sub    = subtractMatrix(A,B);
    auto C_naive  = naiveMultiply(A,B);
    auto C_strass = strassenMultiply(A,B);

    // expected results
    vector<vector<int>> E_add   = {{ 6,  8},
                                   {10, 12}};
    vector<vector<int>> E_sub   = {{-4, -4},
                                   {-4, -4}};
    vector<vector<int>> E_mul   = {{19, 22},
                                   {43, 50}};

    // verify
    assert(C_add    == E_add);
    assert(C_sub    == E_sub);
    assert(C_naive  == E_mul);
    assert(C_strass == E_mul);

    // print
    cout << "addMatrix(A,B) =\n";        printMatrix(C_add);
    cout << "\nsubtractMatrix(A,B) =\n"; printMatrix(C_sub);
    cout << "\nnaiveMultiply(A,B) =\n";  printMatrix(C_naive);
    cout << "\nstrassenMultiply(A,B) =\n"; printMatrix(C_strass);
    cout << "\nAll tests passed!\n";

    return 0;
}
