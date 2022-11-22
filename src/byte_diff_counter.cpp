#include "byte_diff_counter.h"
#include <algorithm>
#include <cmath>
#include <vector>

void ByteDiffCounterBase::update_counter(char first_byte, char second_byte) {
  size_t difference = (first_byte > second_byte) ? (first_byte - second_byte)
                                                 : (second_byte - first_byte);
  if ((0 <= difference) && (difference < kNumDiff)) {
    ++counter_[difference];
  }
}

void ByteDiffCounterBase::count_bytes(const std::vector<char> &batch,
                                      size_t start_idx, size_t end_idx,
                                      char prev_batch_last_token) {
  char first_byte;
  char second_byte;

  first_byte = batch[start_idx];
  if (prev_batch_last_token) {
    update_counter(prev_batch_last_token, first_byte);
  }
  ++start_idx;
  while (start_idx != end_idx) {
    second_byte = batch[start_idx];
    update_counter(first_byte, second_byte);
    first_byte = second_byte;
    ++start_idx;
  }
}

void ByteDiffCounterBase::write_results(std::string filename) const {
  std::ofstream file(filename);
  for (size_t i = 0; i < kNumDiff; ++i) {
    file << i << ':' << counter_[i] << '\n';
  }
  file.close();
}

void ByteDiffCounterSerial::process_file(std::string input_filename,
                                         std::string output_filename) {
  convert_path_to_absolute(input_filename);
  convert_path_to_absolute(output_filename);

  std::ifstream file(input_filename);
  size_t file_size =
      static_cast<size_t>(std::filesystem::file_size(input_filename));
  size_t batch_size = file_size < kBatchSize ? file_size : kBatchSize;
  std::vector<char> batch(batch_size);
  char prev_batch_last_token = '\0';

  while (file.read(batch.data(), batch_size)) {
    count_bytes(batch, 0, batch_size, prev_batch_last_token);
    prev_batch_last_token = batch[batch_size - 1];
  }
  if (file.gcount() != 0) {
    count_bytes(batch, 0, file.gcount(), prev_batch_last_token);
  }
  file.close();
  write_results(output_filename);
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

  std::vector<char> batch(batch_size);
  char prev_batch_last_token = '\0';

  std::vector<std::thread> threads;
  while (file.read(batch.data(), batch_size)) {
    // for (auto &th : threads) {
    //   th.join();
    // }
    threads =
        process_batch(batch, batch_size, num_threads, prev_batch_last_token);
    prev_batch_last_token = batch[batch_size - 1];
  }
  // for (auto &th : threads) {
  //   th.join();
  // }
  if (file.gcount() != 0) {
    threads =
        process_batch(batch, file.gcount(), num_threads, prev_batch_last_token);
  }
  // for (auto &th : threads) {
  //   th.join();
  // }
  file.close();
  write_results(output_filename);
}

std::vector<std::thread>
ByteDiffCounterParallel::process_batch(const std::vector<char> &batch,
                                       size_t batch_size, size_t num_threads,
                                       char prev_batch_last_token) {
  size_t thread_batch_size = batch_size / num_threads;
  num_threads =
      thread_batch_size > 1 ? num_threads : batch_size / kMinBatchSize;
  thread_batch_size = thread_batch_size > 1 ? thread_batch_size : kMinBatchSize;

  std::vector<std::thread> threads;
  size_t start_idx = 0;
  size_t end_idx = 0;
  for (size_t i = 0; i < num_threads; ++i) {
    start_idx = thread_batch_size * i;
    end_idx =
        (i != num_threads - 1) ? (thread_batch_size * (i + 1)) : batch_size;
    // std::cout << start_idx << ' ' << end_idx << '\n';
    threads.push_back(std::thread(&ByteDiffCounterParallel::count_bytes, this,
                                  std::ref(batch), start_idx, end_idx,
                                  prev_batch_last_token));
    prev_batch_last_token = batch[end_idx - 1];
  }
  for (auto &th : threads) {
    th.join();
  }
  return threads;
}
