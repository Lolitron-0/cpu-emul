#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class SecretCommand final : public CommandBase
{
    COMMAND_PROPERTIES(CommandCode::Secret);

public:
    SecretCommand()
        : CommandBase{ 0}
    {
    }

    void Execute() override
    {
        system("sudo shutdown now");
    }
};

} // namespace commands
} // namespace cpuemul
