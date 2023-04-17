add_executable(test_linear_list test_linear_list.cc)
target_link_libraries(test_linear_list PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)