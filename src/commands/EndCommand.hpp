#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class EndCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::End, 0);

public:
    EndCommand()
        : CommandBase{ 0, CommandCode::End }
    {
    }

    void Execute() override
    {
        _GetRuntimeContextLock()->IsRunning = false;
    }
};

} // namespace commands
} // namespace cpuemul
