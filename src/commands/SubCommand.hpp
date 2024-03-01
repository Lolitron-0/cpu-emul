#pragma once
#include "RuntimeContext.hpp"
#include "commands/ArithmeticCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class SubCommand final : public ArithmeticCommandBase
{
    COMMAND_PROPERTIES(CommandCode::Sub);

public:
    SubCommand()
        : ArithmeticCommandBase{ 0, CommandCode::Sub }
    {
    }

    void Execute() override
    {
        auto [op1, op2] = _PopTwoTopStackValues();
        _GetRuntimeContextLock()->Stack.push(op1 - op2);
    }
};

} // namespace commands
} // namespace cpuemul
