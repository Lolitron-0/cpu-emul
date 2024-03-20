#pragma once
#include "RuntimeContext.hpp"
#include "commands/ArithmeticCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class AddCommand final : public ArithmeticCommandBase
{
    COMMAND_PROPERTIES(CommandCode::Add);

public:
    AddCommand()
        : ArithmeticCommandBase{ 0}
    {
    }

    void Execute() override
    {
        auto [op1, op2] = _PopTwoTopStackValues();
        _GetRuntimeContextLock()->Stack.push(op1 + op2);
    }
};

} // namespace commands
} // namespace cpuemul
