include_directories(${CMAKE_SOURCE_DIR}) #make includes relative to project root

#add_compile_options(-fsanitize = address)
#add_link_options(-fsanitize = address)

    include(FetchContent) FetchContent_Declare(
        googletest GIT_REPOSITORY https
        : // github.com/google/googletest.git
        GIT_TAG release - 1.12.1) FetchContent_MakeAvailable(googletest)

        enable_testing()

            add_executable(test_io test_io.cpp) add_executable(
                test_data test_data
                    .cpp) add_executable(test_logic test_logic
                                             .cpp) add_executable(test_utils
                                                                      test_utils
                                                                          .cpp)

                if (WITH_COVERAGE) target_compile_options(
                    test_data PRIVATE - fprofile - arcs - ftest - coverage -
                    O0) target_compile_options(test_io PRIVATE - fprofile -
                                               arcs - ftest - coverage - O0)
                    target_compile_options(
                        test_utils PRIVATE - fprofile - arcs - ftest -
                        coverage -
                        O0) target_compile_options(test_logic PRIVATE -
                                                   fprofile - arcs - ftest -
                                                   coverage - O0)
                        target_link_options(
                            test_io PRIVATE - coverage -
                            lgcov) target_link_options(test_data PRIVATE -
                                                       coverage - lgcov)
                            target_link_options(
                                test_logic PRIVATE - coverage -
                                lgcov) target_link_options(test_utils PRIVATE -
                                                           coverage -
                                                           lgcov) endif()

                                target_link_libraries(
                                    test_io hw1_io hw1_data GTest::gtest_main)
                                    target_link_libraries(
                                        test_data hw1_data GTest::gtest_main)
                                        target_link_libraries(
                                            test_logic hw1_logic
                                                GTest::gtest_main)
                                            target_link_libraries(
                                                test_utils hw1_utils
                                                    GTest::gtest_main)

                                                include(GoogleTest)
                                                    gtest_discover_tests(
                                                        test_io)
                                                        gtest_discover_tests(
                                                            test_data)
                                                            gtest_discover_tests(
                                                                test_logic)
                                                                gtest_discover_tests(
                                                                    test_utils)
