#pragma once
#include <initializer_list>

#include "iterator.h"
#include "rbtree.h"

template <class T>
class Set {
 private:
  RedBlackTree<T> rbtree_;

 public:
  Set() = default;
  Set(std::initializer_list<T> list);
  Set(const Set &other);
  ~Set() = default;

  Set &operator=(const Set &other);
  Set &Swap(Set &other);

  void Insert(T element);
  void Erase(T element);

  size_t Size() const;
  bool Empty() const;

  void print();
  // typedef SetIterator<const T> const_iterator;
  // const_iterator begin() const;
  // const_iterator end() const;
};

template <class T>
Set<T>::Set(std::initializer_list<T> list) {
  for (T element : list) {
    rbtree_.Insert(element);
  }
}

template <class T>
Set<T>::Set(const Set &other) {
  rbtree_.Copy(other.rbtree_);
}


template <class T>
Set<T> &Set<T>::operator=(const Set &other) {
  if (this != &other) {
    Set(other).Swap(*this);
  }
  return *this;
}

template <class T>
Set<T> &Set<T>::Swap(Set &other) {
  rbtree_.Swap(other.rbtree_);
  return *this;
}

template <class T>
void Set<T>::Insert(T element) {
  rbtree_.Insert(element);
}

template <class T>
void Set<T>::Erase(T element) {
  rbtree_.Erase(element);
}

template <class T>
size_t Set<T>::Size() const {
  return rbtree_.Size();
}

template <class T>
bool Set<T>::Empty() const {
  return rbtree_.Size() == 0;
}

template <class T>
void Set<T>::print() {
  rbtree_.printTree();
}
