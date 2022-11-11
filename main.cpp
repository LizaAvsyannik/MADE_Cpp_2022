#include "src/rbtree.h"
#include <iostream>

int main() {
  RedBlackTree<int> bst;
  bst.Insert(55);
  bst.Insert(40);
  bst.Insert(65);
  bst.Insert(60);
  bst.Insert(75);
  bst.Insert(57);
  bst.printTree();
  std::cout << std::endl
     << "After deleting" << std::endl;
  bst.Erase(40);
  // bst.printTree();
}