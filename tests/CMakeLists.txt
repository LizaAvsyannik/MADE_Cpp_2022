include_directories(${CMAKE_SOURCE_DIR})  # make includes relative to project root

add_executable(test1 test1.c)
add_executable(test2 test2.c)

target_link_libraries(test1 hw1_logic)
target_link_libraries(test2 hw1_logic)
