#include <gtest/gtest.h>
#include "src/matrix.h"

TEST(MATRIX, Indexing) {
  std::array<double, 6> arr = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6 }; 
  Matrix<double, 2, 3> matrix(arr);
  EXPECT_EQ(matrix(0, 0), 1.1);
  EXPECT_EQ(matrix(0, 1), 2.2);
  EXPECT_EQ(matrix(0, 2), 3.3);
  EXPECT_EQ(matrix(1, 0), 4.4);
  EXPECT_EQ(matrix(1, 1), 5.5);
  EXPECT_EQ(matrix(1, 2), 6.6);

  Matrix<double, 2, 3> matrix_cm(arr, true);
  EXPECT_EQ(matrix(0, 0), 1.1);
  EXPECT_EQ(matrix(0, 1), 2.2);
  EXPECT_EQ(matrix(0, 2), 3.3);
  EXPECT_EQ(matrix(1, 0), 4.4);
  EXPECT_EQ(matrix(1, 1), 5.5);
  EXPECT_EQ(matrix(1, 2), 6.6);
}

TEST(MATRIX, Diagonal) {
  std::array<double, 9> arr = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9 }; 
  Matrix<double, 3, 3> matrix(arr);

  Matrix<double, 3, 1> main_diag = matrix.GetDiag();
  Matrix<double, 3, 1> other_diag = matrix.GetDiag(false);
  std::array<double, 3> expected_main_diag = { 1.1, 5.5, 9.9 }; 
  std::array<double, 3> expected_other_diag = { 3.3, 5.5, 7.7 }; 
  EXPECT_EQ(main_diag.GetMatrix(), expected_main_diag);
  EXPECT_EQ(other_diag.GetMatrix(), expected_other_diag);

  Matrix<double, 3, 3> matrix_cm(arr, true);

  main_diag = matrix_cm.GetDiag();
  other_diag = matrix_cm.GetDiag(false);
  EXPECT_EQ(main_diag.GetMatrix(), expected_main_diag);
  EXPECT_EQ(other_diag.GetMatrix(), expected_other_diag);
}

TEST(MATRIX, GetRowCol) {
  std::array<double, 9> arr = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9 }; 
  Matrix<double, 3, 3> matrix(arr);

  Matrix<double, 1, 3> row0 = matrix.GetRow(0);
  Matrix<double, 1, 3> row1 = matrix.GetRow(1);
  Matrix<double, 1, 3> row2 = matrix.GetRow(2);
  std::array<double, 3> expected_row0 = { 1.1, 2.2, 3.3 }; 
  std::array<double, 3> expected_row1 = { 4.4, 5.5, 6.6 }; 
  std::array<double, 3> expected_row2 = { 7.7, 8.8, 9.9 }; 
  EXPECT_EQ(row0.GetMatrix(), expected_row0);
  EXPECT_EQ(row1.GetMatrix(), expected_row1);
  EXPECT_EQ(row2.GetMatrix(), expected_row2);

  Matrix<double, 3, 1> col0 = matrix.GetCol(0);
  Matrix<double, 3, 1> col1 = matrix.GetCol(1);
  Matrix<double, 3, 1> col2 = matrix.GetCol(2);
  std::array<double, 3> expected_col0 = { 1.1, 4.4, 7.7 }; 
  std::array<double, 3> expected_col1 = { 2.2, 5.5, 8.8 }; 
  std::array<double, 3> expected_col2 = { 3.3, 6.6, 9.9 }; 
  EXPECT_EQ(col0.GetMatrix(), expected_col0);
  EXPECT_EQ(col1.GetMatrix(), expected_col1);
  EXPECT_EQ(col2.GetMatrix(), expected_col2);

  Matrix<double, 3, 3> matrix_cm(arr, true);

  row0 = matrix_cm.GetRow(0);
  row1 = matrix_cm.GetRow(1);
  row2 = matrix_cm.GetRow(2);
  EXPECT_EQ(row0.GetMatrix(), expected_row0);
  EXPECT_EQ(row1.GetMatrix(), expected_row1);
  EXPECT_EQ(row2.GetMatrix(), expected_row2);

  col0 = matrix_cm.GetCol(0);
  col1 = matrix_cm.GetCol(1);
  col2 = matrix_cm.GetCol(2);
  EXPECT_EQ(col0.GetMatrix(), expected_col0);
  EXPECT_EQ(col1.GetMatrix(), expected_col1);
  EXPECT_EQ(col2.GetMatrix(), expected_col2);
}
