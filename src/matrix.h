#pragma once
#include <array>
#include <iostream>
#include <type_traits>
#include <vector>

template <class T, std::size_t m, std::size_t n> class Matrix;

template <class T, std::size_t m, std::size_t n>
std::ostream &operator<<(std::ostream &stream, const Matrix<T, m, n> &obj);

template <class T, std::size_t m, std::size_t n>
std::istream &operator>>(std::istream &stream, Matrix<T, m, n> &obj);

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> operator*(const T &number, const Matrix<T, m, n> &obj);

template <class T, std::size_t m, std::size_t n> class Matrix {
 private:
  static_assert(std::is_floating_point<T>::value,
                "Wrong type, should be float or double");
  const std::size_t kSize = m * n;
  std::array<T, m * n> matrix_;
  bool is_column_major_ = false;
  std::array<T, m * n> l_;
  std::array<T, m * n> u_;

  friend std::ostream &operator<<<>(std::ostream &stream, const Matrix &obj);
  friend std::istream &operator>><>(std::istream &stream, Matrix &obj);
  friend Matrix operator*<>(const T &number, const Matrix &obj);

 public:
  explicit Matrix(T elem, bool is_column_major = false);
  explicit Matrix(const std::array<T, m * n> &arr,
                  bool is_column_major = false);
  Matrix(const std::vector<Matrix<T, 1, n>> &arr, std::size_t arr_size);

  template <bool condition = (m != 1 && n != 1),
            typename std::enable_if_t<condition, bool> = true>
  Matrix(const std::vector<Matrix<T, m, 1>> &arr, std::size_t arr_size);
  Matrix(const Matrix<T, m, n> &obj);

  bool getColumnMajor() const;
  std::array<T, m * n> getMatrix() const;
  Matrix &toColumnMajor();
  Matrix &toRowMajor();
  Matrix &swap(Matrix &other);

  T operator()(const std::size_t row_index, const std::size_t column_index);
  Matrix<T, m, 1> getDiag(bool main = true);
  Matrix<T, 1, n> getRow(const std::size_t row_index);
  Matrix<T, m, 1> getCol(const std::size_t column_index);

  Matrix &operator=(const Matrix &other);
  Matrix &operator+=(const Matrix &other);
  Matrix &operator-=(const Matrix &other);
  Matrix &operator*=(const Matrix &other);

  Matrix operator+(const Matrix &other);
  Matrix operator-(const Matrix &other);
  Matrix operator*(const Matrix &other);

  Matrix &operator+=(const T &number);
  Matrix &operator-=(const T &number);
  Matrix &operator*=(const T &number);

  Matrix operator+(const T &number);
  Matrix operator-(const T &number);
  Matrix operator*(const T &number);

  Matrix &translateRow(const Matrix<T, 1, n> &other, bool positive = true);
  Matrix &translateColumn(const Matrix<T, m, 1> &other, bool positive = true);

  template <std::size_t k> Matrix<T, m, k> matmul(const Matrix<T, n, k> &other);
  Matrix<T, n, m> transpose();

  void LRDecomposition();
  T det();
  Matrix inverse();

  template <std::size_t row_start, std::size_t row_end, std::size_t col_start,
            std::size_t col_end>
  Matrix<T, row_end - row_start, col_end - col_start> slice();
};

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n>::Matrix(T elem, bool is_column_major) {
  for (std::size_t i = 0; i < this->kSize; ++i) {
    this->matrix_[i] = elem;
  }
  this->is_column_major_ = is_column_major;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n>::Matrix(const std::array<T, m * n> &arr, bool is_column_major) {
  this->is_column_major_ = is_column_major;
  this->matrix_ = arr;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n>::Matrix(const std::vector<Matrix<T, 1, n>> &arr,
                        std::size_t arr_size) {
  if (arr_size != m) {
    throw "Size doesn't match";
  }
  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      this->matrix_[i * n + j] = arr[i].getMatrix()[j];
    }
  }
}

template <class T, std::size_t m, std::size_t n>
template <bool condition, typename std::enable_if_t<condition, bool>>
Matrix<T, m, n>::Matrix(const std::vector<Matrix<T, m, 1>> &arr,
                        std::size_t arr_size) {
  if (arr_size != n) {
    throw "Size doesn't match";
  }
  this->is_column_major_ = true;
  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      this->matrix_[j * m + i] = arr[j].getMatrix()[i];
    }
  }
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n>::Matrix(const Matrix &obj) {
  this->is_column_major_ = obj.is_column_major_;
  for (std::size_t i = 0; i < this->kSize; ++i) {
    this->matrix_[i] = obj.matrix_[i];
  }
}

template <class T, std::size_t m, std::size_t n>
bool Matrix<T, m, n>::getColumnMajor() const {
  return this->is_column_major_;
}

template <class T, std::size_t m, std::size_t n>
std::array<T, m * n> Matrix<T, m, n>::getMatrix() const {
  return this->matrix_;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::toRowMajor() {
  if (!this->is_column_major_)
    return *this;
  std::array<T, m * n> elements;
  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < n; ++j)
      elements[i * n + j] = this->matrix_[j * m + i];
  }
  Matrix<T, m, n> row_major(elements);
  *this = row_major;
  return *this;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::toColumnMajor() {
  if (this->is_column_major_)
    return *this;
  std::array<T, m * n> elements;
  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < n; ++j)
      elements[j * m + i] = this->matrix_[i * n + j];
  }
  Matrix<T, m, n> col_major(elements);
  *this = col_major;
  return *this;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::swap(Matrix &other) {
  std::swap(this->matrix_, other.matrix_);
  std::swap(this->is_column_major_, other.is_column_major_);
  std::swap(this->l_, other.l_);
  std::swap(this->u_, other.u_);
  return *this;
}

template <class T, std::size_t m, std::size_t n>
std::ostream &operator<<(std::ostream &stream, const Matrix<T, m, n> &obj) {
  if (obj.is_column_major_) {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        std::cout << obj.matrix_[j * m + i] << ' ';
      }
      std::cout << '\n';
    }
  } else {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        std::cout << obj.matrix_[i * n + j] << ' ';
      }
      std::cout << '\n';
    }
  }
  return stream;
}

template <class T, std::size_t m, std::size_t n>
std::istream &operator>>(std::istream &stream, Matrix<T, m, n> &obj) {
  if (obj.is_column_major_) {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        std::cin >> obj.matrix_[j * m + i];
      }
    }
  } else {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        std::cin >> obj.matrix_[i * n + j];
      }
    }
  }
  return stream;
}

template <class T, std::size_t m, std::size_t n>
T Matrix<T, m, n>::operator()(const std::size_t row_index,
                              const std::size_t column_index) {
  if (row_index >= m || column_index >= n) {
    throw "Index out of range";
  }
  if (this->is_column_major_) {
    return this->matrix_[column_index * m + row_index];
  } else {
    return this->matrix_[row_index * n + column_index];
  }
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, 1> Matrix<T, m, n>::getDiag(bool main) {
  static_assert(m == n, "Matrix is not square");
  std::array<T, m> elements;
  if (main) {
    for (std::size_t i = 0; i < m; ++i) {
      elements[i] = this->matrix_[i * m + i];
    }
  } else {
    if (this->is_column_major_) {
      for (std::size_t i = 0; i < m; ++i) {
        elements[i] = this->matrix_[(m - i) * (m - 1)];
      }
    } else {
      for (std::size_t i = 0; i < m; ++i) {
        elements[i] = this->matrix_[(i + 1) * (m - 1)];
      }
    }
  }
  Matrix<T, m, 1> diag(elements, m);
  return diag;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, 1, n> Matrix<T, m, n>::getRow(const std::size_t row_index) {
  std::array<T, n> elements;
  if (this->is_column_major_) {
    for (std::size_t i = 0; i < n; ++i) {
      elements[i] = this->matrix_[i * m + row_index];
    }
  } else {
    for (std::size_t i = 0; i < n; ++i) {
      elements[i] = this->matrix_[row_index * n + i];
    }
  }
  Matrix<T, 1, n> row(elements, n);
  return row;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, 1> Matrix<T, m, n>::getCol(const std::size_t column_index) {
  std::array<T, m> elements;
  if (this->is_column_major_) {
    for (std::size_t i = 0; i < m; ++i) {
      elements[i] = this->matrix_[column_index * m + i];
    }
  } else {
    for (std::size_t i = 0; i < m; ++i) {
      elements[i] = this->matrix_[i * n + column_index];
    }
  }
  Matrix<T, m, 1> column(elements, m);
  return column;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::operator=(const Matrix &other) {
  if (this != &other) {
    Matrix(other).swap(*this);
  }
  return *this;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::operator+=(const Matrix &other) {
  if (this->is_column_major_ == other.is_column_major_) {
    for (std::size_t i = 0; i < this->kSize; ++i) {
      this->matrix_[i] += other.matrix_[i];
    }
  } else if (this->is_column_major_) {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        this->matrix_[j * m + i] += other.matrix_[i * n + j];
      }
    }
  } else {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        this->matrix_[i * n + j] += other.matrix_[j * m + i];
      }
    }
  }
  return *this;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::operator-=(const Matrix &other) {
  if (this->is_column_major_ == other.is_column_major_) {
    for (std::size_t i = 0; i < this->kSize; ++i) {
      this->matrix_[i] -= other.matrix_[i];
    }
  } else if (this->is_column_major_) {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        this->matrix_[j * m + i] -= other.matrix_[i * n + j];
      }
    }
  } else {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        this->matrix_[i * n + j] -= other.matrix_[j * m + i];
      }
    }
  }
  return *this;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::operator*=(const Matrix &other) {
  if (this->is_column_major_ == other.is_column_major_) {
    for (std::size_t i = 0; i < this->kSize; ++i) {
      this->matrix_[i] += other.matrix_[i];
    }
  } else if (this->is_column_major_) {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        this->matrix_[j * m + i] *= other.matrix_[i * n + j];
      }
    }
  } else {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        this->matrix_[i * n + j] *= other.matrix_[j * m + i];
      }
    }
  }
  return *this;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> Matrix<T, m, n>::operator+(const Matrix &other) {
  Matrix sum_matrix(*this);
  if (this->is_column_major_ == other.is_column_major_) {
    for (std::size_t i = 0; i < this->kSize; ++i) {
      sum_matrix.matrix_[i] += other.matrix_[i];
    }
  } else if (this->is_column_major_) {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        sum_matrix.matrix_[j * m + i] += other.matrix_[i * n + j];
      }
    }
  } else {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        sum_matrix.matrix_[i * n + j] += other.matrix_[j * m + i];
      }
    }
  }
  return sum_matrix;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> Matrix<T, m, n>::operator-(const Matrix &other) {
  Matrix sub_matrix(*this);
  if (this->is_column_major_ == other.is_column_major_) {
    for (std::size_t i = 0; i < this->kSize; ++i) {
      sub_matrix.matrix_[i] -= other.matrix_[i];
    }
  } else if (this->is_column_major_) {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        sub_matrix.matrix_[j * m + i] -= other.matrix_[i * n + j];
      }
    }
  } else {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        sub_matrix.matrix_[i * n + j] -= other.matrix_[j * m + i];
      }
    }
  }
  return sub_matrix;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> Matrix<T, m, n>::operator*(const Matrix &other) {
  Matrix mult_matrix(*this);
  if (this->is_column_major_ == other.is_column_major_) {
    for (std::size_t i = 0; i < this->kSize; ++i) {
      mult_matrix.matrix_[i] *= other.matrix_[i];
    }
  } else if (this->is_column_major_) {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        mult_matrix.matrix_[j * m + i] *= other.matrix_[i * n + j];
      }
    }
  } else {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        mult_matrix.matrix_[i * n + j] *= other.matrix_[j * m + i];
      }
    }
  }
  return mult_matrix;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::operator+=(const T &number) {
  for (std::size_t i = 0; i < this->kSize; ++i) {
    this->matrix_[i] += number;
  }
  return *this;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::operator-=(const T &number) {
  for (std::size_t i = 0; i < this->kSize; ++i) {
    this->matrix_[i] -= number;
  }
  return *this;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::operator*=(const T &number) {
  for (std::size_t i = 0; i < this->kSize; ++i) {
    this->matrix_[i] *= number;
  }
  return *this;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> Matrix<T, m, n>::operator+(const T &number) {
  Matrix sum_matrix(*this);
  for (std::size_t i = 0; i < this->kSize; ++i) {
    sum_matrix.matrix_[i] += number;
  }
  return sum_matrix;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> Matrix<T, m, n>::operator-(const T &number) {
  Matrix sub_matrix(*this);
  for (std::size_t i = 0; i < this->kSize; ++i) {
    sub_matrix.matrix_[i] -= number;
  }
  return sub_matrix;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> Matrix<T, m, n>::operator*(const T &number) {
  Matrix mult_matrix(*this);
  for (std::size_t i = 0; i < this->kSize; ++i) {
    mult_matrix.matrix_[i] *= number;
  }
  return mult_matrix;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> operator*(const T &number, const Matrix<T, m, n> &obj) {
  return obj * number;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::translateRow(const Matrix<T, 1, n> &other,
                                               bool positive) {
  if (!this->is_column_major_) {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        if (positive) {
          this->matrix_[i * n + j] += other.getMatrix()[j];
        } else {
          this->matrix_[i * n + j] -= other.getMatrix()[j];
        }
      }
    }
  } else {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        if (positive) {
          this->matrix_[j * m + i] += other.getMatrix()[j];
        } else {
          this->matrix_[j * m + i] -= other.getMatrix()[j];
        }
      }
    }
  }
  return *this;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> &Matrix<T, m, n>::translateColumn(const Matrix<T, m, 1> &other,
                                                  bool positive) {
  if (this->is_column_major_) {
    for (std::size_t j = 0; j < n; ++j) {
      for (std::size_t i = 0; i < m; ++i) {
        if (positive) {
          this->matrix_[j * m + i] += other.getMatrix()[i];
        } else {
          this->matrix_[j * m + i] -= other.getMatrix()[i];
        }
      }
    }
  } else {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j) {
        if (positive) {
          this->matrix_[i * n + j] += other.getMatrix()[i];
        } else {
          this->matrix_[i * n + j] -= other.getMatrix()[i];
        }
      }
    }
  }
  return *this;
}

template <class T, std::size_t m, std::size_t n>
template <std::size_t k>
Matrix<T, m, k> Matrix<T, m, n>::matmul(const Matrix<T, n, k> &other) {
  std::array<T, m * k> elements;
  if (!this->is_column_major_ && other.getColumnMajor()) {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < k; ++j) {
        elements[i * k + j] = 0;
        for (std::size_t l = 0; l < n; ++l) {
          elements[i * k + j] +=
              this->matrix_[i * n + l] * other.getMatrix()[j * n + l];
        }
      }
    }
  }
  if (this->is_column_major_ && other.getColumnMajor()) {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < k; ++j) {
        elements[i * k + j] = 0;
        for (std::size_t l = 0; l < n; ++l) {
          elements[i * k + j] +=
              this->matrix_[l * m + i] * other.getMatrix()[j * n + l];
        }
      }
    }
  }
  if (!this->is_column_major_ && !other.getColumnMajor()) {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < k; ++j) {
        elements[i * k + j] = 0;
        for (std::size_t l = 0; l < n; ++l) {
          elements[i * k + j] +=
              this->matrix_[i * n + l] * other.getMatrix()[l * k + j];
        }
      }
    }
  }
  if (this->is_column_major_ && !other.getColumnMajor()) {
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < k; ++j) {
        elements[i * k + j] = 0;
        for (std::size_t l = 0; l < n; ++l) {
          elements[i * k + j] +=
              this->matrix_[l * m + i] * other.getMatrix()[l * k + j];
        }
      }
    }
  }
  Matrix<T, m, k> mult(elements, m * k);
  return mult;
}

template <class T, std::size_t m, std::size_t n>
void Matrix<T, m, n>::LRDecomposition() {
  static_assert(m == n, "Matrix is not square");

  for (std::size_t i = 0; i < this->kSize; ++i) {
    this->l_[i] = 0;
    this->u_[i] = 0;
  }

  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t k = i; k < m; ++k) {
      T sum = 0;
      for (std::size_t j = 0; j < i; ++j) {
        sum += (this->l_[i * n + j] * this->u_[j * n + k]);
      }
      if (!this->is_column_major_) {
        this->u_[i * n + k] = this->matrix_[i * n + k] - sum;
      } else {
        this->u_[i * n + k] = this->matrix_[k * m + i] - sum;
      }
    }
    for (std::size_t k = i; k < n; ++k) {
      if (i == k) {
        this->l_[i * (n + 1)] = 1;
      } else {
        T sum = 0;
        for (std::size_t j = 0; j < i; ++j) {
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
  // for (std::size_t i = 0; i < m * n; ++i) {
  //     std::cout << this->l_[i] <<' ';
  // }
  // for (std::size_t i = 0; i < m * n; ++i) {
  //     std::cout << this->u_[i] <<' ';
  // }
}

template <class T, std::size_t m, std::size_t n> T Matrix<T, m, n>::det() {
  static_assert(m == n, "Matrix is not square");
  this->LRDecomposition();

  T det = 1;
  for (std::size_t i = 0; i < m; ++i) {
    det *= this->u_[i * m + i];
  }
  return det;
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, n, m> Matrix<T, m, n>::transpose() {
  if (this->is_column_major_) {
    Matrix<T, n, m> transposed(this->matrix_);
    return transposed;
  } else {
    std::array<T, m * n> elements;
    for (std::size_t i = 0; i < m; ++i) {
      for (std::size_t j = 0; j < n; ++j)
        elements[j * m + i] = this->matrix_[i * n + j];
    }
    Matrix<T, n, m> transposed(elements);
    return transposed;
  }
}

template <class T, std::size_t m, std::size_t n>
Matrix<T, m, n> Matrix<T, m, n>::inverse() {
  static_assert(m == n, "Matrix is not square");

  if (this->det() == 0) {
    throw "Singular Matrix";
  }

  std::array<T, m * n> inverse_l;
  std::array<T, m * n> inverse_u;
  for (std::size_t i = 0; i < this->kSize; ++i) {
    inverse_l[i] = 0;
    inverse_u[i] = 0;
  }

  std::array<T, m * n> transposed_u;
  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < n; ++j)
      transposed_u[j * m + i] = this->u_[i * n + j];
  }

  for (std::size_t i = 0; i < m; ++i) {
    inverse_l[i * (n + 1)] = this->l_[i * (n + 1)];
    inverse_u[i * (n + 1)] = 1.0 / transposed_u[i * (n + 1)];
    for (std::size_t j = 0; j < i; ++j) {
      for (std::size_t k = j; k < i; ++k) {
        inverse_l[i * n + j] -= this->l_[i * n + k] * inverse_l[k * n + j];
        inverse_u[i * n + j] -= transposed_u[i * n + k] * inverse_u[k * n + j];
      }
      inverse_u[i * n + j] /= transposed_u[i * (n + 1)];
    }
  }

  std::array<T, m * n> inversed;
  for (std::size_t i = 0; i < m; ++i) {
    for (std::size_t j = 0; j < m; ++j) {
      inversed[i * m + j] = 0;
      for (std::size_t l = 0; l < n; ++l) {
        inversed[i * m + j] += inverse_u[l * m + i] * inverse_l[l * m + j];
      }
    }
  }

  return Matrix(inversed);
}

template <class T, std::size_t m, std::size_t n>
template <std::size_t row_start, std::size_t row_end, std::size_t col_start,
          std::size_t col_end>
Matrix<T, row_end - row_start, col_end - col_start> Matrix<T, m, n>::slice() {
  static_assert(row_start < row_end && col_start < col_end && row_start < m &&
                    col_end <= n,
                "Indices out of range");
  const std::size_t rows = row_end - row_start;
  const std::size_t cols = col_end - col_start;
  std::array<T, rows * cols> elements;

  std::size_t index = 0;
  if (this->is_column_major_) {
    for (std::size_t j = col_start; j < col_end; ++j) {
      for (std::size_t i = row_start; i < row_end; ++i) {
        elements[index] = this->matrix_[j * n + i];
        ++index;
      }
    }
  } else {
    for (std::size_t i = row_start; i < row_end; ++i) {
      for (std::size_t j = col_start; j < col_end; ++j) {
        elements[index] = this->matrix_[i * n + j];
        ++index;
      }
    }
  }
  return Matrix<T, row_end - row_start, col_end - col_start>(
      elements, this->is_column_major_);
}
