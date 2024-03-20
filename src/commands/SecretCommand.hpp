#pragma once
#include "CommandBase.hpp"
#include "RuntimeContext.hpp"

namespace cpuemul
{
namespace commands
{

class SecretCommand final : public CommandBase
{

public:
    static constexpr CommandCode Code = CommandCode ::Secret;
    inline CommandCode GetCommandCode() const override
    {
        return CommandCode ::Secret;
    }
    using ArgsTypeList = cul ::typelist ::TypeList<>;
    using ArgsTupleType = std ::tuple<>;

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
