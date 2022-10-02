project(hw1)
cmake_minimum_required(VERSION 3.16)

add_subdirectory(impl)
add_subdirectory(tests)

add_executable(hw1_main main.c)
target_link_libraries(hw1_main hw1_logic)
