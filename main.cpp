#include <array>
#include <iostream>
#include <vector>
#include "src/matrix.h"

int main() {
    // std::cout << "Hello, world!\n";
    // Vector v1 = Vector(2, 2, false);
    // Vector v2 = Vector(2, 2, false);
    // std::cin >> v2;
    // v1 += v2;
    // std::cout << v1;
    // v1 -= v2;
    // std::cout << v1;
    // v1 *= v2;
    // std::cout << v1;
    // Vector v3 = v1 + v2;
    // std::cout << v3;
    // v3 -= 5;
    // std::cout << v3;
    // std::cout << (5 * v3);
    // std::cout << v3[0];
    // Vector for_slice = Vector(3, 0, false);
    // std::cin >> for_slice;
    // std::cout << for_slice.Slice(1, 2);
    // std::cout << for_slice.Slice(-8, -6);
    // std::cout << for_slice.Slice(-8, 1);
    // std::cout << for_slice.Slice(6, 8);
    // std::cout << for_slice.Slice(-3, -1);
    // std::cout << for_slice.Slice(-3, 3);
    Matrix m1 = Matrix<double, 3, 3>(30, true);
    Matrix m2 = Matrix<double, 3, 1>(30, false);
    std::cin >> m1;
    std::cin >> m2;
    m1 -= m2;
    std::cout << m1;
    // std::cout << m1(2, 2);
    // std::cout << 2.0 * m1;
    // std::cout << m1.Inverse();
    // std::cout << m1;
    // std::cout << m1.translateColumn(m2, true);
    // // std::cout << m1.GetDiag(false);
    // // std::cout << m1.GetRow(1);
    // // std::cout << m1.GetCol(1);
    // // std::cout << m1.Transpose();
    // // Matrix m3 = m1;
    // std::cout << m1.Transpose();
    // std::cout << m1.Transpose().MatMul(m1);
    // Matrix vec = Matrix<double, 1, 1>(30, false);
    // vec += vec;
    // std::cout << vec;
    // std::array<Matrix<double, 2, 1>, 3> arr = {vec, Matrix<double, 2, 1>(40, false),  Matrix<double, 2, 1>(3600, false)}; 
    // std::cout << Matrix<double, 2, 3>(arr);
    // Matrix vec1 = Matrix<double, 1, 2>(30, false);
    // std::array<Matrix<double, 1, 2>, 3> arr1 = {vec1, Matrix<double, 1, 2>(40, false),  Matrix<double, 1, 2>(3600, false)}; 
    // std::cout << Matrix<double, 3, 2>(arr1);
    std::cout << m1.Slice<0, 1, 1, 2>();
    return 0;
}
