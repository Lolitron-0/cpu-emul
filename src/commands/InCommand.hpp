#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class InCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::In, 0);

public:
    void Execute() override
    {
        MemoryValueType val;
        std::cin >> val;
        _GetRuntimeContextLock()->Stack.push(val);
    }
};

} // namespace commands
} // namespace cpuemul
