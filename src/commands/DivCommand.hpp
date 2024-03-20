#pragma once
#include "RuntimeContext.hpp"
#include "commands/ArithmeticCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class DivCommand final : public ArithmeticCommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::Div;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::Div;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<>;
    using ArgsTupleType = std ::tuple<>;
    ;

public:
    DivCommand()
        : ArithmeticCommandBase{ 0}
    {
    }

    void Execute() override
    {
        auto [op1, op2] = _PopTwoTopStackValues();
        if (op2 == 0)
        {
            throw std::runtime_error{ "Division by zero" };
        }
        _GetRuntimeContextLock()->Stack.push(op1 / op2);
    }
};

} // namespace commands
} // namespace cpuemul
