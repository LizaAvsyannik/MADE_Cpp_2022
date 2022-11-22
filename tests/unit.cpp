#include "src/byte_diff_counter.h"
#include <fstream>
#include <gtest/gtest.h>

TEST(SET, Serial) {
  ByteDiffCounterSerial byte_diff_counter;
  std::string input_filename = "../../tests/test.txt";
  std::string ouput_filename = "../../tests/output.txt";
  byte_diff_counter.process_file(input_filename, ouput_filename);

  std::ifstream file(ouput_filename);
  std::string line;
  std::array<int, 11> counter = {3, 8, 2, 2, 1, 1, 1, 1, 1, 1, 1};
  int diff = 0;
  int count = 0;

  int i = 0;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    i = 0;
    while (getline(ss, line, ':')) {
      if (i % 2 == 0) {
        std::stringstream(line) >> diff;
      } else {
        std::stringstream(line) >> count;
        EXPECT_EQ(counter[diff], count);
      }
      ++i;
    }
  }
}

TEST(SET, Parallel) {
  ByteDiffCounterParallel byte_diff_counter;
  std::string input_filename = "../../tests/test.txt";
  std::string ouput_filename = "../../tests/output.txt";
  byte_diff_counter.process_file(input_filename, ouput_filename);

  std::ifstream file(ouput_filename);
  std::string line;
  std::array<int, 11> counter = {3, 8, 2, 2, 1, 1, 1, 1, 1, 1, 1};
  int diff = 0;
  int count = 0;

  int i = 0;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    i = 0;
    while (getline(ss, line, ':')) {
      if (i % 2 == 0) {
        std::stringstream(line) >> diff;
      } else {
        std::stringstream(line) >> count;
        EXPECT_EQ(counter[diff], count);
      }
      ++i;
    }
  }
}