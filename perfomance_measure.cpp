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

using std::chrono::duration_cast;
using microseconds = std::chrono::microseconds;
using sclock = std::chrono::steady_clock;
using time_point = std::chrono::steady_clock::time_point;
using duration = std::chrono::duration<long, std::micro>;

static time_point start;

void Tick() { start = sclock::now(); }

duration Tock() { return duration_cast<microseconds>(sclock::now() - start); }

int main() {
  std::unique_ptr<ByteDiffCounterBase> counter;
  if constexpr(false) {
    counter = std::make_unique<ByteDiffCounterParallel>();
  } else {
    counter = std::make_unique<ByteDiffCounterSerial>();
  }
  Tick();
  counter->process_file("100mb.txt", "output_100_p.txt");
  std::cout << Tock().count();
}
