#include <gtest/gtest.h>
#include <Stack/Stack.hpp>

TEST(Constructors, Default)
{
    EXPECT_NO_THROW(stack::Stack<std::string>{});
}