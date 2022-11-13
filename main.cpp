#include "src/set.h"
#include <iostream>
#include <vector>
#include <set>
#include <chrono>
int main() {
  Set<int> set;
  std::set<int> stl_set;

  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  set.insert(1);
  set.insert(5);
  set.insert(2);
  set.insert(3);
  set.insert(4);

  set.find(3);
  set.find(5);
  set.find(8);
  set.lower_bound(-2);
  set.lower_bound(4);
  set.lower_bound(9);
  set.erase(1);
  set.erase(5);
  set.erase(2);
  set.erase(3);
  set.erase(4);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

  start = std::chrono::steady_clock::now();
  stl_set.insert(1);
  stl_set.insert(5);
  stl_set.insert(2);
  stl_set.insert(3);
  stl_set.insert(4);
  stl_set.find(3);
  stl_set.find(5);
  stl_set.find(8);
  stl_set.lower_bound(-2);
  stl_set.lower_bound(4);
  stl_set.lower_bound(9);
  stl_set.erase(1);
  stl_set.erase(5);
  stl_set.erase(2);
  stl_set.erase(3);
  stl_set.erase(4);
  end = std::chrono::steady_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;
}
