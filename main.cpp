#include <iostream>

#include "src/vector.h"
#include "src/matrix.h"

int main() {
    std::cout << "Hello, world!\n";
    Vector v1 = Vector(2, 2, false);
    Vector v2 = Vector(2, 2, false);
    std::cin >> v2;
    v1 += v2;
    std::cout << v1;
    v1 -= v2;
    std::cout << v1;
    v1 *= v2;
    std::cout << v1;
    Vector v3 = v1 + v2;
    std::cout << v3;
    v3 -= 5;
    std::cout << v3;
    std::cout << (5 * v3);
    std::cout << v3[0];
    Vector for_slice = Vector(3, 0, false);
    std::cin >> for_slice;
    std::cout << for_slice.slice(1, 2);
    std::cout << for_slice.slice(-8, -6);
    std::cout << for_slice.slice(-8, 1);
    std::cout << for_slice.slice(6, 8);
    std::cout << for_slice.slice(-3, -1);
    std::cout << for_slice.slice(-3, 3);
    Matrix m1 = Matrix<double, 2, 3>(30, false);
    Matrix m2 = Matrix<double, 2, 3>(30, true);
    std::cin >> m1;
    std::cout << m1;
    m1 += m2;
    std::cout << m1.transpose();
}
