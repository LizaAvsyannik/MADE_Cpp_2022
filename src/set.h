#pragma once
#include <initializer_list>

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

  void insert(const T &element);
  void erase(const T &element);

  size_t size() const;
  bool empty() const;

  void print();

  class SetIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
   private:
    friend class Set;
    friend class RedBlackTree<T>;
    ConstNodePtr<T> node_ptr_;
    const Set<T> *set_;

    SetIterator(ConstNodePtr<T> node_ptr, const Set<T> *set) {
      node_ptr_ = node_ptr;
      set_ = set;
    }

   public:
    SetIterator() = default;
    bool operator==(const SetIterator &other) const {
      return (set_ == other.set_) && (node_ptr_ == other.node_ptr_);
    }
    bool operator!=(const SetIterator &other) const {
      return (set_ != other.set_) || (node_ptr_ != other.node_ptr_);
    }
    const T &operator*() const { return node_ptr_->data; }
    const T *operator->() const { return &node_ptr_->data; }
    SetIterator &operator++();
    SetIterator operator++(int);
    SetIterator &operator--();
    SetIterator operator--(int);
  };

  typedef SetIterator const_iterator;
  const_iterator begin() const;
  const_iterator end() const;
  const_iterator find(const T &element) const;
  const_iterator lower_bound(const T &element) const;
};

template <class T>
Set<T>::Set(std::initializer_list<T> list) {
  for (const T &element : list) {
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
void Set<T>::insert(const T &element) {
  rbtree_.Insert(element);
}

template <class T>
void Set<T>::erase(const T &element) {
  rbtree_.Erase(element);
}

template <class T>
size_t Set<T>::size() const {
  return rbtree_.Size();
}

template <class T>
bool Set<T>::empty() const {
  return rbtree_.Size() == 0;
}

template <class T>
void Set<T>::print() {
  rbtree_.printTree();
}

template <class T>
typename Set<T>::const_iterator Set<T>::begin() const {
  return SetIterator(rbtree_.GetMin(), this);
}

template <class T>
typename Set<T>::const_iterator Set<T>::end() const {
  if (empty()) {
    return SetIterator(rbtree_.GetMax(), this);
  }
  return SetIterator(rbtree_.GetMax()->right, this);
}

template <class T>
typename Set<T>::const_iterator Set<T>::find(const T &element) const {
  return SetIterator(rbtree_.Find(element), this);
}

template <class T>
typename Set<T>::const_iterator Set<T>::lower_bound(const T &element) const {
  return SetIterator(rbtree_.LowerBound(element), this);
}

template <class T>
typename Set<T>::SetIterator &Set<T>::SetIterator::operator++() {
  node_ptr_ = set_->rbtree_.Successor(node_ptr_);
  return *this;
}

template <class T>
typename Set<T>::SetIterator Set<T>::SetIterator::operator++(int) {
  SetIterator temp(node_ptr_, set_);
  node_ptr_ = set_->rbtree_.Successor(node_ptr_);
  return temp;
}

template <class T>
typename Set<T>::SetIterator &Set<T>::SetIterator::operator--() {
  node_ptr_ = set_->rbtree_.Predecessor(node_ptr_);
  return *this;
}

template <class T>
typename Set<T>::SetIterator Set<T>::SetIterator::operator--(int) {
  SetIterator temp(node_ptr_, set_);
  node_ptr_ = set_->rbtree_.Predecessor(node_ptr_);
  return temp;
}
