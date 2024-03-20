#pragma once
#include "CommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class RetCommand final : public CommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::Ret;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::Ret;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<>;
    using ArgsTupleType = std ::tuple<>;

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
