/**
 * @file test_cxxstd_this_thread.cpp
 * @author CrackLewis (ghxx040406@163.com)
 * @brief
 * @version 0.1
 * @date 2023-03-18
 *
 * @copyright Copyright (c) 2023
 *
 * This file involves testing functions in namespace std::this_thread.
 */

#include <chrono>
#include <random>
#include <thread>

#include "gtest/gtest.h"

TEST(CxxstdThisThreadTest, SleepForTest) {
  /**
   * @brief
   *
   * std::this_thread::sleep_for()
   */

  std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}