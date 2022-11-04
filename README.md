[![codecov](https://codecov.io/gh/LizaAvsyannik/MADE_Cpp_2022/branch/hw2/graph/badge.svg?token=NX85UGLETP)](https://codecov.io/gh/LizaAvsyannik/MADE_Cpp_2022)
### TO GET STARTED
```
cmake . -B build
cmake --build build
```
### CMAKE FLAGS
**WITH_COVERAGE** - to generate coverage report
**SANITIZE** - to enable sanitizers
**WITH_CLANG_TIDY** - to enable clang-tidy

### TO RUN TESTS (WITH COVERAGE REPORT)
```
cmake . -B build -DWITH_COVERAGE=ON -DSANITIZE=ON
cmake --build build
cd build/tests
ctest
cd ..
lcov --include <full_path_project>/src/\*  -t tests/ -d tests/ -c -o coverage.info
genhtml -o report coverage.info
```

### ABOUT
The main class is `Matrix`, which allows to work with double/float MxN matrices and vectors being implemented as 1xN and Mx1 matrices. Dimension of matrices is set using template parameters. 

### FUNCTIONALITY
| **INITIALIZERS**                                         | **Explanation**                                          |
|:--------------------------------------------------------:|:--------------------------------------------------------:|
| ```Matrix<T, m, n> matrix(arr, true);```                 | From std::array<T, m * n> of numbers, bool flag to store in column major if true|
| ```Matrix<T, m, n> matrix_from_row_vectors(arr)```       | From std::array of vectors 1xN                           |
| ```Matrix<T, m, n> matrix_from_col_vectors(arr)```       | From std::array of vectors Mx1                           |
| ```Matrix<T, m, n> matrix(other);```                 | From another matrix of the same size                     |


| **Indexing/Extraction**                                                | **Explanation**                              |
|:--------------------------------------------------------:|:--------------------------------------------------------:|
| ```matrix(2, 2)```                                      | Element at position (2, 2)                        |
| ```matrix.getDiag()```                                  | Main diagonal                                             |
| ```matrix.getDiag(false)```                             | Counter diagonal                                          |
| ```matrix.getRow(0)```                                  | 1st row (type Matrix <T, n, 1>)                           |
| ```matrix.getColumn(1)```                               | 2nd column  (type Matrix <T, m, 1>)                       |


| **Element by Element Operations (for two matrices of the same size)**                                               |
|:--------------------------------------------------------:|
| ```matrix1 + matrix2, matrix_1 += matrix_2```                                     
| ```matrix1 - matrix2, matrix_1 -= matrix_2```                                  
| ```matrix1 * matrix2, matrix_1 *= matrix_2```                              


| **Vector Translation**                              | **Explanation**                                          |  
|:--------------------------------------------------------:|:--------------------------------------------------------:|
| ```matrix += vector```                  | Adds vector<T, 1, n> from each row                               |
| ```matrix += vector```                  | Adds vector<T, m, 1> from each column                            |   
|```matrix -= vector```                   | Substracts vector<T, 1, n> from each row                        |
| ```matrix -= vector```                  | Substracts vector<T, m, 1> from each column                     |


| **Operations with Numbers**                              | **Explanation**                                          |  
|:--------------------------------------------------------:|:--------------------------------------------------------:|
| ```matrix + number, matrix += number```                  | Adds number to each element                              | 
| ```matrix - number, matrix -= number```                  | Substructs number from each element                      |   
| ```matrix * number, matrix *= number, number * matrix``` | Multiplies each element  by number                       | 


| **Linear Algebra**                                       | **Explanation**                                          |  
|:--------------------------------------------------------:|:--------------------------------------------------------:|
| ```matrix_1.MatMul(matrix_2)```                          | Matrix multiplication for matrices of corresponding sizes MxN and NxK| 
| ```matrix.Transpose()```                                 | Matrix <T, n, m> stored in row major                     |  
| ```matrix.Det()```                                       | Matrix determinant for square matrices                   |
| ```matrix.Inverse()```                                    |Inverse matrix for square non-singualr matrices          |

| **Slicing**                                              | **Explanation**                                          |  
|:--------------------------------------------------------:|:--------------------------------------------------------:|
| ```matrix.Slice<0, 1, 1, 3>()```                         | Slice: rows [0, 1), columns [1, 3), stored in same order as sliced matrix|


| **Other**                                                | **Explanation**                                          |  
|:--------------------------------------------------------:|:--------------------------------------------------------:|
| ```matrix.ToRowMajor()```                                | Store in row major order                                 | 
| ```matrix.ToColumnMajor()```                             | Store in column major order                              |   
| ```matrix.IsColumnMajor()```                             | Get in which order the matrix is stored                  |
| ```matrix_1.Swap(matrix_2)```                            | Swaps two matrices of same size                          |
