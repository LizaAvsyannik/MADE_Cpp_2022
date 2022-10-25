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
    // std::cout << for_slice.slice(1, 2);
    // std::cout << for_slice.slice(-8, -6);
    // std::cout << for_slice.slice(-8, 1);
    // std::cout << for_slice.slice(6, 8);
    // std::cout << for_slice.slice(-3, -1);
    // std::cout << for_slice.slice(-3, 3);
    Matrix m1 = Matrix<double, 3, 3>(30, true);
    Matrix m2 = Matrix<double, 3, 1>(30, false);
    std::cin >> m1;
    std::cout << m1.getDiag(false);
    // std::cout << m1.inverse();
    // std::cout << m1;
    // std::cout << m1.translateColumn(m2, true);
    // // std::cout << m1.getDiag(false);
    // // std::cout << m1.getRow(1);
    // // std::cout << m1.getCol(1);
    // // std::cout << m1.transpose();
    // // Matrix m3 = m1;
    // std::cout << m1.transpose();
    // std::cout << m1.transpose().matmul(m1);
    // Matrix vec = Matrix<double, 2, 1>(30, false);
    // std::vector<Matrix<double, 2, 1>> arr; 
    // arr.push_back(vec);
    // arr.push_back(Matrix<double, 2, 1>(40, false));
    // arr.push_back(Matrix<double, 2, 1>(3600, false));
    // std::cout << Matrix<double, 2, 3>(arr, 3);
    std::cout << m1.slice<0, 2, 1, 2>();
    return 0;
}
