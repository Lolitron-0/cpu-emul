#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class EndCommand final : public CommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::End;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::End;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<>;
    using ArgsTupleType = std ::tuple<>;
    ;

public:
    EndCommand()
        : CommandBase{ 0 }
    {
    }

    void Execute() override
    {
        _GetRuntimeContextLock()->IsRunning = false;
    }
};

} // namespace commands
} // namespace cpuemul
