#pragma once
#include "Command.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class PushCommand final : public CommandBase
{
public:
    PushCommand(MemoryValueType val)
        : m_Value{ val }
    {
    }

    void Execute() override
    {
        _GetRuntimeContextLock()->stack.push(m_Value);
    }

private:
    MemoryValueType m_Value;
};

} // namespace commands
} // namespace cpuemul
