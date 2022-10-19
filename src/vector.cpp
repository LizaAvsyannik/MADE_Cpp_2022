#include "vector.h"

Vector::Vector(int size, int elem, bool is_column_major) {
    this->mVector = new double[size];
    for (int i = 0; i < size; ++i) {
        this->mVector[i] = elem;
    }
    this->kSize = size;
    this->is_column_major_ =  is_column_major;
}

Vector::Vector(const Vector &obj) {
    this->kSize = obj.kSize;
    this->is_column_major_ = obj.is_column_major_;
    this->mVector = new double[this->kSize];
    for (int i = 0; i < this->kSize; ++i) {
        this->mVector[i] = obj.mVector[i];
    }
}

std::ostream &operator<<(std::ostream &stream, const Vector &obj) {
    char sep = obj.is_column_major_ ? ' ' : '\n';
    for (int i = 0; i < obj.kSize; ++i) {
        stream << obj.mVector[i] << sep;
    }
    return stream;
}

std::istream &operator>>(std::istream &stream, const Vector &obj) {
    for (int i = 0; i < obj.kSize; ++i) {
        stream >> obj.mVector[i];
    }
    return stream;
}

double Vector::operator[](const int &index) {
    if ((index  < 0) || (index >= this->kSize)) {
        throw "Index out of bounds";
    }
    return this->mVector[index];
}

Vector& Vector::operator+=(const Vector &other) {
    if ((this->kSize != other.kSize) || (this->is_column_major_ != other.is_column_major_)) {
        throw "Vector sizes do not match";
    }
    for (int i = 0; i < this->kSize; ++i) {
        this->mVector[i] += other.mVector[i];     
    }
    return *this;
}

Vector& Vector::operator-=(const Vector &other) {
    if ((this->kSize != other.kSize) || (this->is_column_major_ != other.is_column_major_)) {
        throw "Vector sizes do not match";
    }
    for (int i = 0; i < this->kSize; ++i) {
        this->mVector[i] -= other.mVector[i];     
    }
    return *this;
}

Vector& Vector::operator*=(const Vector &other) {
    if ((this->kSize != other.kSize) || (this->is_column_major_ != other.is_column_major_)) {
        throw "Vector sizes do not match";
    }
    for (int i = 0; i < this->kSize; ++i) {
        this->mVector[i] *= other.mVector[i];     
    }
    return *this;
}

Vector Vector::operator+(const Vector &other) {
    if ((this->kSize != other.kSize) || (this->is_column_major_ != other.is_column_major_)) {
        throw "Vector sizes do not match";
    }
    Vector sum_vector = Vector(*this);
    for (int i = 0; i < this->kSize; ++i) {
        sum_vector.mVector[i] += other.mVector[i];  
    }
    return sum_vector;
}

Vector Vector::operator-(const Vector &other) {
    if ((this->kSize != other.kSize) || (this->is_column_major_ != other.is_column_major_)) {
        throw "Vector sizes do not match";
    }
    Vector sub_vector = Vector(*this);
    for (int i = 0; i < this->kSize; ++i) {
        sub_vector.mVector[i] -=other.mVector[i];  
    }
    return sub_vector;
}

Vector Vector::operator*(const Vector &other) {
    if ((this->kSize != other.kSize) || (this->is_column_major_ != other.is_column_major_)) {
        throw "Vector sizes do not match";
    }
    Vector mult_vector = Vector(*this);
    for (int i = 0; i < this->kSize; ++i) {
        mult_vector.mVector[i] *= other.mVector[i];
    }
    return mult_vector;
}

Vector& Vector::operator+=(const double number) {
    for (int i = 0; i < this->kSize; ++i) {
       this->mVector[i] += number;  
    }
    return *this;
}

Vector& Vector::operator-=(const double number) {
    for (int i = 0; i < this->kSize; ++i) {
       this->mVector[i] -= number;  
    }
    return *this;
}

Vector& Vector::operator*=(const double number) {
    for (int i = 0; i < this->kSize; ++i) {
       this->mVector[i] *= number;  
    }
    return *this;
}

Vector Vector::operator+(const double number) {
    Vector sum_vector = Vector(*this);
    for (int i = 0; i < this->kSize; ++i) {
       sum_vector.mVector[i] = this->mVector[i] + number;  
    }
    return sum_vector;
}

Vector Vector::operator-(const double number) {
    Vector sub_vector = Vector(*this);
    for (int i = 0; i < this->kSize; ++i) {
       sub_vector.mVector[i] = this->mVector[i] - number;
    }
    return sub_vector;
}

Vector Vector::operator*(const double number) {
    Vector mult_vector = Vector(*this);
    for (int i = 0; i < this->kSize; ++i) {
       mult_vector.mVector[i] = this->mVector[i] * number;  
    }
    return mult_vector;
}

Vector operator*(const double number, Vector &obj) {
    return obj * number;
}

Vector Vector::slice(int start, int end) {
    std::cout << start << ' ' << end << '\n';

    start = start > this->kSize ? this->kSize : start;
    start = start < -this->kSize ? -this->kSize : start;
    start = start == this->kSize ? start : start % this->kSize;
    start = start < 0 ? -start + 1: start;

    end = end >= this->kSize ? this->kSize : end;
    end = end < -this->kSize ? -this->kSize : end;
    end = end == this->kSize ? end : end % this->kSize;
    end = end < 0 ? -end + 1 : end;

    std::cout << start << ' ' << end << '\n';
    if (start >= end) {
        return Vector(0, 0);
    } else {
        Vector sliced = Vector(end - start, 0, this->is_column_major_);
        for (int i = start; i < end; ++i) {
            sliced.mVector[i - start] = this->mVector[i];
        }
        return sliced;
    }
}
