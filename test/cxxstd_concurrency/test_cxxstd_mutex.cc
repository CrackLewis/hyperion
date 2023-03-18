#include <mutex>
#include <thread>

#include "gtest/gtest.h"

TEST(CxxstdMutexTest, LockTest) {
  /**
   * @brief
   *
   * std::mutex is a structure which guarantees mutual exclusive access to
   * certain resources.
   *
   * The mutex class does not have a copy or move constructor, meaning that
   * mutex objects cannot be moved or copied.
   *
   * std::mutex::lock() will lock the mutex and block all other lock() calls
   * until std::mutex::unlock() is called later.
   */

  std::string shared_res = "";
  std::mutex shared_res_mutex;

  auto routine = [&](const std::string& a) {
    // lock the statement to prevent multiple access.
    shared_res_mutex.lock();
    shared_res += a;
    shared_res_mutex.unlock();
  };

  std::thread r1(routine, "a");
  std::thread r2(routine, "b");
  std::thread r3(routine, "c");
  std::thread r4(routine, "d");

  r1.join(), r2.join(), r3.join(), r4.join();

  EXPECT_EQ(shared_res.length(), 4) << "broken shared_res";
  std::cout << shared_res << std::endl;

  // the characters are unnecessarily in order, but they shall all be present
  int prod = 1 * shared_res[0] * shared_res[1] * shared_res[2] * shared_res[3];
  EXPECT_EQ(prod, 94109400);
}

TEST(CxxstdMutexTest, TryLockTest) {
  /**
   * @brief
   *
   * std::mutex::try_lock() will try to lock the mutex. If the attempt fails, it
   * does not wait and returns false instead. If it returns true then the
   * operation is a success.
   */

  std::mutex mtx;
  std::string str;

  // r1 will lock mtx for 500ms and exit
  auto r1 = [&]() {
    mtx.lock();
    str += "L";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    mtx.unlock();
  };

  // r2 will fail to lock mtx. it will retry until it successfully locks mtx and
  // will keep it locked for another 500ms.
  auto r2 = [&]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    while (1) {
      // lock: keep for 500ms
      if (mtx.try_lock()) {
        str += "l";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return;
      }
      // else: loop
      else {
        str += "w";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
      }
    }
  };

  std::thread t1(r1), t2(r2);
  t1.join(), t2.join();

  // Expected form: Lww...wl
  std::cout << "Flag: " << str << std::endl;

  EXPECT_GE(str.length(), 10);
  EXPECT_EQ(str.front(), 'L');
  EXPECT_EQ(str.back(), 'l');
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}