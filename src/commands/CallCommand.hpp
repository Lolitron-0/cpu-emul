#pragma once
#include "JmpCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class CallCommand final : public JmpCommandBase
{
    COMMAND_PROPERTIES(CommandCode::Call, LabelTag)

public:
    CallCommand()
        : JmpCommandBase(1)
    {
    }

    void Execute() override
    {
        auto lock{ _GetRuntimeContextLock() };
        lock->CallStack.push(
            std::distance(lock->Commands.begin(), lock->ExecutionIterator));
        JmpCommandBase::Execute();
    }
};

} // namespace commands
} // namespace cpuemul
