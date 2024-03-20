#pragma once
#include "RuntimeContext.hpp"
#include "commands/ArithmeticCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class MulCommand final : public ArithmeticCommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::Mul;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::Mul;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<>;
    using ArgsTupleType = std ::tuple<>;

public:
    MulCommand()
        : ArithmeticCommandBase{ 0}
    {
    }

    void Execute() override
    {
        auto [op1, op2] = _PopTwoTopStackValues();
        _GetRuntimeContextLock()->Stack.push(op1 * op2);
    }
};

} // namespace commands
} // namespace cpuemul
