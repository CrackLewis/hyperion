/**
 * @file test_cxxstd_thread.cpp
 * @author CrackLewis (ghxx040406@163.com)
 * @brief
 * @version 0.1
 * @date 2023-03-18
 *
 * @copyright Copyright (c) 2023
 *
 * This file involves testing the member functions of std::thread.
 */

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
   * std::thread::joinable() returns true if a thread is running, or
   * false if not.
   */

  // a thread from default constructor is not joinable.
  std::thread t;
  ASSERT_FALSE(t.joinable());
  std::cout << "t.joinable(): false (not assigned with a task)" << std::endl;

  // once the thread is appointed a routine, it will be running and thus
  // joinable.
  t = std::thread(
      [](int x) {
        while (x--) std::this_thread::sleep_for(std::chrono::milliseconds(200));
      },
      5);
  ASSERT_TRUE(t.joinable());
  std::cout << "t.joinable(): true (running)" << std::endl;

  // the thread will be unjoinable again once it's finished.
  t.join();
  ASSERT_FALSE(t.joinable());
  std::cout << "t.joinable(): false (finished)" << std::endl;
}

TEST(CxxstdThreadTest, HardwareConcurrencyTest) {
  /**
   * @brief
   *
   * std::thread::hardware_concurrency() returns the maximum concurrent threads
   * supported by the hardware.
   */
  int max_thrds = std::thread::hardware_concurrency();
  std::cout << "Max supported thread count: " << max_thrds << std::endl;
  ASSERT_GT(max_thrds, 0) << "bad function";
}

TEST(CxxstdThreadTest, JoinTest) {
  /**
   * @brief
   *
   * std::thread::join() makes the current thread waits for another thread to
   * stop running.
   *
   * Exceptions will occur if the thread is ill-formed.
   */

  // normal case
  if (1) {
    std::string flg;

    std::cout << "Main thread wait" << std::endl;
    flg += "W";

    std::thread t([&]() {
      std::cout << "Sub thread start" << std::endl;
      flg += "S";

      std::this_thread::sleep_for(std::chrono::milliseconds(500));

      std::cout << "Sub thread finish" << std::endl;
      flg += "F";
    });

    t.join();

    std::cout << "Main thread join" << std::endl;
    flg += "J";

    ASSERT_EQ(flg, "WSFJ");
  }

  // no_such_process
  ASSERT_THROW(
      {
        std::thread t;
        t.join();
      },
      std::exception);
  std::cout << "invalid argument test" << std::endl;

  // resource_deadlock_would_occur
  ASSERT_DEATH(
      {
        std::thread* t;

        auto self_join = [&]() {
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
          t->join();
        };

        t = new std::thread(self_join);
        t->join();
        delete t;
      },
      "");
  std::cout << "resource deadlock test" << std::endl;

  // invalid_argument
}

TEST(CxxstdThreadTest, DetachTest) {
  /**
   * @brief
   *
   * std::thread::detach() will make the thread object rid its routine without
   * stopping it. The routine continues running until it exits.
   */

  int n = 0, m = 0;
  auto r1 = [&]() {
    for (int i = 0; i < 5; ++i) {
      n++;
      std::cout << "Routine 1: " << i << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  };
  auto r2 = [&]() {
    for (int i = 0; i < 5; ++i) {
      m++;
      std::cout << "Routine 2: " << i << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  };

  std::cout << "Main: new thread" << std::endl;
  std::thread t(r1);
  std::this_thread::sleep_for(std::chrono::milliseconds(800));
  std::cout << "Main: detach" << std::endl;
  // detach r1 routine. r1 keeps running until it returns.
  t.detach();

  std::cout << "Main: change routine" << std::endl;
  // new routine is r2. r2 keeps running concurrently with main thread and r1.
  t = std::thread(r2);

  t.join();
  std::cout << "Main: end" << std::endl;

  ASSERT_EQ(n, 5);
  ASSERT_EQ(m, 5);
}

TEST(CxxstdThreadTest, SwapTest) {
  /**
   * @brief
   *
   * std::thread::swap(std::thread& r) will swap the thread handle of *this and
   * thread r.
   *
   * std::swap(std::thread&, std::thread&) is an overriden version of std::swap
   * with the same purpose with std::thread::swap.
   */

  int f1 = 0, f2 = 0;
  std::thread t1([&]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(600));
    f1 = 1;
  });
  std::thread t2([&]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    f2 = 1;
  });

  auto id1 = t1.get_id(), id2 = t2.get_id();
  std::cout << "Thread t1 (before swapping): " << id1 << std::endl;
  std::cout << "Thread t2 (before swapping): " << id2 << std::endl;

  // after swapping, t1.join() shall be earlier than t2.join() by ~300ms.
  std::swap(t1, t2);

  auto ids1 = t1.get_id(), ids2 = t2.get_id();
  std::cout << "Thread t1 (after swapping): " << ids1 << std::endl;
  std::cout << "Thread t2 (after swapping): " << ids2 << std::endl;

  // the thread ID shall also be swapped
  ASSERT_EQ(id2, ids1);
  ASSERT_EQ(id1, ids2);

  t1.join();

  // setup a counter thread to help timing when the main thread waits.
  int counter = 0;
  std::thread t3([&]() {
    while (1) {
      if (f1 && f2) return;
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      counter++;
    }
  });

  t2.join();
  t3.join();

  // the counter shall have counted to 5 or 6
  ASSERT_GT(counter, 0) << "std::thread::swap does not work";
  std::cout << "Ticks between t1.join() and t2.join(): " << counter
            << std::endl;
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}