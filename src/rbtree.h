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
class RedBlackTree {
 private:
  NodePtr<T> root_;
  NodePtr<T> TNULL;
  size_t size_ = 0;

  void EraseNode(NodePtr<T> node, T element);
  void FixInsert(NodePtr<T> node);
  void FixErase(NodePtr<T> node);

  void RotateLeft(NodePtr<T> node);
  void RotateRight(NodePtr<T> node);
  void RBTransplant(NodePtr<T> node, NodePtr<T> v);

  NodePtr<T> CopyNodes(const NodePtr<T> &node);
  NodePtr<T> Min(NodePtr<T> node);

  void printHelper(NodePtr<T> root, std::string indent, bool last) {
    if (root != TNULL) {
      std::cout << indent;
      if (last) {
        std::cout << "R----";
        indent += "   ";
      } else {
        std::cout << "L----";
        indent += "|  ";
      }

      std::string sColor = bool(root->color) ? "RED" : "BLACK";
      std::cout << root->data << "(" << sColor << ")" << std::endl;
      printHelper(root->left, indent, false);
      printHelper(root->right, indent, true);
    }
  }

 public:
  RedBlackTree();
  ~RedBlackTree() = default;

  void Insert(T element);
  void Erase(T element);

  size_t Size() const;

  void Copy(const RedBlackTree &other);
  RedBlackTree &Swap(RedBlackTree &other);

  void printTree() {
    if (root_) {
      printHelper(root_, "", true);
    }
  }
};

template <class T>
RedBlackTree<T>::RedBlackTree() {
  TNULL = std::make_shared<Node<T>>();
  TNULL->color = Color::black;
  TNULL->left = nullptr;
  TNULL->right = nullptr;
  root_ = TNULL;
};

template <class T>
void RedBlackTree<T>::Insert(T element) {
  NodePtr<T> node = std::make_shared<Node<T>>();
  node->data = element;
  node->left = TNULL;
  node->right = TNULL;
  node->color = Color::red;
  ++size_;

  NodePtr<T> y = nullptr;
  NodePtr<T> x(root_);

  while (x != TNULL) {
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
  if (y->left != TNULL) {
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
  if (y->right != TNULL) {
    y->right->parent = node;
  }
  y->parent = node->parent;

  NodePtr<T> parent = node->parent.lock();
  if (parent == nullptr) {
    this->root_ = y;
  } else if (node == parent->right) {
    parent->right = y;
  } else {
    parent->left = y;
  }
  y->right = node;
  node->parent = y;
}

template <class T>
void RedBlackTree<T>::Erase(T element) {
  EraseNode(this->root_, element);
}

template <class T>
void RedBlackTree<T>::EraseNode(NodePtr<T> node, T element) {
  NodePtr<T> z(TNULL);
  NodePtr<T> x, y;
  while (node != TNULL) {
    if (!(node->data < element) && !(element < node->data)) {
      --size_;
      z = node;
    }

    if (!(element < node->data)) {
      node = node->right;
    } else {
      node = node->left;
    }
  }

  if (z == TNULL) {
    return;
  }

  y = z;
  Color y_original_color = y->color;
  if (z->left == TNULL) {
    x = z->right;
    RBTransplant(z, z->right);
  } else if (z->right == TNULL) {
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
NodePtr<T> RedBlackTree<T>::Min(NodePtr<T> node) {
  while (node->left != TNULL) {
    node = node->left;
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
  root_ = CopyNodes(other.root_);
}

template <class T>
NodePtr<T> RedBlackTree<T>::CopyNodes(const NodePtr<T> &node) {
  if (node->left == nullptr && node->right == nullptr && node->color == Color::black) {
    return TNULL;
  } else {
    NodePtr<T> new_node = std::make_shared<Node<T>>();
    new_node->data = node->data;
    new_node->parent = node->parent;
    new_node->color = node->color;
    new_node->left = CopyNodes(node->left);
    new_node->right = CopyNodes(node->right);
    return new_node;
  }
}

template <class T>
RedBlackTree<T> &RedBlackTree<T>::Swap(RedBlackTree &other) {
  std::swap(size_, other.size_);
  root_.swap(other.root_);
  TNULL.swap(other.TNULL);
  return *this;
}
