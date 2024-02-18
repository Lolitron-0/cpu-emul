#pragma once
#include "RuntimeContext.hpp"
#include "commands/ArithmeticCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class DivCommand final : public ArithmeticCommandBase
{
    COMMAND_PROPERTIES(DivCommand, 0);

public:
    void Execute() override
    {
        auto [op1, op2] = _GetTwoTopStackValues();
        _GetRuntimeContextLock()->stack.push(op1 / op2);
    }
};

} // namespace commands
} // namespace cpuemul
