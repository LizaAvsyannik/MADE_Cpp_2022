project(hw3)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

option(SANITIZE "Enable sanitizers" OFF)
option(WITH_CLANG_TIDY "Enable clang-tidy" OFF)
option(WITH_COVERAGE "Enable coverage" OFF)
option(THREADED_IMPL "Enable processing in threads" OFF)

add_subdirectory(src)
add_subdirectory(tests)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic")

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

if (SANITIZE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,leak,undefined")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,leak,undefined")
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

if (WITH_CLANG_TIDY)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  find_program(CLANG_TIDY_BIN clang-tidy REQUIRED)
  set(CMAKE_C_CLANG_TIDY
    ${CLANG_TIDY_BIN};
    -format-style='file';
    -header-filter=${CMAKE_CURRENT_SOURCE_DIR};
  )
endif()

add_executable(hw4_perfomance_measure perfomance_measure.cpp)
if (THREADED_IMPL)
  target_link_libraries(hw4_perfomance_measure hw4_counter_parallel)
  target_compile_definitions(hw4_perfomance_measure PUBLIC THREADED_IMPL=1)
else()
  target_link_libraries(hw4_perfomance_measure hw4_counter_serial)
  target_compile_definitions(hw4_perfomance_measure PUBLIC THREADED_IMPL=0)
endif()
