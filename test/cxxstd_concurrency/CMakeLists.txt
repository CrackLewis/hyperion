add_executable(test_cxxstd_thread test_cxxstd_thread.cc)
target_link_libraries(test_cxxstd_thread PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

add_executable(test_cxxstd_this_thread test_cxxstd_this_thread.cc)
target_link_libraries(test_cxxstd_this_thread PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)

add_executable(test_cxxstd_mutex test_cxxstd_mutex.cc)
target_link_libraries(test_cxxstd_mutex PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)