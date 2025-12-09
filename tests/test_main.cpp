// tests/test_main.cpp
#include <gtest/gtest.h>

TEST(SanityCheck, BasicAssertions) {
  EXPECT_STRNE("hello", "world");
  EXPECT_EQ(7 * 6, 42);
}