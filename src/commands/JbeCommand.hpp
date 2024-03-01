#pragma once
#include "JmpCommandBase.hpp"

namespace cpuemul
{
namespace commands
{

class JbeCommand final : public JmpCommandBase
{
    COMMAND_PROPERTIES(CommandCode::Jbe, std::string);

public:
    JbeCommand()
        : JmpCommandBase(1, CommandCode::Jbe)
    {
    }

    void Execute() override
    {
        auto [v1, v2] = _GetTwoTopStackValues();

        if (v1 <= v2)
        {
            JmpCommandBase::Execute();
        }
    }

private:
    std::string m_Label;
};

} // namespace commands
} // namespace cpuemul
