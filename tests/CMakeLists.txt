include_directories(${CMAKE_SOURCE_DIR})  # make includes relative to project root

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests unit.cpp)
add_executable(test_set test_set.cpp)


if (WITH_COVERAGE)
  target_compile_options(unit_tests PRIVATE -fprofile-arcs -ftest-coverage -O0)
  target_compile_options(test_set PRIVATE -fprofile-arcs -ftest-coverage -O0)
  target_link_options(unit_tests PRIVATE -coverage -lgcov)
  target_link_options(test_set PRIVATE -coverage -lgcov)
endif()

target_link_libraries(unit_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
