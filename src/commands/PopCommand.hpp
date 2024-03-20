#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class PopCommand final : public CommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::Pop;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::Pop;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<>;
    using ArgsTupleType = std ::tuple<>;

public:
    PopCommand()
        : CommandBase{ 0 }
    {
    }

    void Execute() override
    {
        auto lock{ _GetRuntimeContextLock() };
        if (lock->Stack.empty())
        {
            throw std::runtime_error{ "Not enough values on stack to pop" };
        }
        lock->Stack.pop();
    }
};

} // namespace commands
} // namespace cpuemul
