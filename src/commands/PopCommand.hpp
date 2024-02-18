#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class PopCommand final : public CommandBase
{
    COMMAND_PROPERTIES(PopCommand, 0);

public:
    void Execute() override
    {
        _GetRuntimeContextLock()->stack.pop();
    }
};

} // namespace commands
} // namespace cpuemul
