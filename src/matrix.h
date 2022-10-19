#pragma once
#include <iostream>

template<class T, unsigned m, unsigned n>
class Matrix;

template<class T, unsigned m, unsigned n>
std::ostream &operator<<(std::ostream &stream, const Matrix<T, m, n> &obj);

template<class T, unsigned m, unsigned n>
std::istream &operator>>(std::istream &stream, Matrix<T, m, n> &obj);

template<class T, unsigned m, unsigned n>
Matrix<T, m, n> operator+(const T &number, Matrix<T, m, n> &obj);

template<class T, unsigned m, unsigned n>
Matrix<T, m, n> operator-(const T &number, Matrix<T, m, n> &obj);

template<class T, unsigned m, unsigned n>
Matrix<T, m, n> operator*(const T &number, Matrix<T, m, n> &obj);

template<class T, unsigned m, unsigned n>
class Matrix {
private:
    const unsigned int kSize = m * n;
    T *matrix_;
    bool is_column_major_;

    friend std::ostream &operator<< <>(std::ostream &stream, const Matrix &obj); //+
    friend std::istream &operator>> <>(std::istream &stream, Matrix &obj); //+
    friend Matrix operator* <>(const T &number, Matrix &obj); //+

public:
    Matrix(int elem, bool is_column_major = false); //+
    Matrix(const T arr[], int arr_size, bool is_column_major = false);
    Matrix(const Matrix<T, m, n> &obj); //+

    T operator[](const int &index);
    
    Matrix& operator+=(const Matrix &other); //+
    Matrix& operator-=(const Matrix &other); //+
    Matrix& operator*=(const Matrix &other); //+

    Matrix operator+(const Matrix &other); //+
    Matrix operator-(const Matrix &other); //+
    Matrix operator*(const Matrix &other); //+

    Matrix& operator+=(const T &number); //+
    Matrix& operator-=(const T &number);//+
    Matrix& operator*=(const T &number);//+

    Matrix operator+(const T &number);//+
    Matrix operator-(const T &number);//+
    Matrix operator*(const T &number);//+

    Matrix<T, n, m> transpose();

    Matrix slice(int start, int end);
};

template<class T, unsigned m, unsigned n>
Matrix<T, m, n>::Matrix(int elem, bool is_column_major) {
    this->matrix_ = new T[this->kSize];
    for (int i = 0; i < this->kSize; i++) {
        this->matrix_[i] = elem;
    }
    this->is_column_major_ = is_column_major;
}

template<class T, unsigned m, unsigned n>
Matrix<T, m, n>::Matrix(const T arr[], int arr_size, bool is_column_major) {
    if (arr_size != this->kSize) {
        throw "Sizes do not match";
    }
    this->is_column_major_ = is_column_major;
    this->matrix_ = new T[this->kSize];
    for (int i = 0; i < this->kSize; ++i) {
        this->matrix_[i] = arr[i];
    }
}

template<class T, unsigned m, unsigned n>
Matrix<T, m, n>::Matrix(const Matrix &obj) {
    this->is_column_major_ = obj.is_column_major_;
    this->matrix_ = new T[this->kSize];
    for (int i = 0; i < this->kSize; ++i) {
        this->matrix_[i] = obj.matrix_[i];
    }
}

template<class T, unsigned m, unsigned n>
std::ostream &operator<<(std::ostream &stream, const Matrix<T, m, n> &obj) {
    if (obj.is_column_major_) {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                std::cout << obj.matrix_[j * m + i] << ' ';
            }
            std::cout << '\n';
        }
    } else {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                std::cout << obj.matrix_[i * n + j] << ' ';
            }
            std::cout << '\n';
        }
    }
    return stream;
}

template<class T, unsigned m, unsigned n>
std::istream &operator>>(std::istream &stream, Matrix<T, m, n> &obj) {
    if (obj.is_column_major_) {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                std::cin >> obj.matrix_[j * m + i];
            }
        }
    } else {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                std::cin >> obj.matrix_[i * n + j];
            }
        }
    }
    return stream;
}

template<class T, unsigned m, unsigned n>
Matrix<T, m, n>& Matrix<T, m, n>::operator+=(const Matrix &other) {
    if (this->is_column_major_ == other.is_column_major_) {
        for (int i = 0; i < this->kSize; ++i) {
            this->matrix_[i] += other.matrix_[i];     
        } 
    } else if (this->is_column_major_) {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                this->matrix_[j * m + i] += other.matrix_[i * n + j];
            }
        }
    } else {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                this->matrix_[i * n + j] += other.matrix_[j * m + i];
            }
        }
    }
    return *this;
}

template<class T, unsigned m, unsigned n>
Matrix<T, m, n>& Matrix<T, m, n>::operator-=(const Matrix &other) {
    if (this->is_column_major_ == other.is_column_major_) {
        for (int i = 0; i < this->kSize; ++i) {
            this->matrix_[i] -= other.matrix_[i];     
        } 
    } else if (this->is_column_major_) {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                this->matrix_[j * m + i] -= other.matrix_[i * n + j];
            }
        }
    } else {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                this->matrix_[i * n + j] -= other.matrix_[j * m + i];
            }
        }
    }
    return *this;
}

template<class T, unsigned m, unsigned n>
Matrix<T, m, n>& Matrix<T, m, n>::operator*=(const Matrix &other) {
    if (this->is_column_major_ == other.is_column_major_) {
        for (int i = 0; i < this->kSize; ++i) {
            this->matrix_[i] += other.matrix_[i];     
        } 
    } else if (this->is_column_major_) {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                this->matrix_[j * m + i] *= other.matrix_[i * n + j];
            }
        }
    } else {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                this->matrix_[i * n + j] *= other.matrix_[j * m + i];
            }
        }
    }
    return *this;
}

template<class T, unsigned m, unsigned n>
Matrix<T, m, n> Matrix<T, m, n>::operator+(const Matrix &other) {
    Matrix sum_matrix(*this);
    if (this->is_column_major_ == other.is_column_major_) {
        for (int i = 0; i < this->kSize; ++i) {
            sum_matrix.matrix_[i] += other.matrix_[i];     
        } 
    } else if (this->is_column_major_) {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                sum_matrix.matrix_[j * m + i] += other.matrix_[i * n + j];
            }
        }
    } else {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                sum_matrix.matrix_[i * n + j] += other.matrix_[j * m + i];
            }
        }
    }
    return sum_matrix;
}

template<class T, unsigned m, unsigned n>
Matrix<T, m, n> Matrix<T, m, n>::operator-(const Matrix &other) {
    Matrix sub_matrix(*this);
    if (this->is_column_major_ == other.is_column_major_) {
        for (int i = 0; i < this->kSize; ++i) {
            sub_matrix.matrix_[i] -= other.matrix_[i];     
        } 
    } else if (this->is_column_major_) {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                sub_matrix.matrix_[j * m + i] -= other.matrix_[i * n + j];
            }
        }
    } else {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                sub_matrix.matrix_[i * n + j] -= other.matrix_[j * m + i];
            }
        }
    }
    return sub_matrix;
}

template<class T, unsigned m, unsigned n>
Matrix<T, m, n> Matrix<T, m, n>::operator*(const Matrix &other) {
    Matrix mult_matrix(*this);
    if (this->is_column_major_ == other.is_column_major_) {
        for (int i = 0; i < this->kSize; ++i) {
            mult_matrix.matrix_[i] *= other.matrix_[i];     
        } 
    } else if (this->is_column_major_) {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                mult_matrix.matrix_[j * m + i] *= other.matrix_[i * n + j];
            }
        }
    } else {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                mult_matrix.matrix_[i * n + j] *= other.matrix_[j * m + i];
            }
        }
    }
    return mult_matrix;
}


template<class T, unsigned m, unsigned n>
Matrix<T, m, n>& Matrix<T, m, n>::operator+=(const T &number) {
    for (int i = 0; i < this->kSize; ++i) {
        this->matrix_[i] += number;     
    } 
    return *this;
}

template<class T, unsigned m, unsigned n>
Matrix<T, m, n>& Matrix<T, m, n>::operator-=(const T &number) {
    for (int i = 0; i < this->kSize; ++i) {
        this->matrix_[i] -= number;     
    } 
    return *this;
}

template<class T, unsigned m, unsigned n>
Matrix<T, m, n>& Matrix<T, m, n>::operator*=(const T &number) {
    for (int i = 0; i < this->kSize; ++i) {
        this->matrix_[i] *= number;     
    } 
    return *this;
}

template<class T, unsigned m, unsigned n>
Matrix<T, m, n> Matrix<T, m, n>::operator+(const T &number) {
    Matrix sum_matrix(*this);
    for (int i = 0; i < this->kSize; ++i) {
        sum_matrix.matrix_[i] += number;     
    } 
    return sum_matrix;
}

template<class T, unsigned m, unsigned n>
Matrix<T, m, n> Matrix<T, m, n>::operator-(const T &number) {
    Matrix sub_matrix(*this);
    for (int i = 0; i < this->kSize; ++i) {
        sub_matrix.matrix_[i] -= number;     
    } 
    return sub_matrix;
}

template<class T, unsigned m, unsigned n>
Matrix<T, m, n> Matrix<T, m, n>::operator*(const T &number) {
    Matrix mult_matrix(*this);
    for (int i = 0; i < this->kSize; ++i) {
        mult_matrix.matrix_[i] *= number;     
    } 
    return mult_matrix;
}

template<class T, unsigned m, unsigned n>
Matrix<T, m, n> operator*(const T &number, Matrix<T, m, n> &obj) {
    return obj * number;
}

template<class T, unsigned m, unsigned n>
Matrix<T, n, m> Matrix<T, m, n>::transpose() {
    if (this->is_column_major_) {
        Matrix<T, n, m> transposed(this->matrix_, this->kSize);
        return transposed;
    } else {
        T elements[this->kSize];
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j)
                elements[j * m + i] = this->matrix_[i * n + j];
        }
        Matrix<T, n, m> transposed(elements, this->kSize);
        return transposed;
    }
}
