// test_matrix.cpp
#include "matrix_class-1.hpp"
#include <iostream>
#include <cassert>

int main() {
    using M = Matrix<double>;

    // 1) Test transpose()
    M A(2,3);
    //  [ 1  2  3 ]
    //  [ 4  5  6 ]
    A(0,0)=1; A(0,1)=2; A(0,2)=3;
    A(1,0)=4; A(1,1)=5; A(1,2)=6;

    M At = A.transpose();
    // At should be 3×2:
    //  [ 1 4 ]
    //  [ 2 5 ]
    //  [ 3 6 ]
    assert(At.numRows()==3 && At.numCols()==2);
    assert(At(0,0)==1 && At(0,1)==4);
    assert(At(1,0)==2 && At(1,1)==5);
    assert(At(2,0)==3 && At(2,1)==6);

    // 2) Test infinityNorm()
    // Row sums of A: row0=1+2+3=6, row1=4+5+6=15 → ∞‐norm=15
    double infA = A.infinityNorm();
    assert(infA == 15);

    // 3) Test operator+
    M B(2,3);
    // Let B = [6 5 4; 3 2 1]
    B(0,0)=6; B(0,1)=5; B(0,2)=4;
    B(1,0)=3; B(1,1)=2; B(1,2)=1;
    M C = A + B;  // should be [7 7 7; 7 7 7]
    for (int i = 0; i < C.numRows(); ++i)
      for (int j = 0; j < C.numCols(); ++j)
        assert(C(i,j) == 7);

    // 4) Test operator*  ( (2×3)*(3×2) → 2×2 )
    // A * At = [1*1+2*2+3*3,  1*4+2*5+3*6;  4*1+5*2+6*3,  4*4+5*5+6*6 ]
    //        = [14, 32; 32, 77]
    M D = A * At;
    assert(D.numRows()==2 && D.numCols()==2);
    assert(D(0,0)==14 && D(0,1)==32);
    assert(D(1,0)==32 && D(1,1)==77);

    std::cout << "All tests passed!\n";
    return 0;
}
