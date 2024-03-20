#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class OutCommand final : public CommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::Out;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::Out;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<>;
    using ArgsTupleType = std ::tuple<>;

public:
    OutCommand()
        : CommandBase{ 0 }
    {
    }

    void Execute() override
    {
        auto contextLock{ _GetRuntimeContextLock() };
        if (contextLock->Stack.empty())
        {
            throw std::runtime_error{ "Not enough values on stack to out" };
        }
        std::cout << contextLock->Stack.top() << std::endl;
        contextLock->Stack.pop();
    }
};

} // namespace commands
} // namespace cpuemul
