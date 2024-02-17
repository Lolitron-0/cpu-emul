#include <Stack/Stack.hpp>
#include <gtest/gtest.h>
#include <string>

TEST(Push, ConvertibleLval)
{
    stack::Stack<std::string> stack{};
    const char* literalLval{ "literal" };
    EXPECT_NO_THROW(stack.push(literalLval));
}

TEST(Push, ConvertibleRval)
{
    stack::Stack<std::string> stack{};
    EXPECT_NO_THROW(stack.push("literal"));
}

TEST(Push, SameLval)
{
    stack::Stack<std::string> stack{};
    std::string stringLval{ "string" };
    EXPECT_NO_THROW(stack.push(stringLval));
}

TEST(Push, SameRval)
{
    stack::Stack<std::string> stack{};
    EXPECT_NO_THROW(stack.push(std::string{ "string" }));
}

TEST(Push, OverCapacity)
{
    stack::Stack<std::string> stack{ 1 };
    EXPECT_NO_THROW({
        for (int32_t i{ 0 }; i < 1000; i++)
            stack.push("str");
    });
}

TEST(Emplace, Lval)
{
    stack::Stack<std::string> stack{};
    int32_t n{ 5 };
    char c{ '*' };
    stack.emplace(n, c);
    EXPECT_EQ(stack.top(), "*****");
}

TEST(Emplace, Rval)
{
    stack::Stack<std::string> stack{};
    stack.emplace(5ul, '*');
    EXPECT_EQ(stack.top(), "*****");
}

TEST(Pop, Basic)
{
    stack::Stack<std::string> stack{};
    stack.push("1");
    stack.push("2");
    stack.push("3");
    stack.pop();
    EXPECT_EQ(stack.size(), 2);
}

TEST(Getters, Size)
{
    stack::Stack<std::string> stack{};
    constexpr int32_t n{ 12 };
    for (int32_t i{ 0 }; i < n; i++)
        stack.push("");
    EXPECT_EQ(stack.size(), n);
}

TEST(Getters, Capacity)
{
    stack::Stack<std::string> stack{ 121 };
    EXPECT_EQ(stack.capacity(), 121);
}

TEST(Getters, Empty)
{
    stack::Stack<std::string> stack{};
    EXPECT_EQ(stack.empty(), true);
    stack.push("");
    EXPECT_EQ(stack.empty(), false);
}

TEST(Getters, TopNonConst)
{
    stack::Stack<std::string> stack{};
    stack.push("1");
    stack.push("2");
    stack.push("3");
    EXPECT_EQ(stack.top(), "3");
    stack.pop();
    EXPECT_EQ(stack.top(), "2");
    stack.pop();
    EXPECT_EQ(stack.top(), "1");
}

TEST(Getters, TopConst)
{
    stack::Stack<std::string> stack{};
    stack.push("1");
    EXPECT_EQ(std::invoke([](const auto& s) { return s.top(); }, stack), "1");
}

TEST(RuleOf, MoveCtor)
{
    EXPECT_NO_THROW(
        stack::Stack<std::string> stack{ stack::Stack<std::string>{} });
}

TEST(RuleOf, AssignmentCopy)
{
    stack::Stack<std::string> a, b;
    EXPECT_NO_THROW(a = b);
}

TEST(RuleOf, AssignmentMove)
{
    stack::Stack<std::string> a;
    EXPECT_NO_THROW(a = stack::Stack<std::string>{ 11 });
}
