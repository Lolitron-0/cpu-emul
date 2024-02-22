#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class OutrCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::Outr, 1);

public:
    OutrCommand()
        : CommandBase{ 1, CommandCode::Outr }
    {
    }

    void Execute() override
    {
        auto contextLock{ _GetRuntimeContextLock() };
        std::cout << contextLock->Registers[static_cast<size_t>(m_Register)]
                  << std::endl;
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

private:
    RegisterName m_Register;
};

} // namespace commands
} // namespace cpuemul
