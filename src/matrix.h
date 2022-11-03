#pragma once
#include "exceptions.h"
#include <array>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <vector>

template <class T, size_t m, size_t n> class Matrix;

template <class T, size_t m, size_t n>
std::ostream &operator<<(std::ostream &stream, const Matrix<T, m, n> &obj);

template <class T, size_t m, size_t n>
std::istream &operator>>(std::istream &stream, Matrix<T, m, n> &obj);

template <class T, size_t m, size_t n>
Matrix<T, m, n> operator*(const T &number, const Matrix<T, m, n> &obj);

template <class T, size_t m, size_t n> class Matrix {
 private:
  static_assert(std::is_floating_point<T>::value,
                "Wrong type, should be float or double");
  const size_t kSize = m * n;
  std::array<T, m * n> matrix_;
  bool is_column_major_ = false;
  std::array<T, m * n> l_;
  std::array<T, m * n> u_;
  bool is_lu_valid = false;

  friend std::ostream &operator<<<>(std::ostream &stream, const Matrix &obj);
  friend std::istream &operator>><>(std::istream &stream, Matrix &obj);

  template <size_t k, size_t l>
  Matrix &Translate(const Matrix<T, k, l> &other, bool positive = true);

 public:
  explicit Matrix(T elem, bool is_column_major = false);
  explicit Matrix(const std::array<T, m * n> &arr,
                  bool is_column_major = false);

  Matrix(const std::array<Matrix<T, 1, n>, m> &arr);
  template <bool condition = (m != 1),
            typename std::enable_if_t<condition, bool> = true>
  Matrix(const std::array<Matrix<T, m, 1>, n> &arr);

  Matrix(const Matrix<T, m, n> &obj);

  bool IsColumnMajor() const;
  const std::array<T, m * n> &GetMatrix() const;
  Matrix &ToColumnMajor();
  Matrix &ToRowMajor();
  Matrix &Swap(Matrix &other);

  T operator()(size_t row_index, size_t column_index) const;
  Matrix<T, m, 1> GetDiag(bool main = true) const;
  Matrix<T, 1, n> GetRow(const size_t row_index) const;
  Matrix<T, m, 1> GetCol(const size_t column_index) const;

  Matrix &operator=(const Matrix &other);
  template <size_t k, size_t l>
  Matrix &operator+=(const Matrix<T, k, l> &other);
  template <size_t k, size_t l>
  Matrix &operator-=(const Matrix<T, k, l> &other);
  Matrix &operator*=(const Matrix &other);

  Matrix operator+(const Matrix &other) const;
  Matrix operator-(const Matrix &other) const;
  Matrix operator*(const Matrix &other) const;

  Matrix &operator+=(const T &number);
  Matrix &operator-=(const T &number);
  Matrix &operator*=(const T &number);

  Matrix operator+(const T &number) const;
  Matrix operator-(const T &number) const;
  Matrix operator*(const T &number) const;

  template <size_t k>
  Matrix<T, m, k> MatMul(const Matrix<T, n, k> &other) const;
  Matrix<T, n, m> Transpose() const;

  void LUDecomposition();
  T Det();
  Matrix Inverse();

  template <size_t row_start, size_t row_end, size_t col_start, size_t col_end>
  Matrix<T, row_end - row_start, col_end - col_start> Slice() const;
};

template <class T, size_t m, size_t n>
Matrix<T, m, n>::Matrix(T elem, bool is_column_major) {
  for (size_t i = 0; i < this->kSize; ++i) {
    this->matrix_[i] = elem;
  }
  this->is_column_major_ = is_column_major;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n>::Matrix(const std::array<T, m * n> &arr, bool is_column_major) {
  this->is_column_major_ = is_column_major;
  this->matrix_ = arr;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n>::Matrix(const std::array<Matrix<T, 1, n>, m> &arr) {
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < n; ++j) {
      this->matrix_[i * n + j] = arr[i].GetMatrix()[j];
    }
  }
}

template <class T, size_t m, size_t n>
template <bool condition, typename std::enable_if_t<condition, bool>>
Matrix<T, m, n>::Matrix(const std::array<Matrix<T, m, 1>, n> &arr) {
  this->is_column_major_ = true;
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < n; ++j) {
      this->matrix_[j * m + i] = arr[j].GetMatrix()[i];
    }
  }
}

template <class T, size_t m, size_t n>
Matrix<T, m, n>::Matrix(const Matrix &obj) {
  this->is_column_major_ = obj.is_column_major_;
  this->matrix_ = obj.matrix_;
}

template <class T, size_t m, size_t n>
bool Matrix<T, m, n>::IsColumnMajor() const {
  return this->is_column_major_;
}

template <class T, size_t m, size_t n>
const std::array<T, m * n> &Matrix<T, m, n>::GetMatrix() const {
  return this->matrix_;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::ToRowMajor() {
  if (!this->is_column_major_)
    return *this;
  std::array<T, m * n> elements;
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < n; ++j)
      elements[i * n + j] = this->matrix_[j * m + i];
  }
  this->matrix_ = elements;
  return *this;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::ToColumnMajor() {
  if (this->is_column_major_)
    return *this;
  std::array<T, m * n> elements;
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < n; ++j)
      elements[j * m + i] = this->matrix_[i * n + j];
  }
  this->matrix_ = elements;
  return *this;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::Swap(Matrix &other) {
  std::swap(this->matrix_, other.matrix_);
  std::swap(this->is_column_major_, other.is_column_major_);
  std::swap(this->l_, other.l_);
  std::swap(this->u_, other.u_);
  std::swap(this->is_lu_valid, other.is_lu_valid);
  return *this;
}

template <class T, size_t m, size_t n>
std::ostream &operator<<(std::ostream &stream, const Matrix<T, m, n> &obj) {
  if (obj.is_column_major_) {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        std::cout << obj.matrix_[j * m + i] << ' ';
      }
      std::cout << '\n';
    }
  } else {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        std::cout << obj.matrix_[i * n + j] << ' ';
      }
      std::cout << '\n';
    }
  }
  return stream;
}

template <class T, size_t m, size_t n>
std::istream &operator>>(std::istream &stream, Matrix<T, m, n> &obj) {
  if (obj.is_column_major_) {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        std::cin >> obj.matrix_[j * m + i];
      }
    }
  } else {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        std::cin >> obj.matrix_[i * n + j];
      }
    }
  }
  obj.is_lu_valid = false;
  return stream;
}

template <class T, size_t m, size_t n>
T Matrix<T, m, n>::operator()(size_t row_index, size_t column_index) const {
  if (row_index >= m || column_index >= n) {
    throw std::out_of_range("Index out of range");
  }
  if (this->is_column_major_) {
    return this->matrix_[column_index * m + row_index];
  } else {
    return this->matrix_[row_index * n + column_index];
  }
}

template <class T, size_t m, size_t n>
Matrix<T, m, 1> Matrix<T, m, n>::GetDiag(bool main) const {
  static_assert(m == n, "Matrix is not square");
  std::array<T, m> elements;
  if (main) {
    for (size_t i = 0; i < m; ++i) {
      elements[i] = this->matrix_[i * m + i];
    }
  } else {
    if (this->is_column_major_) {
      for (size_t i = 0; i < m; ++i) {
        elements[i] = this->matrix_[(m - i) * (m - 1)];
      }
    } else {
      for (size_t i = 0; i < m; ++i) {
        elements[i] = this->matrix_[(i + 1) * (m - 1)];
      }
    }
  }
  Matrix<T, m, 1> diag(elements, m);
  return diag;
}

template <class T, size_t m, size_t n>
Matrix<T, 1, n> Matrix<T, m, n>::GetRow(const size_t row_index) const {
  std::array<T, n> elements;
  if (this->is_column_major_) {
    for (size_t i = 0; i < n; ++i) {
      elements[i] = this->matrix_[i * m + row_index];
    }
  } else {
    for (size_t i = 0; i < n; ++i) {
      elements[i] = this->matrix_[row_index * n + i];
    }
  }
  Matrix<T, 1, n> row(elements, n);
  return row;
}

template <class T, size_t m, size_t n>
Matrix<T, m, 1> Matrix<T, m, n>::GetCol(const size_t column_index) const {
  std::array<T, m> elements;
  if (this->is_column_major_) {
    for (size_t i = 0; i < m; ++i) {
      elements[i] = this->matrix_[column_index * m + i];
    }
  } else {
    for (size_t i = 0; i < m; ++i) {
      elements[i] = this->matrix_[i * n + column_index];
    }
  }
  Matrix<T, m, 1> column(elements, m);
  return column;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::operator=(const Matrix &other) {
  if (this != &other) {
    Matrix(other).Swap(*this);
  }
  return *this;
}

template <class T, size_t m, size_t n>
template <size_t k, size_t l>
Matrix<T, m, n> &Matrix<T, m, n>::Translate(const Matrix<T, k, l> &other,
                                            bool positive) {
  if (k == 1 && l == n) {
    if (!this->is_column_major_) {
      for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
          if (positive) {
            this->matrix_[i * n + j] += other.GetMatrix()[j];
          } else {
            this->matrix_[i * n + j] -= other.GetMatrix()[j];
          }
        }
      }
    } else {
      for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
          if (positive) {
            this->matrix_[j * m + i] += other.GetMatrix()[j];
          } else {
            this->matrix_[j * m + i] -= other.GetMatrix()[j];
          }
        }
      }
    }
  } else if (k == m && l == 1) {
    if (this->is_column_major_) {
      for (std::size_t j = 0; j < n; ++j) {
        for (std::size_t i = 0; i < m; ++i) {
          if (positive) {
            this->matrix_[j * m + i] += other.GetMatrix()[i];
          } else {
            this->matrix_[j * m + i] -= other.GetMatrix()[i];
          }
        }
      }
    } else {
      for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
          if (positive) {
            this->matrix_[i * n + j] += other.GetMatrix()[i];
          } else {
            this->matrix_[i * n + j] -= other.GetMatrix()[i];
          }
        }
      }
    }
  }
  return *this;
}

template <class T, size_t m, size_t n>
template <size_t k, size_t l>
Matrix<T, m, n> &Matrix<T, m, n>::operator+=(const Matrix<T, k, l> &other) {
  static_assert((k == m && l == n) || (k == m && l == 1) || (k == 1 && l == n),
                "Sizes do not match");
  if ((k == 1) || (l == 1)) {
    return Translate(other);
  }

  if (this->is_column_major_ == other.IsColumnMajor()) {
    for (size_t i = 0; i < this->kSize; ++i) {
      this->matrix_[i] += other.GetMatrix()[i];
    }
  } else if (this->is_column_major_) {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        this->matrix_[j * m + i] += other.GetMatrix()[i * n + j];
      }
    }
  } else {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        this->matrix_[i * n + j] += other.GetMatrix()[j * m + i];
      }
    }
  }
  this->is_lu_valid = false;
  return *this;
}

template <class T, size_t m, size_t n>
template <size_t k, size_t l>
Matrix<T, m, n> &Matrix<T, m, n>::operator-=(const Matrix<T, k, l> &other) {
  static_assert((k == m && l == n) || (k == m && l == 1) || (k == 1 && l == n),
                "Sizes do not match");
  if ((k == 1) || (l == 1)) {
    return Translate(other, false);
  }

  if (this->is_column_major_ == other.IsColumnMajor()) {
    for (size_t i = 0; i < this->kSize; ++i) {
      this->matrix_[i] -= other.GetMatrix()[i];
    }
  } else if (this->is_column_major_) {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        this->matrix_[j * m + i] -= other.GetMatrix()[i * n + j];
      }
    }
  } else {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        this->matrix_[i * n + j] -= other.GetMatrix()[j * m + i];
      }
    }
  }
  this->is_lu_valid = false;
  return *this;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::operator*=(const Matrix &other) {
  if (this->is_column_major_ == other.is_column_major_) {
    for (size_t i = 0; i < this->kSize; ++i) {
      this->matrix_[i] += other.matrix_[i];
    }
  } else if (this->is_column_major_) {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        this->matrix_[j * m + i] *= other.matrix_[i * n + j];
      }
    }
  } else {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        this->matrix_[i * n + j] *= other.matrix_[j * m + i];
      }
    }
  }
  this->is_lu_valid = false;
  return *this;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> Matrix<T, m, n>::operator+(const Matrix &other) const {
  Matrix sum_matrix(*this);
  if (this->is_column_major_ == other.is_column_major_) {
    for (size_t i = 0; i < this->kSize; ++i) {
      sum_matrix.matrix_[i] += other.matrix_[i];
    }
  } else if (this->is_column_major_) {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        sum_matrix.matrix_[j * m + i] += other.matrix_[i * n + j];
      }
    }
  } else {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        sum_matrix.matrix_[i * n + j] += other.matrix_[j * m + i];
      }
    }
  }
  return sum_matrix;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> Matrix<T, m, n>::operator-(const Matrix &other) const {
  Matrix sub_matrix(*this);
  if (this->is_column_major_ == other.is_column_major_) {
    for (size_t i = 0; i < this->kSize; ++i) {
      sub_matrix.matrix_[i] -= other.matrix_[i];
    }
  } else if (this->is_column_major_) {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        sub_matrix.matrix_[j * m + i] -= other.matrix_[i * n + j];
      }
    }
  } else {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        sub_matrix.matrix_[i * n + j] -= other.matrix_[j * m + i];
      }
    }
  }
  return sub_matrix;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> Matrix<T, m, n>::operator*(const Matrix &other) const {
  Matrix mult_matrix(*this);
  if (this->is_column_major_ == other.is_column_major_) {
    for (size_t i = 0; i < this->kSize; ++i) {
      mult_matrix.matrix_[i] *= other.matrix_[i];
    }
  } else if (this->is_column_major_) {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        mult_matrix.matrix_[j * m + i] *= other.matrix_[i * n + j];
      }
    }
  } else {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j) {
        mult_matrix.matrix_[i * n + j] *= other.matrix_[j * m + i];
      }
    }
  }
  return mult_matrix;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::operator+=(const T &number) {
  for (size_t i = 0; i < this->kSize; ++i) {
    this->matrix_[i] += number;
  }
  this->is_lu_valid = false;
  return *this;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::operator-=(const T &number) {
  for (size_t i = 0; i < this->kSize; ++i) {
    this->matrix_[i] -= number;
  }
  this->is_lu_valid = false;
  return *this;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::operator*=(const T &number) {
  for (size_t i = 0; i < this->kSize; ++i) {
    this->matrix_[i] *= number;
  }
  this->is_lu_valid = false;
  return *this;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> Matrix<T, m, n>::operator+(const T &number) const {
  Matrix sum_matrix(*this);
  for (size_t i = 0; i < this->kSize; ++i) {
    sum_matrix.matrix_[i] += number;
  }
  return sum_matrix;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> Matrix<T, m, n>::operator-(const T &number) const {
  Matrix sub_matrix(*this);
  for (size_t i = 0; i < this->kSize; ++i) {
    sub_matrix.matrix_[i] -= number;
  }
  return sub_matrix;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> Matrix<T, m, n>::operator*(const T &number) const {
  Matrix mult_matrix(*this);
  for (size_t i = 0; i < this->kSize; ++i) {
    mult_matrix.matrix_[i] *= number;
  }
  return mult_matrix;
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> operator*(const T &number, const Matrix<T, m, n> &obj) {
  return obj * number;
}

template <class T, size_t m, size_t n>
template <size_t k>
Matrix<T, m, k> Matrix<T, m, n>::MatMul(const Matrix<T, n, k> &other) const {
  std::array<T, m * k> elements;
  if (!this->is_column_major_ && other.IsColumnMajor()) {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < k; ++j) {
        elements[i * k + j] = 0;
        for (size_t l = 0; l < n; ++l) {
          elements[i * k + j] +=
              this->matrix_[i * n + l] * other.GetMatrix()[j * n + l];
        }
      }
    }
  }
  if (this->is_column_major_ && other.IsColumnMajor()) {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < k; ++j) {
        elements[i * k + j] = 0;
        for (size_t l = 0; l < n; ++l) {
          elements[i * k + j] +=
              this->matrix_[l * m + i] * other.GetMatrix()[j * n + l];
        }
      }
    }
  }
  if (!this->is_column_major_ && !other.IsColumnMajor()) {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < k; ++j) {
        elements[i * k + j] = 0;
        for (size_t l = 0; l < n; ++l) {
          elements[i * k + j] +=
              this->matrix_[i * n + l] * other.GetMatrix()[l * k + j];
        }
      }
    }
  }
  if (this->is_column_major_ && !other.IsColumnMajor()) {
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < k; ++j) {
        elements[i * k + j] = 0;
        for (size_t l = 0; l < n; ++l) {
          elements[i * k + j] +=
              this->matrix_[l * m + i] * other.GetMatrix()[l * k + j];
        }
      }
    }
  }
  Matrix<T, m, k> mult(elements, m * k);
  return mult;
}

template <class T, size_t m, size_t n> void Matrix<T, m, n>::LUDecomposition() {
  static_assert(m == n, "Matrix is not square");

  if (this->is_lu_valid)
    return;

  this->is_lu_valid = true;
  for (size_t i = 0; i < this->kSize; ++i) {
    this->l_[i] = 0;
    this->u_[i] = 0;
  }

  for (size_t i = 0; i < m; ++i) {
    for (size_t k = i; k < m; ++k) {
      T sum = 0;
      for (size_t j = 0; j < i; ++j) {
        sum += (this->l_[i * n + j] * this->u_[j * n + k]);
      }
      if (!this->is_column_major_) {
        this->u_[i * n + k] = this->matrix_[i * n + k] - sum;
      } else {
        this->u_[i * n + k] = this->matrix_[k * m + i] - sum;
      }
    }
    for (size_t k = i; k < n; ++k) {
      if (i == k) {
        this->l_[i * (n + 1)] = 1;
      } else {
        T sum = 0;
        for (size_t j = 0; j < i; ++j) {
          sum += (this->l_[k * n + j] * this->u_[j * n + i]);
        }
        if (!this->is_column_major_) {
          this->l_[k * n + i] =
              (this->matrix_[k * n + i] - sum) / this->u_[i * (n + 1)];
        } else {
          this->l_[k * n + i] =
              (this->matrix_[i * m + k] - sum) / this->u_[i * (n + 1)];
        }
      }
    }
  }
  // for (size_t i = 0; i < m * n; ++i) {
  //     std::cout << this->l_[i] <<' ';
  // }
  // for (size_t i = 0; i < m * n; ++i) {
  //     std::cout << this->u_[i] <<' ';
  // }
  // std::cout << '\n';
}

template <class T, size_t m, size_t n> T Matrix<T, m, n>::Det() {
  static_assert(m == n, "Matrix is not square");
  LUDecomposition();

  T det = 1;
  for (size_t i = 0; i < m; ++i) {
    det *= this->u_[i * m + i];
    if (det == 0) {
      return det;
    }
  }
  return det;
}

template <class T, size_t m, size_t n>
Matrix<T, n, m> Matrix<T, m, n>::Transpose() const {
  if (this->is_column_major_) {
    Matrix<T, n, m> transposed(this->matrix_);
    return transposed;
  } else {
    std::array<T, m * n> elements;
    for (size_t i = 0; i < m; ++i) {
      for (size_t j = 0; j < n; ++j)
        elements[j * m + i] = this->matrix_[i * n + j];
    }
    Matrix<T, n, m> transposed(elements);
    return transposed;
  }
}

template <class T, size_t m, size_t n>
Matrix<T, m, n> Matrix<T, m, n>::Inverse() {
  static_assert(m == n, "Matrix is not square");

  LUDecomposition();
  if (this->Det() == 0) {
    throw SingularMatrixException();
  }

  std::array<T, m * n> inverse_l;
  std::array<T, m * n> inverse_u;
  for (size_t i = 0; i < this->kSize; ++i) {
    inverse_l[i] = 0;
    inverse_u[i] = 0;
  }

  std::array<T, m * n> transposed_u;
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < n; ++j)
      transposed_u[j * m + i] = this->u_[i * n + j];
  }

  for (size_t i = 0; i < m; ++i) {
    inverse_l[i * (n + 1)] = this->l_[i * (n + 1)];
    inverse_u[i * (n + 1)] = 1.0 / transposed_u[i * (n + 1)];
    for (size_t j = 0; j < i; ++j) {
      for (size_t k = j; k < i; ++k) {
        inverse_l[i * n + j] -= this->l_[i * n + k] * inverse_l[k * n + j];
        inverse_u[i * n + j] -= transposed_u[i * n + k] * inverse_u[k * n + j];
      }
      inverse_u[i * n + j] /= transposed_u[i * (n + 1)];
    }
  }

  std::array<T, m * n> inversed;
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < m; ++j) {
      inversed[i * m + j] = 0;
      for (size_t l = 0; l < n; ++l) {
        inversed[i * m + j] += inverse_u[l * m + i] * inverse_l[l * m + j];
      }
    }
  }

  return Matrix(inversed);
}

template <class T, size_t m, size_t n>
template <size_t row_start, size_t row_end, size_t col_start, size_t col_end>
Matrix<T, row_end - row_start, col_end - col_start>
Matrix<T, m, n>::Slice() const {
  static_assert(row_start < row_end && col_start < col_end && row_start < m &&
                    col_end <= n,
                "Indices out of range");
  const size_t rows = row_end - row_start;
  const size_t cols = col_end - col_start;
  std::array<T, rows * cols> elements;

  size_t index = 0;
  if (this->is_column_major_) {
    for (size_t j = col_start; j < col_end; ++j) {
      for (size_t i = row_start; i < row_end; ++i) {
        elements[index] = this->matrix_[j * n + i];
        ++index;
      }
    }
  } else {
    for (size_t i = row_start; i < row_end; ++i) {
      for (size_t j = col_start; j < col_end; ++j) {
        elements[index] = this->matrix_[i * n + j];
        ++index;
      }
    }
  }
  return Matrix<T, row_end - row_start, col_end - col_start>(
      elements, this->is_column_major_);
}
