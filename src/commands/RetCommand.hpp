#pragma once
#include "CommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class RetCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::Ret);

public:
    RetCommand()
        : CommandBase(0)
    {
    }

    void Execute() override
    {
        auto lock{ _GetRuntimeContextLock() };
        if (lock->CallStack.empty())
        {
            throw std::runtime_error{
                "Return from empty call stack (did you mean \"end\"?)"
            };
        }
        lock->ExecutionIterator =
            lock->Commands.begin() + lock->CallStack.top();
        lock->CallStack.pop();
    }
};

} // namespace commands
} // namespace cpuemul
