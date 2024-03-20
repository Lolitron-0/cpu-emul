#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class EndCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::End);

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
