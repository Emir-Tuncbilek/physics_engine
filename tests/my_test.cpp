//
// Created by Emir Tuncbilek on 12/22/24.
//
#include <gtest/gtest.h>

inline int foo() { return 34; }

// GTEST_COLOR=1 ctest --test-dir tests --output-on-failure -j12

TEST(TestSuite, foo) {
    EXPECT_EQ(foo(), 34);
    ASSERT_EQ(foo(), 33) << "Something went wrong...\n";
}