#pragma once
#include <iostream>
#include <memory>
#include <stdio.h>

enum class Color { black, red };

template <class T>
struct Node {
  T data;
  std::weak_ptr<Node> parent;
  std::shared_ptr<Node> left;
  std::shared_ptr<Node> right;
  Color color;
};

template <class T>
using NodePtr = std::shared_ptr<Node<T>>;
template <class T>
using ConstNodePtr = std::shared_ptr<const Node<T>>;

template <class T>
class RedBlackTree {
 private:
  NodePtr<T> root_;
  NodePtr<T> NIL;
  NodePtr<T> min_node_;
  NodePtr<T> max_node_;
  size_t size_ = 0;

  void EraseNode(NodePtr<T> node, const T &element);
  void FixInsert(NodePtr<T> node);
  void FixErase(NodePtr<T> node);

  void RotateLeft(NodePtr<T> node);
  void RotateRight(NodePtr<T> node);
  void RBTransplant(NodePtr<T> node, NodePtr<T> v);

  NodePtr<T> CopyNodes(const NodePtr<T> &node, const NodePtr<T> &new_parent);
  NodePtr<T> Min(NodePtr<T> node) const;
  NodePtr<T> Max(NodePtr<T> node) const;
  ConstNodePtr<T> FindElement(ConstNodePtr<T> node, const T &element) const;
  ConstNodePtr<T> FindLowerBound(ConstNodePtr<T> node, const T &element) const;

  void printHelper(NodePtr<T> root, std::string indent, bool last) {
    if (root != NIL) {
      std::cout << indent;
      if (last) {
        std::cout << "R----";
        indent += "   ";
      } else {
        std::cout << "L----";
        indent += "|  ";
      }

      std::string sColor = bool(root->color) ? "RED" : "BLACK";
      std::cout << '(' << root->data << ')' << "(" << sColor << ")"
                << std::endl;
      printHelper(root->left, indent, false);
      printHelper(root->right, indent, true);
    }
  }

  void inOrderHelper(NodePtr<T> node) const {
    if (node != NIL) {
      inOrderHelper(node->left);
      std::cout << node->data << " ";
      inOrderHelper(node->right);
    }
  }

 public:
  RedBlackTree();
  ~RedBlackTree() = default;

  void Insert(const T &element);
  void Erase(const T &element);

  size_t Size() const;

  void Copy(const RedBlackTree &other);
  RedBlackTree &Swap(RedBlackTree &other);

  NodePtr<T> GetMin() const;
  NodePtr<T> GetMax() const;
  ConstNodePtr<T> Find(const T &element) const;
  ConstNodePtr<T> LowerBound(const T &element) const;
  ConstNodePtr<T> Successor(ConstNodePtr<T> node) const;
  ConstNodePtr<T> Predecessor(ConstNodePtr<T> node) const;

  void printTree() {
    if (root_) {
      printHelper(root_, "", true);
    }
  }
};

template <class T>
RedBlackTree<T>::RedBlackTree() {
  NIL = std::make_shared<Node<T>>();
  NIL->color = Color::black;
  NIL->left = nullptr;
  NIL->right = nullptr;
  root_ = NIL;
  min_node_ = root_;
  max_node_ = root_;
};

template <class T>
void RedBlackTree<T>::Insert(const T &element) {
  NodePtr<T> node = std::make_shared<Node<T>>();
  node->data = element;
  node->left = NIL;
  node->right = NIL;
  node->color = Color::red;
  ++size_;

  if (size_ == 1) {
    min_node_ = node;
    max_node_ = node;
  } else {
    if (node->data < min_node_->data) {
      min_node_ = node;
    } else if (max_node_->data < node->data) {
      max_node_ = node;
    }
  }

  NodePtr<T> y = nullptr;
  NodePtr<T> x(root_);

  while (x != NIL) {
    y = x;
    if (node->data < x->data) {
      x = x->left;
    } else if (!(node->data < x->data) && !(x->data < node->data)) {
      --size_;
      return;
    } else {
      x = x->right;
    }
  }

  node->parent = y;
  if (y == nullptr) {
    root_ = node;
  } else if (node->data < y->data) {
    y->left = node;
  } else {
    y->right = node;
  }

  NodePtr<T> parent = node->parent.lock();
  if (parent == nullptr) {
    node->color = Color::black;
    return;
  }

  NodePtr<T> grand_parent = parent->parent.lock();
  if (grand_parent == nullptr) {
    return;
  }

  FixInsert(node);
}

template <class T>
void RedBlackTree<T>::FixInsert(NodePtr<T> node) {
  NodePtr<T> u;
  NodePtr<T> parent = node->parent.lock();
  while (parent->color == Color::red) {
    NodePtr<T> grand_parent = parent->parent.lock();
    if (parent == grand_parent->right) {
      u = grand_parent->left;
      if (u->color == Color::red) {
        u->color = Color::black;
        parent->color = Color::black;
        grand_parent->color = Color::red;
        node = grand_parent;
      } else {
        if (node == parent->left) {
          node = parent;
          RotateRight(node);
        }
        parent->color = Color::black;
        grand_parent->color = Color::red;
        RotateLeft(grand_parent);
      }
    } else {
      u = grand_parent->right;

      if (u->color == Color::red) {
        u->color = Color::black;
        parent->color = Color::black;
        grand_parent->color = Color::red;
        node = grand_parent;
      } else {
        if (node == parent->right) {
          node = parent;
          RotateLeft(node);
        }
        parent->color = Color::black;
        grand_parent->color = Color::red;
        RotateRight(grand_parent);
      }
    }
    if (node == root_) {
      break;
    }
  }
  root_->color = Color::black;
}

template <class T>
void RedBlackTree<T>::RotateLeft(NodePtr<T> node) {
  NodePtr<T> y = node->right;
  node->right = y->left;
  if (y->left != NIL) {
    y->left->parent = node;
  }
  y->parent = node->parent;

  NodePtr<T> parent = node->parent.lock();
  if (parent == nullptr) {
    root_ = y;
  } else if (node == parent->left) {
    parent->left = y;
  } else {
    parent->right = y;
  }
  y->left = node;
  node->parent = y;
}

template <class T>
void RedBlackTree<T>::RotateRight(NodePtr<T> node) {
  NodePtr<T> y = node->left;
  node->left = y->right;
  if (y->right != NIL) {
    y->right->parent = node;
  }
  y->parent = node->parent;

  NodePtr<T> parent = node->parent.lock();
  if (parent == nullptr) {
    root_ = y;
  } else if (node == parent->right) {
    parent->right = y;
  } else {
    parent->left = y;
  }
  y->right = node;
  node->parent = y;
}

template <class T>
void RedBlackTree<T>::Erase(const T &element) {
  EraseNode(root_, element);
}

template <class T>
void RedBlackTree<T>::EraseNode(NodePtr<T> node, const T &element) {
  NodePtr<T> z(NIL);
  NodePtr<T> x, y;
  while (node != NIL) {
    if (!(node->data < element) && !(element < node->data)) {
      --size_;
      if (size_ == 0) {
        min_node_ = NIL;
        max_node_ = NIL;
      } else {
        if ((!(min_node_->data < element) && !(element < min_node_->data))) {
          if (min_node_ == root_) {
            min_node_ = root_->right;
          } else {
            min_node_ = min_node_->parent.lock();
          }
        } else if ((!(max_node_->data < element) &&
                    !(element < max_node_->data))) {
          if (max_node_ == root_) {
            max_node_ = root_->left;
          } else {
            max_node_ = max_node_->parent.lock();
          }
        }
      }
      z = node;
    }

    if (!(element < node->data)) {
      node = node->right;
    } else {
      node = node->left;
    }
  }

  if (z == NIL) {
    return;
  }

  y = z;
  Color y_original_color = y->color;
  if (z->left == NIL) {
    x = z->right;
    RBTransplant(z, z->right);
  } else if (z->right == NIL) {
    x = z->left;
    RBTransplant(z, z->left);
  } else {
    y = Min(z->right);
    y_original_color = y->color;
    x = y->right;

    NodePtr<T> y_parent = y->parent.lock();
    if (y_parent == z) {
      x->parent = y;
    } else {
      RBTransplant(y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }

    RBTransplant(z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_original_color == Color::black) {
    FixErase(x);
  }
}

template <class T>
NodePtr<T> RedBlackTree<T>::Min(NodePtr<T> node) const {
  if (node == NIL) {
    return NIL;
  }
  while (node->left != NIL) {
    node = node->left;
  }
  return node;
}

template <class T>
NodePtr<T> RedBlackTree<T>::Max(NodePtr<T> node) const {
  if (node == NIL) {
    return NIL;
  }
  while (node->right != NIL) {
    node = node->right;
  }
  return node;
}

template <class T>
void RedBlackTree<T>::FixErase(NodePtr<T> node) {
  NodePtr<T> s;
  while (node != root_ && node->color == Color::black) {

    NodePtr<T> parent = node->parent.lock();
    if (node == parent->left) {
      s = parent->right;
      if (s->color == Color::red) {
        s->color = Color::black;
        parent->color = Color::red;
        RotateLeft(parent);
        s = parent->right;
      }
      if (s->left->color == Color::black && s->right->color == Color::black) {
        s->color = Color::red;
        node = parent;
      } else {
        if (s->right->color == Color::black) {
          s->left->color = Color::black;
          s->color = Color::red;
          RotateRight(s);
          s = parent->right;
        }

        s->color = parent->color;
        parent->color = Color::black;
        s->right->color = Color::black;
        RotateLeft(parent);
        node = root_;
      }
    } else {
      s = parent->left;
      if (s->color == Color::red) {
        s->color = Color::black;
        parent->color = Color::red;
        RotateRight(parent);
        s = parent->left;
      }

      if (s->right->color == Color::black && s->right->color == Color::black) {
        s->color = Color::red;
        node = parent;
      } else {
        if (s->left->color == Color::black) {
          s->right->color = Color::black;
          s->color = Color::red;
          RotateLeft(s);
          s = parent->left;
        }

        s->color = parent->color;
        parent->color = Color::black;
        s->left->color = Color::black;
        RotateRight(parent);
        node = root_;
      }
    }
  }
  node->color = Color::black;
}

template <class T>
void RedBlackTree<T>::RBTransplant(NodePtr<T> u, NodePtr<T> v) {
  NodePtr<T> u_parent = u->parent.lock();
  if (u_parent == nullptr) {
    root_ = v;
  } else if (u == u_parent->left) {
    u_parent->left = v;
  } else {
    u_parent->right = v;
  }
  v->parent = u->parent;
}

template <class T>
size_t RedBlackTree<T>::Size() const {
  return size_;
}

template <class T>
void RedBlackTree<T>::Copy(const RedBlackTree &other) {
  size_ = other.size_;
  NIL = other.NIL;
  root_ = CopyNodes(other.root_, other.root_->parent.lock());
  min_node_ = Min(root_);
  max_node_ = Max(root_);
}

template <class T>
NodePtr<T> RedBlackTree<T>::CopyNodes(const NodePtr<T> &node,
                                      const NodePtr<T> &new_parent) {
  if (node == NIL) {
    return NIL;
  } else {
    NodePtr<T> new_node = std::make_shared<Node<T>>();
    new_node->data = node->data;
    new_node->parent = new_parent;
    new_node->color = node->color;
    new_node->left = CopyNodes(node->left, new_node);
    new_node->right = CopyNodes(node->right, new_node);
    return new_node;
  }
}

template <class T>
RedBlackTree<T> &RedBlackTree<T>::Swap(RedBlackTree &other) {
  std::swap(size_, other.size_);
  root_.swap(other.root_);
  min_node_.swap(other.min_node_);
  max_node_.swap(other.max_node_);
  NIL.swap(other.NIL);
  return *this;
}

template <class T>
NodePtr<T> RedBlackTree<T>::GetMin() const {
  return min_node_;
}

template <class T>
NodePtr<T> RedBlackTree<T>::GetMax() const {
  return max_node_;
}

template <class T>
ConstNodePtr<T> RedBlackTree<T>::Find(const T &element) const {
  return FindElement(root_, element);
}

template <class T>
ConstNodePtr<T> RedBlackTree<T>::LowerBound(const T &element) const {
  return FindLowerBound(root_, element);
}

template <class T>
ConstNodePtr<T> RedBlackTree<T>::FindElement(ConstNodePtr<T> node,
                                             const T &element) const {
  if (node == NIL || (!(node->data < element) && !(element < node->data))) {
    return node;
  }
  if (element < node->data) {
    return FindElement(node->left, element);
  }
  return FindElement(node->right, element);
}

template <class T>
ConstNodePtr<T> RedBlackTree<T>::FindLowerBound(ConstNodePtr<T> node,
                                                const T &element) const {
  if (node == NIL) {
    return NIL;
  }
  if (!(min_node_->data < element)) {
    return min_node_;
  } else if (max_node_->data < element) {
    return NIL;
  }
  ConstNodePtr<T> subtree_lower_bound;
  if (!(node->data < element) && !(element < node->data)) {
    return node;
  } else if (element < node->data) {
    subtree_lower_bound = FindLowerBound(node->left, element);
    if (subtree_lower_bound == NIL) {
      return node;
    }
  } else {
    subtree_lower_bound = FindLowerBound(node->right, element);
    if (subtree_lower_bound == NIL) {
      return NIL;
    }
  }
  return subtree_lower_bound;
}

template <class T>
ConstNodePtr<T> RedBlackTree<T>::Successor(ConstNodePtr<T> node) const {
  if (node == max_node_) {
    return NIL;
  } else if (node->right != NIL) {
    return Min(node->right);
  }
  NodePtr<T> y = node->parent.lock();
  while (y != nullptr && node == y->right) {
    node = y;
    y = y->parent.lock();
  }
  return y ? y : NIL;
}

template <class T>
ConstNodePtr<T> RedBlackTree<T>::Predecessor(ConstNodePtr<T> node) const {
  if (node == NIL) {
    return max_node_;
  }
  if (node->left != NIL) {
    return Max(node->left);
  }
  NodePtr<T> y = node->parent.lock();
  while (y != nullptr && node == y->left) {
    node = y;
    y = y->parent.lock();
  }
  return y ? y : NIL;
}
