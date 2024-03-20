#pragma once
#include "RuntimeContext.hpp"
#include "commands/ArithmeticCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class SubCommand final : public ArithmeticCommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::Sub;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::Sub;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<>;
    using ArgsTupleType = std ::tuple<>;

public:
    SubCommand()
        : ArithmeticCommandBase{ 0 }
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
