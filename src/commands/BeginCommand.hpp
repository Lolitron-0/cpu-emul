#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class BeginCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::Begin, 0);

public:
    void Execute() override
    {
        _GetRuntimeContextLock()->IsRunning = true;
    }
};

} // namespace commands
} // namespace cpuemul
