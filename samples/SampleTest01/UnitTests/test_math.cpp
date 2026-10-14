#include "pch.h"
#include <gtest/gtest.h>
#include "module_math/math.h"

TEST(MathSuite, AddTest)
{
    EXPECT_EQ(Add(1, 2), 3);
    EXPECT_EQ(Add(-5, 3), -2);
}
