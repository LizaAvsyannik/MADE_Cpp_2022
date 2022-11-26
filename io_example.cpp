#include <iostream>
#include <vector>
#include "src/matrix.h"

int main() {
    std::cout << "Please enter 9 numbers for 3x3 matrix\n";
    Matrix<double, 3, 3> matrix(0);
    std::cin >> matrix;
    std::cout << matrix;
}
