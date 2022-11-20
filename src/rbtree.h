#pragma once
#include <iostream>
#include <memory>
#include <cstdio>

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
  size_t size_ = 0;

  void EraseNode(ConstNodePtr<T> node);
  void FixInsert(NodePtr<T> node);
  void FixErase(NodePtr<T> x, NodePtr<T> xp);

  void RotateLeft(NodePtr<T> node);
  void RotateRight(NodePtr<T> node);
  NodePtr<T> RBTransplant(ConstNodePtr<T> node, NodePtr<T> v);

  NodePtr<T> CopyNodes(const NodePtr<T> &node, const NodePtr<T> &new_parent);
  NodePtr<T> Min(NodePtr<T> node) const;
  NodePtr<T> Max(NodePtr<T> node) const;
  ConstNodePtr<T> FindElement(ConstNodePtr<T> node, const T &element) const;
  ConstNodePtr<T> FindLowerBound(ConstNodePtr<T> node, const T &element) const;

  void printHelper(NodePtr<T> root, std::string indent, bool last) {
    if (root) {
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
    if (node) {
      inOrderHelper(node->left);
      std::cout << node->data << " ";
      inOrderHelper(node->right);
    }
  }

 public:

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
void RedBlackTree<T>::Insert(const T &element) {
  NodePtr<T> node = std::make_shared<Node<T>>();
  node->data = element;
  node->color = Color::red;
  ++size_;

  NodePtr<T> y = nullptr;
  NodePtr<T> x(root_);

  while (x) {
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

  FixInsert(node);
}

template <class T>
void RedBlackTree<T>::FixInsert(NodePtr<T> node) {
  NodePtr<T> parent = node->parent.lock();
  while (parent && parent->color == Color::red) {
    NodePtr<T> grand_parent = parent->parent.lock();
    if (parent == grand_parent->left) {
      NodePtr<T> u = grand_parent->right;
      if (u && u->color == Color::red) {
        u->color = Color::black;
        parent->color = Color::black;
        grand_parent->color = Color::red;
        parent = grand_parent->parent.lock();
      } else {
        if (node == parent->right) {
          RotateLeft(grand_parent->left);
          parent = grand_parent->left;
        }
        parent->color = Color::black;
        grand_parent->color = Color::red;
        NodePtr<T> grand_grand_parent = grand_parent->parent.lock();
        if (!grand_grand_parent) {
          RotateRight(root_);
        } else {
          RotateRight(grand_parent);
        }
      }
    } else {
      NodePtr<T> u = grand_parent->left;
      if (u && u->color == Color::red) {
        u->color = Color::black;
        parent->color = Color::black;
        grand_parent->color = Color::red;
        parent = grand_parent->parent.lock();
      } else {
        if (node == parent->left) {
          RotateRight(grand_parent->right);
          parent = grand_parent->right;
        }
        parent->color = Color::black;
        grand_parent->color = Color::red;
        NodePtr<T> grand_grand_parent = grand_parent->parent.lock();
        if (!grand_grand_parent) {
          RotateLeft(root_);
        } else {
          RotateLeft(grand_parent);
        }
      }
    } 
  }
  root_->color = Color::black;
}

template <class T>
void RedBlackTree<T>::RotateLeft(NodePtr<T> node) {
  NodePtr<T> y = node->right;
  node->right = y->left;
  if (node->right) {
    node->right->parent = node;
  }
  y->parent = node->parent;

  NodePtr<T> parent = node->parent.lock();
  if (!parent) {
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
  if (node->left) {
    node->left->parent = node;
  }
  y->parent = node->parent;

  NodePtr<T> parent = node->parent.lock();
  if (!parent) {
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
  ConstNodePtr<T> node = Find(element);
  EraseNode(node);
}

template <class T>
void RedBlackTree<T>::EraseNode(ConstNodePtr<T> node) {
  if (!node) {
    return;
  }
  Color orig_color = node->color;
  NodePtr<T> x, parent;

  if (!node->left) {
    x = node->right;
    parent = node->parent.lock();
    RBTransplant(node, x);
  } else if (!node->right) {
    x = node->left;
    parent = node->parent.lock();
    RBTransplant(node, x);
  } else {
    NodePtr<T> y = Min(node->right);
    orig_color = y->color;
    x = y->right;
    parent = y;
    if (y->parent.lock() == node) {
      if (x) {
        x->parent = y;
      }
      NodePtr<T> pz = RBTransplant(node, node->right);
      y->left = pz->left;
      y->left->parent = y;
      y->color = pz->color;
    } else {
      parent = y->parent.lock();
      NodePtr<T> py = RBTransplant(y, y->right);
      py->right = node->right;
      py->right->parent = py;
      NodePtr<T> pz = RBTransplant(node, py);
      py->left = pz->left;
      py->left->parent = py;
      py->color = pz->color;
    }
  }

  if (orig_color == Color::black) {
    FixErase(x, parent);
  }
  --size_;
}

template <class T>
NodePtr<T> RedBlackTree<T>::Min(NodePtr<T> node) const {
  if (!node) {
    return node;
  }
  while (node->left) {
    node = node->left;
  }
  return node;
}

template <class T>
NodePtr<T> RedBlackTree<T>::Max(NodePtr<T> node) const {
  if (!node) {
    return node;
  }
  while (node->right) {
    node = node->right;
  }
  return node;
}

template <class T>
void RedBlackTree<T>::FixErase(NodePtr<T> x, NodePtr<T> xp) {
  while (x != root_ && (!x || x->color == Color::black)) {
    if (x == xp->left) {
      NodePtr<T> w = xp->right;
      if (w && w->color == Color::red) {
        w->color = Color::black;
        xp->color = Color::red;
        RotateLeft(xp);
        w = xp->right;
      }
      if (w && (!w->left || w->left->color == Color::black) &&
          (!w->right || w->right->color == Color::black)) {
        w->color = Color::red;
        x = xp;
        xp = xp->parent.lock();
      } else {
        if (w) {
          if (!w->right || w->right->color == Color::black) {
            w->left->color = Color::black;
            w->color = Color::red;
            RotateRight(w);
            w = xp->right;
          }
          w->color = xp->color;
          xp->color = Color::black;
          w->right->color = Color::black;
          RotateLeft(xp);
        }
        x = root_;
      }
    } else {
      NodePtr<T> w = xp->left;
      if (w && w->color == Color::red) {
        w->color = Color::black;
        xp->color = Color::red;
        RotateRight(xp);
        w = xp->left;
      }
      if (w && (!w->left || w->left->color == Color::black) &&
          (!w->right || w->right->color == Color::black)) {
        w->color = Color::red;
        x = xp;
        xp = xp->parent.lock();
      } else {
        if (w) {
          if (!w->left || w->left->color == Color::black) {
            w->right->color = Color::black;
            w->color = Color::red;
            RotateLeft(w);
            w = xp->left;
          }
          w->color = xp->color;
          xp->color = Color::black;
          w->left->color = Color::black;
          RotateRight(xp);
        }
        x = root_;
      }
    }
  }
  if (x) {
    x->color = Color::black;
  }
}

template <class T>
NodePtr<T> RedBlackTree<T>::RBTransplant(ConstNodePtr<T> u, NodePtr<T> v) {
  if (v) {
    v->parent = u->parent;
  }
  NodePtr<T> w = nullptr;
  NodePtr<T> u_parent = u->parent.lock();
  if (u_parent == nullptr) {
    w = root_;
    root_ = v;
  } else if (u == u_parent->left) {
    w = u_parent->left;
    u_parent->left = v;
  } else {
    w = u_parent->right;
    u_parent->right = v;
  }
  return w;
}

template <class T>
size_t RedBlackTree<T>::Size() const {
  return size_;
}

template <class T>
void RedBlackTree<T>::Copy(const RedBlackTree &other) {
  size_ = other.size_;
  if (other.root_) {
    root_ = CopyNodes(other.root_, other.root_->parent.lock());
  }
}

template <class T>
NodePtr<T> RedBlackTree<T>::CopyNodes(const NodePtr<T> &node,
                                      const NodePtr<T> &new_parent) {
  if (!node) {
    return node;
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
  return *this;
}

template <class T>
NodePtr<T> RedBlackTree<T>::GetMin() const {
  return Min(root_);
}

template <class T>
NodePtr<T> RedBlackTree<T>::GetMax() const {
  return Max(root_);
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
  if (!node || (!(node->data < element) && !(element < node->data))) {
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
  if (!node) {
    return node;
  }
  ConstNodePtr<T> min_node = Min(root_);
  ConstNodePtr<T> max_node = Max(root_);
  if (!(min_node->data < element)) {
    return min_node;
  } else if (max_node->data < element) {
    return nullptr;
  }
  ConstNodePtr<T> subtree_lower_bound;
  if (!(node->data < element) && !(element < node->data)) {
    return node;
  } else if (element < node->data) {
    subtree_lower_bound = FindLowerBound(node->left, element);
    if (!subtree_lower_bound) {
      return node;
    }
  } else {
    subtree_lower_bound = FindLowerBound(node->right, element);
    if (!subtree_lower_bound) {
      return nullptr;
    }
  }
  return subtree_lower_bound;
}

template <class T>
ConstNodePtr<T> RedBlackTree<T>::Successor(ConstNodePtr<T> node) const {
  if (node == Max(root_)) {
    return nullptr;
  } else if (node->right) {
    return Min(node->right);
  }
  NodePtr<T> y = node->parent.lock();
  while (y && node == y->right) {
    node = y;
    y = y->parent.lock();
  }
  return y;
}

template <class T>
ConstNodePtr<T> RedBlackTree<T>::Predecessor(ConstNodePtr<T> node) const {
  if (!node) {
    return Max(root_);
  }
  if (node->left) {
    return Max(node->left);
  }
  NodePtr<T> y = node->parent.lock();
  while (y && node == y->left) {
    node = y;
    y = y->parent.lock();
  }
  return y;
}
