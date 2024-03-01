#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class OutCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::Out);

public:
    OutCommand()
        : CommandBase{ 0, CommandCode::Out }
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
