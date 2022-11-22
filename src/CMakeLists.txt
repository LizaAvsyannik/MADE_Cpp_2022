add_library(hw4_counter byte_diff_counter.cpp)

if (WITH_COVERAGE)
  target_compile_options(hw4_counter PUBLIC -fprofile-arcs -ftest-coverage -fPIC -O0)
  target_link_options(hw4_counter PUBLIC -coverage -lgcov)
endif()
