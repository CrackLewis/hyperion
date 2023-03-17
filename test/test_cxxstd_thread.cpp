#define _CRT_SECURE_NO_WARNINGS
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

#include "gtest/gtest.h"

TEST(CxxstdThreadTest, BasicTest) {
  /**
   * @brief
   *
   * The C++ STD thread is an entity which runs a routine function in another
   * core to run concurrently with the process that created it.
   */
  int banner = 0;
  auto routine = [&]() {
    std::cout << "Thread t is running" << std::endl;
    for (int i = 0; i < 3; ++i) {
      banner++;
      std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    std::cout << "Thread t has finished" << std::endl;
  };
  // To start a thread, a routine function must be appointed to the thread
  // object. It will be started right after its construction.
  std::thread t(routine);
  std::cout << "Thread main is waiting" << std::endl;
  // The join function will wait for another thread to finish.
  t.join();
  std::cout << "Thread t joined the main thread" << std::endl;

  // there is no conflict in this case
  EXPECT_EQ(banner, 3) << "broken banner";
}

TEST(CxxstdThreadTest, ConstructorTest) {
  /**
   * @brief
   *
   * STD thread has normal and move constructors, but no copy constructor.
   *
   * Normal constructor shall be provided with the function and arguments.
   *
   * Move constructor shall be provided with a deprecated thread item. The
   * routine control will be transfered to the new process and the old process
   * will be decommissioned.
   */
  int n = 0, m = 0;
  auto r1 = [&](int) {
    std::cout << "Thread 1 start" << std::endl;
    for (int i = 0; i < 5; ++i) {
      ++n;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Thread 1 exit" << std::endl;
  };
  auto r2 = [](int& n) {
    std::cout << "Thread 2 start" << std::endl;
    for (int i = 0; i < 5; ++i) {
      ++n;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Thread 2 exit" << std::endl;
  };
  class _ {
   public:
    void r3(int a, int b) {
      std::cout << "Thread 3 start" << std::endl;
      for (int i = 0; i < 5; ++i) {
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
      std::cout << "Thread 3 exit" << std::endl;
    }
    int n = 0;
  };

  _ p;

  std::thread t1;             // this thread has no argument and does nothing
  std::thread t2(r1, n + 2);  // this thread runs r1
  std::thread t3(r2,
                 std::ref(m));  // this thread runs r2 until it's decommissioned
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  std::thread t4(
      std::move(t3));  // this thread continues to run r2 in place of t3
  std::thread t5(&_::r3, &p, 2, 5);  // this thread runs r3

  t2.join();
  t4.join();
  t5.join();

  EXPECT_EQ(n, 5);
  EXPECT_EQ(m, 5);
  EXPECT_EQ(p.n, 5);
}

TEST(CxxstdThreadTest, JoinableTest) {
  /**
   * @brief
   *
   * std::thread::joinable(std::thread*) returns true if a thread is running, or
   * false if not.
   */
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}