#pragma once
#include "RuntimeContext.hpp"
#include "commands/ArithmeticCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class DivCommand final : public ArithmeticCommandBase
{
    COMMAND_PROPERTIES(CommandCode::Div);

public:
    DivCommand()
        : ArithmeticCommandBase{ 0, CommandCode::Div }
    {
    }

    void Execute() override
    {
        auto [op1, op2] = _GetTwoTopStackValues();
        if (op2 == 0)
        {
            throw std::runtime_error{ "Division by zero" };
        }
        _GetRuntimeContextLock()->Stack.push(op1 / op2);
    }
};

} // namespace commands
} // namespace cpuemul
