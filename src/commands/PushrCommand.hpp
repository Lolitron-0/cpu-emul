#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class PushrCommand final : public CommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::Pushr;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::Pushr;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<RegisterNameWrapper>;
    using ArgsTupleType = std ::tuple<RegisterNameWrapper>;

public:
    PushrCommand()
        : CommandBase{ 1}
    {
    }

    void Execute() override
    {
        auto lock{ _GetRuntimeContextLock() };
        lock->Stack.push(lock->Registers[static_cast<size_t>(m_Register)]);
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
