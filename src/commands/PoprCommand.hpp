#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class PoprCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::Popr, RegisterNameWrapper);

public:
    PoprCommand()
        : CommandBase{ 1}
    {
    }

    void Execute() override
    {
        auto lock{ _GetRuntimeContextLock() };
        if (lock->Stack.empty())
        {
            throw std::runtime_error{ "Not enough values on stack to popr" };
        }
        lock->Registers[static_cast<size_t>(m_Register)] = lock->Stack.top();
        lock->Stack.pop();
    }

protected:
    void _SetArgumentsImpl(std::any argsTuple) override
    {
        auto args = std::any_cast<ArgsTupleType>(argsTuple);
        m_Register = std::get<0>(args).value;
    }

private:
    RegisterName m_Register;
};

} // namespace commands
} // namespace cpuemul
