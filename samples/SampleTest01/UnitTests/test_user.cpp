#include "pch.h"
#include <gtest/gtest.h>
#include "module_user/user.h"

TEST(UserSuite, AdultCheckTest)
{
    EXPECT_TRUE(CheckAdult(20));
    EXPECT_FALSE(CheckAdult(16));
}
