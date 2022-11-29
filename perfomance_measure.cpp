#include "src/set.h"
#include <chrono>
#include <iostream>
#include <random>
#include <set>

using std::chrono::duration_cast;
using nanoseconds = std::chrono::nanoseconds;
using sclock = std::chrono::steady_clock;
using time_point = std::chrono::steady_clock::time_point;
using duration = std::chrono::duration<long, std::nano>;

static time_point start;

void Tick() { start = sclock::now(); }

duration Tock() { return duration_cast<nanoseconds>(sclock::now() - start); }

struct TimeStatistics {
  double initial_insert;
  double final_erase;
  double avg_insert;
  double avg_find;
  double avg_erase;

  TimeStatistics &operator+=(const TimeStatistics &stats) {
    initial_insert += stats.initial_insert;
    final_erase += stats.final_erase;
    avg_insert += stats.avg_insert;
    avg_find += stats.avg_find;
    avg_erase += stats.avg_erase;
    return *this;
  }
};

template <class Container>
TimeStatistics TestPerformanceOnARandomSequence(int n_elements) {
  const int kNumOperations = 10000;
  Container set;
  TimeStatistics stats;
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1, int(1e9));
  Tick();
  for (int i = 0; i < n_elements; ++i) {
    int random_number = dist(rng);
    set.insert(random_number);
  }
  stats.initial_insert = Tock().count();
  double total_insert = 0.0;
  double total_find = 0.0;
  double total_erase = 0.0;
  for (int i = 0; i < kNumOperations; ++i) {
    int random_number = dist(rng);
    Tick();
    set.insert(random_number);
    total_insert += Tock().count();
    Tick();
    set.find(random_number);
    total_find += Tock().count();
    Tick();
    set.erase(random_number);
    total_erase += Tock().count();
  }
  Tick();
  for (int i = 0; i < n_elements; ++i) {
    set.erase(i);
  }
  stats.final_erase = Tock().count();
  stats.avg_insert = total_insert / kNumOperations;
  stats.avg_find = total_find / kNumOperations;
  stats.avg_erase = total_erase / kNumOperations;
  return stats;
}

template <class Container>
void TestPerformance(int n_elements) {
  std::cout << n_elements << " elements\n";
  const int kNumRuns = 10;
  TimeStatistics stats{0.0, 0.0, 0.0, 0.0};
  for (int i = 0; i < kNumRuns; ++i) {
    stats += TestPerformanceOnARandomSequence<Container>(n_elements);
  }
  std::cout << "Initial insert: " << stats.initial_insert / kNumRuns << " ns\n";
  std::cout << "Final erase: " << stats.final_erase / kNumRuns << " ns\n";
  std::cout << "Avg insert: " << stats.avg_insert / kNumRuns << " ns\n";
  std::cout << "Avg find: " << stats.avg_find / kNumRuns << " ns\n";
  std::cout << "Avg erase: " << stats.avg_erase / kNumRuns << " ns\n\n";
}

int main() {
  std::cout << "Custom set\n\n";
  for (int n : {100, 1000, 10000, 100000}) {
    TestPerformance<Set<int>>(n);
  }

  std::cout << "Standard set\n\n";
  for (int n : {100, 1000, 10000, 100000}) {
    TestPerformance<std::set<int>>(n);
  }
}
