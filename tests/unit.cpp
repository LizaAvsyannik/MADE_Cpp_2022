#include "src/set.h"
#include <chrono>
#include <gtest/gtest.h>

TEST(SET, CopyConstructor) {
  Set<int> set_1{3, 2, 1, 5, 6, 4};
  Set<int> set_2(set_1);
  Set<int>::const_iterator it1 = set_1.begin();
  Set<int>::const_iterator it2 = set_2.begin();
  for (int i = 0; i < 6; ++i) {
    EXPECT_EQ(*it1, *it2);
    ++it1;
    ++it2;
  }
  EXPECT_EQ(it1, set_1.end());
  EXPECT_EQ(it2, set_2.end());
}

TEST(SET, AssignmentOperator) {
  Set<int> set_1{3, 2, 1, 5, 6, 4};
  Set<int> set_2{7, 8, 9};
  set_2 = set_1;
  Set<int>::const_iterator it1 = set_1.end();
  Set<int>::const_iterator it2 = set_2.end();
  for (int i = 6; i > 0; --i) {
    --it1;
    --it2;
    EXPECT_EQ(*it1, *it2);
  }
  EXPECT_EQ(it1, set_1.begin());
  EXPECT_EQ(it2, set_2.begin());
}

TEST(SET, Iterator) {
  std::string elems[] = {"abc", "aab", "cab", "bac", "ooo", "cca"};
  Set<std::string> set{elems, elems + 6};
  Set<std::string>::const_iterator it = set.begin();
  EXPECT_EQ(*it++, "aab");
  EXPECT_EQ(*it, "abc");
  EXPECT_EQ(*--it, "aab");
  ++it;
  EXPECT_EQ(*++it, "bac");
  EXPECT_EQ(*--it, "abc");
  it++;
  ++it;
  EXPECT_EQ(*it--, "cab");
  EXPECT_EQ(*it, "bac");
  EXPECT_EQ(*++it, "cab");
  EXPECT_EQ(*it++, "cab");
  EXPECT_EQ(*it, "cca");
  EXPECT_EQ(*++it, "ooo");
  EXPECT_TRUE(++it == set.end());

  Set<std::string>::const_iterator it2 = set.begin();
  it = set.begin();
  EXPECT_TRUE(it == it2);
  EXPECT_FALSE(it != it2);
  ++it;
  EXPECT_TRUE(it != it2);
  EXPECT_FALSE(it == it2);
  Set<std::string> set_2(set);
  it2 = set_2.begin();
  --it;
  EXPECT_TRUE(it != it2);
  EXPECT_FALSE(it == it2);
}

TEST(SET, InsertErase) {
  Set<int> set_1{3, 2, 1, 5, 6, 4};
  Set<int> set_2(set_1);
  set_1.insert(3);
  set_1.insert(2);
  set_1.insert(1);
  set_1.insert(10);
  set_1.insert(-1);
  Set<int>::const_iterator it1 = set_1.begin();
  EXPECT_EQ(*it1, -1);
  ++it1;
  for (auto iter = set_2.begin(); iter != set_2.end(); ++iter) {
    EXPECT_EQ(*it1, *iter);
    ++it1;
  }
  EXPECT_EQ(*it1, 10);
  set_1.erase(10);
  set_1.erase(-1);
  set_1.erase(7);
  it1 = set_1.begin();
  for (auto iter = set_2.begin(); iter != set_2.end(); ++iter) {
    EXPECT_EQ(*it1, *iter);
    ++it1;
  }
}

TEST(SET, SizeEmpty) {
  Set<int> set;
  EXPECT_TRUE(set.empty());
  EXPECT_EQ(set.size(), 0);

  set.insert(3);
  set.insert(2);
  set.insert(1);
  set.insert(10);
  set.insert(-1);
  EXPECT_FALSE(set.empty());
  EXPECT_EQ(set.size(), 5);

  set.insert(2);
  set.insert(1);
  set.insert(10);
  EXPECT_FALSE(set.empty());
  EXPECT_EQ(set.size(), 5);

  set.erase(1);
  set.erase(2);
  set.erase(10);
  EXPECT_FALSE(set.empty());
  EXPECT_EQ(set.size(), 2);

  set.erase(2);
  set.erase(1);
  set.erase(10);
  EXPECT_FALSE(set.empty());
  EXPECT_EQ(set.size(), 2);

  set.erase(3);
  set.erase(-1);
  set.erase(10);
  EXPECT_TRUE(set.empty());
  EXPECT_EQ(set.size(), 0);
}

TEST(SET, Find) {
  Set<int> set{3, 2, 1, 5, 6, 4};
  EXPECT_EQ(*set.find(2), 2);
  EXPECT_TRUE(set.find(8) == set.end());
}

TEST(SET, LowerBound) {
  Set<int> set{3, 2, 1, 5, 6};
  EXPECT_EQ(*set.lower_bound(4), 5);
  EXPECT_EQ(*set.lower_bound(5), 5);
  EXPECT_EQ(*set.lower_bound(0), 1);
  EXPECT_TRUE(set.lower_bound(8) == set.end());
}
