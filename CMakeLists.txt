project(hw3)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

option(SANITIZE "Enable sanitizers" OFF)

add_subdirectory(src)
add_subdirectory(tests)

if (SANITIZE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,leak,undefined")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,leak,undefined")
endif()

add_executable(hw3_main main.cpp)
