project(hw2)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_subdirectory(tests)

option(SANITIZE "Enable sanitizers" OFF)
option(WITH_CLANG_TIDY "Enable clang-tidy" OFF)
option(WITH_COVERAGE "Enable coverage" OFF)

if (SANITIZE)
  set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,leak,undefined")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,leak,undefined")
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
