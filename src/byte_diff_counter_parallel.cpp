#include "byte_diff_counter.h"
#include <algorithm>
#include <cmath>

void ByteDiffCounterParallel::update_counter(
    std::array<size_t, kNumDiff> &counter, char first_byte, char second_byte) {
  int difference = (first_byte > second_byte) ? (first_byte - second_byte)
                                                 : (second_byte - first_byte);
  if ((0 <= difference) && (difference < kNumDiff)) {
    ++counter[difference];
  }
}

std::array<size_t, ByteDiffCounterParallel::kNumDiff>
ByteDiffCounterParallel::count_bytes(const std::vector<char> batch,
                                     char prev_batch_last_token) {
  std::array<size_t, kNumDiff> counter{};
  char first_byte;
  char second_byte;

  size_t start_idx = 0;
  first_byte = batch[start_idx];
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
  size_t file_size =
      static_cast<size_t>(std::filesystem::file_size(input_filename));
  size_t batch_size = file_size < kBatchSize ? file_size : kBatchSize;
  size_t num_threads = std::thread::hardware_concurrency();
  size_t thread_batch_size =
      static_cast<int>(std::ceil(1.0 * batch_size / num_threads));
  std::cout << "Number of Threads: " << num_threads << '\n';
  // num_threads =
  //     thread_batch_size > 1 ? num_threads : batch_size / kMinBatchSize;
  // thread_batch_size = thread_batch_size > 1 ? thread_batch_size :
  // kMinBatchSize;

  boost::asio::thread_pool pool(num_threads);
  // std::cout << batch_size << ' ' << thread_batch_size << ' ' << num_threads
  //           << '\n';

  std::vector<char> batch(batch_size);
  char prev_batch_last_token = '\0';

  std::vector<std::future<std::array<size_t, kNumDiff>>> threads_results;
  while (file.read(batch.data(), batch_size)) {
    for (auto &res : threads_results) {
      res.wait();
    }
    aggregate_results(threads_results);
    threads_results = process_batch(pool, batch, batch_size, thread_batch_size,
                                    num_threads, prev_batch_last_token);
    prev_batch_last_token = batch[batch_size - 1];
  }

  for (auto &res : threads_results) {
    res.wait();
  }
  aggregate_results(threads_results);

  if (file.gcount() != 0) {
    threads_results =
        process_batch(pool, batch, file.gcount(), thread_batch_size,
                      num_threads, prev_batch_last_token);
    for (auto &res : threads_results) {
      res.wait();
    }
    aggregate_results(threads_results);
  }

  file.close();
  write_results(output_filename);
}

std::vector<std::future<std::array<size_t, ByteDiffCounterParallel::kNumDiff>>>
ByteDiffCounterParallel::process_batch(boost::asio::thread_pool &pool,
                                       const std::vector<char> &batch,
                                       size_t batch_size,
                                       size_t thread_batch_size,
                                       size_t num_threads,
                                       char prev_batch_last_token) {
  std::vector<std::future<std::array<size_t, kNumDiff>>> threads_results;
  size_t start_idx;
  size_t end_idx;
  for (size_t i = 0; i < num_threads; ++i) {
    start_idx = thread_batch_size * i;
    end_idx = std::min(thread_batch_size * (i + 1), batch_size);
    std::vector<char> thread_batch(&batch[start_idx], &batch[end_idx]);
    threads_results.push_back(
        post(pool, std::packaged_task<std::array<size_t, kNumDiff>()>(
                       std::bind(&ByteDiffCounterParallel::count_bytes, this,
                                 thread_batch, prev_batch_last_token))));
    prev_batch_last_token = batch[end_idx - 1];
    if (end_idx == batch_size) {
      break;
    }
  }
  return threads_results;
}
