#include "src/byte_diff_counter.h"
#include <fstream>
#include <sstream>
#include <filesystem>

int main() {
  ByteDiffCounterParallel counter;
  counter.process_file("tests/test_1.txt", "tests/output.txt");
  // std::ifstream file("tests/test.txt");
  // std::array<char, 1024 * 1024> batch;
  // while (file.read(batch.data(), 1024 * 1024)) {
  //    std::cout << "ok";
  // }
  // std::cout << "ok";
  // std::cout << std::filesystem::file_size("tests/test.txt");
}
