#pragma once
#include "JmpCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class CallCommand final : public JmpCommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::Call;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::Call;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<LabelTag>;
    using ArgsTupleType = std ::tuple<LabelTag>;

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
