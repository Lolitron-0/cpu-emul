#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class PushrCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::Pushr, 1, RegisterName);

public:
    void Execute() override
    {
        auto lock{ _GetRuntimeContextLock() };
        lock->Stack.push(lock->Registers[static_cast<size_t>(m_Register)]);
    }

    void SetArguments(const std::vector<std::string>& argsVec) override
    {
        TRY_RETHROW_CREATION_EXCEPTION(
            {
                m_Register =
                    internal::ConstructFromString<RegisterNameFromString>(argsVec[0]).value;
            },
            argsVec[0]);
    }

protected:
private:
    RegisterName m_Register;
};

} // namespace commands
} // namespace cpuemul
