add_executable(test_cxxstd_thread test_cxxstd_thread.cpp)
target_link_libraries(test_cxxstd_thread PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)