#include <gtest/gtest.h>

#include "../include/utils.h"

TEST(MonteCarloTest, PiTest)
{
    ASSERT_NEAR(GetPi(generate_random_number, 1000000), 3.14, 1.e-2);
    ASSERT_NEAR(GetPi(generate_random_number, 100000000), 3.141, 1.e-3);
}
