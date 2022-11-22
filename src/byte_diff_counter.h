#pragma once
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

class ByteDiffCounterBase {
 protected:
  static const size_t kNumDiff = 11;
  std::array<size_t, kNumDiff> counter_;
  const size_t kBatchSize = 6;

  void update_counter(char first_byte, char second_byte);
  void count_bytes(const std::vector<char> &batch, size_t start_idx, size_t end_idx,
                   char prev_batch_last_token);
  void write_results(std::string filename) const;

 public:
  ByteDiffCounterBase() { counter_.fill(0); }
  ~ByteDiffCounterBase() = default;

  virtual void process_file(std::string input_filename,
                            std::string output_filename) = 0;
};

class ByteDiffCounterSerial : public ByteDiffCounterBase {
 public:
  void process_file(std::string input_filename,
                    std::string output_filename) override;
};

class ByteDiffCounterParallel : public ByteDiffCounterBase {
 private:
  const size_t kMinBatchSize = 2;

  std::vector<std::thread> process_batch(const std::vector<char> &batch, size_t batch_size,
                                         size_t num_threads, char prev_batch_last_token);

 public:
  void process_file(std::string input_filename,
                    std::string output_filename) override;
};

inline void convert_path_to_absolute(std::string &filename) {
  std::string cwd = std::filesystem::current_path();
  filename = cwd + '/' + filename;
}
