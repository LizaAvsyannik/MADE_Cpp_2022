#include "byte_diff_counter.h"
#include <algorithm>
#include <vector>

void ByteDiffCounterSerial::update_counter(char first_byte, char second_byte) {
  int difference = (first_byte > second_byte) ? (first_byte - second_byte)
                                              : (second_byte - first_byte);
  if ((0 <= difference) && (difference < kNumDiff)) {
    ++counter_[difference];
  }
}

void ByteDiffCounterSerial::count_bytes(const std::vector<char> &batch,
                                        size_t start_idx, size_t end_idx,
                                        char prev_batch_last_token) {
  char first_byte = batch[start_idx];
  char second_byte = '\0';

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

void ByteDiffCounterSerial::process_file(std::string input_filename,
                                         std::string output_filename) {
  convert_path_to_absolute(input_filename);
  convert_path_to_absolute(output_filename);

  std::ifstream file(input_filename);
  const size_t file_size =
      static_cast<size_t>(std::filesystem::file_size(input_filename));
  const size_t batch_size = file_size < kBatchSize ? file_size : kBatchSize;
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
