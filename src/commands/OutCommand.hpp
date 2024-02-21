#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class OutCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::Out, 0);

public:
    void Execute() override
    {
        auto contextLock{ _GetRuntimeContextLock() };
        std::cout << contextLock->Stack.top() << std::endl;
        contextLock->Stack.pop();
    }
};

} // namespace commands
} // namespace cpuemul
