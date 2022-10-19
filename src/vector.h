#include <iostream>

#ifndef VECTOR_H
#define VECTOR_H

class Vector {
private:
    int kSize;
    bool is_column_major_;
    double *mVector;

    friend std::ostream &operator<<(std::ostream &stream, const Vector &obj);
    friend std::istream &operator>>(std::istream &stream, const Vector &obj);
    friend Vector operator*(const double number, Vector &obj);

public:
    Vector(int size, int elem, bool is_column_major = false);
    Vector(const Vector &obj);

    double operator[](const int &index);
    
    Vector& operator+=(const Vector &other);
    Vector& operator-=(const Vector &other);
    Vector& operator*=(const Vector &other);

    Vector operator+(const Vector &other);
    Vector operator-(const Vector &other);
    Vector operator*(const Vector &other);

    Vector& operator+=(const double number);
    Vector& operator-=(const double number);
    Vector& operator*=(const double number);

    Vector operator+(const double number);
    Vector operator-(const double number);
    Vector operator*(const double number);

    Vector slice(int start, int end);
};

#endif