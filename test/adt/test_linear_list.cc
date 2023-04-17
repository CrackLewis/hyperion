/**
 * @file test_linear_list.cc
 * @author CrackLewis (ghxx040406@163.com)
 * @brief
 * @version 0.1
 * @date 2023-03-27
 *
 * @copyright Copyright (c) 2023
 *
 *
 */

#include <iostream>
#include <new>

#include "adt/linear_list.hpp"
#include "gtest/gtest.h"

TEST(LinearListTest, ConstructorTest) {
  if (1) {
    LinearList<int> list;
  }
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}