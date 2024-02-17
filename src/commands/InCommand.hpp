#pragma once
#include "Command.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class InCommand final : public CommandBase
{
public:
    void Execute() override
    {
        MemoryValueType val;
        std::cin >> val;
        _GetRuntimeContextLock()->stack.push(val);
    }
};

} // namespace commands
} // namespace cpuemul
