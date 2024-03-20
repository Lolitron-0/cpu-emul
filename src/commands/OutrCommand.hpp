#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class OutrCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::Outr, RegisterNameWrapper);

public:
    OutrCommand()
        : CommandBase{ 1}
    {
    }

    void Execute() override
    {
        auto contextLock{ _GetRuntimeContextLock() };
        std::cout << contextLock->Registers[static_cast<size_t>(m_Register)]
                  << std::endl;
    }

protected:
    void _SetArgumentsImpl(std::any argsTuple) override
    {
        auto args{ std::any_cast<ArgsTupleType>(argsTuple) };
        m_Register = std::get<0>(args).value;
    }

private:
    RegisterName m_Register;
};

} // namespace commands
} // namespace cpuemul
