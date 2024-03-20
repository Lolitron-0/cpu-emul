#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class InCommand final : public CommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::In;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::In;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<>;
    using ArgsTupleType = std ::tuple<>;

public:
    InCommand()
        : CommandBase{ 0 }
    {
    }

    void Execute() override
    {
        MemoryValueType val;
        std::cin >> val;
        _GetRuntimeContextLock()->Stack.push(val);
    }
};

} // namespace commands
} // namespace cpuemul
