#ifndef THREADED_IMPL
#define THREADED_IMPL 0
#endif

#include "src/byte_diff_counter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <memory>
#include <array>
#include <string>

using std::chrono::duration_cast;
using milliseconds = std::chrono::milliseconds;
using sclock = std::chrono::steady_clock;
using time_point = std::chrono::steady_clock::time_point;
using duration = std::chrono::duration<long, std::milli>;

static time_point start;

void Tick() { start = sclock::now(); }

duration Tock() { return duration_cast<milliseconds>(sclock::now() - start); }

int main() {
  std::unique_ptr<ByteDiffCounterBase> counter;
  std::string output = "";

  if constexpr(THREADED_IMPL) {
    counter = std::make_unique<ByteDiffCounterParallel>();
    output = "output_parallel_";
    std::cout << "Parallel:\n";
  } else {
    counter = std::make_unique<ByteDiffCounterSerial>();
    output = "output_serial_";
    std::cout << "Serial:\n";
  }

  const int kNumFiles = 3;
  const int kNumRuns = 10;
  std::string folder = "large_data/";
  const std::array<std::string, kNumFiles> files{"100mb_1.txt", "100mb_2.txt", "100mb_3.txt"};

  double total_time = 0;
  Tick();
  for (auto file : files) {
    for (int i = 0; i < kNumRuns; ++i) {
      counter->process_file(folder + file, folder + output + file);
    }
  }
  total_time = Tock().count();
  std::cout << "Average time: " << total_time / (kNumRuns * kNumFiles) << " ms\n";
}
