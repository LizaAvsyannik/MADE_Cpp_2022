#include "src/matrix.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(MATRIX, Indexing) {
  std::array<double, 6> arr = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
  Matrix<double, 2, 3> matrix(arr);
  EXPECT_NEAR(matrix(0, 0), 1.1, EPS);
  EXPECT_NEAR(matrix(0, 1), 2.2, EPS);
  EXPECT_NEAR(matrix(0, 2), 3.3, EPS);
  EXPECT_NEAR(matrix(1, 0), 4.4, EPS);
  EXPECT_NEAR(matrix(1, 1), 5.5, EPS);
  EXPECT_NEAR(matrix(1, 2), 6.6, EPS);

  Matrix<double, 2, 3> matrix_cm(arr, true);
  EXPECT_NEAR(matrix(0, 0), 1.1, EPS);
  EXPECT_NEAR(matrix(0, 1), 2.2, EPS);
  EXPECT_NEAR(matrix(0, 2), 3.3, EPS);
  EXPECT_NEAR(matrix(1, 0), 4.4, EPS);
  EXPECT_NEAR(matrix(1, 1), 5.5, EPS);
  EXPECT_NEAR(matrix(1, 2), 6.6, EPS);
}

TEST(MATRIX, IndexingException) {
  std::array<double, 6> arr = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
  Matrix<double, 2, 3> matrix(arr);
  try {
    matrix(5, 5);
    FAIL() << "Expected std::out_of_range";
  } catch (std::out_of_range const &err) {
    EXPECT_STREQ(err.what(), "Index out of range");
  } catch (...) {
    FAIL() << "Expected std::out_of_range";
  }
}

TEST(MATRIX, Diagonal) {
  std::array<double, 9> arr = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9};
  Matrix<double, 3, 3> matrix(arr);

  Matrix<double, 3, 1> main_diag = matrix.GetDiag();
  Matrix<double, 3, 1> other_diag = matrix.GetDiag(false);
  std::array<double, 3> expected_main_diag = {1.1, 5.5, 9.9};
  std::array<double, 3> expected_other_diag = {3.3, 5.5, 7.7};
  EXPECT_THAT(main_diag.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_main_diag));
  EXPECT_THAT(other_diag.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_other_diag));

  Matrix<double, 3, 3> matrix_cm(arr, true);

  main_diag = matrix_cm.GetDiag();
  other_diag = matrix_cm.GetDiag(false);
  EXPECT_THAT(main_diag.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_main_diag));
  EXPECT_THAT(other_diag.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_other_diag));
}

TEST(MATRIX, GetRowCol) {
  std::array<double, 9> arr = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9};
  Matrix<double, 3, 3> matrix(arr);

  Matrix<double, 1, 3> row0 = matrix.GetRow(0);
  Matrix<double, 1, 3> row1 = matrix.GetRow(1);
  Matrix<double, 1, 3> row2 = matrix.GetRow(2);
  std::array<double, 3> expected_row0 = {1.1, 2.2, 3.3};
  std::array<double, 3> expected_row1 = {4.4, 5.5, 6.6};
  std::array<double, 3> expected_row2 = {7.7, 8.8, 9.9};
  EXPECT_THAT(row0.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_row0));
  EXPECT_THAT(row1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_row1));
  EXPECT_THAT(row2.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_row2));

  Matrix<double, 3, 1> col0 = matrix.GetCol(0);
  Matrix<double, 3, 1> col1 = matrix.GetCol(1);
  Matrix<double, 3, 1> col2 = matrix.GetCol(2);
  std::array<double, 3> expected_col0 = {1.1, 4.4, 7.7};
  std::array<double, 3> expected_col1 = {2.2, 5.5, 8.8};
  std::array<double, 3> expected_col2 = {3.3, 6.6, 9.9};
  EXPECT_THAT(col0.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_col0));
  EXPECT_THAT(col1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_col1));
  EXPECT_THAT(col2.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_col2));

  Matrix<double, 3, 3> matrix_cm(arr, true);

  row0 = matrix_cm.GetRow(0);
  row1 = matrix_cm.GetRow(1);
  row2 = matrix_cm.GetRow(2);
  EXPECT_THAT(row0.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_row0));
  EXPECT_THAT(row1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_row1));
  EXPECT_THAT(row2.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_row2));

  col0 = matrix_cm.GetCol(0);
  col1 = matrix_cm.GetCol(1);
  col2 = matrix_cm.GetCol(2);
  EXPECT_THAT(col0.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_col0));
  EXPECT_THAT(col1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_col1));
  EXPECT_THAT(col2.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_col2));
}

TEST(MATRIX, ElementByElementSameMajor) {
  std::array<double, 6> arr_1 = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
  Matrix<double, 3, 2> matrix_1(arr_1);
  std::array<double, 6> arr_2 = {7.7, 8.8, 9.9, 10.1, 11.11, 12.12};
  Matrix<double, 3, 2> matrix_2(arr_2);

  Matrix<double, 3, 2> matrix_res = matrix_1 + matrix_2;
  matrix_1 += matrix_2;
  std::array<double, 6> expected = {8.8, 11, 13.2, 14.5, 16.61, 18.72};
  EXPECT_THAT(matrix_1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix_res = matrix_1 - matrix_2;
  matrix_1 -= matrix_2;
  EXPECT_THAT(matrix_1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), arr_1));
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), arr_1));

  matrix_res = matrix_1 * matrix_2;
  matrix_1 *= matrix_2;
  expected = {8.47, 19.36, 32.67, 44.44, 61.105, 79.992};
  EXPECT_THAT(matrix_1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
}

TEST(MATRIX, ElementByElementDiffMajor1) {
  std::array<double, 6> arr_1 = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
  Matrix<double, 3, 2> matrix_1(arr_1);
  std::array<double, 6> arr_2 = {7.7, 8.8, 9.9, 10.1, 11.11, 12.12};
  Matrix<double, 3, 2> matrix_2(arr_2, false);

  Matrix<double, 3, 2> matrix_res = matrix_1 + matrix_2;
  matrix_1 += matrix_2;
  std::array<double, 6> expected = {8.8, 11, 13.2, 14.5, 16.61, 18.72};
  EXPECT_THAT(matrix_1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix_res = matrix_1 - matrix_2;
  matrix_1 -= matrix_2;
  EXPECT_THAT(matrix_1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), arr_1));
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), arr_1));

  matrix_res = matrix_1 * matrix_2;
  matrix_1 *= matrix_2;
  expected = {8.47, 19.36, 32.67, 44.44, 61.105, 79.992};
  EXPECT_THAT(matrix_1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
}

TEST(MATRIX, ElementByElementDiffMajor2) {
  std::array<double, 6> arr_1 = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
  Matrix<double, 3, 2> matrix_1(arr_1, true);
  std::array<double, 6> arr_2 = {7.7, 8.8, 9.9, 10.1, 11.11, 12.12};
  Matrix<double, 3, 2> matrix_2(arr_2);

  Matrix<double, 3, 2> matrix_res = matrix_1 + matrix_2;
  matrix_1 += matrix_2;
  std::array<double, 6> expected = {8.8, 13.2, 16.61, 11, 14.5, 18.72};
  EXPECT_THAT(matrix_1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix_res = matrix_1 - matrix_2;
  matrix_1 -= matrix_2;
  expected = {1.1, 3.3, 5.5, 2.2, 4.4, 6.6};
  EXPECT_THAT(matrix_1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix_res = matrix_1 * matrix_2;
  matrix_1 *= matrix_2;
  expected = {8.47, 32.67, 61.105, 19.36, 44.44, 79.992};
  EXPECT_THAT(matrix_1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
}

TEST(MATRIX, NumberByMatrix) {
  std::array<double, 6> arr = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
  Matrix<double, 3, 2> matrix(arr);

  Matrix<double, 3, 2> matrix_res = 7.75 * matrix;
  std::array<double, 6> expected = {8.525, 17.05, 25.575, 34.1, 42.625, 51.15};
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix.ToColumnMajor();
  matrix_res = 7.75 * matrix;
  expected = {8.525, 25.575, 42.625, 17.05, 34.1, 51.15};
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
}

TEST(MATRIX, VectorByMatrix) {
  std::array<double, 6> arr = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
  Matrix<double, 3, 2> matrix(arr);

  std::array<double, 3> arr_vec = {7.7, 8.8, 9.9};
  Matrix<double, 1, 3> vector(arr_vec);

  Matrix<double, 1, 2> vector_res = vector.MatMul(matrix);
  std::array<double, 2> expected = {91.96, 121.};
  EXPECT_THAT(vector_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix.ToColumnMajor();
  vector_res = vector.MatMul(matrix);
  EXPECT_THAT(vector_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
}

TEST(MATRIX, MatrixByVector) {
  std::array<double, 6> arr = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
  Matrix<double, 3, 2> matrix(arr);

  std::array<double, 2> arr_vec = {7.7, 8.8};
  Matrix<double, 2, 1> vector(arr_vec);

  Matrix<double, 3, 1> vector_res = matrix.MatMul(vector);
  std::array<double, 3> expected = {27.83, 64.13, 100.43};
  EXPECT_THAT(vector_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix.ToColumnMajor();
  vector_res = matrix.MatMul(vector);
  EXPECT_THAT(vector_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
}

TEST(MATRIX, MatrixByMatrix) {
  std::array<double, 6> arr_1 = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
  Matrix<double, 3, 2> matrix_1(arr_1);

  std::array<double, 8> arr_2 = {7.7,   8.8,   9.9,   10.1,
                                 11.11, 12.12, 13.13, 14.14};
  Matrix<double, 2, 4> matrix_2(arr_2);

  Matrix<double, 3, 4> matrix_res = matrix_1.MatMul(matrix_2);
  std::array<double, 12> expected = {32.912,  36.344,  39.776,  42.218,
                                     74.294,  82.368,  90.442,  95.546,
                                     115.676, 128.392, 141.108, 148.874};
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix_1.ToColumnMajor();
  matrix_res = matrix_1.MatMul(matrix_2);
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix_2.ToColumnMajor();
  matrix_res = matrix_1.MatMul(matrix_2);
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix_1.ToRowMajor();
  matrix_res = matrix_1.MatMul(matrix_2);
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
}

TEST(MATRIX, MatrixAndNumber) {
  std::array<double, 6> arr = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
  Matrix<double, 3, 2> matrix(arr);
  double number = 7.75;

  Matrix<double, 3, 2> matrix_res = matrix + number;
  matrix += number;
  std::array<double, 6> expected = {8.85, 9.95, 11.05, 12.15, 13.25, 14.35};
  EXPECT_THAT(matrix.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix_res = matrix - number;
  matrix -= number;
  EXPECT_THAT(matrix.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), arr));
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), arr));

  matrix_res = matrix * number;
  matrix *= number;
  expected = {8.525, 17.05, 25.575, 34.1, 42.625, 51.15};
  EXPECT_THAT(matrix.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
  EXPECT_THAT(matrix_res.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
}

TEST(MATRIX, TranslateVectorCol) {
  std::array<double, 6> arr = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
  Matrix<double, 3, 2> matrix(arr);
  std::array<double, 3> vec_arr_col = {7.7, 8.8, 9.9};
  Matrix<double, 3, 1> vector_col(vec_arr_col);

  matrix -= vector_col;
  std::array<double, 6> expected = {-6.6, -5.5, -5.5, -4.4, -4.4, -3.3};
  EXPECT_THAT(matrix.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix += vector_col;
  EXPECT_THAT(matrix.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), arr));

  matrix.ToColumnMajor();
  matrix -= vector_col;
  expected = {-6.6, -5.5, -4.4, -5.5, -4.4, -3.3};
  EXPECT_THAT(matrix.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix += vector_col;
  expected = {1.1, 3.3, 5.5, 2.2, 4.4, 6.6};
  EXPECT_THAT(matrix.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
}

TEST(MATRIX, TranslateVectorRow) {
  std::array<double, 6> arr = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
  Matrix<double, 3, 2> matrix(arr);
  std::array<double, 2> vec_arr_row = {7.7, 8.8};
  Matrix<double, 1, 2> vector_row(vec_arr_row);

  matrix -= vector_row;
  std::array<double, 6> expected = {-6.6, -6.6, -4.4, -4.4, -2.2, -2.2};
  EXPECT_THAT(matrix.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix += vector_row;
  EXPECT_THAT(matrix.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), arr));

  matrix.ToColumnMajor();
  matrix -= vector_row;
  expected = {-6.6, -4.4, -2.2, -6.6, -4.4, -2.2};
  EXPECT_THAT(matrix.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix += vector_row;
  expected = {1.1, 3.3, 5.5, 2.2, 4.4, 6.6};
  EXPECT_THAT(matrix.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
}

TEST(MATRIX, Transpose) {
  std::array<double, 6> arr = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
  Matrix<double, 3, 2> matrix(arr);

  Matrix<double, 2, 3> matrix_transposed = matrix.Transpose();
  std::array<double, 6> expected = {1.1, 3.3, 5.5, 2.2, 4.4, 6.6};
  EXPECT_THAT(matrix_transposed.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix.ToColumnMajor();
  matrix_transposed = matrix.Transpose();
  EXPECT_THAT(matrix_transposed.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
}

TEST(MATRIX, Inverse) {
  std::array<double, 9> arr = {1.5, 2.8, 3.7, 4.6, 5.5, 6.4, 7.3, 8.2, 9.1};
  Matrix<double, 3, 3> matrix(arr);

  Matrix<double, 3, 3> matrix_inverse = matrix.Inverse();
  std::array<double, 9> expected = {-2.5, 5.,           -2.5,
                                    5.,   -13.74485597, 7.63374486,
                                    -2.5, 8.3744856,    -4.76337449};
  EXPECT_THAT(matrix_inverse.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix.ToColumnMajor();
  matrix_inverse = matrix.Inverse();
  EXPECT_THAT(matrix_inverse.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
}

TEST(MATRIX, InverseException) {
  std::array<double, 9> arr = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9};
  Matrix<double, 3, 3> matrix(arr);
  try {
    matrix.Inverse();
    FAIL() << "Expected SingularMatrixException";
  } catch (SingularMatrixException const &err) {
    EXPECT_STREQ(err.what(), "Matrix is singular");
  } catch (...) {
    FAIL() << "Expected SingularMatrixException";
  }
}

TEST(MATRIX, Det) {
  std::array<double, 9> arr = {1.5, 2.8, 3.7, 4.6, 5.5, 6.4, 7.3, 8.2, 9.1};
  Matrix<double, 3, 3> matrix(arr);

  double det = matrix.Det();
  EXPECT_NEAR(det, 0.972, EPS);

  matrix.ToColumnMajor();
  det = matrix.Det();
  EXPECT_NEAR(det, 0.972, EPS);

  arr = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9};
  matrix = Matrix<double, 3, 3>(arr);

  det = matrix.Det();
  EXPECT_NEAR(det, 0.0, EPS);

  matrix.ToColumnMajor();
  det = matrix.Det();
  EXPECT_NEAR(det, 0.0, EPS);
}

TEST(MATRIX, Slice) {
  std::array<double, 9> arr = {1.5, 2.8, 3.7, 4.6, 5.5, 6.4, 7.3, 8.2, 9.1};
  Matrix<double, 3, 3> matrix(arr);

  Matrix<double, 2, 2> matrix_sliced = matrix.Slice<0, 2, 0, 2>();
  std::array<double, 4> expected = {1.5, 2.8, 4.6, 5.5};
  EXPECT_THAT(matrix_sliced.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  matrix.ToColumnMajor();
  matrix_sliced = matrix.Slice<0, 2, 0, 2>();
  expected = {1.5, 4.6, 2.8, 5.5};
  EXPECT_THAT(matrix_sliced.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
}
