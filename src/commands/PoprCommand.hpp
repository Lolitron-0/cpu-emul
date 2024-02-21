#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class PoprCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::Popr, 1, RegisterName);

public:
    void Execute() override
    {
        auto lock{ _GetRuntimeContextLock() };
        if (lock->Stack.empty())
        {
            throw std::runtime_error{ "Not enough vlaues on stack to popr" };
        }
        lock->Registers[static_cast<size_t>(m_Register)] = lock->Stack.top();
        lock->Stack.pop();
    }

    void SetArguments(const std::vector<std::string>& argsVec) override
    {
        TRY_RETHROW_CREATION_EXCEPTION(
            {
                m_Register =
                    internal::ConstructFromString<RegisterNameFromString>(
                        argsVec[0])
                        .value;
            },
            argsVec[0]);
    }

protected:
private:
    RegisterName m_Register;
};

} // namespace commands
} // namespace cpuemul
