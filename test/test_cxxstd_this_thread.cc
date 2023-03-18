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
   * std::this_thread::sleep_for(std::chrono::duration) makes the current thread
   * sleep for a specific period of time.
   */

  std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());

  int count_ena = 0;
  int count_val = 0;

  // counter thread that helps counting ticks when the main thread sleeps
  std::thread cth([&]() {
    while (1) {
      if (count_ena == -1) return;

      if (count_ena == 1) count_val++;
      // 5 millis per tick
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
  });

  for (int kase = 0, millis; kase < 10; ++kase) {
    count_ena = 1;
    millis = 10 * (5 + rnd() % 20);
    std::cout << "SubCase #" << kase << ": " << millis << " msec" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));
    count_ena = 0;

    std::cout << "SubCase #" << kase << ": slept for " << count_val << " ticks"
              << std::endl;
    // the test allows up to 1msec excess delay
    EXPECT_GE(count_val, millis / 6) << "sleep_for too long";
    EXPECT_LE(count_val, millis / 5) << "sleep_for too short";
    count_val = 0;

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  // stop the counting thread
  count_ena = -1;
  cth.join();
}

TEST(CxxstdThisThreadTest, SleepUntilTest) {
  /**
   * @brief
   *
   * std::this_thread::sleep_until(std::chrono::time_point) makes the current
   * thread sleep until a specified time point.
   */

  // enable literals like h, s, ms
  using namespace std::literals;

  int count_ena = 0;
  int count_val = 0;

  // counter thread that helps counting ticks when the main thread sleeps
  std::thread cth([&]() {
    while (1) {
      if (count_ena == -1) return;

      if (count_ena == 1) count_val++;
      // 5 millis per tick
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
  });

  const auto tbeg = std::chrono::steady_clock::now();
  // set wake-up time to be 500ms later
  const auto sleep_due = std::chrono::steady_clock::now() + 500ms;

  count_ena = 1;
  std::this_thread::sleep_until(sleep_due);

  const auto tend = std::chrono::steady_clock::now();

  count_ena = -1;
  cth.join();

  std::cout << "Actual Sleep time: "
            << std::chrono::duration_cast<std::chrono::microseconds>(tend -
                                                                     tbeg)
                   .count()
            << " microsecs" << std::endl;

  EXPECT_TRUE(count_val >= 90 && count_val <= 102)
      << "incorrect sleep time: " << count_val;
}

TEST(CxxstdThisThreadTest, YieldTest) {
  /**
   * @brief
   *
   * std::this_thread::yield() makes the thread abandon the processor and lets
   * other threads run instead.
   *
   * This function is heavily reliant on the OS process scheduling mechanics.
   * For Linux, it suspend the current thread and place it at the end of the
   * queue of threads of the same priority that are ready to run.
   */

  using namespace std::literals;

  const auto tbeg = std::chrono::steady_clock::now();
  const auto tend = tbeg + 10ms;

  do {
    // the thread always yield the processor before the due
    std::this_thread::yield();
  } while (std::chrono::steady_clock::now() < tend);

  const auto trend = std::chrono::steady_clock::now();

  int actual_time_us =
      std::chrono::duration_cast<std::chrono::microseconds>(trend - tbeg)
          .count();
  std::cout << "Yield time elapsed (expected 10ms): " << actual_time_us
            << std::endl;
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}