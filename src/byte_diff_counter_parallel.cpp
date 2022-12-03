#include "byte_diff_counter.h"
#include <algorithm>
#include <cmath>

ByteDiffCounterParallel::ByteDiffCounterParallel()
    : num_threads_(std::thread::hardware_concurrency()),
      pool_(std::thread::hardware_concurrency()) {
  std::cout << "Number of Threads: " << num_threads_ << '\n';
}

ByteDiffCounterParallel::~ByteDiffCounterParallel() { pool_.join(); }

void ByteDiffCounterParallel::update_counter(
    std::array<size_t, kNumDiff> &counter, char first_byte, char second_byte) {
  int difference = (first_byte > second_byte) ? (first_byte - second_byte)
                                              : (second_byte - first_byte);
  if ((0 <= difference) && (difference < kNumDiff)) {
    ++counter[difference];
  }
}

std::array<size_t, ByteDiffCounterParallel::kNumDiff>
ByteDiffCounterParallel::count_bytes(const std::vector<char> &batch,
                                     char prev_batch_last_token) {
  std::array<size_t, kNumDiff> counter{};
  size_t start_idx = 0;
  char first_byte = batch[start_idx];
  ;
  char second_byte = '\0';

  if (prev_batch_last_token) {
    update_counter(counter, prev_batch_last_token, first_byte);
  }
  ++start_idx;
  while (start_idx != batch.size()) {
    second_byte = batch[start_idx];
    update_counter(counter, first_byte, second_byte);
    first_byte = second_byte;
    ++start_idx;
  }
  return counter;
}

void ByteDiffCounterParallel::aggregate_results(
    std::vector<std::future<std::array<size_t, kNumDiff>>> &threads_results) {
  for (auto &res : threads_results) {
    auto thread_counter = res.get();
    for (int i = 0; i < kNumDiff; ++i) {
      counter_[i] += thread_counter[i];
    }
  }
}

void ByteDiffCounterParallel::process_file(std::string input_filename,
                                           std::string output_filename) {
  convert_path_to_absolute(input_filename);
  convert_path_to_absolute(output_filename);

  std::ifstream file(input_filename);
  const size_t file_size =
      static_cast<size_t>(std::filesystem::file_size(input_filename));
  const size_t batch_size = file_size < kBatchSize ? file_size : kBatchSize;
  const size_t thread_batch_size =
      static_cast<int>(std::ceil(1.0 * batch_size / num_threads_));
  // num_threads =
  //     thread_batch_size > 1 ? num_threads : batch_size / kMinBatchSize;
  // thread_batch_size = thread_batch_size > 1 ? thread_batch_size :
  // kMinBatchSize;

  // std::cout << batch_size << ' ' << thread_batch_size << ' ' << num_threads
  //           << '\n';

  std::vector<char> batch(batch_size);
  char prev_batch_last_token = '\0';

  std::vector<std::future<std::array<size_t, kNumDiff>>> threads_results;
  threads_results.reserve(num_threads_);
  while (file.read(batch.data(), batch_size)) {
    for (auto &res : threads_results) {
      res.wait();
    }
    aggregate_results(threads_results);
    threads_results = process_batch(batch, batch_size, thread_batch_size,
                                    prev_batch_last_token);
    prev_batch_last_token = batch[batch_size - 1];
  }

  for (auto &res : threads_results) {
    res.wait();
  }
  aggregate_results(threads_results);

  if (file.gcount() != 0) {
    threads_results = process_batch(batch, file.gcount(), thread_batch_size,
                                    prev_batch_last_token);
    for (auto &res : threads_results) {
      res.wait();
    }
    aggregate_results(threads_results);
  }

  file.close();
  write_results(output_filename);
}

std::vector<std::future<std::array<size_t, ByteDiffCounterParallel::kNumDiff>>>
ByteDiffCounterParallel::process_batch(const std::vector<char> &batch,
                                       size_t batch_size,
                                       size_t thread_batch_size,
                                       char prev_batch_last_token) {
  std::vector<std::future<std::array<size_t, kNumDiff>>> threads_results;
  size_t start_idx = 0;
  size_t end_idx = 0;
  for (size_t i = 0; i < num_threads_; ++i) {
    start_idx = thread_batch_size * i;
    end_idx = std::min(thread_batch_size * (i + 1), batch_size);
    const std::vector<char> thread_batch(batch.begin() + start_idx,
                                         batch.begin() + end_idx);
    threads_results.push_back(
        post(pool_, std::packaged_task<std::array<size_t, kNumDiff>()>(
                        std::bind(&ByteDiffCounterParallel::count_bytes, this,
                                  thread_batch, prev_batch_last_token))));
    prev_batch_last_token = batch[end_idx - 1];
    if (end_idx == batch_size) {
      break;
    }
  }
  return threads_results;
}
