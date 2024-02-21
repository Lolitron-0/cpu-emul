#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class PopCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::Pop, 0);

public:
    void Execute() override
    {
        auto lock{ _GetRuntimeContextLock() };
        if (lock->Stack.empty())
        {
            throw std::runtime_error{ "Not enough vlaues on stack to pop" };
        }
        lock->Stack.pop();
    }
};

} // namespace commands
} // namespace cpuemul
