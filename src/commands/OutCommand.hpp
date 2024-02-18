#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class OutCommand final : public CommandBase
{
    COMMAND_PROPERTIES(OutCommand, 0);

public:
    void Execute() override
    {
        auto contextLock{ _GetRuntimeContextLock() };
        std::cout << contextLock->stack.top() << std::endl;
        contextLock->stack.pop();
    }
};

} // namespace commands
} // namespace cpuemul
