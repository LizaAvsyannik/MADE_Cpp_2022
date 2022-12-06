#pragma once
#include <array>
#include <boost/asio.hpp>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

class ByteDiffCounterBase {
 protected:
  static const size_t kNumDiff = 11;
  std::array<size_t, kNumDiff> counter_{};
  const size_t kBatchSize = 8 * 1024 * 1024;

  void write_results(std::string filename) const;

 public:
  virtual void process_file(std::string input_filename,
                            std::string output_filename) = 0;
};

class ByteDiffCounterSerial : public ByteDiffCounterBase {
 private:
  inline void update_counter(char first_byte, char second_byte);
  void count_bytes(const std::vector<char> &batch, size_t start_idx,
                   size_t end_idx, char prev_batch_last_token);

 public:
  void process_file(std::string input_filename,
                    std::string output_filename) override;
};

class ByteDiffCounterParallel : public ByteDiffCounterBase {
 private:
 private:
  const size_t num_threads_;
  boost::asio::thread_pool pool_;

  inline void update_counter(std::array<size_t, kNumDiff> &counter,
                             char first_byte, char second_byte);
  std::array<size_t, kNumDiff> count_bytes(const std::vector<char> &batch,
                                           char prev_batch_last_token);
  void aggregate_results(
      std::vector<std::future<std::array<size_t, kNumDiff>>> &threads_results);

  std::vector<std::future<std::array<size_t, kNumDiff>>>
  process_batch(const std::vector<char> &batch, size_t batch_size,
                size_t thread_batch_size, char prev_batch_last_token);

 public:
  ByteDiffCounterParallel();
  ~ByteDiffCounterParallel();
  void process_file(std::string input_filename,
                    std::string output_filename) override;
};

inline void convert_path_to_absolute(std::string &filename) {
  std::string cwd = std::filesystem::current_path();
  filename = cwd + '/' + filename;
}

inline void ByteDiffCounterBase::write_results(std::string filename) const {
  std::ofstream file(filename);
  for (size_t i = 0; i < kNumDiff; ++i) {
    file << i << ':' << counter_[i] << '\n';
  }
  file.close();
}
