add_library(hw4_counter_serial STATIC byte_diff_counter_serial.cpp)
add_library(hw4_counter_parallel SHARED byte_diff_counter_parallel.cpp)

if (WITH_COVERAGE)
  target_compile_options(hw4_counter_serial PUBLIC -fprofile-arcs -ftest-coverage -fPIC -O0)
  target_compile_options(hw4_counter_parallel PUBLIC -fprofile-arcs -ftest-coverage -fPIC -O0)
  target_link_options(hw4_counter_serial PUBLIC -coverage -lgcov)
  target_link_options(hw4_counter_parallel PUBLIC -coverage -lgcov)
endif()
