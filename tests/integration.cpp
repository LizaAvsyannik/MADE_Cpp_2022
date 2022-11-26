#include "src/matrix.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(MATRIX, IntegrationTest1) {
  std::array<Matrix<double, 3, 1>, 3> arr = {Matrix<double, 3, 1>(1.1),
                                             Matrix<double, 3, 1>(1.2),
                                             Matrix<double, 3, 1>(1.3)};
  Matrix<double, 3, 3> matrix(arr);

  Matrix<double, 3, 1> vec_1 = matrix(1, 1) * matrix.GetDiag();
  std::array<double, 3> expected_vec = {1.32, 1.44, 1.56};
  EXPECT_THAT(vec_1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_vec));

  Matrix<double, 1, 3> vec_2 = matrix(0, 1) * matrix.GetRow(0);
  EXPECT_THAT(vec_2.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_vec));

  Matrix<double, 3, 1> vec_3 = matrix(2, 1) * matrix.GetCol(2);
  expected_vec = {1.56, 1.56, 1.56};
  EXPECT_THAT(vec_3.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_vec));

  vec_2 = (vec_1 + vec_3).Transpose();
  expected_vec = {2.88, 3, 3.12};
  EXPECT_THAT(vec_2.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_vec));

  matrix += vec_1;
  matrix -= vec_2;
  vec_1 = matrix.MatMul(vec_3);
  expected_vec = {-2.2464, -1.6848, -1.1232};
  EXPECT_THAT(vec_1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_vec));
  try {
    matrix.Inverse();
    FAIL() << "Expected SingularMatrixException";
  } catch (SingularMatrixException const &err) {
    EXPECT_STREQ(err.what(), "Matrix is singular");
  } catch (...) {
    FAIL() << "Expected SingularMatrixException";
  }
}

TEST(MATRIX, IntegrationTest2) {
  std::array<Matrix<double, 3, 1>, 3> arr = {Matrix<double, 3, 1>(1.1),
                                             Matrix<double, 3, 1>(1.2),
                                             Matrix<double, 3, 1>(1.3)};
  Matrix<double, 3, 3> matrix(arr);

  Matrix<double, 3, 1> vec_1 = matrix(1, 1) * matrix.GetDiag();
  std::array<double, 3> expected_vec = {1.32, 1.44, 1.56};
  EXPECT_THAT(vec_1.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_vec));

  Matrix<double, 1, 3> vec_2 = matrix(0, 1) * matrix.GetRow(0);
  EXPECT_THAT(vec_2.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_vec));

  Matrix<double, 3, 1> vec_3 = matrix(2, 1) * matrix.GetCol(2);
  expected_vec = {1.56, 1.56, 1.56};
  EXPECT_THAT(vec_3.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected_vec));

  std::array<Matrix<double, 1, 3>, 2> arr_2 = {vec_1.Transpose(),
                                               vec_3.Transpose()};
  Matrix<double, 2, 3> matrix_2(arr_2);
  Matrix<double, 2, 2> matrix_2_inv = matrix_2.Slice<0, 2, 0, 2>().Inverse();
  std::array<double, 4> expected = {-8.33333333, 7.69230769, 8.33333333,
                                    -7.05128205};
  EXPECT_THAT(matrix_2_inv.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));

  Matrix<double, 2, 2> identity_matrix =
      matrix_2_inv.MatMul(matrix_2.Slice<0, 2, 0, 2>());
  expected = {1.0, 0.0, 0.0, 1.0};
  EXPECT_THAT(identity_matrix.GetMatrix(),
              testing::Pointwise(testing::FloatNear(EPS), expected));
}
