#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class PushCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::Push, MemoryValueType);

public:
    PushCommand()
        : CommandBase{ 1, CommandCode::Push }
    {
    }

    void Execute() override
    {
        _GetRuntimeContextLock()->Stack.push(m_Value);
    }

protected:
    void _SetArgumentsImpl(std::any argsTuple) override
    {
        auto args{ std::any_cast<ArgsTupleType>(argsTuple) };
		m_Value = std::get<0>(args);
    }

private:
    MemoryValueType m_Value;
};

} // namespace commands
} // namespace cpuemul
