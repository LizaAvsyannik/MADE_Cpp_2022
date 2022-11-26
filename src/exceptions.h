#pragma once
#include <stdexcept>

class SingularMatrixException : public std::exception {
 public:
  const char *what() const throw() { return "Matrix is singular"; }
};
