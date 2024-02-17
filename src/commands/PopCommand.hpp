#pragma once
#include "Command.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class PopCommand final : public CommandBase
{
public:
    void Execute() override
    {
        _GetRuntimeContextLock()->stack.pop();
    }
};

} // namespace commands
} // namespace cpuemul
