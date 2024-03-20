#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class BeginCommand final : public CommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::Begin;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::Begin;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<>;
    using ArgsTupleType = std ::tuple<>;

public:
    BeginCommand()
        : CommandBase{ 0 }
    {
    }

    void Execute() override
    {
        _GetRuntimeContextLock()->IsRunning = true;
    }
};

} // namespace commands
} // namespace cpuemul
